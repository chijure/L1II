#include "lg_diag_dload.h"
#include <mach/msm_battery.h>


/*-----------------------         WEB DLOAD     ------------------------------*/

/*                                                                         */
#ifdef LG_FW_WEB_DOWNLOAD

extern int lge_emmc_misc_write(unsigned int blockNo, const char* buffer, int size);
extern int lge_emmc_misc_read(unsigned int blockNo, char* buffer, int size);
extern PACK(void *) diagpkt_alloc (diagpkt_cmd_code_type code, unsigned int length);
extern void diagpkt_free(PACK(void *)pkt);
extern char* remote_get_sw_version(void);
extern unsigned char swv_buff[100];
extern void msm_get_MEID_type(char*);

static int atoi(const char *name)
{
	int val = 0;

	for (;; name++) {
		switch (*name) {
		case '0' ... '9':
			val = 10*val+(*name-'0');
			break;
		default:
			return val;
		}
	}
}

PACK (void *)webdload_common_cmd(PACK (void *)req_pkt, uint16 pkt_len)
{
	diag_webdload_req_type *req = (diag_webdload_req_type*)req_pkt;
	diag_webdload_rsp_type *rsp = NULL;
	char buffer[MAX_WEB_BUFFER_SIZE];
	char *pbuf, **ppbuf;
	int rspsize=0;

	memset(buffer, 0, MAX_WEB_BUFFER_SIZE);			
		
	printk("WEBUP : cmd_code=0x%X, subcmd = 0x%X len=%d\n", req->cmd_code, req->sub_cmd, pkt_len);
		 
	switch(req->sub_cmd)    
	{	
		case WEBDLOAD_CONNECT_MODE:
			rspsize = sizeof(diag_webdload_rsp_header_type) + 1;
			break;
		default:
			rspsize = sizeof(diag_webdload_rsp_type); 
	}

	/*                               */
	if (rspsize >= MAX_WEB_BUFFER_SIZE) {
		printk("WEBUP : rspsize is too large\n");
		return NULL;
	}

	if (pkt_len > 30)
			return NULL;
			
	if (rspsize != 0)
	{
		rsp = (diag_webdload_rsp_type*) diagpkt_alloc(DIAG_WEBDLOAD_COMMON_F,rspsize);
		if(rsp == NULL)
			return NULL;
		memset((void*)rsp, 0, rspsize);
		rsp->header.cmd_code = DIAG_WEBDLOAD_COMMON_F;
		rsp->header.sub_cmd = req->sub_cmd;
		rsp->header.ErrorCode = WEBUPGRADE_NOERROR;;
	}

	switch(req->sub_cmd)
	{
		case WEBDLOAD_CONNECT_MODE:
			rsp->data.connection_mode = CONNECT_MODE_DIAG_1_1;		
			break;

		
		case WEBDLOAD_GETINFO:
			printk("WEBUP :get information\n");
			rsp->data.info.UpgradeStatus = STEP_WEBUPGRADE_FINISHED;

			//sFactorySWVersion
			remote_get_sw_version();
			memcpy(buffer,(&swv_buff[0])+50,50);
			buffer[49] = '\0';
			
			/*                                   */
			if(buffer[0] == '\0') {
				printk("WEBUP : remote_get_sw_version() error. string is empty\n");
				rsp->header.ErrorCode = WEBUPGRADE_UNKNOWN;
				break;
			}

			pbuf = buffer; ppbuf = &pbuf;
			sprintf(rsp->data.info.sModelName, "%s", strsep(ppbuf, "-"));
			printk("WEBUP :Get sModelName : %s \n", rsp->data.info.sModelName);

			strsep(ppbuf,"-");
			sprintf(rsp->data.info.sSWVersion, "%s", strsep(ppbuf, "-"));
			printk("WEBUP :Get sSWVersion : %s \n", rsp->data.info.sSWVersion);

			strsep(ppbuf,"+");
			rsp->data.info.sSWSubVersion = atoi(*ppbuf);
			printk("WEBUP :Get sSWSubVersion : %c \n", rsp->data.info.sSWSubVersion);
			
			//sSerialNumber
			memset(buffer, 0, MAX_WEB_BUFFER_SIZE);
			msm_get_MEID_type(buffer);			
			memcpy(rsp->data.info.sSerialNumber, buffer, 15);
			printk("WEBUP :Get IMEI : %s \n", rsp->data.info.sSerialNumber);

			//nBatteryLevel
			rsp->data.info.nCurrentBatteryLevel = msm_batt_get_vbatt_capacity();//50;
			rsp->data.info.nPASSBatteryLevel = 30;
			printk("WEBUP :Get msm_battery_cap : %d \n", msm_batt_get_vbatt_capacity());
			printk("WEBUP :Get BatteryLevel : %d \n", rsp->data.info.nCurrentBatteryLevel);
			printk("WEBUP :Get nPASSBatteryLevel : %d \n", rsp->data.info.nPASSBatteryLevel);			

			//sPhoneType
			sprintf(rsp->data.info.sPhoneType, "ANDROID");
			printk("WEBUP :Get PhoneType : %s \n", rsp->data.info.sPhoneType);
				
			//sPhoneNumber
			// Not Supported

			memset(buffer, 0, MAX_WEB_BUFFER_SIZE);
			memcpy(buffer, rsp, rspsize );
			if (lge_emmc_misc_write(48, buffer, rspsize) <0)
			{
				printk("Download Informatin not saved\r\n");
				rsp->header.ErrorCode =WEBUPGRADE_UNKNOWN;
			}			
			break;
					
		case   WEBDLOAD_READ_MISC:/*only for test*/ 
			printk("WEBUP :read memory\n");
			lge_emmc_misc_read(48, buffer, MAX_WEB_BUFFER_SIZE);
			memcpy(rsp, buffer, rspsize);
			break;
			
		default :
			rsp->header.ErrorCode = WEBUPGRADE_UNKNOWN;


	}

	return rsp;
}

int lge_webdload_init(void)
{
	diag_webdload_rsp_type *rtn = NULL;
	char buffer_getinfo[3]={DIAG_WEBDLOAD_COMMON_F, WEBDLOAD_GETINFO};

	printk("WEBUP : Auto update webup misc\n");
	rtn = webdload_common_cmd(buffer_getinfo, 2);

	if(rtn != NULL)
		diagpkt_free(rtn);

	return 0;
}
EXPORT_SYMBOL(lge_webdload_init);

#endif /*LG_FW_WEB_DOWNLOAD*/	
/*                                                                         */


