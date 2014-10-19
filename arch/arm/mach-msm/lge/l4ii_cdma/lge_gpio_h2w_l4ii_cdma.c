/*
 *  H2W device detection driver.
 *
 * Copyright (C) 2008 -2013 LGE Corporation.
 * Copyright (C) 2008 Google, Inc.
 * 
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/
#include <linux/module.h>
#include <linux/sysdev.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/hrtimer.h>
#include <linux/switch.h>
#include <linux/input.h>
#include <linux/debugfs.h>
#include <asm/gpio.h>
#include <asm/atomic.h>
#include <mach/board.h>
#include <mach/vreg.h>
#include CONFIG_LGE_BOARD_HEADER_FILE
#include <linux/slab.h>
#include <linux/wakelock.h>

#define DEBUG_H2W
#ifdef DEBUG_H2W
#define H2W_DBG(fmt, arg...) printk(KERN_INFO "[H2W] %s " fmt "\n", __FUNCTION__, ## arg)
#else
#define H2W_DBG(fmt, arg...) do {} while (0)
#endif

static struct workqueue_struct *g_detection_work_queue;
static void detection_work(struct work_struct *work);
static DECLARE_WORK(g_detection_work, detection_work);

static int ip_dev_reg;

/*Unplug State*/
#define BIT_NO_DEVICE				0
/*4Pole Headset - headset with mic*/
#define BIT_HEADSET					(1 << 0)
/*3Pole Headset - headset without mic*/
#define BIT_HEADSET_SPEAKER_ONLY	(1 << 1) 

/*adaptive pole detection */
#define H2W_ADAPTIVE_POLE_DETECT

/*button irq  - delayed registeration */
#define H2W_BUTTON_IRQ_DELAYED_REGISTER

struct h2w_info {
	struct switch_dev sdev;
	struct input_dev *input;
	
	/*GPIO Number - Headset Detect */
	int gpio_detect;
	/*GPIO Number - Headset Button Detect */	
	int gpio_button_detect;
	/*GPIO Number - Mic Mode*/	
	int gpio_mic_mode;

	/*Button Down State*/
	atomic_t btn_state;
	/*Button Check Skip*/
	int ignore_btn;

	/*Headset Detect*/
	unsigned int irq;
	/*Headset Button Detect*/	
	unsigned int irq_btn;

	/*Headset Detect Timer*/		
	struct hrtimer timer;
	ktime_t debounce_time;
	ktime_t unplug_debounce_time;

	/*Headset Button Detect Timer*/		
	struct hrtimer btn_timer;
	ktime_t btn_down_debounce_time;
	ktime_t btn_up_debounce_time;
	ktime_t btn_down_delay_time;

	/*Wake Lock/Unlock*/		
	struct wake_lock wake_lock;

	/*State*/
	int enable_btn_irq;
	
	/*detect direction*/
	int headset_low_detect;
	int button_low_detect;

	/*adative pole detection*/
	int enable_adaptive_detect;
	int adaptive_detect_state;

	/*button irq register work*/
	struct delayed_work	dwork;
	struct workqueue_struct *button_irq_reg_wq;
	int button_irq_interval;
};
static struct h2w_info *hi;

static int is_headset_attached(unsigned int gpio){
	int detected = !!gpio_get_value(gpio);

	irq_set_irq_type(hi->irq, detected ?IRQF_TRIGGER_LOW : IRQF_TRIGGER_HIGH);
	
	if(hi->headset_low_detect){
		detected = detected?0: 1;
	}
	
	return detected;
}

static int is_button_pressed(unsigned int gpio){
	int detected = !!gpio_get_value(gpio);

	irq_set_irq_type(hi->irq_btn, detected ?IRQF_TRIGGER_LOW : IRQF_TRIGGER_HIGH);
	
	if(hi->button_low_detect){
		detected = detected?0: 1;
	}
	return detected;
}

