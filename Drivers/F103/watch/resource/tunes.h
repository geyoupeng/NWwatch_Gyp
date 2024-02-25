#ifndef __TUNES_H
#define __TUNES_H

typedef enum
{
	LA =262,
	LB =294,
	LC =330,
	LD =349,
	LE =392,
	LF =440,
	LG =494,
	
	MA =523,
	MB =578,
	MC =659,
	MD =698,
	ME =784,
	MF =880,
	MG =988,
	
	HA =1064,
	HB =1175,
	HC =1318,
	HD =1397,
	HE =1568,
	HF =1760,
	HG =1976
}music_t;


typedef enum
{
	TONE_STOP = 0,
	TONE_PAUSE = 1,
	TONE_REPEAT = 2,
	TONE_2KHZ = 2000,
	TONE_2_5KHZ = 2500,
	TONE_3KHZ = 3000,
	TONE_3_5KHZ = 3500,
	TONE_4KHZ = 4000,
	TONE_4_5KHZ = 4500,
	TONE_5KHZ = 5000,
} tone_t;

typedef enum
{
	VOL_OTHER = 0,
	VOL_UI = 1,
	VOL_ALARM = 2,
	VOL_HOUR = 3
}vol_t;

typedef enum
{
	PRIO_MIN = 0,
	PRIO_LOW = 1,
	PRIO_NML = 2,
	PRIO_HIGH = 3,
	PRIO_MAX = 255
} tonePrio_t;

extern const uint32_t tuneBtn1[];
extern const uint32_t tuneBtn2[];
extern const uint32_t tuneBtn3[];
extern const uint32_t tuneHours[];
extern const uint32_t tuneAlarm[];
extern const uint32_t TUNE[];
extern const uint32_t STAY[];

#endif	//__TUNES_H
