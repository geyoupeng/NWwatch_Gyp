#include "common.h"
#include <string.h>
#define FEB_LEAP_YEAR	29
static const uint8_t monthDayCount[]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static bool TimeIsLeapYear(uint8_t year){
	return (year % 4 == 0);
}

uint8_t TimeMonthDayCount(month_t month, uint8_t year)
{
	uint8_t numDays = monthDayCount[month];

	if(month == MONTH_FEB && TimeIsLeapYear(year))
		numDays = FEB_LEAP_YEAR;
	return numDays;
}

void TimeSetMode(time_s *time,timemode_t mode){
	uint8_t hour = time->hour;
	if(mode == TIMEMODE_12HR)
	{
		if(time->ampm != CHAR_24) // Already 12hr
			return;
		else if(hour >= 12)
		{
			if(hour > 12)
				hour -= 12;
			time->ampm = CHAR_PM;
		}
		else
		{
			if(hour == 0)
				hour = 12;
			time->ampm = CHAR_AM;
		}
	}
	else // 24 hour
	{
		if(time->ampm == CHAR_AM && hour == 12) // Midnight 12AM = 00:00
			hour = 0;
		else if(time->ampm == CHAR_PM && hour < 12) // No change for 12PM (midday)
			hour += 12;
		time->ampm = CHAR_24;
	}
	
	time->hour = hour;
}

// Workout day of week from year, month and date
// http://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
day_t TimeDow(uint8_t yy, month_t m, uint8_t d)
{
	//static byte t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	//y -= m < 3;
	//byte dow = (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;

    m =(month_t)(m + (month_t)1);
	int y = yy + 2000;
	uint8_t dow = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7 ;

	// 0 = Sunday, 1 = Monday, but Monday should be 0
	dow--;
	if(dow == 255) // Overflowed, must have been 0 which is Sunday
		dow = 6;
	return (day_t)dow;
}


void SetTime(timeDate_s *newTimeDate){
	memcpy(&timeDate,newTimeDate,sizeof(timeDate_s));
	timeDate.time.secs = 0;

	TimeSetMode(&timeDate.time,appConfig.timeMode);
	
}

void TimeUpdate(void){ 

	if(timeDate.time.secs == 60)
	{
		timeDate.time.secs = 0;
		if(++timeDate.time.mins == 60)
		{
			timeDate.time.mins = 0;
			if(++timeDate.time.hour == 24)
			{
				uint8_t numDays = TimeMonthDayCount(timeDate.date.month, timeDate.date.year);

				timeDate.time.hour = 0;
				if (++timeDate.date.date == numDays + 1)
				{
					timeDate.date.month++;
					timeDate.date.date = 1;
				}

				if (timeDate.date.month == 13)
				{
					timeDate.date.month = (month_t)1;
					timeDate.date.year++;
					if(timeDate.date.year == 100)
						timeDate.date.year = 0;
				}

				if(++timeDate.date.day == 7)
					timeDate.date.day = (day_t)0;
			}

		}
	}
	
}