static void set_mic_mode(unsigned int gpio, int enable ){
	if(gpio)
		gpio_set_value(gpio, enable?1: 0);
}
static void set_button_irq(int enable ){
	unsigned long irq_flags;
		
	if(enable){
		if(hi->enable_btn_irq !=0)
			return;
		H2W_DBG("Enabled");
		local_irq_save(irq_flags);
		enable_irq(hi->irq_btn);
		irq_set_irq_wake(hi->irq_btn, 1);
		hi->enable_btn_irq = 1;
		local_irq_restore(irq_flags);
	}
	else{
		if(hi->enable_btn_irq ==0)
			return;
		H2W_DBG("Disabled");
		local_irq_save(irq_flags);
		disable_irq_nosync(hi->irq_btn);
		irq_set_irq_wake(hi->irq_btn, 0);
		hi->enable_btn_irq = 0;
		local_irq_restore(irq_flags);
	}
}

static void hw2_button_irq(int enable ){
	
#ifdef H2W_BUTTON_IRQ_DELAYED_REGISTER
	__cancel_delayed_work(&hi->dwork);
#endif

	if(enable){
#ifdef H2W_BUTTON_IRQ_DELAYED_REGISTER
		queue_delayed_work(hi->button_irq_reg_wq, &hi->dwork, hi->button_irq_interval);
#else
		set_button_irq(1);
#endif
	}
	else{
		set_button_irq(0);
		
	}

}
static void hw2_button_irq_reg_work(struct work_struct *work)
{
	hi->btn_down_delay_time = ktime_set(0, 500000000); /*500ms */
	set_button_irq(1);
}

static void hw2_wake_lock_timeout(long time){
	if(wake_lock_active(&hi->wake_lock))
		wake_unlock(&hi->wake_lock);
	wake_lock_timeout(&hi->wake_lock, time * HZ);
}
static ssize_t gpio_h2w_print_name(struct switch_dev *sdev, char *buf)
{
	switch (switch_get_state(&hi->sdev)) 
	{
		case BIT_NO_DEVICE:
			return sprintf(buf, "No Device\n");
		case BIT_HEADSET:
		case BIT_HEADSET_SPEAKER_ONLY:		
			return sprintf(buf, "Headset\n");
	}
	return -EINVAL;
}

static void button_pressed(void)
{
	if(atomic_read(&hi->btn_state))
		return;
	
	if(hi->ignore_btn)
		return;

	H2W_DBG(" ");
	
	atomic_set(&hi->btn_state, 1);
	input_report_key(hi->input, KEY_MEDIA, 1);
	input_sync(hi->input);
}

static void button_released(void)
{
	if(!atomic_read(&hi->btn_state))
		return;

	H2W_DBG(" ");
	
	atomic_set(&hi->btn_state, 0);
	input_report_key(hi->input, KEY_MEDIA, 0);
	input_sync(hi->input);
}

static void insert_headset(void)
{
	int four_pole;

	H2W_DBG("");

	/*wait for stabilization*/
	msleep(100);

	four_pole = !is_button_pressed(hi->gpio_button_detect);

	 /* 4pole */
	if(four_pole)
   	{
	   	
		H2W_DBG("4pole");
		hi->ignore_btn =0;
		/*Report Headset State*/
		switch_set_state(&hi->sdev, BIT_HEADSET);
		
		set_mic_mode(hi->gpio_mic_mode, 1);
		
		/* Enable button irq */
		hw2_button_irq(1);
   	}
	else
	/* 3pole*/
	{
		H2W_DBG("3pole");
		hi->ignore_btn = 1;
		/*Report Headset State*/
		switch_set_state(&hi->sdev, BIT_HEADSET_SPEAKER_ONLY);

		/* When user don't insert headset tightly, It can be detected with 3pole
		    So, if button irq is enabled, we can detect correctly again.
		*/
		if(hi->enable_adaptive_detect){
			hw2_button_irq(1);
		}
	}

	hi->debounce_time = ktime_set(0, 20000000);  /* 20 ms */
	hi->unplug_debounce_time = ktime_set(0, 10000000); /*10ms */
}

static void remove_headset(void)
{
	H2W_DBG("");

	/*Report Headset State*/
	input_report_switch(hi->input, SW_HEADPHONE_INSERT, 0);
	switch_set_state(&hi->sdev, BIT_NO_DEVICE);
	input_sync(hi->input);

	/* Disable button */
	hw2_button_irq(0);
	
	/* sleep 1500ms because the detect event is too slower than button event*/
	if(atomic_read(&hi->btn_state))
		msleep(1500);
	
	/*if button pressed state, send release event*/
	button_released();
	
	set_mic_mode(hi->gpio_mic_mode, 0);
	hi->debounce_time = ktime_set(0, 250000000);  /* 250ms */
}

