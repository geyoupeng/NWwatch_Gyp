#ifndef __DRIVER_TIMER_H
#define __DRIVER_TIMER_H

#include "tim.h"

typedef enum{
    SET_PRESCALER_VALUE = 0,
    SET_CLKDIV_VALUE,
    SET_PERIOD_VALUE,
    SET_PULSE_VALUE
}TimerCfgCmd;
	
uint32_t mills(void);

void TimerInit(TIM_HandleTypeDef *ptimer);

void TimerStart(TIM_HandleTypeDef *ptimer);

void TimerStop(TIM_HandleTypeDef *ptimer);

void TimerConfig(TIM_HandleTypeDef *htim,uint8_t cmd,uint32_t cfg);

#endif //__DRIVER_TIMER_H

