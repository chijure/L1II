/*
 *  Copyright (C) 2010,Imagis Technology Co. Ltd. All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>

#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/mutex.h>

#include CONFIG_LGE_BOARD_HEADER_FILE
#include "ist30xx.h"
#include "ist30xx_update.h"

#if IST30XX_DEBUG
#include "ist30xx_misc.h"
#endif

#if IST30XX_TRACKING_MODE
#include "ist30xx_tracking.h"
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))
#include <linux/input/mt.h>
#endif

#define MAX_ERR_CNT             (100)

/*                       */
#define ON			1
#define OFF			0
/*                       */
#if IST30XX_USE_KEY
int ist30xx_key_code[] = { 0, KEY_BACK, KEY_HOMEPAGE, KEY_MENU, KEY_RECENT };
#endif

DEFINE_MUTEX(ist30xx_mutex);
#if IST30XX_DETECT_TA
static int ist30xx_ta_status = -1;
#endif

static bool ist30xx_initialized = 0;
struct ist30xx_data *ts_data;
static struct delayed_work work_reset_check;
#if IST30XX_INTERNAL_BIN && IST30XX_UPDATE_BY_WORKQUEUE
static struct delayed_work work_fw_update;
#endif

static void clear_input_data(struct ist30xx_data *data);


#if IST30XX_EVENT_MODE
bool get_event_mode = true;

static struct timer_list idle_timer;
static struct timespec t_event, t_current;      // ns
#define EVENT_TIMER_INTERVAL     (HZ / 2)       // 0.5sec

# if IST30XX_NOISE_MODE
# define IST30XX_IDLE_STATUS     (0x1D4E0000)
# define IDLE_ALGORITHM_MODE     (1U << 15)
# endif // IST30XX_NOISE_MODE

#endif  // IST30XX_EVENT_MODE


/*                       */
struct ist30xx_ts_device {
	struct i2c_client *client;
	int (*power)(unsigned char onoff);
	int num_irq;
	int scl_gpio;
	int sda_gpio;
};
struct ist30xx_ts_device ist30xx_ts_dev;
/*                       */
void ist30xx_disable_irq(struct ist30xx_data *data)
{
	if (data->irq_enabled) {
		disable_irq(data->client->irq);
		data->irq_enabled = 0;
	}
}

void ist30xx_enable_irq(struct ist30xx_data *data)
{
	if (!data->irq_enabled) {
		enable_irq(data->client->irq);
		msleep(50);
		data->irq_enabled = 1;
	}
}


int ist30xx_max_error_cnt = MAX_ERR_CNT;
int ist30xx_error_cnt = 0;
static void ist30xx_request_reset(void)
{
	ist30xx_error_cnt++;
	if (ist30xx_error_cnt >= ist30xx_max_error_cnt) {
		schedule_delayed_work(&work_reset_check, 0);
		printk("[ TSP ] ist30xx_request_reset!\n");
		ist30xx_error_cnt = 0;
	}
}


void ist30xx_start(struct ist30xx_data *data)
{
#if IST30XX_DETECT_TA
	if (ist30xx_ta_status > -1) {
		ist30xx_write_cmd(data->client, CMD_SET_TA_MODE, ist30xx_ta_status);

		printk("[ TSP ] ist30xx_start, ta_mode : %d\n",
		     ist30xx_ta_status);
	}
#endif

	ist30xx_cmd_start_scan(data->client);

#if IST30XX_EVENT_MODE
	if ((data->status.update != 1) && (data->status.calib != 1))
		ktime_get_ts(&t_event);
#endif
}


