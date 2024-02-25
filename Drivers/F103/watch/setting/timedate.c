#include "common.h"
#include "drivers.h"
#include <string.h>
#include <stdio.h>

static timemode_t timeMode;
static timeDate_s timeDateSet;
static bool saved;
static void SelectDate(void);
static void SelectTime(void);
static void SaveTimeDate(void);

static void TimeStrAdd(void)
{
	char buff[17];
	uint8_t timeModeVal = (timeDateSet.time.ampm != CHAR_24) ? 12 : 24;
	sprintf(buff, PSTR("   %hhuhr %02hhu:%02hhu%c"), timeModeVal, timeDateSet.time.hour, timeDateSet.time.mins, timeDateSet.time.ampm);
	SetMenuOption(3, buff, NULL, SelectTime);
}

static void DateStrAdd(void)
{
	char buff[BUFFSIZE_STR_MONTHS + 14];
	char month[BUFFSIZE_STR_MONTHS];
	strcpy(month, months[timeDateSet.date.month]);
	sprintf(buff, PSTR("   %02hhu %s 20%02hhu"), timeDateSet.date.date, month, timeDateSet.date.year);
	SetMenuOption(1, buff, NULL, SelectDate);
}

static void TimedateLoader(uint8_t num){
	(void)num;
	DateStrAdd();
	TimeStrAdd();
	SetMenuOption(5, saved ? PSTR(STR_SAVED) : PSTR(STR_SAVE),  &menu_volume[appConfig.volAlarm][0], SaveTimeDate);
	addExitOption();
}

static void timeDateDraw()
{
	uint8_t x;
	uint8_t y;
	uint8_t w = 16;
	switch(setting.now)
	{
		case SETTING_NOW_DATE:
			x = 30;
			w = 16;
			y = 16;
			break;
		case SETTING_NOW_MONTH:
			x = 54;
			w = 24;
			y = 16;
			break;
		case SETTING_NOW_YEAR:
			x = 102;
			w = 16;
			y = 16;
			break;
		case SETTING_NOW_TIMEMODE:
			x = 30;
			w = 16;
			y = 32;
			break;
		case SETTING_NOW_HOUR:
			x = 70;
			w = 16;
			y = 32;
			break;
		case SETTING_NOW_MIN:
			x = 94;
			w = 16;
			y = 32;
			break;
		case SETTING_NOW_AMPM:
			x = 110;
			w = 8;
			y = 32;
			break;
		default:
			return;
	}

	OledClearArea(x, y, w);

	char buff[5];

	if(setting.now == SETTING_NOW_MONTH)
		strcpy(buff, months[setting.val]);
	else if(setting.now == SETTING_NOW_AMPM)
	{
		buff[0] = setting.val ? CHAR_AM : CHAR_PM;
		buff[1] = 0x00;
	}
	else if(setting.now == SETTING_NOW_TIMEMODE)
	{
		// Setting these bytes individually creates smaller code
		if(setting.val)
		{
			buff[0] = '1';
			buff[1] = '2';
		}
		else
		{
			buff[0] = '2';
			buff[1] = '4';
		}
		buff[2] = 'h';
		buff[3] = 'r';
		buff[4] = 0x00;
	}
	else
		sprintf(buff, PSTR("%02hhu"), setting.val);

	OledShowStr(x,y,buff,OLED_8X8);
}

static uint8_t GetMaxValForSetting()
{
	switch(setting.now)
	{
		case SETTING_NOW_TIMEMODE:
		case SETTING_NOW_AMPM:
			return 1;
		case SETTING_NOW_HOUR:
		{
			if(timeMode == TIMEMODE_12HR)
				return 12;
			return 23;
		}
		case SETTING_NOW_DATE:
			return 31;
		case SETTING_NOW_MONTH:
			return 11;
		case SETTING_NOW_YEAR:
			return 99;
	}
	return 59;
}

static prev_menu_s TimedatePrevMenu;

static void TimedateSelect(void){
	bool isExiting = exitSelected();
	GetMenupre(&TimedatePrevMenu);
	DoAction(isExiting);
}

/*
 * 描述：设置日期界面的按建1执行函数 
 * 参数：无	 
 * 返回值：无	 
 */
static void TimedateUp(void){
	preOption();

	if( (Main_Menu.selected == 0) || (Main_Menu.selected == 2) || (Main_Menu.selected == 4))
		preOption();
}

/*
 * 描述：设置日期界面的按建3执行函数 
 * 参数：无	 
 * 返回值：无	 
 */
static void TimedateDown(void){
	nextOption();
	if( (Main_Menu.selected == 0) || (Main_Menu.selected == 2) || (Main_Menu.selected == 4))
		nextOption();
}