static void detection_work(struct work_struct *work)
{
	//H2W_DBG("");

	if (!is_headset_attached(hi->gpio_detect)) {
		if (switch_get_state(&hi->sdev)!= BIT_NO_DEVICE){
			remove_headset();
		}
	}
	else{
		if (switch_get_state(&hi->sdev)== BIT_NO_DEVICE){
			insert_headset();
		}
		else if(hi->adaptive_detect_state){		
			/*This code is unnecessary, 
			  but we need to report headset disconnect event,
			  because headset icon disappeares when changed from 3pole to 4pole
			 */			
			input_report_switch(hi->input, SW_HEADPHONE_INSERT, 0);
			switch_set_state(&hi->sdev, BIT_NO_DEVICE);
			input_sync(hi->input);
			/*call insert headset*/
			insert_headset();
		}
	}	
	hi->adaptive_detect_state =0;
}

static enum hrtimer_restart button_event_timer_func(struct hrtimer *data)
{
	int headset_in	=0;
	int btn_down	=0;

	//H2W_DBG("");
	if(hi->ignore_btn && !hi->enable_adaptive_detect){
		H2W_DBG("ignore_btn");
		return HRTIMER_NORESTART;
	}
	
	/* "button timer < headset detach timer" makes abnormal operation*/
	headset_in	= is_headset_attached(hi->gpio_detect);
	btn_down	= is_button_pressed(hi->gpio_button_detect); 

	if(!headset_in)
		return HRTIMER_NORESTART;
	
	if(switch_get_state(&hi->sdev) == BIT_HEADSET){
		
		if (btn_down)
			button_pressed();
		else			
			button_released();
	}
	else if(switch_get_state(&hi->sdev) == BIT_HEADSET_SPEAKER_ONLY){
		
		if(hi->enable_adaptive_detect && !btn_down){
			H2W_DBG("correction : 3pole -> 4pole");
			hi->adaptive_detect_state = 1;
			queue_work(g_detection_work_queue, &g_detection_work);
		}
	}	

	return HRTIMER_NORESTART;
}

static enum hrtimer_restart detect_event_timer_func(struct hrtimer *data)
{
	//H2W_DBG("");

	queue_work(g_detection_work_queue, &g_detection_work);
	return HRTIMER_NORESTART;
}

static irqreturn_t detect_irq_handler(int irq, void *dev_id)
{
	int attached_1st, attached_2nd;
	int retry_limit = 10;
	int cancel_btn_timer = 0;
	
	H2W_DBG("");

	attached_1st = is_headset_attached(hi->gpio_detect);
	do {
		attached_2nd = is_headset_attached(hi->gpio_detect);
		
	} while (attached_1st != attached_2nd && retry_limit-- > 0);
	
	if(attached_1st != attached_2nd){
		H2W_DBG("check1 = %d, check2 = %d (%d retries)", attached_1st, attached_2nd, (10-retry_limit));
	}

	hw2_wake_lock_timeout(3);
		
	if (switch_get_state(&hi->sdev)== BIT_NO_DEVICE) {
		hrtimer_start(&hi->timer, hi->debounce_time, HRTIMER_MODE_REL); 
	}	
	else{
		hrtimer_start(&hi->timer, hi->unplug_debounce_time, HRTIMER_MODE_REL);
	}

	/*If cancel_btn_timer exists, it means activate state*/
	cancel_btn_timer = hrtimer_cancel(&hi->btn_timer);
	if(cancel_btn_timer) 
		H2W_DBG("cancel_btn_timer : %d", cancel_btn_timer);

	/*When detached, turn off mode to prevent noise */
	if(!attached_2nd)
		set_mic_mode(hi->gpio_mic_mode, 0);

	return IRQ_HANDLED;
}