int ist30xx_get_ver_info(struct ist30xx_data *data)
{
	int ret;

	data->fw.pre_ver = data->fw.ver;
	data->fw.ver = 0;

	ret = ist30xx_read_cmd(data->client, CMD_GET_CHIP_ID, &data->chip_id);
	if (ret)
		return -EIO;

	ret = ist30xx_read_cmd(data->client, CMD_GET_FW_VER, &data->fw.ver);
	if (ret)
		return -EIO;

	ret = ist30xx_read_cmd(data->client, CMD_GET_PARAM_VER, &data->param_ver);
	if (ret)
		return -EIO;

	printk("[ TSP ] Chip ID : %x F/W: %x Param: %x\n",
	     data->chip_id, data->fw.ver, data->param_ver);

	if ((data->chip_id != IST30XX_CHIP_ID) &&
	    (data->chip_id != IST30XXA_CHIP_ID))
		return -EPERM;

	return 0;
}


int ist30xx_init_touch_driver(struct ist30xx_data *data)
{
	int ret = 0;

	mutex_lock(&ist30xx_mutex);
	ist30xx_disable_irq(data);

	ret = ist30xx_cmd_run_device(data->client);
	if (ret)
		goto init_touch_end;

	ret = ist30xx_get_ver_info(data);
	if (ret)
		goto init_touch_end;

init_touch_end:
	ist30xx_start(data);

	ist30xx_enable_irq(data);
	mutex_unlock(&ist30xx_mutex);

	return ret;
}


int ist30xx_get_info(struct ist30xx_data *data)
{
	int ret;

	mutex_lock(&ist30xx_mutex);
	ist30xx_disable_irq(data);

	ret = ist30xx_cmd_run_device(data->client);
	if (ret) goto get_info_end;

#if !(IST30XX_INTERNAL_BIN)
	ret = ist30xx_get_ver_info(data);
	if (ret) goto get_info_end;
#endif  // !(IST30XX_INTERNAL_BIN)

#if IST30XX_DEBUG
# if IST30XX_INTERNAL_BIN
	ist30xx_get_tsp_info();
	ist30xx_get_tkey_info();
# else
	ret = ist30xx_tsp_update_info();
	if (ret) goto get_info_end;

	ret = ist30xx_tkey_update_info();
	if (ret) goto get_info_end;
# endif
#endif  // IST30XX_DEBUG

    ist30xx_start(ts_data);
    
#if IST30XX_EVENT_MODE
    ktime_get_ts(&t_event);
#endif

get_info_end:
	if (ret == 0)
		ist30xx_enable_irq(data);
	mutex_unlock(&ist30xx_mutex);

	return ret;
}


#define PRESS_MSG_MASK          (0x01)
#define MULTI_MSG_MASK          (0x02)
#define PRESS_MSG_KEY           (0x6)
#define CALIB_MSG_MASK          (0xF0000FFF)
#define CALIB_MSG_VALID         (0x80000CAB)

#define TOUCH_DOWN_MESSAGE      ("Touch down")
#define TOUCH_UP_MESSAGE        ("Touch up  ")
#define TOUCH_MOVE_MESSAGE      ("Touch move")
bool tsp_touched[IST30XX_MAX_MT_FINGERS] = { 0, };

void print_tsp_event(finger_info *finger)
{
#if PRINT_TOUCH_EVENT
	int idx = finger->bit_field.id - 1;
	int press = finger->bit_field.udmg & PRESS_MSG_MASK;

	if (press == PRESS_MSG_MASK) {
		if (tsp_touched[idx] == 0) { // touch down
			printk("[ TSP ] %s - %d (%d, %d)\n", TOUCH_DOWN_MESSAGE,
			     finger->bit_field.id,
			     finger->bit_field.x, finger->bit_field.y);
			tsp_touched[idx] = 1;
		}
#if 0
		else {                // touch move
			printk("[ TSP ] %s - %d (%d,%d)\n", idx, TOUCH_MOVE_MESSAGE,
			     finger->bit_field.x, finger->bit_field.y);
		}
#endif
	} else {
		if (tsp_touched[idx] == 1) { // touch up
			printk("[ TSP ] %s - %d (%d, %d)\n", TOUCH_UP_MESSAGE,
			     finger->bit_field.id,
			     finger->bit_field.x, finger->bit_field.y);
			tsp_touched[idx] = 0;
		}
	}
#endif  // PRINT_TOUCH_EVENT
}


