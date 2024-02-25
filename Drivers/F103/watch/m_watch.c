#include "drivers.h"
#include "common.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

timeDate_s timeDate = {{50,11,14,CHAR_24},{0,19,1,24}};

static void drawTickerNum(tickerData_t *data);

static void drawDate(){
	char day[BUFFSIZE_STR_DAYS];
    strcpy(day,days[timeDate.date.day]);
    
    char month[BUFFSIZE_STR_MONTHS];
    strcpy(month,months[timeDate.date.month]);
    
    char buff[BUFFSIZE_DATE_FORMAT];
    sprintf(buff,PSTR(DATE_FORMAT),day,timeDate.date.date,month,timeDate.date.year);
	OledPrintf(0, 0, OLED_16X8, buff);
}

static void ticker(void){
	static uint8_t yPos;
	static uint8_t yPos_secs;
	static bool moving = false;
	static bool moving2[5] = {0};

	static uint8_t secs = 50;
	static uint8_t mins = 11;
	static uint8_t hour = 14;
	if(timeDate.time.secs != secs){
		yPos = 0;
        yPos_secs = 0;
		moving = true;

		moving2[0] = (timeDate.time.secs/10)!=(secs/10);
		moving2[1] = (timeDate.time.mins%10)!=(mins%10);
		moving2[2] = (timeDate.time.mins/10)!=(mins/10);
		moving2[3] = (timeDate.time.hour%10)!=(hour%10);
		moving2[4] = (timeDate.time.hour/10)!=(hour/10);

		secs = timeDate.time.secs;
		mins = timeDate.time.mins;
		hour = timeDate.time.hour;

		BuzzerPlay(tuneHours,VOL_HOUR,PRIO_HOUR);
	}

	if(moving){
		if(yPos <= 3){
			yPos++;
		}else if(yPos <= 6){
			yPos+=3;
		}else if(yPos <= 16){
			yPos+=5;
		}else if(yPos <= 22){
			yPos+=3;
		}else if(yPos <= MIDFONT_HEIGHT){
			yPos++;
		}

		if(yPos >= MIDFONT_HEIGHT){
			yPos = 255;
		}

		if(yPos_secs <= 1){
			yPos_secs++;
		}else if(yPos_secs <= 13){
            yPos_secs+=3;
        }else if(yPos_secs <= SMLFONT_HEIGHT){
            yPos_secs++;
        }

		if(yPos_secs > SMLFONT_HEIGHT){
			yPos_secs = 255;
		}
		
		if( (yPos > MIDFONT_HEIGHT) && (yPos_secs > SMLFONT_HEIGHT) ){

			yPos = 0;
			yPos_secs = 0;
			moving = false;
			memset(moving2,0,sizeof(moving2));
		}
	}

	tickerData_t data;

    //秒
	data.y = 24;
	data.w = SMLFONT_WIDTH;
	data.h = SMLFONT_HEIGHT;

	data.x = 88;
	data.val = timeDate.time.secs/10;
	data.maxval = 5;
	data.moving = moving2[0];
	data.offset = yPos_secs;
	data.font = OLED_24X16;
	drawTickerNum(&data);

	data.x = 104;
	data.val = timeDate.time.secs%10;
	data.maxval = 9;
	data.moving = moving;
	data.offset = yPos_secs;
	drawTickerNum(&data);

	data.y = 16;
	data.w = MIDFONT_WIDTH;
	data.h = MIDFONT_HEIGHT;
    //分
    data.x = 72;
	data.val = timeDate.time.mins%10;
	data.maxval = 9;
	data.moving = moving2[1];
	data.offset = yPos;
	data.font = OLED_32X16;
	drawTickerNum(&data);
    
    data.x = 56;
	data.val = timeDate.time.mins/10;
	data.maxval = 5;
	data.moving = moving2[2];
	data.offset = yPos;
	drawTickerNum(&data);
    
    //时
    data.x = 24;
	data.val = timeDate.time.hour%10;
	data.maxval = 9;
	data.moving = moving2[3];
	data.offset = yPos;
	drawTickerNum(&data);
    
    data.x = 8;
	data.val = timeDate.time.hour/10;
	data.maxval = 5;
	data.moving = moving2[4];
	data.offset = yPos;
	drawTickerNum(&data);

	// Draw AM/PM character
	char tmp = 0;
	tmp = timeDate.time.ampm;
	OledShowChar(80, 16,tmp,OLED_8X8);
    
    if(timeDate.time.secs%2 == 0){
        OledShowImage(40, 16,MIDFONT_WIDTH,MIDFONT_HEIGHT,OLED_F32X16[10*4],0);
    }else{
        OledShowImage(40, 16,MIDFONT_WIDTH,MIDFONT_HEIGHT,OLED_F32X16[11*4],0);
    }
}

void drawWatch(void){
	ticker();
	drawDate();
}

static void drawTickerNum(tickerData_t *data){
	if(!data->moving || data->offset == 0 || data->offset == 255 ){
		if(data->font == OLED_24X16){
			OledShowChar(data->x, data->y, data->val*3, OLED_24X16);
		}else{
			OledShowChar(data->x, data->y, data->val*4, OLED_32X16);
		}
	}else{
		uint8_t prev = data->val - 1;
		if(prev == 255){
			prev = data ->maxval;
		}

		if(data->font == OLED_24X16){
			OledShowImage(data->x, data->y,data->w,data->h,OLED_F24X16[data->val*3],data->offset-SMLFONT_HEIGHT);
			OledShowImage(data->x, data->y,data->w,data->h,OLED_F24X16[prev*3],data->offset);
		}else{
			OledShowImage(data->x, data->y,data->w,data->h,OLED_F32X16[data->val*4],data->offset-MIDFONT_HEIGHT);
			OledShowImage(data->x, data->y,data->w,data->h,OLED_F32X16[prev*4 ],data->offset);
		}
	}

}

