#include "common.h"
#include "drivers.h"
#include <string.h>
static volatile uint16_t strobe = 0;

static void FlashLightDraw(){
	static bool invert;
	static uint32_t lastStrobe;

	if (strobe)
	{
		uint32_t now = mills();
		if (now - lastStrobe >= strobe)
		{
			lastStrobe = now;
			invert = !invert;
			if(invert){
				OledSendCmd(0xA7);	
			}else{
				OledSendCmd(0xA6);	
			}
			OledSendCmd(0xAF);
			LedFlash(invert ? LEDGREEN : LEDRED, 20);
		}
		return;
	}

	OledSendCmd(0xA7);
	OledSendCmd(0xAF);
	LedFlash(LEDGREEN, 20);
	LedFlash(LEDRED, 20);
}

static bool FlashLightExit(){		
	if(appConfig.invert){
		OledSendCmd(0xA7);	
	}else{
		OledSendCmd(0xA6);	
	}
	OledSendCmd(0xAF);
	watchLoad();
    return true;
}

static bool FlashLightSelect(){
	if (strobe < 500)
		strobe += 50 * ((strobe / 50) + 1);
	else
		strobe = 0;
     return true;
}

void FlashLightMenu(void){
	memset(&Main_Menu,0,sizeof(Main_Menu));
	strobe = 0;
	SetDrawFunc(FlashLightDraw);
	SetButtonFunc(FlashLightExit, FlashLightSelect, FlashLightExit);
}

