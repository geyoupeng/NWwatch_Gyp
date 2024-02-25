#include "common.h"
#include "drivers.h"

static void SetBrightness()
{
 	uint8_t brightness = appConfig.brightness;
    brightness++;
	if(brightness > 3)
		brightness = 0;
	appConfig.brightness = brightness;
	
	int _brightness=0;

	//设置屏幕亮度
	switch(brightness)       
	{
		case 0:	_brightness= 0x01; break;
		case 1:	_brightness= 0x0f; break;
		case 2:	_brightness= 0x5f; break;
		case 3:	_brightness= 0xff; break;
	}	
	
	OledSendCmd(0x81);				//--set contrast control register    
	OledSendCmd(_brightness);		// Set SEG Output Current Brightness 
 
}

static void SetInvert(){
	appConfig.invert = !appConfig.invert;
	if(appConfig.invert){
		OledSendCmd(0xA7);	
	}else{
		OledSendCmd(0xA6);	
	}
	OledSendCmd(0xAF);	
}

static void SetRotate(){
	appConfig.display180 = !appConfig.display180;
	if(appConfig.display180){
		OledSendCmd(0xC0);
		OledSendCmd(0xA0);	
	}else{
		OledSendCmd(0xC8);
		OledSendCmd(0xA1);
	}
}

static void SetAnimation(){
    appConfig.animations = !appConfig.animations;
}

static void SetLeds(){
	 appConfig.CTRL_LEDs = !appConfig.CTRL_LEDs;
}

static void DisplayLoader(uint8_t num){
	uint8_t i = 0;
	SetMenuOption(i++, PSTR(STR_BRIGHTNESS), &menu_brightness[appConfig.brightness][0], SetBrightness);
	SetMenuOption(i++, PSTR(STR_INVERT), menu_invert, SetInvert);
	SetMenuOption(i++, PSTR(STR_ROTATE), menu_rotate, SetRotate);
	SetMenuOption(i++, PSTR(STR_ANIMATIONS), &menu_anim[appConfig.animations][0], SetAnimation);
	SetMenuOption(i++, PSTR(STR_LEDS), &menu_LEDs[appConfig.CTRL_LEDs][0], SetLeds);
	addExitOption();
}

static prev_menu_s prevMenus;

static void DisplaySelect(void){
	bool isExiting = exitSelected();
	if(isExiting)
		AppConfigSave();
	GetMenupre(&prevMenus);
	DoAction(isExiting);
}

void DisplayMenu(void){
	SetButtonFunc(menuUp, (bool (*)(void))DisplaySelect, menuDown);
	SetMenuFunc(preOption, DisplaySelect, nextOption,DisplayLoader);
    SetMenuInfo(6,MENU_TYPE_ICON,PSTR(STR_DISPLAYMENU));
	SetMenuPre(&prevMenus,DisplayMenu);
	AnimotionInto(NULL);
}
 
