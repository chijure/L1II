#ifndef T4K28_REG_H
#define T4K28_REG_H

#include "msm_sensor.h"


static struct msm_camera_i2c_reg_conf t4k28_no_active_settings[] = {
	{-1,-1,-1,-1,-1}
};

static struct msm_camera_i2c_reg_conf t4k28_start_settings[] = {
/*                                                                */
	{ 0x3010, 0x01},
/*                                                                */
         {0x3011,0x03}
};

static struct msm_camera_i2c_reg_conf t4k28_stop_settings[] = {
	{ 0x3010, 0x00},

};
static struct msm_camera_i2c_reg_conf t4k28_prev_settings[] = {
/*                                                                */
{0x3012,0x02,MSM_CAMERA_I2C_BYTE_DATA},
{0x3015,0x05,MSM_CAMERA_I2C_BYTE_DATA},
{0x3016,0x16,MSM_CAMERA_I2C_BYTE_DATA},
{0x3017,0x01,MSM_CAMERA_I2C_BYTE_DATA},
{0x3018,0x75,MSM_CAMERA_I2C_BYTE_DATA},
{0x3019,0x00,MSM_CAMERA_I2C_BYTE_DATA},
{0x301A,0x20,MSM_CAMERA_I2C_BYTE_DATA},
{0x301B,0x10,MSM_CAMERA_I2C_BYTE_DATA},
{0x301C,0x01,MSM_CAMERA_I2C_BYTE_DATA},
{0x3020,0x03,MSM_CAMERA_I2C_BYTE_DATA},
{0x3021,0x20,MSM_CAMERA_I2C_BYTE_DATA},
{0x3022,0x02,MSM_CAMERA_I2C_BYTE_DATA},
{0x3023,0x58,MSM_CAMERA_I2C_BYTE_DATA},
{0x334E,0x01,MSM_CAMERA_I2C_BYTE_DATA},
{0x334F,0x30,MSM_CAMERA_I2C_BYTE_DATA},
{0x351B,0xA8,MSM_CAMERA_I2C_BYTE_DATA},
{0x3012,0x02,MSM_CAMERA_I2C_BYTE_DATA}, 
{0x3548,0x84,MSM_CAMERA_I2C_BYTE_DATA}, 
{0x3500,0xc5,MSM_CAMERA_I2C_BYTE_DATA},
{0x3517,0x01,MSM_CAMERA_I2C_BYTE_DATA},

/*                                                                */

};

