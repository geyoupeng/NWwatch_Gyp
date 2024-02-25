#ifndef __TYPEDEF_H
#define __TYPEDEF_H

#include <stdbool.h>
#include <stdint.h>
#define MAX_MENU_ITEM 7

//*******************最重要的结构体 菜单结构体***
typedef struct{
	uint8_t now;
	uint8_t val;
}s_menuNowSetting;

typedef enum{
	MENU_TYPE_STR,
	MENU_TYPE_ICON
}menu_type_t;

typedef void (*draw_f)(void);
typedef void (*menu_f)(void);
typedef bool (*button_f)(void);
typedef void (*itemLoader_f)(uint8_t);

typedef enum{
	OPERATION_DRAWICON,       
	OPERATION_DRAWNAME_ICON,  
	OPERATION_DRAWNAME_STR,   
	OPERATION_ACTION    	  
}operation_t;

typedef struct{
	int data;
	operation_t op;  
  	uint8_t id;
}operation_s;

typedef struct{
	menu_f btn1;
	menu_f btn2;
	menu_f btn3;
	draw_f draw;
	itemLoader_f loader;
}menuFuncs_t;

typedef struct{
	uint8_t lastSelected;
	menu_f last;
}prev_menu_s;

typedef struct{
	uint8_t selected;
	uint8_t scroll;
	uint8_t optionCount;
	bool isOpen;
	const char* title;
	menu_type_t menuType;
	menuFuncs_t func;
	menu_f prevMenu;
}menu_s;

//*******************第二重要的 日期结构体***
typedef struct{
	uint8_t x;		
	uint8_t y;
	uint8_t font;
	uint8_t w;		
	uint8_t h;
	uint8_t offset;		
	uint8_t val;
	uint8_t maxval;
	bool moving;
}tickerData_t;

typedef struct{
	uint8_t secs;		
	uint8_t mins;
	uint8_t hour;
	char ampm;
}time_s;

typedef enum{
	DAY_MON = 0,
	DAY_TUE = 1,
	DAY_WED = 2,
	DAY_THU = 3,
	DAY_FRI = 4,
	DAY_SAT = 5,
	DAY_SUN = 6,
}day_t;

typedef enum{
	MONTH_JAN = 0,
	MONTH_FEB = 1,
	MONTH_MAR = 2,
	MONTH_APR = 3,
	MONTH_MAY = 4,
	MONTH_JUN = 5,
	MONTH_JUL = 6,
	MONTH_AUG = 7,
	MONTH_SEP = 8,
	MONTH_OCT = 9,
	MONTH_NOV = 10,
	MONTH_DEC = 11
}month_t;
	
typedef struct{
	day_t day;		
	uint8_t date;
	month_t month;
	uint8_t year;
}date_s;

typedef struct{
	time_s time;
	date_s date;
}timeDate_s;

typedef enum
{
	TIMEMODE_24HR = 0,
	TIMEMODE_12HR = 1
}timemode_t;


//ALarm 结构体
//闹钟结构体
typedef struct{
	uint8_t hour;
	uint8_t min;
	union{
		uint8_t days;
		struct{
			bool mon:1;
			bool tues:1;
			bool wed:1;
			bool thurs:1;
			bool fri:1;
			bool sat:1;
			bool sun:1;
			bool enabled:1;
		};
	};
}alarm_s;

//APPConfig 结构体
// Could use bitfields for the bools to save a few bytes of RAM and EEPROM, but uses an extra ~82 bytes of flash
typedef struct{
//	uint8_t sleepTimeout;
	bool invert;
	bool animations;
	bool display180;
    bool CTRL_LEDs;
	bool showFPS;
	timemode_t timeMode;
	union {
		uint8_t volumes;
		struct{ 								// get rid of these bitfields?
			uint8_t volUI:2;
			uint8_t volAlarm:2;
			uint8_t volHour:2;
			uint8_t brightness:2;
			};
		};
}appconfig_s;


//datetime 枚举结构体
typedef enum{
	SETTING_NOW_NONE = 0,
	SETTING_NOW_TIMEMODE,	
	SETTING_NOW_HOUR,	
 	SETTING_NOW_MIN,	
 	SETTING_NOW_AMPM,	
	SETTING_NOW_DATE,	
	SETTING_NOW_MONTH,
	SETTING_NOW_YEAR	
}settingDate_t;

//stopwatch 枚举结构体
typedef enum
{
	STATE_STOPPED = 0,
	STATE_TIMING
} stopwatch_state_t;

//utill
//animotion 结构体
typedef struct{
	bool active;
	uint8_t offsetY;
	void (*animOnComplete)(void);
	bool goingOffScreen;
}anim_s;
//button 结构体
typedef enum{
	BTN1 = 0,
	BTN2 = 1,
	BTN3 = 2,	
	BTNCOUNT = 3
}but_t;
	
typedef struct{
	uint32_t counter;
	const uint32_t *tune;
	bool processed;
	button_f onPress;
}button_s;

#endif //__TYPEDEF_H
