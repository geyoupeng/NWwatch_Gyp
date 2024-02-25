#include "common.h"
#include "drivers.h"
static bool resetFlag = false;

static void ResetDraw(void)
{
	if(Main_Menu.selected == 0)
	{
		char ch = 0;		
		ch = resetFlag ? 'Y' : 'N';	
		OledShowChar(60,30,ch,OLED_8X8);
	}
}

static void SetReset(void){
	resetFlag = !resetFlag;
}

static void ResetLoader(uint8_t num){
	SetMenuOption(0, PSTR(STR_RESET), menu_rotate, SetReset);
	addExitOption();
}

static prev_menu_s ResetPrevMenu;

static void ResetSelect(void){
	bool isExiting = exitSelected();
	if(isExiting){
		Main_Menu.func.draw = NULL;
		if(resetFlag){
			AppConfigReset();
			NVIC_SystemReset();
		}
	}
	GetMenupre(&ResetPrevMenu);
	DoAction(isExiting);
}

/*
 * 描述：设置复位界面的 按钮函数等菜单基本信息 
 * 参数：无	 
 * 返回值：无	 
 */
void ResetMenu(void){
	SetButtonFunc(menuUp, (bool (*)(void))ResetSelect, menuDown);
	SetMenuFunc(preOption, ResetSelect, nextOption,ResetLoader);
    SetMenuInfo(2,MENU_TYPE_ICON,PSTR(STR_RESETMENU));
	SetMenuPre(&ResetPrevMenu,ResetMenu);
	Main_Menu.func.draw = ResetDraw;
	AnimotionInto(NULL);
}

