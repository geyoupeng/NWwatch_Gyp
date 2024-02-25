#include <stdbool.h>
#include "drivers.h"
#include "common.h"
static prev_menu_s prevMenus;

static void WatchSelect(void){
	mainMenu();
}

void watchLoad(void){
	SetDrawFunc(drawWatch);
	SetButtonFunc(NULL, (bool (*)(void))WatchSelect, NULL);
	SetMenuPre(&prevMenus,watchLoad);
	
	AnimotionInto(NULL);
}