static struct msm_camera_i2c_reg_conf t4k28_snap_settings[] = {
/*                                                                */
        {0x3548,0x80,MSM_CAMERA_I2C_BYTE_DATA},
	{0x3500,0xE5, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3517,0x0A, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3012,0x03, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3015,0x05, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3016,0x16, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3017,0x04, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3018,0x5c, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3019,0x00, MSM_CAMERA_I2C_BYTE_DATA},
	{0x301A,0x10, MSM_CAMERA_I2C_BYTE_DATA},
	{0x301B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
	{0x301C,0x01, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3020,0x06, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3021,0x40, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3022,0x04, MSM_CAMERA_I2C_BYTE_DATA},
	{0x3023,0xB0, MSM_CAMERA_I2C_BYTE_DATA},
	{0x334E,0x00, MSM_CAMERA_I2C_BYTE_DATA},
	{0x334F,0x98, MSM_CAMERA_I2C_BYTE_DATA},
	{0x351B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
   {0x3012,0x02, MSM_CAMERA_I2C_BYTE_DATA}, 

/*                                                                */


};


//DONE


static struct msm_camera_i2c_reg_conf t4k28_exposure[][3] = {
// need to check
	{{0x3500,0xC5}, {0x3501,0x00}, {0x3502,0x14},}, /*EXPOSURECOMPENSATIONN6*/
	{{0x3500,0xC5}, {0x3501,0x00}, {0x3502,0x32},}, /*EXPOSURECOMPENSATIONN5*/
	{{0x3500,0xC5}, {0x3501,0x00}, {0x3502,0x50},}, /*EXPOSURECOMPENSATIONN4*/
	{{0x3500,0xC5}, {0x3501,0x00}, {0x3502,0x6e},}, /*EXPOSURECOMPENSATIONN3*/
	{{0x3500,0xC5}, {0x3501,0x00}, {0x3502,0x8c},}, /*EXPOSURECOMPENSATIONN2*/
	{{0x3500,0xC5}, {0x3501,0x00}, {0x3502,0xaa},}, /*EXPOSURECOMPENSATIONN1*/
	{{0x3500,0xC5}, {0x3501,0x00}, {0x3502,0xc8},}, /*EXPOSURECOMPENSATIOND*/
	{{0x3500,0xC5}, {0x3501,0x00}, {0x3502,0xe6},}, /*EXPOSURECOMPENSATIONp1*/
	{{0x3500,0xC5}, {0x3501,0x01}, {0x3502,0x04},}, /*EXPOSURECOMPENSATIONp2*/
	{{0x3500,0xC5}, {0x3501,0x01}, {0x3502,0x22},}, /*EXPOSURECOMPENSATIONp3*/
	{{0x3500,0xC5}, {0x3501,0x01}, {0x3502,0x40},}, /*EXPOSURECOMPENSATIONp4*/
	{{0x3500,0xC5}, {0x3501,0x01}, {0x3502,0x5e},}, /*EXPOSURECOMPENSATIONp5*/
	{{0x3500,0xC5}, {0x3501,0x01}, {0x3502,0x7c},}, /*EXPOSURECOMPENSATIONp6*/

};
//DONE


/*
static struct msm_camera_i2c_reg_conf t4k28_no_effect[] = {
	//Filter OFF
	{0x03, 0x10},
	{0x12, 0x10}, //constant OFF
	{0x44, 0x80}, //cb_constant
	{0x45, 0x80}, //cr_constant
};
*/

static struct msm_camera_i2c_reg_conf t4k28_special_effect[][1] = {
// need to check

	{
		//Filter OFF
		{0x3402, 0x00},


	},	/*for special effect OFF*/
	{
		// Mono
		{0x3402, 0x06},

	},	/*for special effect MONO*/
	{
		//Negative
		{0x3402, 0x03},
		 //cr_constant
	},	/*for special efefct Negative*/
	{
		//Sepia
		{0x3402, 0x05},

	},/*for sepia*/
	{
		{-1, -1,-1},

	},		/*MAX value*/
 
};

static struct msm_camera_i2c_reg_conf t4k28_wb_oem[][8] = {
// need to check

	{
	   {0x3322,0x00},
       {0x3323,0x00},
       {0x3324,0x00},
       {0x3325,0x38},
	   {0x352A,0x20},
       {0x352B,0x20},
       {0x352C,0x20},
       {0x352D,0x20},



	}, /*WHITEBALNACE AUTO*/
	{
	        {0x3322,0x1C},
		{0x3323,0x1C},
		{0x3324,0x00},
		{0x3325,0x70},
		{0x352A,0x05},
		{0x352B,0x20},
		{0x352C,0x10},
		{0x352D,0x05},


	},	/*INCANDISCENT*/
	{
		{0x3322,0x12},
		{0x3323,0x12},
		{0x3324,0x00},
		{0x3325,0x4F},
		{0x352A,0x10},
		{0x352B,0x05},
		{0x352C,0x0C},
		{0x352D,0x20},

	},	/*FLOURESECT */
	{
		{0x3322,0x00},
		{0x3323,0x00},
		{0x3324,0x00},
		{0x3325,0x20},
		{0x352A,0x05},
		{0x352B,0x08},
		{0x352C,0x08},
		{0x352D,0x05},


	},	/*SUNNY*/
	{
		{0x3322,0x00},
		{0x3323,0x00},
		{0x3324,0x26},
		{0x3325,0x1C},
		{0x352A,0x02},
		{0x352B,0x02},
		{0x352C,0x02},
		{0x352D,0x02},


	},	/*CLOUDY*/
	
};

static struct msm_camera_i2c_reg_conf t4k28_iso[][3] = {
	
	{
		{0x3503,0x25},
		{0x3504,0x01},
		{0x3505,0x00},
 // ISO Mode Select B[6:4] 000 Auto,001 ISO1,010 ISO2,011 ISO3,100 ISO4,101 ISO5,110 ISO6
	},   /*ISO_AUTO*/
	{
		{-1, -1 },
		{-1, -1 },
		{-1, -1 },
	},   /*ISO_DEBLUR*/
	{
		{0x3503,0x29},
		{0x3504,0x00},
		{0x3505,0x29},
 // ISO 1 control
	},   /*ISO_100*/
	{
		{0x3503,0x53},
		{0x3504,0x00},
		{0x3505,0x53},
 // ISO 2 control
	},   /*ISO_200*/
	{
		{0x3503,0xa7},//a7->bf,9e,9a
		{0x3504,0x00},
		{0x3505,0xa7},
 // ISO 3 control
	},   /*ISO_400*/

};


static struct msm_camera_i2c_reg_conf t4k28_auto_fps_settings0[] = {

//{0x351B,0x90},
		//15~30fps		
		{0x351B,0xA8},
		//10~30fps
		
};  
/*  
                                                                    
    
                
                             
    
              
                        
                 
    
                        
                       
                       
    
                       
                                                     
                                                  
                                                  
                                                          
                                                          

       
                       
                                                   
                                                   
     
                                               
                                               
                                                          
                                                          

       
                       
                                
                                 
      
                              
                               

                                                                                 
                                        
              
              
              

              
                                                     
                                                           
     
                                                           
      

  

                                                                    

                
                             
                       
              

              
                          

                            
                               

              
              
                       

  
*/
static struct msm_camera_i2c_reg_conf t4k28_fixed_fps_settings0[] = {
 {0x351B,0x00},
			//30fps
};
/*
                                                                     

                
                             

              
                       
                 

                        
                       
                       

                       
                                                     
                                                  
                                                  
                                               
                                               

       
                       
                                                   
                                                   
     
                               
                                
                                               
                                               

       
                       
                                
                                 
      
                              
                               

                                                                                 
                                         
              
              
              

              
                                                     
                                                           
     
                             
      

  

                                                                     


                
                             

                       
              

              
                          

                            
              

              
              
                       
  
*/
static struct msm_camera_i2c_reg_conf t4k28_attached_fps_settings0[] = {

  	{0x351B, 0x10},
			//15fixed fps
	
};
/*
                                                                        

                
                             

              
                       
                        

                        
                       
                       

                       
                                                     
                                                 
                                                 
                                                     
                                                     

       
                       
                                                  
                                                  
     
                                            
                                            
                                                     
                                                     

       
                       
                                
                                 
      
                              
                               

                                                                               
                                         
              
              
              

              
                                                     
                                                           
     
                             
      

  

                                                                        

                
                             

                       
              

              
                          

                            
              

              
              
                       

  
*/

// Scene MODE
/*
                                                                       
         
 

                                              
               
               
               
               
               
               
               
               
               
               
               
               
               
               
               
               
               
               
               
 
  
*/
static struct msm_camera_i2c_reg_conf t4k28_scene_normal_settings[] = {



    {0x3316,0x18}, //                            
	{0x3317,0x00},
	{0x3318,0x20},
	{0x3319,0xd0},
	{0x343F,0x00},
	{0x350D,0x19},
	{0x350E,0x1b},
	{0x350F,0x92},
	{0x3510,0xa0},
	{0x3518,0x00},
	{0x3519,0xff},
	{0x3422,0xA8},
	{0x3322,0x00},
	{0x3323,0x00},
	{0x3324,0x00},
	{0x3325,0x38},
	{0x352A,0x20},
	{0x352B,0x20},
	{0x352C,0x20},
	{0x352D,0x20},
};



static struct msm_camera_i2c_reg_conf t4k28_scene_portrait_settings[] = {





	{0x3316,0x18},//                            
	{0x3317,0x00},
	{0x3318,0x50},
	{0x3319,0xF0},
	{0x343F,0x00},
	{0x350D,0x08},
	{0x350E,0x0B},
	{0x350F,0x80},
	{0x3510,0x80},
	{0x3518,0x00},
	{0x3519,0xff},
	{0x3422,0xb0},
	{0x3322,0x00},
	{0x3323,0x00},
	{0x3324,0x00},
	{0x3325,0x38},
	{0x352A,0x20},
	{0x352B,0x20},
	{0x352C,0x20},
  {0x352D,0x20},



};

static struct msm_camera_i2c_reg_conf t4k28_scene_landscape_settings[] = {




	{0x3316,0x18},//                            
	{0x3317,0x00},
	{0x3318,0x08},
	{0x3319,0x80},
	{0x343F,0x00},
	{0x350D,0x19},
	{0x350E,0x1b},
	{0x350F,0x92},
	{0x3510,0xa0},
	{0x3518,0x00},
	{0x3519,0xff},
	{0x3422,0xC0},
	{0x3322,0x00},
	{0x3323,0x00},
	{0x3324,0x00},
	{0x3325,0x38},
	{0x352A,0x20},
	{0x352B,0x20},
	{0x352C,0x20},
  {0x352D,0x20},


};

static struct msm_camera_i2c_reg_conf t4k28_scene_sport_settings[] = {


	


	{0x3316,0x18},//                            
	{0x3317,0x00},
	{0x3318,0x20},
	{0x3319,0xd0},
	{0x343F,0x00},
	{0x350D,0x19},
	{0x350E,0x1b},
	{0x350F,0x92},
	{0x3510,0xa0},
	{0x3518,0x00},
	{0x3519,0xff},
	{0x3422,0xb0},
	{0x3322,0x00},
	{0x3323,0x00},
	{0x3324,0x00},
	{0x3325,0x38},
	{0x352A,0x20},
	{0x352B,0x20},
	{0x352C,0x20},
  {0x352D,0x20},




};

static struct msm_camera_i2c_reg_conf t4k28_scene_sunset_settings[] = {



	{0x3316,0x18},//                            
	{0x3317,0x00},
	{0x3318,0x20},
	{0x3319,0xd0},
	{0x343F,0x00},
	{0x350D,0x19},
	{0x350E,0x1b},
	{0x350F,0x92},
	{0x3510,0xa0},
	{0x3518,0x00},
	{0x3519,0xff},
	{0x3422,0xb0},
	{0x3322,0x00},
	{0x3323,0x00},
	{0x3324,0x26},
	{0x3325,0x1C},
	{0x352A,0x02},
	{0x352B,0x02},
	{0x352C,0x02},
  {0x352D,0x02},




};

static struct msm_camera_i2c_reg_conf t4k28_scene_night_settings[] = {



	{0x3316,0x18},//                            
	{0x3317,0x00},
	{0x3318,0x20},
	{0x3319,0xd0},
	{0x343F,0x50},
	{0x350D,0x19},
	{0x350E,0x1b},
	{0x350F,0x92},
	{0x3510,0xa0},
	{0x3518,0x00},
	{0x3519,0xff},
	{0x3422,0xA8},
	{0x3322,0x00},
	{0x3323,0x00},
	{0x3324,0x00},
	{0x3325,0x38},
	{0x352A,0x20},
	{0x352B,0x20},
	{0x352C,0x20},
  {0x352D,0x20},







};


static struct msm_camera_i2c_reg_conf t4k28_recommend_settings[] = {
/*                                                                */

{0x3000,0x08, MSM_CAMERA_I2C_BYTE_DATA},
{0x3001,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x3002,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3003,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3004,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3005,0xA8, MSM_CAMERA_I2C_BYTE_DATA},
{0x3011,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3012,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x3014,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3015,0x05, MSM_CAMERA_I2C_BYTE_DATA},
{0x3016,0x16, MSM_CAMERA_I2C_BYTE_DATA},
{0x3017,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3018,0x75, MSM_CAMERA_I2C_BYTE_DATA},
{0x3019,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x301A,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x301B,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x301C,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3020,0x03, MSM_CAMERA_I2C_BYTE_DATA},
{0x3021,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x3022,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x3023,0x58, MSM_CAMERA_I2C_BYTE_DATA},
{0x3025,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3026,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3027,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x302C,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x302D,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x302E,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x302F,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3030,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3031,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3032,0x98, MSM_CAMERA_I2C_BYTE_DATA},
{0x3033,0x83, MSM_CAMERA_I2C_BYTE_DATA},
{0x3034,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3037,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x303C,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x303E,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x303F,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3040,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x3044,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x3045,0x04, MSM_CAMERA_I2C_BYTE_DATA},
{0x3046,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3047,0x64, MSM_CAMERA_I2C_BYTE_DATA},
{0x3048,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3049,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x304A,0x0a, MSM_CAMERA_I2C_BYTE_DATA},
{0x304B,0x0a, MSM_CAMERA_I2C_BYTE_DATA},
{0x304C,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x304E,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3050,0x60, MSM_CAMERA_I2C_BYTE_DATA},
{0x3051,0x82, MSM_CAMERA_I2C_BYTE_DATA},
{0x3052,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x3053,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3055,0x84, MSM_CAMERA_I2C_BYTE_DATA},
{0x3056,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x3059,0x18, MSM_CAMERA_I2C_BYTE_DATA},
{0x305A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3068,0xf0, MSM_CAMERA_I2C_BYTE_DATA},
{0x3069,0xf0, MSM_CAMERA_I2C_BYTE_DATA},
{0x306C,0x06, MSM_CAMERA_I2C_BYTE_DATA},
{0x306D,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x306E,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x306F,0x04, MSM_CAMERA_I2C_BYTE_DATA},
{0x3070,0x06, MSM_CAMERA_I2C_BYTE_DATA},
{0x3071,0x43, MSM_CAMERA_I2C_BYTE_DATA},
{0x3072,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x3073,0x58, MSM_CAMERA_I2C_BYTE_DATA},
{0x3074,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3075,0x04, MSM_CAMERA_I2C_BYTE_DATA},
{0x3076,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x3077,0x5b, MSM_CAMERA_I2C_BYTE_DATA},
{0x307F,0x03, MSM_CAMERA_I2C_BYTE_DATA},
{0x3080,0x70, MSM_CAMERA_I2C_BYTE_DATA},
{0x3081,0x28, MSM_CAMERA_I2C_BYTE_DATA},
{0x3082,0x60, MSM_CAMERA_I2C_BYTE_DATA},
{0x3083,0x48, MSM_CAMERA_I2C_BYTE_DATA},
{0x3084,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x3085,0x28, MSM_CAMERA_I2C_BYTE_DATA},
{0x3086,0xf8, MSM_CAMERA_I2C_BYTE_DATA},
{0x3087,0x38, MSM_CAMERA_I2C_BYTE_DATA},
{0x3088,0x03, MSM_CAMERA_I2C_BYTE_DATA},
{0x3089,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x308A,0x58, MSM_CAMERA_I2C_BYTE_DATA},
{0x3091,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3092,0x44, MSM_CAMERA_I2C_BYTE_DATA},
{0x3093,0x8A, MSM_CAMERA_I2C_BYTE_DATA},
{0x3095,0x78, MSM_CAMERA_I2C_BYTE_DATA},
{0x3097,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3098,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x309A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x309B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x309D,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x309E,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x309F,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A0,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A1,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A2,0xa7, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A3,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A4,0xff, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A5,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A6,0xff, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A7,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x30A8,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x30F1,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x30F2,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x30FE,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x3100,0xd2, MSM_CAMERA_I2C_BYTE_DATA},
{0x3101,0xd3, MSM_CAMERA_I2C_BYTE_DATA},
{0x3102,0x65, MSM_CAMERA_I2C_BYTE_DATA},
{0x3103,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x3104,0x31, MSM_CAMERA_I2C_BYTE_DATA},
{0x3105,0x04, MSM_CAMERA_I2C_BYTE_DATA},
{0x3106,0x23, MSM_CAMERA_I2C_BYTE_DATA},
{0x3107,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x3108,0x7b, MSM_CAMERA_I2C_BYTE_DATA},
{0x3109,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x310A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x310B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3110,0x11, MSM_CAMERA_I2C_BYTE_DATA},
{0x3111,0x11, MSM_CAMERA_I2C_BYTE_DATA},
{0x3112,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3113,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3114,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x3115,0x22, MSM_CAMERA_I2C_BYTE_DATA},
{0x3120,0x08, MSM_CAMERA_I2C_BYTE_DATA},
{0x3121,0x13, MSM_CAMERA_I2C_BYTE_DATA},
{0x3122,0x33, MSM_CAMERA_I2C_BYTE_DATA},
{0x3123,0x0e, MSM_CAMERA_I2C_BYTE_DATA},
{0x3124,0x26, MSM_CAMERA_I2C_BYTE_DATA},
{0x3125,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3126,0x0c, MSM_CAMERA_I2C_BYTE_DATA},
{0x3127,0x08, MSM_CAMERA_I2C_BYTE_DATA},
{0x3128,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x3129,0x65, MSM_CAMERA_I2C_BYTE_DATA},
{0x312A,0x27, MSM_CAMERA_I2C_BYTE_DATA},
{0x312B,0x77, MSM_CAMERA_I2C_BYTE_DATA},
{0x312C,0x77, MSM_CAMERA_I2C_BYTE_DATA},
{0x312D,0x1a, MSM_CAMERA_I2C_BYTE_DATA},
{0x312E,0xb8, MSM_CAMERA_I2C_BYTE_DATA},
{0x312F,0x38, MSM_CAMERA_I2C_BYTE_DATA},
{0x3130,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x3131,0x33, MSM_CAMERA_I2C_BYTE_DATA},
{0x3132,0x63, MSM_CAMERA_I2C_BYTE_DATA},
{0x3133,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3134,0xdd, MSM_CAMERA_I2C_BYTE_DATA},
{0x3135,0x07, MSM_CAMERA_I2C_BYTE_DATA},
{0x3136,0xb7, MSM_CAMERA_I2C_BYTE_DATA},
{0x3137,0x11, MSM_CAMERA_I2C_BYTE_DATA},
{0x3138,0x0b, MSM_CAMERA_I2C_BYTE_DATA},
{0x313B,0x0a, MSM_CAMERA_I2C_BYTE_DATA},
{0x313C,0x05, MSM_CAMERA_I2C_BYTE_DATA},
{0x313D,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x313E,0x62, MSM_CAMERA_I2C_BYTE_DATA},
{0x313F,0x85, MSM_CAMERA_I2C_BYTE_DATA},
{0x3140,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3141,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x3142,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x3143,0x22, MSM_CAMERA_I2C_BYTE_DATA},
{0x3144,0x3e, MSM_CAMERA_I2C_BYTE_DATA},
{0x3145,0x32, MSM_CAMERA_I2C_BYTE_DATA},
{0x3146,0x2e, MSM_CAMERA_I2C_BYTE_DATA},
{0x3147,0x23, MSM_CAMERA_I2C_BYTE_DATA},
{0x3148,0x22, MSM_CAMERA_I2C_BYTE_DATA},
{0x3149,0x11, MSM_CAMERA_I2C_BYTE_DATA},
{0x314A,0x6b, MSM_CAMERA_I2C_BYTE_DATA},
{0x314B,0x30, MSM_CAMERA_I2C_BYTE_DATA},
{0x314C,0x69, MSM_CAMERA_I2C_BYTE_DATA},
{0x314D,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x314E,0x31, MSM_CAMERA_I2C_BYTE_DATA},
{0x314F,0x32, MSM_CAMERA_I2C_BYTE_DATA},
{0x3150,0x32, MSM_CAMERA_I2C_BYTE_DATA},
{0x3151,0x03, MSM_CAMERA_I2C_BYTE_DATA},
{0x3152,0x0c, MSM_CAMERA_I2C_BYTE_DATA},
{0x3153,0xb3, MSM_CAMERA_I2C_BYTE_DATA},
{0x3154,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x3155,0x13, MSM_CAMERA_I2C_BYTE_DATA},
{0x3156,0x66, MSM_CAMERA_I2C_BYTE_DATA},
{0x3157,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x3158,0x03, MSM_CAMERA_I2C_BYTE_DATA},
{0x3159,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x315A,0x16, MSM_CAMERA_I2C_BYTE_DATA},
{0x315B,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x315C,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x315D,0x44, MSM_CAMERA_I2C_BYTE_DATA},
{0x315E,0x1b, MSM_CAMERA_I2C_BYTE_DATA},
{0x315F,0x52, MSM_CAMERA_I2C_BYTE_DATA},
{0x3160,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3161,0x03, MSM_CAMERA_I2C_BYTE_DATA},
{0x3162,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3163,0xff, MSM_CAMERA_I2C_BYTE_DATA},
{0x3164,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3165,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3166,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3167,0xff, MSM_CAMERA_I2C_BYTE_DATA},
{0x3168,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3169,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3180,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3181,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x3182,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x3183,0x96, MSM_CAMERA_I2C_BYTE_DATA},
{0x3184,0x30, MSM_CAMERA_I2C_BYTE_DATA},
{0x3185,0x8f, MSM_CAMERA_I2C_BYTE_DATA},
{0x3186,0x31, MSM_CAMERA_I2C_BYTE_DATA},
{0x3187,0x06, MSM_CAMERA_I2C_BYTE_DATA},
{0x3188,0x0c, MSM_CAMERA_I2C_BYTE_DATA},
{0x3189,0x44, MSM_CAMERA_I2C_BYTE_DATA},
{0x318A,0x42, MSM_CAMERA_I2C_BYTE_DATA},
{0x318B,0x0b, MSM_CAMERA_I2C_BYTE_DATA},
{0x318C,0x11, MSM_CAMERA_I2C_BYTE_DATA},
{0x318D,0xaa, MSM_CAMERA_I2C_BYTE_DATA},
{0x318E,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x318F,0x30, MSM_CAMERA_I2C_BYTE_DATA},
{0x3190,0x03, MSM_CAMERA_I2C_BYTE_DATA},
{0x3191,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3192,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3193,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3194,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3195,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3196,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3197,0xde, MSM_CAMERA_I2C_BYTE_DATA},
{0x3198,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3199,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x319A,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x319B,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x319C,0x16, MSM_CAMERA_I2C_BYTE_DATA},   
{0x319D,0x0a, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31A0,0xbf, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31A1,0xff, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31A2,0x11, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B0,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B1,0x30, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B2,0x17, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B3,0xD9, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B4,0x03, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B5,0x2B, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B6,0x18, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B7,0x54, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B8,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31B9,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31BA,0x3f, MSM_CAMERA_I2C_BYTE_DATA},   
{0x31BB,0xff, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3300,0xff, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3301,0x35, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3303,0x40, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3304,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3305,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3306,0x30, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3307,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3308,0x87, MSM_CAMERA_I2C_BYTE_DATA},   
{0x330A,0x60, MSM_CAMERA_I2C_BYTE_DATA},   
{0x330B,0x56, MSM_CAMERA_I2C_BYTE_DATA},   
{0x330D,0x39, MSM_CAMERA_I2C_BYTE_DATA},   
{0x330E,0xff, MSM_CAMERA_I2C_BYTE_DATA},   
{0x330F,0xff, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3310,0xff, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3311,0x7f, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3312,0x0f, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3313,0x0f, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3314,0x02, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3315,0xff, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3316,0x18, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3317,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3318,0x20, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3319,0xd0, MSM_CAMERA_I2C_BYTE_DATA},   
{0x331B,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x331C,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x331D,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x331E,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3322,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3323,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3324,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3325,0x38, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3327,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3328,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3329,0x80, MSM_CAMERA_I2C_BYTE_DATA},   
{0x332A,0x80, MSM_CAMERA_I2C_BYTE_DATA},   
{0x332B,0x80, MSM_CAMERA_I2C_BYTE_DATA},   
{0x332C,0x80, MSM_CAMERA_I2C_BYTE_DATA},   
{0x332D,0x80, MSM_CAMERA_I2C_BYTE_DATA},   
{0x332E,0x80, MSM_CAMERA_I2C_BYTE_DATA},   
{0x332F,0x07, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3330,0x08, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3331,0x10, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3332,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3333,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3334,0x08, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3335,0x02, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3336,0x04, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3337,0x0b, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3338,0x05, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3339,0x0c, MSM_CAMERA_I2C_BYTE_DATA},   
{0x333A,0x0c, MSM_CAMERA_I2C_BYTE_DATA},   
{0x333B,0x88, MSM_CAMERA_I2C_BYTE_DATA},   
{0x333C,0xd0, MSM_CAMERA_I2C_BYTE_DATA},   
{0x333D,0x54, MSM_CAMERA_I2C_BYTE_DATA},   
{0x333E,0x38, MSM_CAMERA_I2C_BYTE_DATA},   
{0x333F,0x72, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3340,0x20, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3341,0x68, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3342,0xB4, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3343,0x43, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3344,0xa0, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3345,0xee, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3346,0x6c, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3347,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3348,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3349,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x334A,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x334B,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x334C,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x334D,0x58, MSM_CAMERA_I2C_BYTE_DATA},   
{0x334E,0x01, MSM_CAMERA_I2C_BYTE_DATA},   
{0x334F,0x30, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3350,0x01, MSM_CAMERA_I2C_BYTE_DATA},   
{0x335F,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3360,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3400,0xa4, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3401,0x6a, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3402,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3403,0x06, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3404,0x53, MSM_CAMERA_I2C_BYTE_DATA},
{0x3405,0xFF, MSM_CAMERA_I2C_BYTE_DATA},
{0x3406,0x09, MSM_CAMERA_I2C_BYTE_DATA},
{0x3407,0x22, MSM_CAMERA_I2C_BYTE_DATA},
{0x3408,0x09, MSM_CAMERA_I2C_BYTE_DATA},
{0x3409,0x5A, MSM_CAMERA_I2C_BYTE_DATA},
{0x340A,0x1a, MSM_CAMERA_I2C_BYTE_DATA},   
{0x340B,0x29, MSM_CAMERA_I2C_BYTE_DATA},   
{0x340C,0x2d, MSM_CAMERA_I2C_BYTE_DATA},   
{0x340D,0x34, MSM_CAMERA_I2C_BYTE_DATA},   
{0x340E,0x34, MSM_CAMERA_I2C_BYTE_DATA},   
{0x340F,0x4e, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3410,0x4e, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3411,0x38, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3412,0x38, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3413,0x56, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3414,0x47, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3415,0x65, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3416,0x47, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3417,0x38, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3418,0x2d, MSM_CAMERA_I2C_BYTE_DATA},   
{0x3419,0x29, MSM_CAMERA_I2C_BYTE_DATA},   
{0x341B,0x00, MSM_CAMERA_I2C_BYTE_DATA},   
{0x341C,0x40, MSM_CAMERA_I2C_BYTE_DATA},   
{0x341D,0x63, MSM_CAMERA_I2C_BYTE_DATA},   
{0x341E,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x341F,0x88, MSM_CAMERA_I2C_BYTE_DATA},
{0x3420,0x6e, MSM_CAMERA_I2C_BYTE_DATA},
{0x3421,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3422,0xa8, MSM_CAMERA_I2C_BYTE_DATA},
{0x3423,0x08, MSM_CAMERA_I2C_BYTE_DATA},
{0x3424,0x08, MSM_CAMERA_I2C_BYTE_DATA},
{0x3425,0x08, MSM_CAMERA_I2C_BYTE_DATA},
{0x3426,0x08, MSM_CAMERA_I2C_BYTE_DATA},
{0x342B,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x342C,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x342D,0x60, MSM_CAMERA_I2C_BYTE_DATA},
{0x342E,0x90, MSM_CAMERA_I2C_BYTE_DATA},
{0x342F,0x60, MSM_CAMERA_I2C_BYTE_DATA},
{0x3430,0x90, MSM_CAMERA_I2C_BYTE_DATA},
{0x3431,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x3432,0x40, MSM_CAMERA_I2C_BYTE_DATA},
{0x3433,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3434,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3435,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3436,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x343F,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3440,0x04, MSM_CAMERA_I2C_BYTE_DATA},
{0x3441,0x8c, MSM_CAMERA_I2C_BYTE_DATA},
{0x3442,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3443,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3444,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3446,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3447,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3448,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3449,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x344A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x344B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x344C,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x344D,0xff, MSM_CAMERA_I2C_BYTE_DATA},
{0x344E,0x0f, MSM_CAMERA_I2C_BYTE_DATA},
{0x344F,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x3450,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x3451,0x0f, MSM_CAMERA_I2C_BYTE_DATA},
{0x3452,0x55, MSM_CAMERA_I2C_BYTE_DATA},
{0x3453,0x49, MSM_CAMERA_I2C_BYTE_DATA},
{0x3454,0x6a, MSM_CAMERA_I2C_BYTE_DATA},
{0x3455,0x93, MSM_CAMERA_I2C_BYTE_DATA},
{0x345C,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x345D,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x345E,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3500,0xc5, MSM_CAMERA_I2C_BYTE_DATA},
{0x3501,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3502,0xc8, MSM_CAMERA_I2C_BYTE_DATA},
{0x3503,0x25, MSM_CAMERA_I2C_BYTE_DATA},
{0x3504,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3505,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3506,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3507,0x78, MSM_CAMERA_I2C_BYTE_DATA},
{0x3508,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3509,0x86, MSM_CAMERA_I2C_BYTE_DATA},
{0x350A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x350B,0x1c, MSM_CAMERA_I2C_BYTE_DATA},
{0x350C,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x350D,0x19, MSM_CAMERA_I2C_BYTE_DATA},
{0x350E,0x1b, MSM_CAMERA_I2C_BYTE_DATA},
{0x350F,0x92, MSM_CAMERA_I2C_BYTE_DATA},
{0x3510,0xA0, MSM_CAMERA_I2C_BYTE_DATA},
{0x3511,0x1f, MSM_CAMERA_I2C_BYTE_DATA},
{0x3512,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x3513,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3514,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3515,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x3516,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x3517,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3518,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3519,0xff, MSM_CAMERA_I2C_BYTE_DATA},
{0x351A,0xc0, MSM_CAMERA_I2C_BYTE_DATA},
{0x351B,0xa8, MSM_CAMERA_I2C_BYTE_DATA},
{0x351C,0x6c, MSM_CAMERA_I2C_BYTE_DATA},
{0x351D,0x78, MSM_CAMERA_I2C_BYTE_DATA},
{0x351E,0xd6, MSM_CAMERA_I2C_BYTE_DATA},
{0x351F,0x80, MSM_CAMERA_I2C_BYTE_DATA},
{0x3520,0x26, MSM_CAMERA_I2C_BYTE_DATA},
{0x3521,0x02, MSM_CAMERA_I2C_BYTE_DATA},
{0x3522,0x08, MSM_CAMERA_I2C_BYTE_DATA},
{0x3523,0x0c, MSM_CAMERA_I2C_BYTE_DATA},
{0x3524,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3525,0x45, MSM_CAMERA_I2C_BYTE_DATA},
{0x3526,0x3d, MSM_CAMERA_I2C_BYTE_DATA},
{0x3527,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3528,0x55, MSM_CAMERA_I2C_BYTE_DATA},
{0x3529,0x5a, MSM_CAMERA_I2C_BYTE_DATA},
{0x352A,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x352B,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x352C,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x352D,0x20, MSM_CAMERA_I2C_BYTE_DATA},
{0x352E,0x30, MSM_CAMERA_I2C_BYTE_DATA},
{0x352F,0xe0, MSM_CAMERA_I2C_BYTE_DATA},
{0x3530,0x34, MSM_CAMERA_I2C_BYTE_DATA},
{0x3531,0x30, MSM_CAMERA_I2C_BYTE_DATA},
{0x3532,0x50, MSM_CAMERA_I2C_BYTE_DATA},
{0x3533,0x34, MSM_CAMERA_I2C_BYTE_DATA},
{0x3534,0x30, MSM_CAMERA_I2C_BYTE_DATA},
{0x3535,0xE0, MSM_CAMERA_I2C_BYTE_DATA},
{0x3536,0x1C, MSM_CAMERA_I2C_BYTE_DATA},
{0x3537,0x24, MSM_CAMERA_I2C_BYTE_DATA},
{0x3538,0x48, MSM_CAMERA_I2C_BYTE_DATA},
{0x3539,0x10, MSM_CAMERA_I2C_BYTE_DATA},
{0x353A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x353B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x353C,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x353D,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x353E,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x353F,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3540,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3541,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3542,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3543,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3544,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3545,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3546,0xc8, MSM_CAMERA_I2C_BYTE_DATA},
{0x3547,0x0a, MSM_CAMERA_I2C_BYTE_DATA},
{0x3548,0x84, MSM_CAMERA_I2C_BYTE_DATA},
{0x3549,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x354A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x354B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x354C,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x354D,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x354E,0x06, MSM_CAMERA_I2C_BYTE_DATA},
{0x354F,0xB0, MSM_CAMERA_I2C_BYTE_DATA},
{0x3550,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3551,0x03, MSM_CAMERA_I2C_BYTE_DATA},
{0x3552,0x28, MSM_CAMERA_I2C_BYTE_DATA},
{0x3553,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3554,0x60, MSM_CAMERA_I2C_BYTE_DATA},
{0x3555,0xf0, MSM_CAMERA_I2C_BYTE_DATA},
{0x355D,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x355E,0xd3, MSM_CAMERA_I2C_BYTE_DATA},
{0x355F,0x06, MSM_CAMERA_I2C_BYTE_DATA},
{0x3560,0xC0, MSM_CAMERA_I2C_BYTE_DATA},
{0x3561,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3562,0x25, MSM_CAMERA_I2C_BYTE_DATA},
{0x3563,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3564,0x71, MSM_CAMERA_I2C_BYTE_DATA},
{0x3565,0x0B, MSM_CAMERA_I2C_BYTE_DATA},
{0x3566,0xD2, MSM_CAMERA_I2C_BYTE_DATA},
{0x3567,0x98, MSM_CAMERA_I2C_BYTE_DATA},
{0x3568,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3569,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x356A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x356B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x356C,0xC1, MSM_CAMERA_I2C_BYTE_DATA},
{0x356D,0xDB, MSM_CAMERA_I2C_BYTE_DATA},
{0x356E,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x356F,0x1B, MSM_CAMERA_I2C_BYTE_DATA},
{0x3570,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3571,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3572,0x01, MSM_CAMERA_I2C_BYTE_DATA},
{0x3573,0x65, MSM_CAMERA_I2C_BYTE_DATA},
{0x3574,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3575,0xc8, MSM_CAMERA_I2C_BYTE_DATA},
{0x3576,0x0E, MSM_CAMERA_I2C_BYTE_DATA},
{0x3577,0xF7, MSM_CAMERA_I2C_BYTE_DATA},
{0x3578,0x27, MSM_CAMERA_I2C_BYTE_DATA},
{0x3579,0x09, MSM_CAMERA_I2C_BYTE_DATA},
{0x357A,0x99, MSM_CAMERA_I2C_BYTE_DATA},
{0x357B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x357C,0xe0, MSM_CAMERA_I2C_BYTE_DATA},
{0x357D,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3900,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3901,0x07, MSM_CAMERA_I2C_BYTE_DATA},
{0x3902,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3903,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3904,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3905,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3906,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3907,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3908,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3909,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x390A,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x390B,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x390C,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x30F0,0x00, MSM_CAMERA_I2C_BYTE_DATA},
{0x3010,0x01, MSM_CAMERA_I2C_BYTE_DATA},




         

 
 
 
/*                                                                */
 
};
 
 
#endif	/*t4k28_REG_H */
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 