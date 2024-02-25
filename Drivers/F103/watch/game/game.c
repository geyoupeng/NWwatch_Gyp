#include "common.h"

static prev_menu_s GamePrevMenu;
static bool GameSelect()
{
	GetMenupre(&GamePrevMenu);
	DoAction(true);
    return true;
}

static void GameLoader(uint8_t num)
{
	SetMenuOption(0, PSTR(STR_GAME1) , NULL, Game1Start);
	SetMenuOption(1, PSTR(STR_GAME2) , NULL, Game2Start);
	addExitOption();
}

void GameMenu(void){
	SetDrawFunc(drawMenu);
	SetButtonFunc(menuUp, GameSelect, menuDown);
	SetMenuFunc(nextOption, (void (*)(void))GameSelect, preOption,GameLoader);
    SetMenuInfo(3,MENU_TYPE_STR,PSTR(STR_GAMEMENU));
	SetMenuPre(&GamePrevMenu,GameMenu);
	AnimotionInto(NULL);
}
                          