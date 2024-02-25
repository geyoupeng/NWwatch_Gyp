#include "common.h"
#include <string.h>
#include <stdio.h>

static prev_menu_s prevMenus;		//保存当前层菜单的上一层菜单

void DiagSelect(){
	bool isExiting = exitSelected();
	GetMenupre(&prevMenus);
	DoAction(isExiting);
}

/**
  * 函    数：显示设备当前温度
  * 参    数：无
  * 返 回 值：无
  */
void UpDateTemperature(){
	char buff[24];
	memset(buff,0,sizeof(buff));
	sprintf(buff,PSTR(STR_TEMPERATURE),1,12);
	SetMenuOption(0, buff, NULL, NULL);
}

/**
  * 函    数：显示设备当前电压
  * 参    数：无
  * 返 回 值：无
  */
void UpDateVoltage(){
	char buff[24];
	memset(buff,0,sizeof(buff));
	sprintf(buff,PSTR(STR_BATTERY),3);
	SetMenuOption(1, buff, NULL, NULL);
}

/**
  * 函    数：显示设备当前Fps
  * 参    数：无
  * 返 回 值：无
  */
void UpDateFps(){
	char buff[20];
	memset(buff,0,sizeof(buff));
	char c =  appConfig.showFPS ? CHAR_YES : CHAR_NO;
	sprintf(buff,PSTR(STR_SHOWFPS),c);
	SetMenuOption(2, buff, NULL, NULL);
}


/**
  * 函    数：Diag菜单的 刷新和执行函数
  * 参    数：num：要操作的应用
  * 返 回 值：无
  */
void DiagLoader(uint8_t num){
	switch (num)
	{
		case 0:
			UpDateTemperature();
			return;
		case 1:
			UpDateVoltage();
			return;
		case 2:
			UpDateFps();
			return;
	}
	SetMenuOption(3, PSTR("FW  " FW_VERSION), NULL, NULL);
	SetMenuOption(4, PSTR("USer  " USER_NAME), NULL, NULL);
	addExitOption();
}

/**
  * 函    数：打开Diag后加载菜单函数
  * 参    数：无
  * 返 回 值：无
  */
void DiagMenu(void){
	SetButtonFunc(menuUp, (bool (*)(void))DiagSelect, menuDown);
	SetMenuFunc(preOption, DiagSelect, nextOption,DiagLoader);
    SetMenuInfo(6,MENU_TYPE_STR,PSTR(STR_DIAGNOSTICSMENU));
	SetMenuPre(&prevMenus,DiagMenu);
	AnimotionInto(NULL);
}

