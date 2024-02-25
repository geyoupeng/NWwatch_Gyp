#include "common.h"
#include "string.h"
#include "rtc.h"
#include "FreeRTOS.h"

#define BKPCheck_SAVE_ADDR	RTC_BKP_DR1
#define EEPROM_CHECK_NUM	0x1234

appconfig_s appConfig;
uint16_t BKPbuf[6];
uint16_t BKPDataRR[]={RTC_BKP_DR1,RTC_BKP_DR2,RTC_BKP_DR3,RTC_BKP_DR4,RTC_BKP_DR5,
RTC_BKP_DR7};

void AppConfigSave(void){
		
	BKPbuf[0]=(uint16_t)appConfig.invert ;
	BKPbuf[1]=(uint16_t)appConfig.animations;
	BKPbuf[2]=(uint16_t)appConfig.display180;
	BKPbuf[3]=(uint16_t)appConfig.CTRL_LEDs;
	BKPbuf[4]=(uint16_t)appConfig.showFPS;
	BKPbuf[5]=(uint16_t)appConfig.volumes;
		
	int index =0;

	HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
	HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
	HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
	HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
	HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
	HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
}

void AppConfigReset(void){
		appConfig.invert = false;
		appConfig.timeMode = TIMEMODE_24HR;
		appConfig.animations = true;
		appConfig.display180 = true;
	    appConfig.CTRL_LEDs = false;
		appConfig.showFPS = true;
		appConfig.volumes = 255;	
		AppConfigSave();
		//alarm_reset();
}

void AppConfigInit(void){
	uint16_t BKPCheck = 0;
	BKPCheck = HAL_RTCEx_BKUPRead(&hrtc,BKPCheck_SAVE_ADDR); 

		
	appConfig = *(appconfig_s *)pvPortMalloc(sizeof(appconfig_s));
	memset(&appConfig, 0x00, sizeof(appconfig_s));

	if(BKPCheck == EEPROM_CHECK_NUM){
		int index =0;
		appConfig.invert	   = HAL_RTCEx_BKUPRead(&hrtc,BKPDataRR[index]); index++;
		appConfig.animations  =  HAL_RTCEx_BKUPRead(&hrtc,BKPDataRR[index]); index++;
		appConfig.display180  =  HAL_RTCEx_BKUPRead(&hrtc,BKPDataRR[index]); index++;
		appConfig.CTRL_LEDs	=  HAL_RTCEx_BKUPRead(&hrtc,BKPDataRR[index]); index++;
		appConfig.showFPS	  =  HAL_RTCEx_BKUPRead(&hrtc,BKPDataRR[index]); index++;
		appConfig.volumes	  =  HAL_RTCEx_BKUPRead(&hrtc,BKPDataRR[index]); index++;
	}else{
		BKPCheck = EEPROM_CHECK_NUM;
		HAL_RTCEx_BKUPWrite(&hrtc,BKPCheck_SAVE_ADDR,EEPROM_CHECK_NUM);	
	
		memset(&BKPbuf, 0x0000, sizeof(BKPbuf));
		
		int index =0;
		HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
		HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
		HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
		HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
		HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
		HAL_RTCEx_BKUPWrite(&hrtc, BKPDataRR[index], BKPbuf[index]); index++;
		AppConfigReset();
	}
}