static void clear_input_data(struct ist30xx_data *data)
{
	int i, pressure, count;

	for (i = 0, count = 0; i < IST30XX_MAX_MT_FINGERS; i++) {
		if (data->prev_fingers[i].bit_field.id == 0)
			continue;

		pressure = (data->prev_fingers[i].bit_field.udmg & PRESS_MSG_MASK);
		if (pressure) {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))
			input_mt_slot(data->input_dev, data->prev_fingers[i].bit_field.id - 1);
			input_mt_report_slot_state(data->input_dev, MT_TOOL_FINGER, false);
#else
			input_report_abs(data->input_dev, ABS_MT_POSITION_X,
					 data->prev_fingers[i].bit_field.x);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y,
					 data->prev_fingers[i].bit_field.y);
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 0);
			input_mt_sync(data->input_dev);
#endif                  // (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))

			data->prev_fingers[i].bit_field.udmg &= ~(PRESS_MSG_MASK);
			print_tsp_event(&data->prev_fingers[i]);

			data->prev_fingers[i].bit_field.id = 0;

			count++;
		}
	}

	if (count > 0)
		input_sync(data->input_dev);
}


static void release_finger(finger_info *finger)
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))
	input_mt_slot(ts_data->input_dev, finger->bit_field.id - 1);
	input_mt_report_slot_state(ts_data->input_dev, MT_TOOL_FINGER, false);
#else
	input_report_abs(ts_data->input_dev, ABS_MT_POSITION_X, finger->bit_field.x);
	input_report_abs(ts_data->input_dev, ABS_MT_POSITION_Y, finger->bit_field.y);
	input_report_abs(ts_data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
	input_report_abs(ts_data->input_dev, ABS_MT_WIDTH_MAJOR, 0);
	input_mt_sync(ts_data->input_dev);
#endif                  // (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))

	printk("[ TSP ] force release %d(%d, %d)\n", finger->bit_field.id,
	       finger->bit_field.x, finger->bit_field.y);

	finger->bit_field.udmg &= ~(PRESS_MSG_MASK);
	print_tsp_event(finger);

	finger->bit_field.id = 0;

	input_sync(ts_data->input_dev);
}

#if IST30XX_DEBUG
extern TSP_INFO ist30xx_tsp_info;
extern TKEY_INFO ist30xx_tkey_info;
#endif
static int check_report_data(struct ist30xx_data *data, int finger_counts, int key_counts)
{
	int ret = -EPERM;
	int i, j;
	bool valid_id;

	/* current finger info */
	for (i = 0; i < finger_counts; i++) {
		if ((data->fingers[i].bit_field.id == 0) ||
		    (data->fingers[i].bit_field.id > ist30xx_tsp_info.finger_num + 1) ||
		    (data->fingers[i].bit_field.x > IST30XX_MAX_X) ||
		    (data->fingers[i].bit_field.y > IST30XX_MAX_Y)) {
			pr_err("[ TSP ] Error, %d[%d] - (%d, %d)\n", i,
			       data->fingers[i].bit_field.id,
			       data->fingers[i].bit_field.x,
			       data->fingers[i].bit_field.y);

			data->fingers[i].bit_field.id = 0;
			return ret;
		}
	}

	/* previous finger info */
	if (data->num_fingers >= finger_counts) {
		for (i = 0; i < IST30XX_MAX_MT_FINGERS; i++) { // prev_fingers
			if (data->prev_fingers[i].bit_field.id != 0 &&
			    (data->prev_fingers[i].bit_field.udmg & PRESS_MSG_MASK)) {
				valid_id = false;
				for (j = 0; j < ist30xx_tsp_info.finger_num; j++) { // fingers
					if ((data->prev_fingers[i].bit_field.id) ==
					    (data->fingers[j].bit_field.id)) {
						valid_id = true;
						break;
					}
				}
				if (valid_id == false)
					release_finger(&data->prev_fingers[i]);
			}
		}
	}

	return 0;
}

