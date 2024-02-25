#include <stdbool.h>
#include "drivers.h"
#include "common.h"

#define optionCount 7

static prev_menu_s mainPreMenu;

static void menuSelect(void){
	GetMenupre(&mainPreMenu);
	DoAction(true);
}

static void loader(uint8_t num){
	uint8_t i = 0;
    
	SetMenuOption(i++,PSTR(STR_SETTINGS),&MAIN_MUNE[0][0],SettingMenu);
	SetMenuOption(i++,PSTR(STR_GAMES),&MAIN_MUNE[1][0],GameMenu);
	SetMenuOption(i++,PSTR(STR_FLASHLIGHT),&MAIN_MUNE[2][0],FlashLightMenu);
	SetMenuOption(i++,PSTR(STR_STOPWATCH),&MAIN_MUNE[3][0],StopWatchMenu);
	SetMenuOption(i++,PSTR(STR_MUSIC),&MAIN_MUNE[4][0],MusicMenu);
	SetMenuOption(i++,PSTR(STR_ALARMS),&MAIN_MUNE[5][0],AlarmMenu);
	SetMenuOption(i++,PSTR(EXIT),&MAIN_MUNE[6][0],watchLoad);
}

void mainMenu(void){
	SetDrawFunc(drawMenu);
	SetButtonFunc(menuUp, (bool (*)(void))menuSelect, menuDown);
	SetMenuFunc(preOption, menuSelect, nextOption,loader);
    SetMenuInfo(optionCount,MENU_TYPE_ICON,PSTR(STR_MAINMENU));
	SetMenuPre(&mainPreMenu,mainMenu);
	AnimotionInto(NULL);
}