static void TimeOpenDown()
{
	setting.val--;
	
	if(setting.val == 0)
	{
		if(setting.now == SETTING_NOW_HOUR && timeMode == TIMEMODE_12HR)
			setting.val = 12;
		else if(setting.now == SETTING_NOW_DATE)
			setting.val = 31;
	}
	
	uint8_t max = GetMaxValForSetting();
	if(setting.val > max) 	// Overflow
		setting.val = max;

	saved = false;
}

static void TimeOpenup()
{
	setting.val++;
	if(setting.val > GetMaxValForSetting())
		setting.val = 0;

	if(((setting.now == SETTING_NOW_HOUR && timeMode == TIMEMODE_12HR) || setting.now == SETTING_NOW_DATE) && setting.val == 0)
		setting.val = 1;

	saved = false;
}

static void BeginSelect(void){
	Main_Menu.func.draw = timeDateDraw;
	SetMenuFunc(TimeOpenup, TimedateSelect, TimeOpenDown,TimedateLoader);
}

static void EndSelect(void){
	setting.now = SETTING_NOW_NONE;
	Main_Menu.func.draw = NULL;
	SetMenuFunc(TimedateUp, TimedateSelect, TimedateDown,TimedateLoader);
}

static void SelectDate(void)
{
	BeginSelect();
	
	switch(setting.now)
	{
		case SETTING_NOW_NONE:
			setting.now = SETTING_NOW_DATE;
			setting.val = timeDateSet.date.date;
			break;
		case SETTING_NOW_DATE:
			timeDateSet.date.date = setting.val;
			if(timeDateSet.date.date < 1)
				timeDateSet.date.date = 1;
			setting.now = SETTING_NOW_MONTH;
			setting.val = timeDateSet.date.month;
			break;
		case SETTING_NOW_MONTH:
		{
			timeDateSet.date.month = (month_t)setting.val;
			uint8_t numDays = TimeMonthDayCount(timeDateSet.date.month, timeDateSet.date.year);
			if(timeDateSet.date.date > numDays)
				timeDateSet.date.date = numDays;
			setting.now = SETTING_NOW_YEAR;
			setting.val = timeDateSet.date.year;
		}
			break;
		default: // Also SETTING_NOW_YEAR
			timeDateSet.date.year = setting.val;
			timeDateSet.date.day = TimeDow(timeDateSet.date.year, timeDateSet.date.month, timeDateSet.date.date);

			EndSelect();
			break;
	}
}

static void SaveTimeDate(void)
{
	appConfig.timeMode = timeMode;
	AppConfigSave();

	SetTime(&timeDateSet);
	memcpy(&timeDateSet, &timeDate, sizeof(timeDate_s));
	saved = true;
	GetNextAlarm();
}

static void SelectTime(void)
{
	BeginSelect();

	switch(setting.now)
	{
		case SETTING_NOW_NONE:
			setting.now = SETTING_NOW_TIMEMODE;
			setting.val = timeMode;
			break;
		case SETTING_NOW_TIMEMODE:
			timeMode = (timemode_t)setting.val;
			TimeSetMode(&timeDateSet.time, timeMode);
			setting.val = timeDateSet.time.hour;
			setting.now = SETTING_NOW_HOUR;
			break;
		case SETTING_NOW_HOUR:
			timeDateSet.time.hour = setting.val;
			setting.val = timeDateSet.time.mins;
			setting.now = SETTING_NOW_MIN;
			break;
		case SETTING_NOW_MIN:
			timeDateSet.time.mins = setting.val;
			if(timeMode == TIMEMODE_12HR)
			{
				setting.now = SETTING_NOW_AMPM;
				setting.val = (timeDateSet.time.ampm == CHAR_AM);
			}
			else
			{
				timeDateSet.time.ampm = CHAR_24;
				EndSelect();
			}
			break;
		default: 	// Also SETTING_NOW_AMPM
			timeDateSet.time.ampm = setting.val ? CHAR_AM : CHAR_PM;
			EndSelect();
			break;
	}
}

/*
 * 描述：设置日期界面的按钮函数等菜单基本信息 
 * 参数：无	 
 * 返回值：无	 
 */
void TimedateMenu(void){
	memcpy(&timeDateSet,&timeDate,sizeof(timeDate_s));
	timeMode = appConfig.timeMode;
	saved = false;
	SetButtonFunc(menuUp, (bool (*)(void))TimedateSelect, menuDown);
	SetMenuFunc(TimedateUp, TimedateSelect, TimedateDown,TimedateLoader);
    SetMenuInfo(7,MENU_TYPE_STR,PSTR(STR_TIMEDATEMENU));
	SetMenuPre(&TimedatePrevMenu,TimedateMenu);

	Main_Menu.selected = 1;
	AnimotionInto(NULL);
}