bool get_zvalue_mode = false;
int ist30xx_zvalue = 0;
static void report_input_data(struct ist30xx_data *data, int finger_counts, int key_counts)
{
	int i, pressure, count;

	memset(data->prev_fingers, 0, sizeof(data->prev_fingers));

	for (i = 0, count = 0; i < finger_counts; i++) {
		pressure = data->fingers[i].bit_field.udmg & PRESS_MSG_MASK;

/*		printk("[ TSP ] [%d][%d][%d] x, y, z = %03d, %03d, %04d\n", i,
		     data->fingers[i].bit_field.id, pressure,
		     data->fingers[i].bit_field.x,
		     data->fingers[i].bit_field.y,
		     data->fingers[i].bit_field.w);
*/
		print_tsp_event(&data->fingers[i]);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))
		input_mt_slot(data->input_dev, data->fingers[i].bit_field.id - 1);
		input_mt_report_slot_state(data->input_dev, MT_TOOL_FINGER,
					   (pressure ? true : false));
		if (pressure) {
			input_report_abs(data->input_dev, ABS_MT_POSITION_X,
					 data->fingers[i].bit_field.x);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y,
					 data->fingers[i].bit_field.y);
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR,
					 data->fingers[i].bit_field.w);
		}
#else
		input_report_abs(data->input_dev, ABS_MT_POSITION_X,
				 data->fingers[i].bit_field.x);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y,
				 data->fingers[i].bit_field.y);
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR,
				 pressure);
		input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR,
				 data->fingers[i].bit_field.w);
		input_mt_sync(data->input_dev);
#endif          // (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0)

		count++;

		ist30xx_error_cnt = 0;

		data->prev_fingers[i].full_field = data->fingers[i].full_field;
		data->num_fingers = finger_counts;
	}

#if IST30XX_USE_KEY
	for (i = finger_counts; i < finger_counts + key_counts; i++) {
		int press, id;
		press = (data->fingers[i].bit_field.w == PRESS_MSG_KEY) ? 1 : 0;
		id = data->fingers[i].bit_field.id;

		printk("[ TSP ] [%08x] id: %d, event: %d, z-value: %d\n",
		     data->fingers[i].full_field, id,
		     data->fingers[i].bit_field.w,
		     data->fingers[i].bit_field.y);

		ist30xx_zvalue = data->fingers[i].bit_field.y;

		input_report_key(data->input_dev, ist30xx_key_code[id], press);
		count++;
	}
#endif  // IST30XX_USE_KEY

	if (count > 0)
		input_sync(data->input_dev);
}

/*
 * CMD : CMD_GET_COORD
 *               [31:30]  [29:26]  [25:16]  [15:10]  [9:0]
 *   Multi(1st)  UDMG     Rsvd.    NumOfKey Rsvd.    NumOfFinger
 *    Single &   UDMG     ID       X        Area     Y
 *   Multi(2nd)
 *
 *   UDMG [31] 0/1 : single/multi
 *   UDMG [30] 0/1 : unpress/press
 */
