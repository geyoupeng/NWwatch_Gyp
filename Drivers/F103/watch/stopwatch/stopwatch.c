#include "common.h"
#include "drivers.h"
#include <string.h>

static stopwatch_state_t state;
static uint32_t timer;
static uint32_t lastMS;

static void StopWatchDraw(){
	
	uint8_t num1;
	uint8_t num2;
	uint8_t num3;
	uint32_t timer1=timer/1;     
	uint32_t secs = timer1 / 1000;

	if(timer1 < 3600000)
	{
		num3 = (timer1 % 1000) / 10; // ms
		num2 = secs % 60; // secs
		num1 = secs / 60; // mins
	}
	else
	{
		num3 = secs % 60; // secs
		num2 = (secs / 60) % 60; // mins
		num1 = (secs / 3600); // hours
	}

	OledShowImage(8, 16, MIDFONT_WIDTH, MIDFONT_HEIGHT, OLED_F32X16[(num1/10)*4], 0);
	OledShowImage(24, 16, MIDFONT_WIDTH, MIDFONT_HEIGHT, OLED_F32X16[(num1%10)*4], 0);
	OledShowImage(56, 16, MIDFONT_WIDTH, MIDFONT_HEIGHT, OLED_F32X16[(num2/10)*4], 0);
	OledShowImage(72, 16, MIDFONT_WIDTH, MIDFONT_HEIGHT, OLED_F32X16[(num2%10)*4], 0);
	OledShowImage(88, 24, SMLFONT_WIDTH, SMLFONT_HEIGHT,OLED_F24X16[(num3/10)*3], 0);
	OledShowImage(104, 24, SMLFONT_WIDTH, SMLFONT_HEIGHT,OLED_F24X16[(num3%10)*3], 0);
	OledShowImage(40, 16,MIDFONT_WIDTH,MIDFONT_HEIGHT,OLED_F32X16[10*4],0);
	
	// Draw time
	char buff[8];
	sprintf(buff, PSTR(TIME_FORMAT_SMALL), timeDate.time.hour, timeDate.time.mins, timeDate.time.ampm);
	OledShowStr(48, 0,buff,OLED_16X8);
}

static bool StopWatchExit(){		
	watchLoad();
    return true;
}

static bool StopWatchSelect(){
	state = !state;
	lastMS = mills();
	return true;
}

static bool StopWatchReset(){
	timer = 0;
	lastMS = mills();
	return true;
}

void StopWatchUpdate()
{
	if(state == STATE_TIMING)
	{
		uint32_t now = mills();
		timer += now - lastMS;
		lastMS = now;
		if(timer > 359999999) // 99 hours, 59 mins, 59 secs, 999 ms
			timer = 359999999;
	}
}

void StopWatchMenu(void){
	memset(&Main_Menu,0,sizeof(Main_Menu));
	SetDrawFunc(StopWatchDraw);
	SetButtonFunc(StopWatchReset, StopWatchSelect, StopWatchExit);
}

