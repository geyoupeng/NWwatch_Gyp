#ifndef __ENGLISH_H
#define __ENGLISH_H

#define PSTR(a)  a
#define BUFFSIZE_DATE_FORMAT		((BUFFSIZE_STR_DAYS - 1) + (BUFFSIZE_STR_MONTHS - 1) + 12)

#define DATE_FORMAT ("%s %02hhu %s 20%02hhu")
#define TIME_FORMAT_SMALL ("%02hhu:%02hhu%c")

// Days
// Also see BUFFSIZE_STR_DAYS
#define STR_MON			"Mon"
#define STR_TUE			"Tue"
#define STR_WED			"Wed"
#define STR_THU			"Thu"
#define STR_FRI			"Fri"
#define STR_SAT			"Sat"
#define STR_SUN			"Sun"

// Months
// Also see BUFFSIZE_STR_MONTHS
#define STR_JAN			"Jan"
#define STR_FEB			"Feb"
#define STR_MAR			"Mar"
#define STR_APR			"Apr"
#define STR_MAY			"May"
#define STR_JUN			"Jun"
#define STR_JUL			"Jul"
#define STR_AUG			"Aug"
#define STR_SEP			"Sep"
#define STR_OCT			"Oct"
#define STR_NOV			"Nov"
#define STR_DEC			"Dec"

//主菜单
#define STR_MAINMENU	"< MAIN MENU >"
#define STR_ALARMS		"Alarms"
#define STR_FLASHLIGHT	"Flashlight"
#define STR_STOPWATCH	"Stopwatch"
#define STR_GAMES		"Games"
#define STR_MUSIC		"Music"
#define STR_SETTINGS	"Settings"
#define EXIT	"EXIT"

//Alarm 使用
#define STR_ALARMMENU   " < ALARM > "

// Game strings
#define STR_GAMEMENU " < Games > "
#define STR_GAME1		"Breakout"
#define STR_GAME2		"Car Dodge"

#define STR_WIN			"WIN!"
#define STR_GAMEOVER	"GAMEOVER!"
#define STR_SCORE		"Score:"
#define STR_HIGHSCORE	"Highscore:"
#define STR_NEWHIGHSCORE	"!NEW HIGHSCORE!"

// Setting
#define STR_SETTINGMENU	"< SETTINGS >"
#define STR_TIMEDATE	"Time & date"
#define STR_SOUND		"Sound"
#define STR_DISPLAY		"Display"
#define STR_DIAGNOSTICS	"Diagnostics"
#define STR_Reset		"Reset"
//diag
#define STR_DIAGNOSTICSMENU	"< DIAGNOSTICS >"
#define STR_TEMPERATURE		"Temp %hhd.%hhuC"
#define STR_BATTERY			"Battery    %umV"
#define STR_SHOWFPS			"Show FPS%6c"
#define CHAR_YES 	'Y'
#define CHAR_NO 	'N'
#define FW_VERSION	"2.16 T2024"
#define USER_NAME 	"xiaoge"
//m_display
#define STR_DISPLAYMENU	"< DISPLAY >"
#define STR_BRIGHTNESS	"Brightness"
#define STR_INVERT		"Invert"
#define STR_ROTATE		"Rotate"
#define STR_ANIMATIONS	"Animations"
#define STR_LEDS		"LEDs"
//reset
#define STR_RESETMENU	"< RESET >"
#define STR_RESET		"Reset"
//sound
#define STR_SOUNDMENU	"< SOUND >"
#define STR_UI			"UI"
#define STR_HOURBEEPS	"Hour beeps"
#define STR_ALARMS		"Alarms"
//datetime
#define STR_TIMEDATEMENU	"< TIME & DATE >"
#define STR_SAVE		"Save"
#define STR_SAVED		"Saved"


#define CHAR_AM	'A'
#define CHAR_PM	'P'
#define CHAR_24	' '

#endif //__ENGLISH_H

