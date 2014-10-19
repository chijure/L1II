

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/slab.h>

#include <mach/lge/lge_proc_comm.h>
#include <mach/hardware.h>

#include <linux/reboot.h>

#include <linux/delay.h>		//for rock screen late view problem. solved Device Driver layer parts

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include CONFIG_LGE_BOARD_HEADER_FILE

#define		RT8966SetCurrentDef    4		//25mA overheat issue change 20mA


#define RT8966BL_MAX_INTENSITY		31
#define CONFIG_BACKLIGHT_LEDS_CLASS

#define		LCD_LED_MAX  31
#define		LCD_LED_MIN  0



#ifdef CONFIG_BACKLIGHT_LEDS_CLASS
#include <linux/leds.h>
#define LEDS_BACKLIGHT_NAME "lcd-backlight"
#endif


struct rt8966_backlight {
	struct backlight_device *bd;
	struct led_classdev *led;

	int powermode;
	int current_intensity;
	int max_intensity;
	struct device *dev;
	#ifdef CONFIG_HAS_EARLYSUSPEND
		struct early_suspend early_suspend;
	#endif
	
};

static void inline rt8966bl_send_intensity(int intensity)
{
	//rt8966_writeb(intensity, OMAP_PWL_ENABLE);
	printk("[%s] intensity [%d]\n", __func__, intensity);
	
	if(intensity==0)
	{
		lge_rt8966a_backlight_control(0);
	}
}

extern	int lge_rt8966a_backlight_set_level( int scale, int level );
extern	int lge_rt8966a_backlight_control( int onoff );


extern int StatusBacklightOnOff;


#ifdef CONFIG_PM
#ifdef CONFIG_HAS_EARLYSUSPEND

static void rt8966_early_suspend(struct early_suspend * h)
{
	if(StatusBacklightOnOff==1) {
		printk("[%s] back 8966 backlight off\n", __func__);
		lge_rt8966a_backlight_control(0);
		StatusBacklightOnOff = 0;
	}
	return;
}

int late_resume_value = 0;   // add lcd backlight resume timming

static void rt8966_late_resume(struct early_suspend * h)
{
	int		resume_count = 0;
	
	if(StatusBacklightOnOff==0) {
		printk("[%s] backlight turn ON!!!\n", __func__);
		
		resume_count = 0;
		//lock screen late change problem Solve. Device Driver Layer
		if(late_resume_value == 0){
			while(1) {
				msleep(50);
				if((late_resume_value!=0) || (resume_count >= 40)) {
					printk("zero value is coming, wait the next value %d\n", late_resume_value);
                    late_resume_value = 0;
                    break;
				}
				resume_count++;
			}
		}
		
		lge_rt8966a_backlight_control(1);
		StatusBacklightOnOff =1;
	}
	return;
}
#else

static int rt8966bl_suspend(struct platform_device *pdev, pm_message_t state)
{

//	struct backlight_device *dev = platform_get_drvdata(pdev);
//	struct rt8966_backlight *bl = dev_get_drvdata(&dev->dev);

	//rt8966bl_blank(bl, FB_BLANK_POWERDOWN);

	printk("[%s] \n", __func__);
	return 0;

}

static int rt8966bl_resume(struct platform_device *pdev)
{

	printk("[%s] \n", __func__);

	return 0;
}
#endif

#define rt8966bl_suspend	NULL
#define rt8966bl_resume		NULL

#endif

#if 0

static int rt8966bl_set_power(struct backlight_device *dev, int state)
{

	struct rt8966_backlight *bl = dev_get_drvdata(&dev->dev);

	
	printk("[%s] bl->powermode [%d] \n", __func__, state);
	bl->powermode = state;

	return 0;
}
#endif

static int rt8966bl_update_status(struct backlight_device *dev)
{
#if 0
	struct rt8966_backlight *bl = dev_get_drvdata(&dev->dev);

	if (bl->current_intensity != dev->props.brightness) {
		if (bl->powermode == FB_BLANK_UNBLANK)
			rt8966bl_send_intensity(0);
		bl->current_intensity = dev->props.brightness;
	}

	if (dev->props.fb_blank != bl->powermode)
		rt8966bl_set_power(dev, dev->props.fb_blank);
#endif	
	printk("[%s] \n", __func__);
	return 0;
}





