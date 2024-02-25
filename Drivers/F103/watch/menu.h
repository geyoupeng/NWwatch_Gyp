#ifndef __MENU_H
#define __MENU_H
#include <stdint.h>
#include "common.h"

void addExitOption(void);
void SetMenuFunc(menu_f btn1,menu_f btn2,menu_f btn3,itemLoader_f loader);
void SetMenuOption(uint8_t num,const char *name,const uint8_t *icon,menu_f actionFunc);
void SetMenuInfo(uint8_t optionCount,menu_type_t menuType,const char *title);
void SetMenuPre(prev_menu_s *prevMenus,menu_f curPrevMenu);
void GetMenupre(prev_menu_s *prevMenus);
bool DoAction(bool anim);
bool exitSelected(void);
void preOption(void);
void nextOption(void);
bool menuUp(void);
bool menuDown(void);
void drawMenu(void);
void back(void);

extern menu_s Main_Menu;
extern s_menuNowSetting setting;

#endif /*__MENU_H */
