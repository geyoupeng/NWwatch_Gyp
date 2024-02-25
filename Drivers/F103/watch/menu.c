#include "drivers.h"
#include "common.h"
#include <stdbool.h>
#include <string.h>
#define BUFFSIZE_STR_MENU 16
menu_s Main_Menu = {0};
operation_s Main_operation = {0};
s_menuNowSetting setting = {0};

void SetMenuFunc(menu_f btn1,menu_f btn2,menu_f btn3,itemLoader_f loader){
	Main_Menu.func.btn1 = btn1;
	Main_Menu.func.btn2 = btn2;
	Main_Menu.func.btn3 = btn3;
	Main_Menu.func.loader = loader;
}

void SetMenuInfo(uint8_t optionCount,menu_type_t menuType,const char *title){
	Main_Menu.optionCount = optionCount;
	Main_Menu.menuType = menuType;
	Main_Menu.title = title;
}

/* 功能:判断当前项目是否为该菜单的最后一个项目
 * 参数:无
 * 返回值：ture：是；	false：否
 *
 */
bool exitSelected(void){
	return Main_Menu.selected == Main_Menu.optionCount - 1;
}

void back()
{
	Main_Menu.prevMenu != NULL ? Main_Menu.prevMenu() : watchLoad();
}

void addExitOption(){
	SetMenuOption(Main_Menu.optionCount - 1,PSTR(EXIT),&MAIN_MUNE[6][0],back);
}

/* 功能:检查字符菜单界面是否需要滚动
 * 参数:无
 * 返回值：无
 *
 */
static void checkScroll(){
	uint8_t scroll = Main_Menu.scroll;
	if(Main_Menu.selected >= scroll + MAX_MENU_ITEM){
		scroll = (Main_Menu.selected+1) - MAX_MENU_ITEM;
	}else if(Main_Menu.selected < scroll){
		scroll = Main_Menu.selected;
	}
	Main_Menu.scroll = scroll;
}

/* 功能:移动到当前项目的下一个项目
 * 参数:无
 * 返回值：无
 *
 */
void nextOption(void){
	Main_Menu.selected++;
	if(Main_Menu.selected >= Main_Menu.optionCount){
		Main_Menu.selected = 0;
	}
	checkScroll();
}

/* 功能:移动到当前项目的上一个项目
 * 参数:无
 * 返回值：无
 *
 */
void preOption(void){
	Main_Menu.selected--;
	if(Main_Menu.selected >= Main_Menu.optionCount){
		Main_Menu.selected = Main_Menu.optionCount - 1;
	}
	checkScroll();
}

static void loader(operation_t op,uint8_t num,int data){
	Main_operation.data = data;
	Main_operation.op = op;
	Main_operation.id = num;

	if(Main_Menu.func.loader){
		Main_Menu.func.loader(num);
	}
}

static void doBtn(menu_f btn){
	if(!AnimotionActive() && btn != NULL){
		btn();
	}
}

bool DoAction(bool anim){
	loader(OPERATION_ACTION,Main_Menu.selected, anim);
    return true;
}

bool menuUp(void){
	doBtn(Main_Menu.func.btn1);
	return true;
}

bool menuDown(void){
	doBtn(Main_Menu.func.btn3);
	return true;
}

void SetMenuPre(prev_menu_s *prevMenus,menu_f curPrevMenu){
	if(prevMenus != NULL){
		if(Main_Menu.prevMenu != curPrevMenu) 
			prevMenus->last = Main_Menu.prevMenu;

		Main_Menu.selected = prevMenus->lastSelected;
		Main_Menu.prevMenu = curPrevMenu;
	}
}

void GetMenupre(prev_menu_s *prevMenus){
	if(!exitSelected()){
		prevMenus->lastSelected = Main_Menu.selected;
	}else{
		Main_Menu.prevMenu = prevMenus->last;
		prevMenus->lastSelected = 0;
	}

}

