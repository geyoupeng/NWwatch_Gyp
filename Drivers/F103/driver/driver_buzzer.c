#include "drivers.h"
#include "common.h"
#include "tim.h"
#include "gpio.h"
static uint32_t* tune;
static uint32_t prio;
static vol_t vol;

static uint16_t Buzzlen = 0;
static uint32_t Startmill = 0;
static uint8_t idx = 0;
static void (* OnFinish)(void);

void BuzzerInit(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
}

void BuzzerSetFreqDuty(int freq,int duty){
	TimerConfig(&htim3, SET_PERIOD_VALUE, freq);
	TimerConfig(&htim3, SET_PULSE_VALUE, duty);
}

void BuzzerStop(void){
	prio = PRIO_MIN;
	Buzzlen = 0;
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
}

void BuzzerBuzz(uint16_t len,uint16_t tone,vol_t volType,uint32_t _prio,void (* _OnFinish)(void)){
	if(_prio < prio){
		return;
	}
	
	if(tone == TONE_STOP){
		BuzzerStop();
		return;
	}

	Buzzlen = len;
	Startmill = mills();
	OnFinish = _OnFinish;

    if(tone == TONE_PAUSE){
        //HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
        return;
    }
	uint32_t ocr = 0;
	uint32_t icr = 0;
	ocr = 1000000000/tone;

	uint8_t vol = 0;
	switch(volType){
		case VOL_UI:
			vol = appConfig.volUI;
			break;
		case VOL_ALARM:
			vol = appConfig.volAlarm;
			break;
		case VOL_HOUR:
			vol = appConfig.volHour;
			break;
		default:
			vol = 3;
			break;
	}

	switch(vol){
		case 0:
			icr = ocr/64;
			break;
		case 1:
			icr = ocr/32;
			break;
		case 2:
			icr = ocr/16;
			break;
		case 3:
			icr = ocr/2;
			break;
		default:
			icr = ocr/2;
			break;
	}

	BuzzerSetFreqDuty(ocr,icr);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}

void BuzzerNext(void){
	uint16_t len;
	uint16_t tone;

	uint32_t data = tune[idx++];
	len = data;
	tone = (data>>16);

	if(tone != TONE_REPEAT)
	{
		BuzzerBuzz(len,tone,vol,prio,BuzzerNext);
	}else{
		// Repeat
		idx = 0;
		BuzzerNext();;
	}	
}

void BuzzerPlay(const uint32_t* _tune, vol_t _vol, uint32_t _prio){

	if(_prio < prio){
		return;
	}
	
	tune = (uint32_t*)_tune;
	vol = _vol;
	prio = _prio;
    idx = 0;
	BuzzerNext();
}

void BuzzerUpdate(void){
	static uint32_t now = 0;

	now = mills();
	if( Buzzlen && ( (Buzzlen+Startmill)<now ) ){
		BuzzerStop();
		if(OnFinish){
			OnFinish();
		}
	}
}

