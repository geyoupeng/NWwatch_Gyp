#include "common.h"
#include "drivers.h"
#include <stdio.h>
#include <string.h>


#define SETTING_NOW_NONE		0
#define SETTING_NOW_EN			1
#define SETTING_NOW_HOUR		2
#define SETTING_NOW_MIN			3
#define SETTING_NOW_AMPM		4
#define SETTING_NOW_DAY_MON		5
#define SETTING_NOW_DAY_TUE		6
#define SETTING_NOW_DAY_WED		7
#define SETTING_NOW_DAY_THUR	8
#define SETTING_NOW_DAY_FRI		9
#define SETTING_NOW_DAY_SAT		10
#define SETTING_NOW_DAY_SUN		11
#define CHAR_TICK		128
#define ALARM_SET_CHAR (CHAR_TICK)
#define ALARM_UNSET_CHAR ('.')

static prev_menu_s alarmMenus;
static alarm_s alarm = {0};

static void AlarmLoader(uint8_t num);
static void AlarmSelect(void);

static uint8_t getMaxValForSetting(){
	switch(setting.now)
	{
		case SETTING_NOW_HOUR:
			return appConfig.timeMode  == TIMEMODE_12HR ? 12 : 23;
		case SETTING_NOW_MIN:
			return 59;
	}
	return 1;
}

static void AlarmUp(void){
	setting.val++;
	if(setting.val > getMaxValForSetting())
		setting.val = 0;

	if(setting.now == SETTING_NOW_HOUR && appConfig.timeMode  == TIMEMODE_12HR && setting.val == 0)
		setting.val = 1;
}

static void AlarmDown(void){
	setting.val--;
	if(setting.now == SETTING_NOW_HOUR && appConfig.timeMode  == TIMEMODE_12HR && setting.val == 0)
		setting.val = 12;

	uint8_t max = getMaxValForSetting();
	if(setting.val > max)
		setting.val = max;
}

static void AlarmDraw(void){
	uint8_t w = 5;
	uint8_t x;
	char buff[4];
	buff[1] = 0x00;
	switch(setting.now)
	{
		case SETTING_NOW_EN:
			x = 6;
			w = 8;
			buff[0] = setting.val ? ALARM_SET_CHAR : ALARM_UNSET_CHAR;
			break;
		case SETTING_NOW_HOUR:
			x = 22;
			w = 16;
			break;
		case SETTING_NOW_MIN:
			x = 46;
			w = 16;
			break;
		case SETTING_NOW_AMPM:
			x = 62;
			w = 8;
			buff[0] = setting.val ? CHAR_PM : CHAR_AM;
			break;
		case SETTING_NOW_DAY_MON:
		case SETTING_NOW_DAY_TUE:
		case SETTING_NOW_DAY_WED:
		case SETTING_NOW_DAY_THUR:
		case SETTING_NOW_DAY_FRI:
		case SETTING_NOW_DAY_SAT:
		case SETTING_NOW_DAY_SUN:
			{
				uint8_t dow = setting.now - SETTING_NOW_DAY_MON;
				x = 70 + (8 * dow);
				w = 8;
				buff[0] = setting.val ? dowChars[dow] : '-';
			}
			break;
		default:
			return;
	}

	uint8_t y = 8 + ((Main_Menu.selected * 8) - (Main_Menu.scroll * 8));
	
	OledClearArea(x, y, w);
	
	if(setting.now == SETTING_NOW_HOUR || setting.now == SETTING_NOW_MIN)
		sprintf(buff, PSTR("%02hhu"), setting.val);

	OledShowStr(x,y,buff,OLED_8X8);
}

