#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H
#include <stdbool.h>
typedef struct{
	unsigned char flashlen;
	unsigned int starttime;
}led_s;

typedef enum{
	LEDRED,
	LEDGREEN
}led_t;

void LedUpdate(void);
void LedInit(void);
void LedFlash(led_t led,unsigned char len);
void LedStop(void);
bool LedFlashing(void);

#endif