u32 touch_msg = 0;
static irqreturn_t ist30xx_irq_thread(int irq, void *ptr)
{
	int i, ret;
	int key_cnt, finger_cnt, read_cnt;
	struct ist30xx_data *data = ptr;
	u32 msg[IST30XX_MAX_MT_FINGERS];
	bool unknown_idle = false;

#if IST30XX_TRACKING_MODE
	u32 ms;
#endif

	if (!data->irq_enabled)
		return IRQ_HANDLED;

	memset(msg, 0, sizeof(msg));

	ret = ist30xx_get_position(data->client, msg, 1);
	if (ret)
		goto irq_err;

	//printk("[ TSP ] intr msg: 0x%08x\n", *msg);

	if (msg[0] == 0)
		return IRQ_HANDLED;

#if IST30XX_EVENT_MODE
	if ((data->status.update != 1) && (data->status.calib != 1))
		ktime_get_ts(&t_event);
#endif

#if IST30XX_TRACKING_MODE
	ms = t_event.tv_sec * 1000 + t_event.tv_nsec / 1000000;
	if (touch_msg != msg[0]) {
		ist30xx_put_track(ms, msg[0]);
		touch_msg = msg[0];
	}
#endif

#if IST30XX_NOISE_MODE
	if (get_event_mode) {
		if ((msg[0] & 0xFFFF0000) == IST30XX_IDLE_STATUS) {
			//printk("[ TSP ] idle status: 0x%08x\n", *msg);
			if (msg[0] & IDLE_ALGORITHM_MODE)
				return IRQ_HANDLED;

			for (i = 0; i < IST30XX_MAX_MT_FINGERS; i++) {
				if (data->prev_fingers[i].bit_field.id == 0)
					continue;

				if (data->prev_fingers[i].bit_field.udmg & PRESS_MSG_MASK) {
					printk("[ TSP ] prev_fingers: %08x\n",
					       data->prev_fingers[i].full_field);
					release_finger(&data->prev_fingers[i]);
					unknown_idle = true;
				}
			}

			if (unknown_idle) {
				schedule_delayed_work(&work_reset_check, 0);
				pr_err("[ TSP ] Find unknown pressure\n");
			}

			return IRQ_HANDLED;
		}
	}
#endif  // IST30XX_NOISE_MODE

	if ((msg[0] & CALIB_MSG_MASK) == CALIB_MSG_VALID) {
		data->status.calib_msg = msg[0];
		printk("[ TSP ] calib status: 0x%08x\n", data->status.calib_msg);
		return IRQ_HANDLED;
	}

	for (i = 0; i < IST30XX_MAX_MT_FINGERS; i++)
		data->fingers[i].full_field = 0;

	key_cnt = 0;
	finger_cnt = 1;
	read_cnt = 1;
	data->fingers[0].full_field = msg[0];

	if (data->fingers[0].bit_field.udmg & MULTI_MSG_MASK) {
		key_cnt = data->fingers[0].bit_field.x;
		finger_cnt = data->fingers[0].bit_field.y;
		read_cnt = finger_cnt + key_cnt;

		if (finger_cnt > ist30xx_tsp_info.finger_num ||
		    key_cnt > ist30xx_tkey_info.key_num) {
			pr_err("[ TSP ] Invalid touch count - finger: %d(%d), key: %d(%d)\n",
			       finger_cnt, ist30xx_tsp_info.finger_num,
			       key_cnt, ist30xx_tkey_info.key_num);
			goto irq_err;
		}

#if I2C_BURST_MODE
		ret = ist30xx_get_position(data->client, msg, read_cnt);
		if (ret)
			goto irq_err;

		for (i = 0; i < read_cnt; i++)
			data->fingers[i].full_field = msg[i];
#else
		for (i = 0; i < read_cnt; i++) {
			ret = ist30xx_get_position(data->client, &msg[i], 1);
			if (ret)
				goto irq_err;

			data->fingers[i].full_field = msg[i];
		}
#endif          // I2C_BURST_MODE

#if IST30XX_TRACKING_MODE
		for (i = 0; i < read_cnt; i++)
			ist30xx_put_track(ms, msg[i]);
		touch_msg = msg[0];
#endif
	}

	if (get_zvalue_mode == true) {
		u32 zvalue = 0;

		ret = ist30xx_get_position(data->client, &zvalue, 1);
		if (ret)
			goto irq_err;

		if (zvalue > 0)
			ist30xx_zvalue = zvalue;
		//printk("[ TSP ] [%08x] key raw data: %d\n", zvalue, ist30xx_zvalue);
	}

	if (check_report_data(data, finger_cnt, key_cnt))
		return IRQ_HANDLED;

	if (read_cnt > 0)
		report_input_data(data, finger_cnt, key_cnt);

	return IRQ_HANDLED;

irq_err:
	pr_err("[ TSP ] intr msg[0]: 0x%08x, ret: %d\n", msg[0], ret);
	ist30xx_request_reset();
	return IRQ_HANDLED;
}


