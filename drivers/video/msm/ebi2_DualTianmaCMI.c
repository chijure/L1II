
#include "msm_fb.h"

#include <linux/memory.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include "linux/proc_fs.h"

#include <linux/delay.h>

#include <mach/hardware.h>
#include <linux/io.h>

#include <asm/system.h>
#include <asm/mach-types.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <mach/vreg.h>
#include CONFIG_LGE_BOARD_HEADER_FILE
#include <linux/pm_qos_params.h>

#include <mach/lge/lge_proc_comm.h>	

#define QVGA_WIDTH        240
#define QVGA_HEIGHT       320

#define  GPIO_LCD_TID      126

static void *DISP_CMD_PORT;
static void *DISP_DATA_PORT;

#define EBI2_WRITE16C(x, y) outpw(x, y)
#define EBI2_WRITE16D(x, y) outpw(x, y)
#define EBI2_READ16(x) inpw(x)

static boolean disp_initialized = FALSE;
struct msm_fb_panel_data tovis_qvga_panel_data;
struct msm_panel_ilitek_pdata *tovis_qvga_panel_pdata;
struct pm_qos_request_list *tovis_pm_qos_req;

extern int display_on; 

#define LCD_INIT_SKIP_FOR_BOOT_TIME
#ifdef LCD_INIT_SKIP_FOR_BOOT_TIME
int lcd_init_skip_cnt = 0;
int	lcd_off_skip_cnt =0;
#endif

#define LCD_RESET_SKIP 1
int IsFirstDisplayOn = LCD_RESET_SKIP; 


#define DISP_SET_RECT(csp, cep, psp, pep) \
	{ \
		EBI2_WRITE16C(DISP_CMD_PORT, 0x2a);			\
		EBI2_WRITE16D(DISP_DATA_PORT,(csp)>>8);		\
		EBI2_WRITE16D(DISP_DATA_PORT,(csp)&0xFF);	\
		EBI2_WRITE16D(DISP_DATA_PORT,(cep)>>8);		\
		EBI2_WRITE16D(DISP_DATA_PORT,(cep)&0xFF);	\
		EBI2_WRITE16C(DISP_CMD_PORT, 0x2b);			\
		EBI2_WRITE16D(DISP_DATA_PORT,(psp)>>8);		\
		EBI2_WRITE16D(DISP_DATA_PORT,(psp)&0xFF);	\
		EBI2_WRITE16D(DISP_DATA_PORT,(pep)>>8);		\
		EBI2_WRITE16D(DISP_DATA_PORT,(pep)&0xFF);	\
	}


#ifdef TUNING_INITCODE
module_param(te_lines, uint, 0644);
module_param(mactl, uint, 0644);
#endif

static void tovis_qvga_disp_init(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;

	if (disp_initialized)
		return;

	mfd = platform_get_drvdata(pdev);

	DISP_CMD_PORT = mfd->cmd_port;
	DISP_DATA_PORT = mfd->data_port;

	disp_initialized = TRUE;
}

static void msm_fb_ebi2_power_save(int on)
{
	struct msm_panel_ilitek_pdata *pdata = tovis_qvga_panel_pdata;	                                    

	if(pdata && pdata->lcd_power_save)
		pdata->lcd_power_save(on);
}

int StatusBacklightOnOff = 1;
extern	int	stepSuspend;


static int ilitek_qvga_disp_off(struct platform_device *pdev)
{
	//do not control when suspend after just working chargerlogo
	 if( lcd_off_skip_cnt < 1) {
      lcd_off_skip_cnt =1;
      printk("%s: display off...Skip!!!!!! and back light off charger logo mode\n", __func__);

	}
	 else 
	 {

	if (!disp_initialized)
		tovis_qvga_disp_init(pdev);

#ifndef CONFIG_ARCH_MSM7X27A
	pm_qos_update_request(tovis_pm_qos_req, PM_QOS_DEFAULT_VALUE);
#endif

	EBI2_WRITE16C(DISP_CMD_PORT, 0x28);
				msleep(50);
	EBI2_WRITE16C(DISP_CMD_PORT, 0x10); // SPLIN
				msleep(80);

	msm_fb_ebi2_power_save(0);
	display_on = FALSE;
	}
	return 0;
}