static int CalBrightnessVal(int next)
{
	int output=0;

	printk("[%s] next[%d]!!!\n", __func__, next);
	
	if(next >= 1 && next < 8)
	{
		output = 30;
		return output;
	}
	if(next >= 7 && next < 13)
	{	//30~28
		if(next >= 7 && next < 9)
		{
			output = 30;
		}
		else if(next >= 9 && next < 11)
		{
			output = 29;
		}		
		else if(next >= 11 && next < 13)
		{
			output = 28;
		}
		return output;
	}
	if(next >=13 && next < 19)
	{	//27~22
		if(next >= 13 && next < 15)
		{
			if(next == 13) 
				output = 27;
			if(next == 14) 
				output = 26;

			return output;
		}
		else if(next >= 15 && next < 17)
		{			
			if(next == 15) 
				output = 25;
			if(next == 16) 
				output = 23;

			return output;
			
		}		
		else if(next >= 17 && next < 19)
		{
			if(next == 17) 
				output = 22;
			if(next == 18) 
				output = 21;
			return output;
		}
	}
	if(next >=19 && next < 26)
	{	//21~14
		if(next >= 19 && next < 21)
		{
			if(next == 19) 
				output = 21;
			if(next == 20) 
				output = 19;
			return output;
		}
		else if(next >= 21 && next < 23)
		{			
			if(next == 21) 
				output = 18;
			if(next == 22) 
				output = 17;
			return output;
		}		
		else if(next >= 23 && next < 26)
		{
			if(next == 23) 
				output = 16;
			if(next == 24) 
				output = 14;
			if(next == 25) 
				output = 13;
			return output;
		}
	}
	if(next >= 26 && next < 32)
	{	//13 ~ 1
		if(next >= 26 && next < 28)
		{
			if(next == 26) 
				output = 12;
			if(next == 27) 
				output = 11;
			return output;
		}
		else if(next >= 28 && next < 30)
		{			
			if(next == 28) 
				output = 9;
			if(next == 29) 
				output = 7;
			return output;
		}		
		else if(next >= 30 && next < 32)
		{
			if(next == 30) 
				output = 4;
			if(next == 31) 
				output = 2;
			
			return output;
		}
	}
	printk("[%s] Nochoice Data  Error!!!\n", __func__);
	return 0;
}

int	display_on;
extern int stepSuspend;


static void leds_brightness_set(struct led_classdev *led_cdev, enum led_brightness value)
{
	struct rt8966_backlight *bl =  dev_get_drvdata(led_cdev->dev->parent);
	
	//int brightness;
	int next = 0;	
	int summery = 0;
	
	if (!bl) {
		printk("[%s] Error getting drvier data\n", __func__);
		return;
	}
	printk("[%s] value  [%d]\n", __func__, value);

	late_resume_value = value;

	if((value==0) && (display_on==1))
	{
		//back light off command
		
		printk("[%s] back light off command recived!!!\n",__func__);
		if(StatusBacklightOnOff==1) {
			lge_rt8966a_backlight_control(0);
			StatusBacklightOnOff = 0;
		}
		return;
	}
	if(value<=9) {
		// trans from data minimum value is 10		
		printk("[%s] Skip Set!!!!!\n", __func__);		
		//lcd default white type bad [1] turn 0n
		return;
	}
	
	printk("[%s] stepSuspend : %d !!!!!\n", __func__, stepSuspend);
	
	if( (value!=0) && (StatusBacklightOnOff==0) && (display_on==1) && (stepSuspend!=1) )
	{
		lge_rt8966a_backlight_control(1);
		StatusBacklightOnOff = 1;
	}

	next = ( (value+1)/8 );		//temp process. will chage later

	if(next >= LCD_LED_MAX)
		next = LCD_LED_MAX;
	if(next < 0)
		next =  LCD_LED_MIN;
	
	if(value==10)
	{
			lge_rt8966a_backlight_set_level( RT8966SetCurrentDef, LCD_LED_MAX);		
			printk("[%s] LCD-backlight Set !!!!!\n", __func__);
			return;
			
	}
	else if(value==255){
			lge_rt8966a_backlight_set_level( RT8966SetCurrentDef, LCD_LED_MIN);		
			printk("[%s] LCD-backlight Set !!!!!\n", __func__);
			return;
	}
	
	if(value!=10){
		summery = CalBrightnessVal(next);		
				
		lge_rt8966a_backlight_set_level( RT8966SetCurrentDef, summery);		
		printk("[%s] LCD-backlight Set summery [%d]!!!!!\n", __func__, summery);
	}

}