#ifdef CONFIG_HAS_EARLYSUSPEND
#define ist30xx_suspend NULL
#define ist30xx_resume  NULL
static void ist30xx_early_suspend(struct early_suspend *h)
{
	struct ist30xx_data *data = container_of(h, struct ist30xx_data,
						 early_suspend);

	mutex_lock(&ist30xx_mutex);
	ist30xx_disable_irq(data);
	ist30xx_internal_suspend(data);
	clear_input_data(data);
	mutex_unlock(&ist30xx_mutex);
}
static void ist30xx_late_resume(struct early_suspend *h)
{
	struct ist30xx_data *data = container_of(h, struct ist30xx_data,
						 early_suspend);

	mutex_lock(&ist30xx_mutex);
	ist30xx_internal_resume(data);
	ist30xx_start(data);
	ist30xx_enable_irq(data);
	mutex_unlock(&ist30xx_mutex);
}
#else
static int ist30xx_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct ist30xx_data *data = i2c_get_clientdata(client);

	return ist30xx_internal_suspend(data);
}
static int ist30xx_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct ist30xx_data *data = i2c_get_clientdata(client);

	return ist30xx_internal_resume(data);
}
#endif // CONFIG_HAS_EARLYSUSPEND

/*                       */
void ist30xx_ts_reset(void)
{
	ist30xx_ts_off();
	msleep(50);
	ist30xx_ts_on();

	printk("[ TSP ] ist30xx_ts_reset\n");
	msleep(50);
}

int ist30xx_ts_off(void)
{
	struct ist30xx_ts_device *dev = NULL;
	int ret = 0;

	dev = &ist30xx_ts_dev;

	ret = dev->power(OFF);

	printk("[ TSP ] ist30xx_ts_off\n");

	if(ret < 0) {
		printk(KERN_ERR "ist30xx_ts_off power off failed\n");
		goto err_power_failed;
	}

	ts_data->status.power = 0;

	msleep(10);

err_power_failed:
	return ret;
}

int ist30xx_ts_on(void)
{
	struct ist30xx_ts_device *dev = NULL;
	int ret = 0;

	dev = &ist30xx_ts_dev;

	ret = dev->power(ON);

	printk("[ TSP ] ist30xx_ts_on\n");

	if(ret < 0) {
		printk(KERN_ERR "ist30xx_ts_on power on failed\n");
		goto err_power_failed;
	}

	ts_data->status.power = 1;

	msleep(30);

err_power_failed:
	return ret;
}
/*                       */

void ist30xx_set_ta_mode(bool charging)
{
#if IST30XX_DETECT_TA
	if ((ist30xx_ta_status == -1) || (charging == ist30xx_ta_status))
		return;

	ist30xx_ta_status = charging ? 1 : 0;

	printk("[ TSP ] %s(), charging = %d\n", __func__, ist30xx_ta_status);

	schedule_delayed_work(&work_reset_check, 0);
#endif
}
EXPORT_SYMBOL(ist30xx_set_ta_mode);


static void reset_work_func(struct work_struct *work)
{
	if ((ts_data == NULL) || (ts_data->client == NULL))
		return;

	printk("[ TSP ] Request reset function\n");

	if ((ts_data->status.power == 1) &&
	    (ts_data->status.update != 1) && (ts_data->status.calib != 1)) {
		mutex_lock(&ist30xx_mutex);
		ist30xx_disable_irq(ts_data);

		clear_input_data(ts_data);

		ist30xx_cmd_run_device(ts_data->client);

		ist30xx_start(ts_data);

		ist30xx_enable_irq(ts_data);
		mutex_unlock(&ist30xx_mutex);
	}
}

#if IST30XX_INTERNAL_BIN && IST30XX_UPDATE_BY_WORKQUEUE
static void fw_update_func(struct work_struct *work)
{
	if ((ts_data == NULL) || (ts_data->client == NULL))
		return;

	printk("[ TSP ] FW update function\n");

	if (ist30xx_auto_bin_update(ts_data))
		ist30xx_disable_irq(ts_data);
}
#endif // IST30XX_INTERNAL_BIN && IST30XX_UPDATE_BY_WORKQUEUE