static void ilitek_qvga_disp_set_rect(int x, int y, int xres, int yres) // xres = width, yres - height
{
	if (!disp_initialized)
		return;

	DISP_SET_RECT(x, x+xres-1, y, y+yres-1);
	EBI2_WRITE16C(DISP_CMD_PORT,0x2c); // Write memory start
}

static void panel_CMIDisplay_init(void)
{

    EBI2_WRITE16C(DISP_CMD_PORT,0x3A);
                EBI2_WRITE16D(DISP_DATA_PORT,0x55);

	EBI2_WRITE16C(DISP_CMD_PORT, 0xCF); //EXTC option
	EBI2_WRITE16D(DISP_DATA_PORT, 0x00);
	EBI2_WRITE16D(DISP_DATA_PORT, 0xA1);
	EBI2_WRITE16C(DISP_CMD_PORT, 0xB1); //
	EBI2_WRITE16D(DISP_DATA_PORT, 0x00);
	EBI2_WRITE16D(DISP_DATA_PORT, 0x1E); //frame rate control
	EBI2_WRITE16C(DISP_CMD_PORT, 0XB4); //inversion control
	EBI2_WRITE16D(DISP_DATA_PORT, 0x02); //2 dots inversion

	EBI2_WRITE16C(DISP_CMD_PORT, 0X36); 
	EBI2_WRITE16D(DISP_DATA_PORT, 0x00); 


	EBI2_WRITE16C(DISP_CMD_PORT, 0XB6);
	EBI2_WRITE16D(DISP_DATA_PORT, 0x02);
	
	EBI2_WRITE16C(DISP_CMD_PORT, 0XC0); //power control
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0F); //
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0D); //
	EBI2_WRITE16C(DISP_CMD_PORT, 0xC1); //power control
	EBI2_WRITE16D(DISP_DATA_PORT, 0x01); //VGH =6x VCI VGL= -5xVCI
	EBI2_WRITE16C(DISP_CMD_PORT, 0xC2); //power control
	EBI2_WRITE16D(DISP_DATA_PORT, 0x33);
	EBI2_WRITE16C(DISP_CMD_PORT, 0xC5); // VCOMDC=-1
	EBI2_WRITE16D(DISP_DATA_PORT, 0xEB);

	/* Blanking Porch control*/
	EBI2_WRITE16C(DISP_CMD_PORT, 0xb5);
	EBI2_WRITE16D(DISP_DATA_PORT,0x02);
	EBI2_WRITE16D(DISP_DATA_PORT,0x02);
	EBI2_WRITE16D(DISP_DATA_PORT,0x0a);
	EBI2_WRITE16D(DISP_DATA_PORT,0x14);

	/* Tearing Effect Line On */
	EBI2_WRITE16C(DISP_CMD_PORT, 0x35);
	EBI2_WRITE16D(DISP_DATA_PORT,0x00);

	/* Tearing effect Control Parameter */
	EBI2_WRITE16C(DISP_CMD_PORT, 0x44);
	EBI2_WRITE16D(DISP_DATA_PORT,0x00);
	EBI2_WRITE16D(DISP_DATA_PORT,0xef);

	EBI2_WRITE16C(DISP_CMD_PORT, 0xE0); //Set Gamma
	EBI2_WRITE16D(DISP_DATA_PORT, 0x05); //0
	EBI2_WRITE16D(DISP_DATA_PORT, 0x08); //1
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0C); //2
	EBI2_WRITE16D(DISP_DATA_PORT, 0x04); //4
	EBI2_WRITE16D(DISP_DATA_PORT, 0x12); //6
	EBI2_WRITE16D(DISP_DATA_PORT, 0x07); //13 
	EBI2_WRITE16D(DISP_DATA_PORT, 0x32); //20
	EBI2_WRITE16D(DISP_DATA_PORT, 0x97); //36 
	EBI2_WRITE16D(DISP_DATA_PORT, 0x3F); //43  
	EBI2_WRITE16D(DISP_DATA_PORT, 0x08); //50 
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0E); //57
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0A); //59  
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0F); //61
	EBI2_WRITE16D(DISP_DATA_PORT, 0x13); //62
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0F); //63
	
	EBI2_WRITE16C(DISP_CMD_PORT, 0XE1); //Set Gamma
	EBI2_WRITE16D(DISP_DATA_PORT, 0x06); //63
	EBI2_WRITE16D(DISP_DATA_PORT, 0x22); //62
	EBI2_WRITE16D(DISP_DATA_PORT, 0x24); //61
	EBI2_WRITE16D(DISP_DATA_PORT, 0x03); //59
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0F); //57
	EBI2_WRITE16D(DISP_DATA_PORT, 0x05); //50
	EBI2_WRITE16D(DISP_DATA_PORT, 0x3A); //43 
	EBI2_WRITE16D(DISP_DATA_PORT, 0x36); //27 
	EBI2_WRITE16D(DISP_DATA_PORT, 0x4C); //20
	EBI2_WRITE16D(DISP_DATA_PORT, 0x03); //13
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0D); //6
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0B); //4
	EBI2_WRITE16D(DISP_DATA_PORT, 0x33); //2
	EBI2_WRITE16D(DISP_DATA_PORT, 0x36); //1
	EBI2_WRITE16D(DISP_DATA_PORT, 0x0F); //0
	
	EBI2_WRITE16C(DISP_CMD_PORT, 0x11); //Exit Sleep
	msleep(65);
	

}

