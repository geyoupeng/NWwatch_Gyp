#ifndef __TIME_H
#define __TIME_H

void TimeUpdate(void);
uint8_t TimeMonthDayCount(month_t month, uint8_t year);
day_t TimeDow(uint8_t yy, month_t m, uint8_t d);
void SetTime(timeDate_s *newTimeDate);
void TimeSetMode(time_s *time,timemode_t mode);

#endif //__TIME_H
