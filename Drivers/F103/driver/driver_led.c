#include "common.h"
#include "gpio.h"
#include "drivers.h"
#include <string.h>
static led_s ledRed;
static led_s ledGreen;

static void __UPDATE(led_s *led,uint16_t pin){
	if( (mills() - led->starttime) >= led->flashlen){
		HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_SET);
		led->flashlen = 0;
	}else{
		HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_RESET);
	}
}
void LedUpdate(void){
	__UPDATE(&ledRed,GPIO_PIN_12);
	__UPDATE(&ledGreen,GPIO_PIN_13);
}
void LedInit(void){
	memset(&ledRed,0,sizeof(led_s));
	memset(&ledGreen,0,sizeof(led_s));
}

static void flash(led_s *led,unsigned char len){
	led->flashlen = len;
	led->starttime = mills();
}

void LedFlash(led_t led,unsigned char len){
	if(appConfig.CTRL_LEDs){
		switch (led)
		{
			case LEDRED:
				flash(&ledRed,len);
				break;
			case LEDGREEN:
				flash(&ledGreen,len);
				break;
			default:
				break;
		}
	}
}

bool LedFlashing(void){
	return (ledRed.flashlen || ledGreen.flashlen);
}

void LedStop(void){
	flash(&ledRed,0);
	flash(&ledGreen,0);
	LedUpdate();
}

