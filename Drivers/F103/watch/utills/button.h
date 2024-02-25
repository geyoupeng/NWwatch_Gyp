#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdint.h>
#include "common.h"
#include "gpio.h"

#define BTN_1 	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define BTN_2 	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)
#define BTN_3 	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)

button_f SetButtonSignedFunc(uint8_t num,button_f bt);
void SetButtonFunc(button_f bt0,button_f bt1,button_f bt2);
void ButtonUpdate(void);
void ButtonInit(void);

#endif //__BUTTON_H
