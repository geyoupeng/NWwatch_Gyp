#include "common.h"
#include "drivers.h"

static void SettingLoader(uint8_t num){
	uint8_t i = 0;
	SetMenuOption(i++, PSTR(STR_TIMEDATE), &SETTING_MUNE[0][0], TimedateMenu);
	SetMenuOption(i++, PSTR(STR_SOUND), &SETTING_MUNE[1][0], SoundMenu);
	SetMenuOption(i++, PSTR(STR_DISPLAY), &SETTING_MUNE[2][0], DisplayMenu);
	SetMenuOption(i++, PSTR(STR_DIAGNOSTICS), &SETTING_MUNE[3][0], DiagMenu);
	SetMenuOption(i++, PSTR(STR_Reset), menu_rotate, ResetMenu);
	addExitOption();
}

static prev_menu_s prevMenus;

static void SettingSelect(void){
	GetMenupre(&prevMenus);
	DoAction(true);
}

void SettingMenu(void){
	SetButtonFunc(menuUp, (bool (*)(void))SettingSelect, menuDown);
	SetMenuFunc(preOption, SettingSelect, nextOption,SettingLoader);
    SetMenuInfo(6,MENU_TYPE_ICON,PSTR(STR_SETTINGMENU));
	SetMenuPre(&prevMenus,SettingMenu);
	AnimotionInto(NULL);
}