#if IST30XX_EVENT_MODE
void timer_handler(unsigned long data)
{
	int event_ms;
	int curr_ms;

	if (get_event_mode) {
		if ((ts_data->status.power == 1) && (ts_data->status.update != 1)) {
			ktime_get_ts(&t_current);

			curr_ms = t_current.tv_sec * 1000 + t_current.tv_nsec / 1000000;
			event_ms = t_event.tv_sec * 1000 + t_event.tv_nsec / 1000000;

			//printk("[ TSP ] event_ms %d, current: %d\n", event_ms, curr_ms);

			if (ts_data->status.calib == 1) {
				if (curr_ms - event_ms >= 2000) {
					ts_data->status.calib = 0;
					//printk("[ TSP ] calibration timeout over 3sec\n");
					schedule_delayed_work(&work_reset_check, 0);
					ktime_get_ts(&t_event);
				}
			}
#if IST30XX_NOISE_MODE
			else if (curr_ms - event_ms >= 5000) {
				pr_err("[ TSP ] idle timeout over 5sec\n");
				schedule_delayed_work(&work_reset_check, 0);
			}
#endif                  // IST30XX_NOISE_MODE
		}
	}

	mod_timer(&idle_timer, get_jiffies_64() + EVENT_TIMER_INTERVAL);
}
#endif // IST30XX_EVENT_MODE


static int __devinit ist30xx_probe(struct i2c_client *		client,
				   const struct i2c_device_id * id)
{
	int ret;
	struct ist30xx_data *data;
	struct input_dev *input_dev;

	/*                       */
	struct touch_platform_data *ts_pdata;
	struct ist30xx_ts_device *dev;

	ts_pdata = client->dev.platform_data;
	dev = &ist30xx_ts_dev;
	/*                       */

	printk("[ TSP ] %s() ,the i2c addr=0x%x", __func__, client->addr);

	dev->power = ts_pdata->power;
	dev->num_irq = ts_pdata->irq;
	dev->sda_gpio = ts_pdata->sda;
	dev->scl_gpio  = ts_pdata->scl;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	input_dev = input_allocate_device();
	if (!input_dev) {
		ret = -ENOMEM;
		pr_err("%s: input_allocate_device failed (%d)\n", __func__, ret);
		goto err_alloc_dev;
	}

	printk("[ TSP ] irq : %d, scl : %d, sda : %d\n", client->irq, ts_pdata->scl, ts_pdata->sda);
	data->num_fingers = IST30XX_MAX_MT_FINGERS;
	data->irq_enabled = 1;
	data->client = client;
	data->input_dev = input_dev;
	i2c_set_clientdata(client, data);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))
	input_mt_init_slots(input_dev, IST30XX_MAX_MT_FINGERS);
#endif

	input_dev->name = "ist30xx_ts_input";
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = &client->dev;

	set_bit(EV_ABS, input_dev->evbit);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))
	set_bit(INPUT_PROP_DIRECT, input_dev->propbit);
#endif

	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, IST30XX_MAX_X, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, IST30XX_MAX_Y, 0, 0);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0))
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, IST30XX_MAX_W, 0, 0);
#else
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, IST30XX_MAX_Z, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, IST30XX_MAX_W, 0, 0);
#endif

#if IST30XX_USE_KEY
	{
		int i;
		set_bit(EV_KEY, input_dev->evbit);
		set_bit(EV_SYN, input_dev->evbit);
		for (i = 1; i < ARRAY_SIZE(ist30xx_key_code); i++)
			set_bit(ist30xx_key_code[i], input_dev->keybit);
	}
