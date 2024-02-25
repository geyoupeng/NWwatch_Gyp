#include "driver_log.h"
#include <stdarg.h>
#include "rtc.h"

static RTC_DateTypeDef MyDate = {0};
static RTC_TimeTypeDef MyTime = {0};

void DataInit(){
	#if 0
	//保证掉电以后 时间继续增加
	if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1)!= 0x5052){
		MyTime.Hours = 0x10;
		MyTime.Minutes = 0x50;
		MyTime.Seconds = 0x0;

		HAL_RTC_SetTime(&hrtc, &MyTime, RTC_FORMAT_BCD);
		MyDate.WeekDay = RTC_WEEKDAY_TUESDAY;
		MyDate.Month = RTC_MONTH_JANUARY;
		MyDate.Date = 0x16;
		MyDate.Year = 0x24;
		HAL_RTC_SetDate(&hrtc, &MyDate, RTC_FORMAT_BCD);

		//保存设置的初始值
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x5052);//向指定的后备区域寄存器写入数据
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, MyDate.Year);
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, MyDate.Month);
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, MyDate.Date);
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, MyDate.WeekDay);	
	}else{
		MyDate.Year    = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
		MyDate.Month   = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
		MyDate.Date    = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
		MyDate.WeekDay = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);
        //HAL_RTC_GetDate(&hrtc, &MyDate, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &MyDate, RTC_FORMAT_BCD);
	}
	#endif
}
void GetCurDate(){
	//获取系统的时间和日期
	//HAL_RTC_GetDate(&hrtc, &MyDate, RTC_FORMAT_BIN);
	//HAL_RTC_GetTime(&hrtc, &MyTime, RTC_FORMAT_BIN);
}

void LogPrint(LogLevel level, const char *file, int line, const char *format,...){

	va_list ap;
	char buf[100] = {0};
	char datatime[50] = {0};
	/* Display date Format : yy/mm/dd-hh:mm:ss */
	GetCurDate();
	sprintf(datatime,"%02d/%02d/%02d-%02d:%02d:%02d",2000 + MyDate.Year, MyDate.Month,MyDate.Date,MyTime.Hours, MyTime.Minutes, MyTime.Seconds);

	va_start(ap,format);
	vsprintf(buf, format, ap);
	va_end(ap);

	switch (level)
	{
		case LOG_INFO:
			printf("[%s-%s Line:%d] INFO: %s",datatime,file,line,buf);
			break;
		case LOG_DEBUG:
			printf("[%s-%s Line:%d] DEBUG: %s",datatime,file,line,buf);
			break;
		case LOG_WARN:
			printf("[%s-%s Line:%d] WARN: %s",datatime,file,line,buf);
			break;
		case LOG_ERROR:
			printf("[%s-%s Line:%d] ERROR: %s",datatime,file,line,buf);
			break;
        default:
            break;
	}
}