static void SelectedAlarm()
{
	static uint8_t dayBit;
	static bool wasPM;

	SetMenuFunc(AlarmUp, AlarmSelect, AlarmDown, AlarmLoader);
	Main_Menu.func.draw = AlarmDraw;

	switch(setting.now)
	{
		case SETTING_NOW_NONE:
			Alarmget(Main_Menu.selected, &alarm);
			setting.now = SETTING_NOW_EN;
			setting.val = alarm.enabled;
			break;
		case SETTING_NOW_EN:
			alarm.enabled = setting.val;
			setting.now = SETTING_NOW_HOUR;

			time_s time;
			time.hour = alarm.hour;
			time.ampm = CHAR_24;
			TimeSetMode(&time, appConfig.timeMode);

			setting.val = time.hour;
			
			wasPM = (alarm.hour >= 12);

			break;
		case SETTING_NOW_HOUR:
		{
			uint8_t hour = setting.val;
			uint8_t max = appConfig.timeMode == TIMEMODE_12HR ? 12 : 23;
			if(hour > max)
				hour = max;

			if(appConfig.timeMode  == TIMEMODE_12HR)
			{
				time_s time;
				time.hour = hour;
				time.ampm = wasPM ? CHAR_PM : CHAR_AM;
				TimeSetMode(&time, TIMEMODE_24HR);
				hour = time.hour;
			}

			alarm.hour = hour;

			setting.now = SETTING_NOW_MIN;
			setting.val = alarm.min;
		}
			break;
		case SETTING_NOW_MIN:
			alarm.min = setting.val;
			if(appConfig.timeMode  == TIMEMODE_12HR)
			{
				setting.now = SETTING_NOW_AMPM;
				setting.val = wasPM;
			}
			else
			{
				setting.now = SETTING_NOW_DAY_MON;
				setting.val = alarm.mon;
			}
			dayBit = 0;
			break;
		case SETTING_NOW_AMPM:
		{
			time_s time;
			time.hour = alarm.hour;
			time.ampm = setting.val ? CHAR_PM : CHAR_AM;
			TimeSetMode(&time, TIMEMODE_24HR);
			alarm.hour = time.hour;
			setting.now = SETTING_NOW_DAY_MON;
			setting.val = alarm.mon;
		}
			break;
		case SETTING_NOW_DAY_MON:
		case SETTING_NOW_DAY_TUE:
		case SETTING_NOW_DAY_WED:
		case SETTING_NOW_DAY_THUR:
		case SETTING_NOW_DAY_FRI:
		case SETTING_NOW_DAY_SAT:
		case SETTING_NOW_DAY_SUN:
			if(setting.val)
				alarm.days |= _BV(dayBit);
			else
				alarm.days &= ~_BV(dayBit);
			dayBit++;
			setting.val = (bool)(alarm.days & _BV(dayBit));
			if(setting.now != SETTING_NOW_DAY_SUN)
				setting.now++;
			else
			{
				AlarmSave(Main_Menu.selected, &alarm);
				setting.now = SETTING_NOW_NONE;
				SetMenuFunc(preOption, AlarmSelect, nextOption,AlarmLoader);
				Main_Menu.func.draw = NULL;
			}
			break;
		default:
			break;
	}
}

static void MakeAlarmStr(char* buff, alarm_s* alarm)
{
	time_s time;
	time.hour = alarm->hour;
	time.ampm = CHAR_24;
	TimeSetMode(&time, TIMEMODE_24HR);

	sprintf(buff + 1, PSTR(" %02hhu:%02hhu "), time.hour, alarm->min);

	buff[0] = alarm->enabled ? ALARM_SET_CHAR : ALARM_UNSET_CHAR;
	buff[15] = 0x00;

	for(uint8_t i;i< 7;i++){
		buff[i + 8] = AlarmDayEnabled(alarm->days, i) ? dowChars[i] : '-';
	}	
}

/*
 * 描述：添加alarm的基本组成单元
 * 参数：num ： 		闹钟编号
 * 参数：alarm_s ： 闹钟具体信息
 * 返回值：无	 
 */
static void AddAlarmBasic(uint8_t num, alarm_s *alarm){
	char buff[15];
	MakeAlarmStr(buff, alarm);
	SetMenuOption(num, buff , NULL, SelectedAlarm);
}

/*
 * 描述：设置闹钟界面的页面加载函数
 * 参数：无  
 * 返回值：无	 
 */
static void AlarmLoader(uint8_t num){
	if(num < ALARM_COUNT-1)
	{
		alarm_s alarm2;
		if(setting.now == SETTING_NOW_NONE || num != Main_Menu.selected)
			Alarmget(num, &alarm2);
		else
			memcpy(&alarm2, &alarm, sizeof(alarm_s));
		AddAlarmBasic(num, &alarm2);
	}
	addExitOption();
}
/*
 * 描述：设置闹钟界面的确认函数
 * 参数：无  
 * 返回值：无	 
 */
static void AlarmSelect(){
	bool isExiting = exitSelected();
	GetMenupre(&alarmMenus);
	DoAction(isExiting);
}

/*
 * 描述：设置闹钟界面的按钮函数等菜单基本信息 
 * 参数：无  
 * 返回值：无	 
 */
void AlarmMenu(void){
        SetButtonFunc(menuUp, (bool (*)(void))AlarmSelect, menuDown);
		SetMenuFunc(preOption, AlarmSelect, nextOption,AlarmLoader);
		SetMenuInfo(ALARM_COUNT,MENU_TYPE_STR,PSTR(STR_ALARMMENU));
		SetMenuPre(&alarmMenus,AlarmMenu);
		AnimotionInto(NULL);
}

