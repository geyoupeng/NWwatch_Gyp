#include "common.h"

static inline uint8_t SetVolume(uint8_t vol){
	vol++;
	if(vol > 3){
		vol = 0;
	}
	return vol;
}

static void SetVolUI(){
	appConfig.volUI = SetVolume(appConfig.volUI);
}

static void SetVolHour(){
	appConfig.volHour = SetVolume(appConfig.volHour);
}

static void SetVolAlarm(){
	appConfig.volAlarm = SetVolume(appConfig.volAlarm);
}

static void SoundLoader(uint8_t num){
	SetMenuOption(0, PSTR(STR_UI), &menu_volume[appConfig.volUI][0], SetVolUI);
	SetMenuOption(1, PSTR(STR_HOURBEEPS), &menu_volume[appConfig.volHour][0], SetVolHour);
	SetMenuOption(2, PSTR(STR_ALARMS),  &menu_volume[appConfig.volAlarm][0], SetVolAlarm);
	addExitOption();
}

static prev_menu_s SoundPrevMenu;

static void SoundSelect(void){
	bool isExiting = exitSelected();
	if(isExiting){
		AppConfigSave();
	}
	GetMenupre(&SoundPrevMenu);
	DoAction(isExiting);
}

/*
 * 描述：设置声音界面的 按钮函数等菜单基本信息 
 * 参数：无	 
 * 返回值：无	 
 */
void SoundMenu(void){
	SetButtonFunc(menuUp, (bool (*)(void))SoundSelect, menuDown);
	SetMenuFunc(preOption, SoundSelect, nextOption,SoundLoader);
    SetMenuInfo(4,MENU_TYPE_ICON,PSTR(STR_SOUNDMENU));
	SetMenuPre(&SoundPrevMenu,SoundMenu);
	AnimotionInto(NULL);
}