#endif

	input_set_drvdata(input_dev, data);
	ret = input_register_device(input_dev);
	if (ret) {
		input_free_device(input_dev);
		goto err_reg_dev;
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	data->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	data->early_suspend.suspend = ist30xx_early_suspend;
	data->early_suspend.resume = ist30xx_late_resume;
	register_early_suspend(&data->early_suspend);
#endif

	ts_data = data;

	ret = ist30xx_init_system();
	if (ret) {
		dev_err(&client->dev, "chip initialization failed\n");
		goto err_init_drv;
	}

	ret = ist30xx_init_update_sysfs();
	if (ret)
		goto err_init_drv;

#if IST30XX_DEBUG
	ret = ist30xx_init_misc_sysfs();
	if (ret)
		goto err_init_drv;
#endif

# if IST30XX_FACTORY_TEST
	ret = ist30xx_factory_sysfs();
	if (ret)
		goto err_init_drv;
#endif

#if IST30XX_TRACKING_MODE
	ist30xx_tracking_init();
#endif

	ret = request_threaded_irq(client->irq, NULL, ist30xx_irq_thread,
				   IRQF_TRIGGER_FALLING | IRQF_ONESHOT, "ist30xx_ts", data);
	if (ret)
		goto err_irq;

	ist30xx_disable_irq(data);

#if IST30XX_INTERNAL_BIN
# if IST30XX_UPDATE_BY_WORKQUEUE
	INIT_DELAYED_WORK(&work_fw_update, fw_update_func);
	schedule_delayed_work(&work_fw_update, IST30XX_UPDATE_DELAY);
# else
	ret = ist30xx_auto_bin_update(data);
	if (ret != 0)
		goto err_irq;
# endif
#endif  // IST30XX_INTERNAL_BIN

	ist30xx_get_info(data);

	INIT_DELAYED_WORK(&work_reset_check, reset_work_func);

#if IST30XX_DETECT_TA
	ist30xx_ta_status = 0;
#endif

#if IST30XX_EVENT_MODE
	init_timer(&idle_timer);
	idle_timer.function = timer_handler;
	idle_timer.expires = jiffies_64 + (EVENT_TIMER_INTERVAL);

	mod_timer(&idle_timer, get_jiffies_64() + EVENT_TIMER_INTERVAL);

	ktime_get_ts(&t_event);
#endif

	ist30xx_initialized = 1;

	return 0;

err_irq:
	ist30xx_disable_irq(data);
	free_irq(client->irq, data);
err_init_drv:
#if IST30XX_EVENT_MODE
	get_event_mode = false;
#endif
	pr_err("[ TSP ] Error, ist30xx init driver\n");
//	ist30xx_power_off();
	ist30xx_ts_off();
	input_unregister_device(input_dev);
	return 0;

err_reg_dev:
err_alloc_dev:
	pr_err("[ TSP ] Error, ist30xx mem free\n");
	kfree(data);
	return 0;
}


static int __devexit ist30xx_remove(struct i2c_client *client)
{
	struct ist30xx_data *data = i2c_get_clientdata(client);

#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&data->early_suspend);
#endif

	free_irq(client->irq, data);
//	ist30xx_power_off();
	ist30xx_ts_off();

	input_unregister_device(data->input_dev);
	kfree(data);

	return 0;
}


static struct i2c_device_id ist30xx_idtable[] = {
	{ IST30XX_DEV_NAME, 0 },
	{},
};


MODULE_DEVICE_TABLE(i2c, ist30xx_idtable);

#ifdef CONFIG_HAS_EARLYSUSPEND
static const struct dev_pm_ops ist30xx_pm_ops = {
	.suspend	= ist30xx_suspend,
	.resume		= ist30xx_resume,
};
#endif


static struct i2c_driver ist30xx_i2c_driver = {
	.id_table	= ist30xx_idtable,
	.probe		= ist30xx_probe,
	.remove		= __devexit_p(ist30xx_remove),
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= IST30XX_DEV_NAME,
#ifdef CONFIG_HAS_EARLYSUSPEND
		.pm	= &ist30xx_pm_ops,
#endif
	},
};


static int __init ist30xx_init(void)
{
	return i2c_add_driver(&ist30xx_i2c_driver);
}


static void __exit ist30xx_exit(void)
{
	i2c_del_driver(&ist30xx_i2c_driver);
}

module_init(ist30xx_init);
module_exit(ist30xx_exit);

MODULE_DESCRIPTION("Imagis IST30XX touch driver");
MODULE_LICENSE("GPL");
