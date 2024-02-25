#include "common.h"
#include "drivers.h"
#include "freertos.h"
#include "task.h"
#include <string.h>

#define NOALARM	128
#define FLASH_SECTOR_ALARM		1*4096
#define UINT_MAX    0xFFFFFFFF

static volatile alarm_s flashAlarms[ALARM_COUNT] = {{22, 45, 0},{01, 48, 4},{14, 12, 63},{14, 15, 0},{0, 1, 0}};
static uint8_t nextAlarm = NOALARM;
static bool alarmSetOff;

static void (*oldDrawFunc)(void);
static bool (*oldBtn1Func)(void);
static bool (*oldBtn2Func)(void);
static bool (*oldBtn3Func)(void);

void Alarmget(uint8_t num, alarm_s *alarm){
	memcpy(alarm,(void *)&flashAlarms[num],sizeof(alarm_s));
}

static bool AlarmGetNext(alarm_s *alarm){
	if(nextAlarm == NOALARM)
		return false;
	memcpy(alarm,(void *)&flashAlarms[nextAlarm],sizeof(alarm_s));
	return true;
}

void AlarmSave(uint8_t num, alarm_s *alarm){
	memcpy((void *)&flashAlarms[num],alarm,sizeof(alarm_s));
	W25QEraseSector(FLASH_SECTOR_ALARM);
	W25QPageWrite(FLASH_SECTOR_ALARM, (unsigned char*)flashAlarms, sizeof(alarm_s)*ALARM_COUNT);
    vTaskDelay(100);

	GetNextAlarm();
}

static uint32_t toMinutes(uint8_t hour,uint8_t mins,uint8_t day){
	uint32_t total = mins;
	total += hour * 60;
	total += day * 1440;
	return total;
}

void GetNextAlarm(void){
	uint8_t next = NOALARM;
	uint32_t nextTime = (uint32_t)UINT_MAX;

	// Make sure time is in 24 hour mode
	time_s timeNow;
	timeNow.hour = timeDate.time.hour;
	timeNow.ampm = timeDate.time.ampm;
	TimeSetMode(&timeNow, TIMEMODE_24HR);

	// Now in minutes from start of week
	uint32_t now = toMinutes(timeNow.hour, timeDate.time.mins + 1, timeDate.date.day);

	// Loop through alarms
	for(uint8_t i = 0;i <ALARM_COUNT; i++)
	{
		// Get alarm data
		alarm_s alarm;
		Alarmget(i, &alarm);

		// Not enabled
		if(!alarm.enabled)
			continue;

		// Loop through days
		for(uint8_t d = 0;d <7; d++)
		{
			// Day not enabled
			if(!AlarmDayEnabled(alarm.days, d))
				continue;

			// Alarm time in minutes from start of week
			uint32_t alarmTime = toMinutes(alarm.hour, alarm.min, d);

			// Minutes to alarm
			int timeTo = alarmTime - now;

			// Negative result, must mean alarm time is earlier in the week than now, add a weeks time
			if(timeTo < 0)
				timeTo += ((6*1440) + (23*60) + 59); // 10079

			// Is minutes to alarm less than the last minutes to alarm?
			if((uint32_t)timeTo < nextTime)
			{
				// This is our next alarm
				nextTime = timeTo;
				next = i;
			}
		}
	}
	// Set next alarm
	nextAlarm = next;
}

static void AlarmStop(void){
	GetNextAlarm();
	SetDrawFunc(oldDrawFunc);
	SetButtonFunc(oldBtn1Func, oldBtn2Func, oldBtn3Func);

	BuzzerStop();
	alarmSetOff = false;
}

static bool goingOff(){
	alarm_s nextAlarm;
	
	// Make sure we're in 24 hour mode
	time_s time;
	time.hour = timeDate.time.hour;
	time.ampm = timeDate.time.ampm;
	TimeSetMode(&time, TIMEMODE_24HR);

	if(AlarmGetNext(&nextAlarm) && AlarmDayEnabled(nextAlarm.days, timeDate.date.day) && nextAlarm.hour == time.hour && nextAlarm.min == timeDate.time.mins)
	{
		if(timeDate.time.secs == 0)
			alarmSetOff = true;
		return true;
	}
	return false;

}

static void AlarmDraw(void){
	if((uint8_t)xTaskGetTickCount() < 128)
		OledShowImage(16, 16, 32, 32, &MAIN_MUNE[3][0], 0);
	
	// Draw time
	char buff[8];
	sprintf(buff, PSTR(TIME_FORMAT_SMALL), timeDate.time.hour, timeDate.time.mins, timeDate.time.ampm);
	OledShowStr(60, 20,buff,OLED_8X8);

	// Draw day
	strcpy(buff, days[timeDate.date.day]);
	OledShowStr(68,36,buff,OLED_8X8);

}

void AlarmUpdate(void){
	bool wasGoingOff = alarmSetOff;
	bool alarmNow = goingOff();
  	if(alarmSetOff)
	{
		if(alarmNow)
		{
			if(wasGoingOff != alarmSetOff)
			{
				oldDrawFunc = SetDrawFunc(AlarmDraw);
				oldBtn1Func = SetButtonSignedFunc(BTN1, NULL);
				oldBtn2Func = SetButtonSignedFunc(BTN2, (bool (*)(void))AlarmStop);
				oldBtn3Func = SetButtonSignedFunc(BTN3, NULL);
				BuzzerPlay(tuneAlarm,VOL_ALARM,PRIO_ALARM);
			}
			if(!LedFlashing())
			{
				static led_t ledFlash = LEDGREEN;
				ledFlash = (ledFlash == LEDGREEN) ? LEDRED : LEDGREEN;
				LedFlash(ledFlash, 150);
			}
		}
		else
			AlarmStop();
	}
}

void AlarmInit()
{
	uint8_t try_count = 3;
	uint32_t flash_test_data = 0;
	alarm_s flashAlarms_test_datas = {0, 0, 0};
	
	W25QRead(FLASH_SECTOR_ALARM, (uint8_t *)&flash_test_data, 4);
	while(try_count--)
	{
		if((flash_test_data != 0) || (flash_test_data != 0xff))
		{
			vTaskDelay(1);
			W25QRead(FLASH_SECTOR_ALARM, (uint8_t *)&flashAlarms_test_datas, sizeof(alarm_s));
			
			// Not yet saved
			if((flashAlarms_test_datas.hour == 255) || (flashAlarms_test_datas.min == 255))
				break;
			//if((flashAlarms_test_datas.hour == 0) || (flashAlarms_test_datas.min == 0) || (flashAlarms_test_datas.days == 0))
			//	break;
			
			vTaskDelay(1);
			memset((void *)flashAlarms, 0x00, sizeof(alarm_s) * ALARM_COUNT);
			W25QRead(FLASH_SECTOR_ALARM, (uint8_t *)flashAlarms, sizeof(alarm_s) * ALARM_COUNT);
			break;
		}
		
		W25QRead(FLASH_SECTOR_ALARM, (uint8_t *)&flash_test_data, 4);
		vTaskDelay(10);
	}
	
	GetNextAlarm();
}

