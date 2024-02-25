#ifndef __DRIVER_BUZZER_H
#define __DRIVER_BUZZER_H
#include "common.h"
#define PRIO_UI		PRIO_LOW
#define PRIO_ALARM	PRIO_HIGH
#define PRIO_HOUR	PRIO_NML

void BuzzerInit(void);
void BuzzerPlay(const uint32_t* _tune, vol_t _vol, uint32_t _prio);
void BuzzerUpdate(void);
void BuzzerStop(void);
void BuzzerBuzz(uint16_t len,uint16_t tone,vol_t volType,uint32_t _prio,void (* _OnFinish)(void));

#endif