static void panel_tianmaDisplay_init(void)
{
	int x, y;
	
	 EBI2_WRITE16C(DISP_CMD_PORT, 0xCB);
                EBI2_WRITE16D(DISP_DATA_PORT, 0x39);
                EBI2_WRITE16D(DISP_DATA_PORT, 0x2C);
                EBI2_WRITE16D(DISP_DATA_PORT, 0x00);
                EBI2_WRITE16D(DISP_DATA_PORT, 0x34);   //Vcore 1.6V
                EBI2_WRITE16D(DISP_DATA_PORT, 0x02);   //AVDD 5.8V

	EBI2_WRITE16C(DISP_CMD_PORT,0xCF);
                EBI2_WRITE16D(DISP_DATA_PORT,0x00);
                EBI2_WRITE16D(DISP_DATA_PORT,0xf9);//83
                EBI2_WRITE16D(DISP_DATA_PORT,0X30);


    EBI2_WRITE16C(DISP_CMD_PORT,0xE8);
                EBI2_WRITE16D(DISP_DATA_PORT,0x85); 
                EBI2_WRITE16D(DISP_DATA_PORT,0x10); 	   //00
                EBI2_WRITE16D(DISP_DATA_PORT,0x78); 

    EBI2_WRITE16C(DISP_CMD_PORT,0xEA);
                EBI2_WRITE16D(DISP_DATA_PORT,0x00);
                EBI2_WRITE16D(DISP_DATA_PORT,0x00);

    EBI2_WRITE16C(DISP_CMD_PORT,0xED);
                EBI2_WRITE16D(DISP_DATA_PORT,0x64);
                EBI2_WRITE16D(DISP_DATA_PORT,0x03);
                EBI2_WRITE16D(DISP_DATA_PORT,0X12);
                EBI2_WRITE16D(DISP_DATA_PORT,0x81);

    EBI2_WRITE16C(DISP_CMD_PORT,0xF2);
                EBI2_WRITE16D(DISP_DATA_PORT,0x00);

    EBI2_WRITE16C(DISP_CMD_PORT,0xF7);
                EBI2_WRITE16D(DISP_DATA_PORT,0x20);

    EBI2_WRITE16C(DISP_CMD_PORT,0xC0);
                EBI2_WRITE16D(DISP_DATA_PORT,0x1B);	//	GVDD 4.20V

    EBI2_WRITE16C(DISP_CMD_PORT,0xC1);
                EBI2_WRITE16D(DISP_DATA_PORT,0x10);	//VGH X7   VGL X -4	   AVDD X2

    EBI2_WRITE16C(DISP_CMD_PORT,0xC5);
                EBI2_WRITE16D(DISP_DATA_PORT,0x3f);	  //VCOMH 4.275V
                EBI2_WRITE16D(DISP_DATA_PORT,0x48);	  //VCOML -0.7V

    EBI2_WRITE16C(DISP_CMD_PORT,0xC7);
                EBI2_WRITE16D(DISP_DATA_PORT,0xb7);	 //8F	//VCOM OFFSET	

    EBI2_WRITE16C(DISP_CMD_PORT,0x36);
                EBI2_WRITE16D(DISP_DATA_PORT,0x00);   //80 -> 00 бзб■?воиби╧во 	RGB

    EBI2_WRITE16C(DISP_CMD_PORT,0x3A);
                EBI2_WRITE16D(DISP_DATA_PORT,0x55);

    EBI2_WRITE16C(DISP_CMD_PORT,0xb6);
                EBI2_WRITE16D(DISP_DATA_PORT,0x0a);
				EBI2_WRITE16D(DISP_DATA_PORT,0xa2);

	EBI2_WRITE16C(DISP_CMD_PORT,0xB1);                   
                EBI2_WRITE16D(DISP_DATA_PORT,0x00);
				EBI2_WRITE16D(DISP_DATA_PORT,0x10);

				/* Blanking Porch control*/
				EBI2_WRITE16C(DISP_CMD_PORT, 0xB5);
				EBI2_WRITE16D(DISP_DATA_PORT, 0x5c);
				EBI2_WRITE16D(DISP_DATA_PORT, 0x63);
				EBI2_WRITE16D(DISP_DATA_PORT, 0x0A);  
				EBI2_WRITE16D(DISP_DATA_PORT, 0x14);  
				
				/* Tearing Effect Line On */
				EBI2_WRITE16C(DISP_CMD_PORT, 0x35);
				EBI2_WRITE16D(DISP_DATA_PORT,0x00);
				
				/* Tearing effect Control Parameter */
				EBI2_WRITE16C(DISP_CMD_PORT, 0x44);
				EBI2_WRITE16D(DISP_DATA_PORT,0x00);
				EBI2_WRITE16D(DISP_DATA_PORT,0xef);

    EBI2_WRITE16C(DISP_CMD_PORT,0xE0);
                EBI2_WRITE16D(DISP_DATA_PORT, 0x0F);//0F  63
                EBI2_WRITE16D(DISP_DATA_PORT, 0x1D);//1C  62
                EBI2_WRITE16D(DISP_DATA_PORT, 0x1E);//1D  61
                EBI2_WRITE16D(DISP_DATA_PORT, 0x0F);//0E  59
                EBI2_WRITE16D(DISP_DATA_PORT, 0x11);//10  57
                EBI2_WRITE16D(DISP_DATA_PORT, 0x0A);//0A  50
                EBI2_WRITE16D(DISP_DATA_PORT, 0x51);//4D  43
                EBI2_WRITE16D(DISP_DATA_PORT, 0xB6);//B6  27.36
                EBI2_WRITE16D(DISP_DATA_PORT, 0x3C);//38  20
                EBI2_WRITE16D(DISP_DATA_PORT, 0x08);//08  13
                EBI2_WRITE16D(DISP_DATA_PORT, 0x13);//13  06
                EBI2_WRITE16D(DISP_DATA_PORT, 0x02);//02  04
                EBI2_WRITE16D(DISP_DATA_PORT, 0x06);//05  02
                EBI2_WRITE16D(DISP_DATA_PORT, 0x04);//03  01
                EBI2_WRITE16D(DISP_DATA_PORT, 0x00);//00  00

    EBI2_WRITE16C(DISP_CMD_PORT,0xE1);
       		EBI2_WRITE16D(DISP_DATA_PORT, 0x00);//00  63
                EBI2_WRITE16D(DISP_DATA_PORT, 0x1F);//1F  62
                EBI2_WRITE16D(DISP_DATA_PORT, 0x1E);//1E  61
                EBI2_WRITE16D(DISP_DATA_PORT, 0x03);//03  59
                EBI2_WRITE16D(DISP_DATA_PORT, 0x0D);//0D  57
                EBI2_WRITE16D(DISP_DATA_PORT, 0x02);//02  50
                EBI2_WRITE16D(DISP_DATA_PORT, 0x34);//34  43
                EBI2_WRITE16D(DISP_DATA_PORT, 0x44);//44  27.36
                EBI2_WRITE16D(DISP_DATA_PORT, 0x48);//48  20
                EBI2_WRITE16D(DISP_DATA_PORT, 0x03);//03  13
                EBI2_WRITE16D(DISP_DATA_PORT, 0x0B);//0B  06
                EBI2_WRITE16D(DISP_DATA_PORT, 0x0D);//0D  04
                EBI2_WRITE16D(DISP_DATA_PORT, 0x39);//39  02
                EBI2_WRITE16D(DISP_DATA_PORT, 0x3A);//3A  01
                EBI2_WRITE16D(DISP_DATA_PORT, 0x0F);//0F  00

    EBI2_WRITE16C(DISP_CMD_PORT,0x11);
	
	msleep(65);

	EBI2_WRITE16C(DISP_CMD_PORT, 0x2A);
					   EBI2_WRITE16D(DISP_DATA_PORT,0x00);
					   EBI2_WRITE16D(DISP_DATA_PORT,0x00);
					   EBI2_WRITE16D(DISP_DATA_PORT,0x00);
					   EBI2_WRITE16D(DISP_DATA_PORT,0xEF);

	EBI2_WRITE16C(DISP_CMD_PORT, 0x2B);
					   EBI2_WRITE16D(DISP_DATA_PORT,0x00);
					   EBI2_WRITE16D(DISP_DATA_PORT,0x00);
					   EBI2_WRITE16D(DISP_DATA_PORT,0x01);
					   EBI2_WRITE16D(DISP_DATA_PORT,0x3F);
			   
	 EBI2_WRITE16C(DISP_CMD_PORT,0x2c); // Write memory start

	for(y = 0; y < QVGA_HEIGHT; y++) {
		 for(x = 0; x < QVGA_WIDTH; x++) {
			 EBI2_WRITE16D(DISP_DATA_PORT, 0);
		 }
}
	EBI2_WRITE16C(DISP_CMD_PORT,0x29); // Display On
 	msleep(30);

}