bool MenuOption(uint8_t num,const char *name,const uint8_t *icon,menu_f actionFunc){
	switch (Main_operation.op)
	{
		case OPERATION_DRAWICON:
			OledShowImage(Main_operation.data, 16, 32, 32, icon, 0);
			break;
        case OPERATION_DRAWNAME_ICON:
			OledShowStr(0, 56, (char *)name, OLED_8X8);
			break;
		case OPERATION_DRAWNAME_STR:
			OledShowStr(6, Main_operation.data, (char *)name, OLED_8X8);
			break;
		case OPERATION_ACTION:
			Main_operation.data?AnimotionLeave(actionFunc):actionFunc();
			break;
		default:	break;
	}
    
    return true;
}

void SetMenuOption(uint8_t num,const char *name,const uint8_t *icon,menu_f actionFunc){
	if(num != Main_operation.id){
		return;
	}

	char buff[BUFFSIZE_STR_MENU];
	strcpy(buff,name);
	MenuOption(num,name,icon,actionFunc);
}

static void MenuDrawTitle(void){
	char buff[BUFFSIZE_STR_MENU];
	memset(buff,' ',sizeof(buff));
	strcpy((buff+( 8-(strlen(Main_Menu.title)/2) ) ),Main_Menu.title);
	OledShowStr(0, 0, buff, OLED_8X8);
}
static void MenuDrawButton(void){
	OledShowImage(46, 14, 36, 8, UP, 0);
	OledShowImage(46, 42, 36, 8, DOWN, 0);
}	

/*
 ** 函数：  绘制图像菜单
 **	返回值：无
 ** 参数：		无
 */
void MenuDrawIcon(void){
	uint8_t speed;
	static int Anim_x = 48;

	int x = 48 - Main_Menu.selected * 48;

	if(appConfig.animations){
		if(x > Anim_x){
			speed = ( (x - Anim_x)/4) + 1;
			if(speed > 16){
				speed = 16;
			}
			Anim_x += speed;

			if(x <= Anim_x){
				Anim_x = x;
			}
		}else if(x < Anim_x){
			speed = ( (Anim_x - x)/4) + 1;
			if(speed > 16){
				speed = 16;
			}
			Anim_x -= speed;

			if( Anim_x <= x){
				Anim_x = x;
			}
		}else{

		}
	}else{	
		Anim_x = x;
	}

	x = Anim_x;
	for(uint8_t i = 0;i<Main_Menu.optionCount;i++){
		if( (x>-32) && (x<128) ){
			loader(OPERATION_DRAWICON,i,x);
		}
		x+=48;
	}
	MenuDrawTitle();
	MenuDrawButton();
	loader(OPERATION_DRAWNAME_ICON,Main_Menu.selected,0);
}

/*
 ** 函数：  绘制字符串菜单
 **	返回值：无
 ** 参数：		无
 */
void MenuDrawStr(void){
	MenuDrawTitle();
	uint8_t scroll = Main_Menu.scroll;
	uint8_t count =( (MAX_MENU_ITEM < Main_Menu.optionCount) ? MAX_MENU_ITEM : Main_Menu.optionCount ) + scroll;
	for(uint8_t i = scroll;i< count;i++){
		uint8_t y = 8 + (8 * (i - scroll));
		if(i == Main_Menu.selected){
			OledShowStr(0, y, ">", OLED_8X8);
		}
		loader(OPERATION_DRAWNAME_STR, i, y);
	}
}

/*
 ** 函数：  进入主菜单后所有菜单的绘制函数
 **	返回值：无
 ** 参数：		无
 */
void drawMenu(void){
	if(Main_Menu.menuType == MENU_TYPE_ICON){
		MenuDrawIcon();
	}else if(Main_Menu.menuType == MENU_TYPE_STR){
		MenuDrawStr();
	}

	if(Main_Menu.func.draw != NULL)
		Main_Menu.func.draw();
}

