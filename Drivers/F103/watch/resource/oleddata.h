#ifndef __OLEDDATA_H
#define __OLEDDATA_H
#include <stdint.h>

#define BUFFSIZE_STR_DAYS	4
#define BUFFSIZE_STR_MONTHS	4

#define SMLFONT_WIDTH 16
#define SMLFONT_HEIGHT 24

#define MIDFONT_WIDTH 16
#define MIDFONT_HEIGHT 32

typedef enum{
	OLED_8X8 = 0,
	OLED_16X8,
	OLED_24X16,
	OLED_32X16
}OLEDFont_t;

#define OLED_CHN_CHAR_WIDTH	(3)

typedef struct{
	char index[OLED_CHN_CHAR_WIDTH+1];	//汉字索引
	uint8_t Data[32];					//汉字对应的字模数据
}ChineseCell_t;


extern const unsigned char livesImg[];

extern const char dowChars[];

extern const uint8_t OLED_F8X8[][8];
extern const uint8_t OLED_F16X8[][16];
extern const ChineseCell_t OLED_CF16X16[];
extern const uint8_t OLED_F32X16[][16];
extern const uint8_t OLED_F24X16[][16];
extern const char days[7][BUFFSIZE_STR_DAYS];
extern const char months[12][BUFFSIZE_STR_MONTHS];

extern const unsigned char MAIN_MUNE[][128];

extern const unsigned char SETTING_MUNE[][128];
extern const uint8_t menu_brightness[][128];
extern const uint8_t menu_LEDs[][128];
extern const uint8_t menu_invert[];
extern const uint8_t menu_rotate[];
extern const uint8_t menu_anim[][128];
extern const uint8_t menu_volume[][128];

extern const unsigned char UP[];
extern const unsigned char DOWN[];
extern const unsigned char ClearImage[];

#endif
