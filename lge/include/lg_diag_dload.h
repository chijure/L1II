#ifndef LG_DIAG_DLOAD_H
#define LG_DIAG_DLOAD_H

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/syscalls.h>

#include "lg_diag_cfg.h"
#include "lg_comdef.h"
#include "lg_diagcmd.h"


/*********************** BEGIN PACK() Definition ***************************/
#if defined __GNUC__
  #define PACK(x)       x __attribute__((__packed__))
  #define PACKED        __attribute__((__packed__))
#elif defined __arm
  #define PACK(x)       __packed x
  #define PACKED        __packed
#else
  #error No PACK() macro defined for this compiler
#endif
/********************** END PACK() Definition *****************************/

/*                                                                         */
#ifdef LG_FW_WEB_DOWNLOAD	

#pragma pack(1)

typedef enum
{
   WEBDLOAD_CONNECT_MODE	= 0x00,
   WEBDLOAD_GETINFO			= 0xA0,
   WEBDLOAD_READ_MISC		= 0xA1,
   WEBDLOAD_REMOVE_MISC		= 0xA2,
   WEBDLOAD_INITIALIZE_MISC	= 0xA3
} webdload_sub_cmd_type;

#define CONNECT_MODE_DIAG_1_0	0x04

#define SIZE_SERIALNUMBER 20
#define SIZE_FACTORY_SWVERSION 50  //GSM 버전체계의 version명
#define SIZE_OSTYPE 10  //GSM 버전체계의 version명
#define SIZE_DIR_NUMBER_PCS 15
#define MAX_WEB_BUFFER_SIZE 300

#define SIZE_SWVERSION 30
#define SIZE_MODELNAME 10

#define CONNECT_MODE_DIAG_1_1 5

typedef enum {
	STEP_WEBUPGRADE_FINISHED 	=0,
	STEP_WEBUPGRADE_NOTFINISHED =1, 
	STEP_WEBUPGRADE_NOTDEFINED  = 0xFF 
} UpgradeStatus_type;

#define WEBUPGRADE_NOERROR	0
#define WEBUPGRADE_STATUS	1 
#define WEBUPGRADE_SWVERSION 2
#define WEBUPGRADE_SERIAL	3 
#define WEBUPGRADE_BATTERY	4 
#define WEBUPGRADE_TYPE     5 
#define WEBUPGRADE_PNUMBER	6 
#define WEBUPGRADE_UNKNOWN	7 

typedef struct 
{
	UpgradeStatus_type UpgradeStatus;  //finished :0    not finished :1
	char sModelName[SIZE_MODELNAME];  //출력되는 모델명
	char sSWVersion[SIZE_SWVERSION];  //PDM의뢰되는 버전명과 동일하며 대문자만 사용.  ex: VS93010A
	char sSWSubVersion; 			// number as subversion.
	char sSerialNumber[SIZE_SERIALNUMBER];//MEID,ESN,IMEI등 폰의 고유값
	byte nCurrentBatteryLevel;		  // current battery level.
	byte nPASSBatteryLevel; 		  // battery level for at least percentage
	char sPhoneType[SIZE_OSTYPE];	  // String like "ANDROID, WinMobile, Feature"
	char sOSVersion[SIZE_OSTYPE];	  // OS Version. for 2level OS upgrade
	char sPhoneNumber[SIZE_DIR_NUMBER_PCS]; //PhoneNumber [VZW only]
	byte bCurrentMode;	   // 0: normal mode   1: download mode
	byte bPreventUpgrade;  // if 1, do not allow download.
	char sReserved3[40];
}PACKED webdload_info_type;

/*============= response packet ==================*/

typedef struct
{	
	char cmd_code;	
	char sub_cmd;				 /* Use webdload_sub_cmd_type */	
	char ErrorCode;
}PACKED diag_webdload_rsp_header_type;

typedef struct
{	
	char cmd_code;	
	char sub_cmd;				 /* Use webdload_sub_cmd_type */
}PACKED diag_webdload_req_type;

typedef union
{	
	char    connection_mode;	
	webdload_info_type info;
} PACKED webdload_rsp_type;

typedef struct
{	
	diag_webdload_rsp_header_type header;	
	webdload_rsp_type   data;   
} PACKED diag_webdload_rsp_type;

#pragma pack()

#endif /*LG_FW_WEB_DOWNLOAD*/	
/*                                                                         */

#endif /* LG_DIAG_DLOAD_H */

