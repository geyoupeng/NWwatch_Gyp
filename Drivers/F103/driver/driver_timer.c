#include "tim.h"
#include "drivers.h"
#include "common.h"

void TimerStart(TIM_HandleTypeDef *ptimer){
	TIM_HandleTypeDef *htim = NULL;
	htim = ptimer;
	if(htim){
		HAL_StatusTypeDef status = HAL_TIM_Base_Start_IT(htim);
		if(status != HAL_OK){
			 LogDebug("HAL_TIM_Base_Start_IT ERROR\r\n");
		}
	}
	
}

void TimerStop(TIM_HandleTypeDef *ptimer){
	TIM_HandleTypeDef *htim = NULL;
	htim = ptimer;
	if(htim){
		HAL_StatusTypeDef status = HAL_TIM_Base_Stop_IT(htim);
		if(status != HAL_OK){
			 LogDebug("HAL_TIM_Base_Start_IT ERROR\r\n");
		}
	}
	
}

void TimerConfig(TIM_HandleTypeDef *ptimer,uint8_t cmd,uint32_t cfg){
	uint32_t freq = 0;
	TIM_HandleTypeDef *htim = NULL;
	htim = ptimer;
	if(htim){
		if(htim->Instance == TIM2){
			freq = HAL_RCC_GetPCLK1Freq() * 2;
		}else if(htim->Instance == TIM3){
            freq = HAL_RCC_GetPCLK1Freq() * 2;
        }
        
	}else{
		return;
	}

	freq = freq/1000000;
	switch(cmd){
		case SET_PRESCALER_VALUE:
			__HAL_TIM_SET_PRESCALER(htim, cfg);
			break;
		case SET_CLKDIV_VALUE:
			if(cfg == 1)
			   __HAL_TIM_SET_CLOCKDIVISION(htim, TIM_CLOCKDIVISION_DIV1);
		   else if(cfg == 2)
			   __HAL_TIM_SET_CLOCKDIVISION(htim, TIM_CLOCKDIVISION_DIV2);
		   else if(cfg == 4)
			   __HAL_TIM_SET_CLOCKDIVISION(htim, TIM_CLOCKDIVISION_DIV4);
		   break;
		case SET_PERIOD_VALUE:{
			uint32_t maxint = (0xffffffff)/freq;
			uint32_t period = 1;
			if(cfg < maxint){
				period = freq * cfg / 1000;
			}else{
				period = cfg / 1000 * freq;
			}

			uint32_t prescaler = (period >> 16) + 1;
			period = period / prescaler;
			__HAL_TIM_SET_PRESCALER(htim, prescaler - 1);
		    __HAL_TIM_SET_AUTORELOAD(htim, period - 1);
		    break;
		}
		case SET_PULSE_VALUE:
        {
            unsigned int period = (htim->Instance->ARR + 1) * (htim->Instance->PSC + 1) / freq;
            unsigned int pulse = cfg / 1000 * (htim->Instance->ARR + 1) / period;
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, pulse);
            break;
        }
		default:break;
	}
	HAL_TIM_GenerateEvent(htim, TIM_EGR_UG);

}

void TimerInit(TIM_HandleTypeDef *ptimer){
	TimerConfig(ptimer, SET_PERIOD_VALUE, 1000000);
    TimerStart(ptimer);
}

static uint32_t millsecs = 0;
extern timeDate_s timeDate;

uint32_t mills(void){
	return millsecs;
}

void Timer2Callback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2){
  		millsecs++;
		if(millsecs%1000 == 0){
			timeDate.time.secs++;
		}
  }else if(htim->Instance == TIM3){
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  	}
}

