#ifndef __ALARM_H
#define __ALARM_H

#include "common.h"
#define _BV(bit) (1 << (bit))
#define AlarmDayEnabled(days, day) (days & _BV(day))

void Alarmget(uint8_t num, alarm_s *alarm);
void AlarmSave(uint8_t num, alarm_s *alarm);
void AlarmInit(void);
void GetNextAlarm(void);
void AlarmUpdate(void);

#endif //__ALARM_H