static struct led_classdev rt8996_led_dev = {
	.name = LEDS_BACKLIGHT_NAME,
	.brightness_set = leds_brightness_set,
};

static int rt8966bl_get_intensity(struct backlight_device *dev)
{
	struct rt8966_backlight *bl = dev_get_drvdata(&dev->dev);
	
	printk("[%s] current_intensity [%d]\n", __func__, bl->current_intensity);
	return bl->current_intensity;
}

static const struct backlight_ops rt8966bl_ops = {
	.get_brightness = rt8966bl_get_intensity,
	.update_status  = rt8966bl_update_status,
};


static int rt8966bl_probe(struct platform_device *pdev)
{
	struct backlight_properties props;
	struct lge_backlight_platform_data *pdata = pdev->dev.platform_data;
	struct backlight_device *dev;
	struct rt8966_backlight *bl;
	
	printk("[%s] \n", __func__);

	printk(KERN_INFO "rt8966 version : %d \n", pdata->version);
	
#if 1
	

	if (!pdata)
		return -ENXIO;

	bl = devm_kzalloc(&pdev->dev, sizeof(struct rt8966_backlight),
			  GFP_KERNEL);
	if (unlikely(!bl))
		return -ENOMEM;

	memset(&props, 0, sizeof(struct backlight_properties));
	props.type = BACKLIGHT_RAW;
	props.max_brightness = RT8966BL_MAX_INTENSITY;
	dev = backlight_device_register("rt8966-bl", &pdev->dev, bl, &rt8966bl_ops,
					&props);
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	bl->powermode = FB_BLANK_POWERDOWN;
	bl->current_intensity = 0;

	bl->dev = &pdev->dev;

	platform_set_drvdata(pdev, dev);
	
	if (led_classdev_register(&pdev->dev, &rt8996_led_dev) == 0) {
			printk("Registering led class dev successfully.\n");
	}

	dev->props.fb_blank = FB_BLANK_UNBLANK;
	dev->props.brightness = pdata->initialized;
	rt8966bl_update_status(dev);
	
#ifdef CONFIG_HAS_EARLYSUSPEND
		bl->early_suspend.suspend = rt8966_early_suspend;
		bl->early_suspend.resume = rt8966_late_resume;
		bl->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN-40;
		register_early_suspend(&bl->early_suspend);
#endif

#endif
	printk(KERN_INFO "rt8966 LCD backlight initialised\n");

	return 0;
}

static int rt8966bl_remove(struct platform_device *pdev)
{
	struct backlight_device *dev = platform_get_drvdata(pdev);

	backlight_device_unregister(dev);
	printk("[%s] \n", __func__);
	return 0;
}

static struct platform_driver rt8966bl_driver = {
	.probe		= rt8966bl_probe,
	.remove		= rt8966bl_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= rt8966bl_suspend,
	.resume		= rt8966bl_resume,
#endif	
	.driver		= {
		.name	= "rt8966-bl",
		.owner = THIS_MODULE,
	},
};

static int __init rt8966_init(void)
{
	printk("rt8966 init start\n");
 	return platform_driver_register(&rt8966bl_driver);
}
static void __exit rt8966_exit(void)
{
	platform_driver_unregister(&rt8966bl_driver);
}


module_init(rt8966_init);
module_exit(rt8966_exit);

MODULE_AUTHOR("...");
MODULE_DESCRIPTION("LGE rt8966 LCD Backlight driver");
MODULE_LICENSE("GPL");