static int ilitek_qvga_disp_on(struct platform_device *pdev)
{

	struct msm_panel_ilitek_pdata *pdata = tovis_qvga_panel_pdata;

	int readport;
	int x, y;
	
	printk("%s: display on... , %d\n", __func__, display_on);

	if(display_on==TRUE)
	{
		return 0;
	}
		
	if (!disp_initialized)
		tovis_qvga_disp_init(pdev);

#ifdef LCD_INIT_SKIP_FOR_BOOT_TIME
   if((pdata->initialized && system_state == SYSTEM_BOOTING) || lcd_init_skip_cnt < 1) {
   
      lcd_init_skip_cnt =1;
      printk("%s: display on...Skip!!!!!! and back light off charger logo mode\n", __func__);
	  
#else
	if(pdata->initialized && system_state == SYSTEM_BOOTING) {
		/* Do not hw initialize */      
#endif
	}
	else 
	{

		msm_fb_ebi2_power_save(1);

		/* use pdata->maker_id to detect panel */		
		
		gpio_tlmm_config(GPIO_CFG(GPIO_LCD_TID, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		readport = gpio_get_value(GPIO_LCD_TID);

		if(readport==0)
		{
			if(pdata->gpio) {
				//mdelay(10);	// prevent stop to listen to music with BT
				gpio_set_value(pdata->gpio, 1);
				mdelay(1);
				gpio_set_value(pdata->gpio, 0);
				mdelay(10);
				gpio_set_value(pdata->gpio, 1);
				msleep(120);
			}
			
			panel_tianmaDisplay_init();
			printk("----------tianma Enabled-----------\n");

		}
		else 
		{
			if(pdata->gpio) {
				//mdelay(10);	// prevent stop to listen to music with BT
				gpio_set_value(pdata->gpio, 1);
				mdelay(1);
				gpio_set_value(pdata->gpio, 0);
				mdelay(10);
				gpio_set_value(pdata->gpio, 1);
				msleep(120);
			}
			panel_CMIDisplay_init();
			printk("----------CMI Enabled--------------\n");
   	    for(y = 0; y < QVGA_HEIGHT; y++) {
			 for(x = 0; x < QVGA_WIDTH; x++) {
				 EBI2_WRITE16D(DISP_DATA_PORT, 0);
			 }
		}

		EBI2_WRITE16C(DISP_CMD_PORT,  0XF6); 
		EBI2_WRITE16D(DISP_DATA_PORT, 0x41); 

	 	EBI2_WRITE16C(DISP_CMD_PORT,0x29); // Display On
	 		msleep(30);
		}
	}
	stepSuspend = 0;

	pm_qos_update_request(tovis_pm_qos_req, 65000);
	display_on = TRUE;
	  
	return 0;
		
}
   
ssize_t tovis_qvga_show_onoff(struct device *dev, struct device_attribute *attr, char *buf)
{
   return snprintf(buf, PAGE_SIZE, "%d\n", display_on);
}

ssize_t tovis_qvga_store_onoff(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
   int onoff;
   struct msm_fb_panel_data *pdata = dev_get_platdata(dev);
   struct platform_device *pd = to_platform_device(dev);

   sscanf(buf, "%d", &onoff);

   if (onoff) {
	   pdata->on(pd);
   } else {
	   pdata->off(pd);
   }

   return count;
}


DEVICE_ATTR(lcd_onoff, 0664, tovis_qvga_show_onoff, tovis_qvga_store_onoff);

static int __init tovis_qvga_probe(struct platform_device *pdev)
{
	int ret;

	if (pdev->id == 0) {
		tovis_qvga_panel_pdata = pdev->dev.platform_data;
		return 0;
	}

	msm_fb_add_device(pdev);

	ret = device_create_file(&pdev->dev, &dev_attr_lcd_onoff);
	if (ret) {
		printk("tovis_qvga_probe device_creat_file failed!!!\n");
	}

#ifndef CONFIG_ARCH_MSM7X27A
	tovis_pm_qos_req = pm_qos_add_request(PM_QOS_SYSTEM_BUS_FREQ, PM_QOS_DEFAULT_VALUE);
#endif
	return 0;
}

struct msm_fb_panel_data tovis_qvga_panel_data = {
	.on = ilitek_qvga_disp_on,
	.off = ilitek_qvga_disp_off,
	.set_backlight = NULL,
	.set_rect = ilitek_qvga_disp_set_rect,
};

static struct platform_device this_device = {
	.name   = "ebi2_tovis_qvga",
	.id	= 1,
	.dev	= {
		.platform_data = &tovis_qvga_panel_data,
	}
};


static struct platform_driver __refdata this_driver = {
	.probe  = tovis_qvga_probe,
	.driver = {
		.name   = "ebi2_tovis_qvga",
	},
};

static int __init tovis_qvga_init(void)
{
	int ret, readport;
	struct msm_panel_info *pinfo;
	
	ret = platform_driver_register(&this_driver);
	 printk("%s: InitCode excute!!ret [%d]\n", __func__, ret);
	if (!ret) {
		pinfo = &tovis_qvga_panel_data.panel_info;
		pinfo->xres = QVGA_WIDTH;
		pinfo->yres = QVGA_HEIGHT;
		pinfo->type = EBI2_PANEL;
		pinfo->pdest = DISPLAY_1;

		pinfo->bpp = 16;
#ifdef CONFIG_FB_MSM_TRIPLE_BUFFER
		pinfo->fb_num = 3;
#else
		pinfo->fb_num = 2;
#endif
		pinfo->lcd.vsync_enable = TRUE;

	    ///for CTS test pass             dualitic support      2013-4-17      
		gpio_tlmm_config(GPIO_CFG(GPIO_LCD_TID, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		readport = gpio_get_value(GPIO_LCD_TID);

		if(readport==0) {
			//vsync 119hz
			//Tianma 
			pinfo->wait_cycle = 0x218000;
			
			pinfo->lcd.refx100 = 8300;		//CTS     framerate      about 81~85
			pinfo->lcd.v_back_porch = 150/2;
			pinfo->lcd.v_front_porch = 140/2;
			pinfo->lcd.v_pulse_width = 40/2;
		}
		else {
                        //CMI
			pinfo->wait_cycle = 0x428000;
			pinfo->lcd.refx100 = 6200;		//CTS     framerate      about 60~64

	 	        pinfo->lcd.v_back_porch = 150;
		        pinfo->lcd.v_front_porch = 140;
		        pinfo->lcd.v_pulse_width = 40;
		}

/*                                                                                      */
/*                                                                                    */

		pinfo->lcd.hw_vsync_mode = TRUE;
		pinfo->lcd.vsync_notifier_period = 0;

		ret = platform_device_register(&this_device);
		if (ret)
			platform_driver_unregister(&this_driver);
	}

	return ret;
}


module_init(tovis_qvga_init);



