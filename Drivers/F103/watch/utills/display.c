#include "drivers.h"
#include "common.h"
static draw_f drawFunc;

draw_f SetDrawFunc(draw_f newdraw){
	if(newdraw != NULL){
		draw_f olddraw = drawFunc;
		drawFunc = newdraw;
		return olddraw;
	}
    return NULL;
}

void displayupdata(void){
    AnimotionUpdata();
	drawFunc();
}

