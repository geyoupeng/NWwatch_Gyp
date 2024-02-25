#ifndef __DRIVER_OLED_H
#define __DRIVER_OLED_H
#include <stdint.h>
void OledInit(void);
void OledShowChar(uint8_t x,uint8_t y,char ch,uint8_t FontSize);
int OledSendCmd(uint8_t cmd);

void OledShowStr(uint8_t x,uint8_t y,char *str,uint8_t FontSize);
void OledShowNum(uint8_t x,uint8_t y,uint32_t number,uint8_t length,uint8_t FontSize);
void OledShowSignedNum(uint8_t x,uint8_t y,int32_t number,uint8_t length,uint8_t FontSize);
void OledShowChinese(uint8_t x,uint8_t y,char *chinese);
void OledShowImage(int x,uint8_t y,uint8_t Width,uint8_t Heigth,const uint8_t *image,int16_t offset);
void OledShowImage1(uint8_t x,uint8_t y,uint8_t Width,uint8_t Heigth,const uint8_t *image);

void OledPrintf(uint8_t x,uint8_t y,uint8_t FontSize,char *format,...);
void OledReverse(void);
void OledReverseArea(uint8_t x,uint8_t y,uint8_t Width,uint8_t Heigth);
void OledUpdate(void);
void OledClear(void);
void OledClearArea(uint8_t x,uint8_t y,uint8_t w);

uint8_t OledGetPoint(uint8_t x,uint8_t y);
void OledDrawLine(uint8_t X0,uint8_t Y0,uint8_t X1,uint8_t Y1);
void OledDrawRectangle(uint8_t x,uint8_t y,uint8_t Width,uint8_t Heigth,uint8_t IsFill);
void OledDrawTriangle(uint8_t X0,uint8_t Y0,uint8_t X1,uint8_t Y1,uint8_t X2,uint8_t Y2,uint8_t IsFilled);
void OledDrawCircle(uint8_t X,uint8_t Y,uint8_t Radius,uint8_t Isfiled);

#endif //__DRIVER_OLED_H