static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
	int press_1st, press_2nd;
	int retry_limit = 10;

	H2W_DBG("");

	press_1st = is_button_pressed(hi->gpio_button_detect);
	do {		
		press_2nd = is_button_pressed(hi->gpio_button_detect);
	} while (press_1st != press_2nd && retry_limit-- > 0);

	if(press_1st!=press_2nd){
		H2W_DBG("Ignore Button (%d!=%d), retries(%d ms)",press_1st,press_2nd,(10-retry_limit));
		return IRQ_HANDLED;
	}

	hw2_wake_lock_timeout(3);


	if(ktime_to_ns(hi->btn_down_delay_time)){
		hrtimer_start(&hi->btn_timer, hi->btn_down_delay_time, HRTIMER_MODE_REL);
		hi->btn_down_delay_time = ktime_set(0, 0);
	}
	else if(press_2nd){
		hrtimer_start(&hi->btn_timer, hi->btn_down_debounce_time, HRTIMER_MODE_REL);
	}
	else{
		hrtimer_start(&hi->btn_timer, hi->btn_up_debounce_time, HRTIMER_MODE_REL);
	}

	return IRQ_HANDLED;
}

static int gpio_h2w_probe(struct platform_device *pdev)
{
	int ret;
	struct gpio_h2w_platform_data *pdata = pdev->dev.platform_data;

	H2W_DBG("H2W: Registering H2W (headset) driver\n");
	hi = kzalloc(sizeof(struct h2w_info), GFP_KERNEL);
	if (!hi)
		return -ENOMEM;

	hi->button_irq_reg_wq = create_singlethread_workqueue("button_irq");
	if(!hi->button_irq_reg_wq){
		return -ENOMEM;
	}
	hi->button_irq_interval = (1 * HZ); /*1sec*/
	INIT_DELAYED_WORK_DEFERRABLE(&hi->dwork, hw2_button_irq_reg_work);
	
	atomic_set(&hi->btn_state, 0);
	hi->headset_low_detect = 1;
	hi->button_low_detect = 1;
#ifdef H2W_ADAPTIVE_POLE_DETECT
	hi->enable_adaptive_detect = 1;
#else
	hi->enable_adaptive_detect = 0;
#endif
	hi->adaptive_detect_state = 0;
	
	hi->ignore_btn = 0;
	hi->debounce_time = ktime_set(0, 250000000);   /*250ms */
	hi->unplug_debounce_time = ktime_set(0, 10000000); /* 10ms */

	hi->btn_down_debounce_time	= ktime_set(0, 40000000); /*40ms */
	hi->btn_up_debounce_time	= ktime_set(0, 30000000); /*30ms */
	hi->btn_down_delay_time		= ktime_set(0, 0);
	
	hi->gpio_detect = pdata->gpio_detect;
	hi->gpio_button_detect = pdata->gpio_button_detect;
 	hi->gpio_mic_mode = pdata->gpio_mic_mode;
//	hi->gpio_mic_bias_en = pdata->gpio_mic_bias_en;
	hi->sdev.name = "h2w";
	hi->sdev.print_name = gpio_h2w_print_name;
	wake_lock_init(&hi->wake_lock, WAKE_LOCK_SUSPEND, "h2w_detect_lock");

	ret = switch_dev_register(&hi->sdev);
	if (ret < 0)
		goto err_switch_dev_register;

	g_detection_work_queue = create_workqueue("detection");
	if (g_detection_work_queue == NULL) {
		ret = -ENOMEM;
		goto err_create_work_queue;
	}

	gpio_tlmm_config(GPIO_CFG(hi->gpio_detect, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	ret = gpio_request(hi->gpio_detect, "h2w_detect");
	if (ret < 0)
		goto err_request_detect_gpio;

	gpio_tlmm_config(GPIO_CFG(hi->gpio_button_detect, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	ret = gpio_request(hi->gpio_button_detect, "h2w_button");
	if (ret < 0)
		goto err_request_button_gpio;

	gpio_tlmm_config(GPIO_CFG(hi->gpio_mic_mode, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	ret = gpio_request(hi->gpio_mic_mode, "h2w_mic_mode");
	if (ret < 0)
		goto err_request_mic_mode_gpio;
	
	ret = gpio_direction_input(hi->gpio_detect);
	if (ret < 0)
		goto err_set_detect_gpio;

	ret = gpio_direction_input(hi->gpio_button_detect);
	if (ret < 0)
		goto err_set_button_gpio;

	hi->irq = gpio_to_irq(hi->gpio_detect);
	if (hi->irq < 0) {
		ret = hi->irq;
		goto err_get_h2w_detect_irq_num_failed;
	}

	hi->irq_btn = gpio_to_irq(hi->gpio_button_detect);
	if (hi->irq_btn < 0) {
		ret = hi->irq_btn;
		goto err_get_button_irq_num_failed;
	}

 	set_mic_mode(hi->gpio_mic_mode, 0);
	hrtimer_init(&hi->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hi->timer.function = detect_event_timer_func;
	hrtimer_init(&hi->btn_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hi->btn_timer.function = button_event_timer_func;

	ret = request_irq(hi->irq, detect_irq_handler,
			  IRQF_TRIGGER_LOW, "h2w_detect", NULL); 
	

	if (ret < 0)
		goto err_request_detect_irq;

	/* Disable button until plugged in */
	hi->enable_btn_irq = 0;
	set_irq_flags(hi->irq_btn, IRQF_VALID | IRQF_NOAUTOEN);
	ret = request_irq(hi->irq_btn, button_irq_handler,
			  IRQF_TRIGGER_LOW, "h2w_button", NULL);



	if (ret < 0)
		goto err_request_h2w_headset_button_irq;

	ret = irq_set_irq_wake(hi->irq, 1);
	if (ret < 0)
		goto err_request_input_dev;

	hi->input = input_allocate_device();
	if (!hi->input) {
		ret = -ENOMEM;
		goto err_request_input_dev;
	}

	hi->input->name = "h2w";  // "h2w headset" 
	hi->input->evbit[0] = BIT_MASK(EV_KEY);
	hi->input->keybit[BIT_WORD(KEY_MEDIA)] = BIT_MASK(KEY_MEDIA);

	ret = input_register_device(hi->input);
	if (ret < 0)
		goto err_register_input_dev;
	
	ip_dev_reg = 1;

	/* check the inital state of headset */
	queue_work(g_detection_work_queue, &g_detection_work);

	return 0;

err_register_input_dev:
	input_free_device(hi->input);
err_request_input_dev:
	free_irq(hi->irq_btn, 0);
err_request_h2w_headset_button_irq:
	free_irq(hi->irq, 0);
err_request_detect_irq:
err_get_button_irq_num_failed:
err_get_h2w_detect_irq_num_failed:
err_set_button_gpio:
err_set_detect_gpio:
	gpio_free(hi->gpio_button_detect);
err_request_button_gpio:
	gpio_free(hi->gpio_detect);
err_request_mic_mode_gpio:
 	gpio_free(hi->gpio_mic_mode);
err_request_detect_gpio:
	destroy_workqueue(g_detection_work_queue);
err_create_work_queue:
	switch_dev_unregister(&hi->sdev);
err_switch_dev_register:
	printk(KERN_ERR "H2W: Failed to register driver\n");

	wake_lock_destroy(&hi->wake_lock);
	
	return ret;
}

static int gpio_h2w_remove(struct platform_device *pdev)
{
	H2W_DBG("");
	if (switch_get_state(&hi->sdev))
		remove_headset();
	input_unregister_device(hi->input);
	gpio_free(hi->gpio_button_detect);
	gpio_free(hi->gpio_detect);
	free_irq(hi->irq_btn, 0);
	free_irq(hi->irq, 0);
	destroy_workqueue(g_detection_work_queue);
	switch_dev_unregister(&hi->sdev);
	ip_dev_reg = 0;
	wake_lock_destroy(&hi->wake_lock);
	destroy_workqueue(hi->button_irq_reg_wq);
	
	return 0;
}

static struct platform_driver gpio_h2w_driver = {
	.probe		= gpio_h2w_probe,
	.remove		= gpio_h2w_remove,
	.driver		= {
		.name		= "lge-switch-gpio",
		.owner		= THIS_MODULE,
	},
};

static int __init gpio_h2w_init(void)
{
	H2W_DBG("");
	return platform_driver_register(&gpio_h2w_driver);
}

static void __exit gpio_h2w_exit(void)
{
	platform_driver_unregister(&gpio_h2w_driver);
}

module_init(gpio_h2w_init);
module_exit(gpio_h2w_exit);

MODULE_AUTHOR("LG Electronics");
MODULE_DESCRIPTION("LGE 2 Wire detection driver");
MODULE_LICENSE("GPL");
