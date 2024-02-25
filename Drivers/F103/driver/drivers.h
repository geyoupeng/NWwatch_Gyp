#ifndef __DRIVERS_H
#define __DRIVERS_H

#define UART_DRIVER_USE	    (1)
#define LOG_DRIVER_USE	    (1)
#define OLED_DRIVER_USE	    (1)
#define TIMER_DRIVER_USE	(1)
#define BUZZER_DRIVER_USE	(1)
#define LED_DRIVER_USE		(1)
#define I2C_DRIVER_USE		(1)
#define W25Q_DRIVER_USE		(1)

#ifdef	UART_DRIVER_USE
	#include "driver_uart.h"
#endif

#ifdef	LOG_DRIVER_USE
    #include <stdio.h>
	#include "driver_log.h"
#endif

#ifdef	OLED_DRIVER_USE
	#include "driver_oled.h"
#endif

#ifdef	TIMER_DRIVER_USE
	#include "driver_timer.h"
#endif


#ifdef	BUZZER_DRIVER_USE
	#include "driver_buzzer.h"
#endif

#ifdef	LED_DRIVER_USE
	#include "driver_led.h"
#endif

#ifdef	I2C_DRIVER_USE
	#include "driver_i2c.h"
#endif

#ifdef	W25Q_DRIVER_USE
	#include "driver_w25q.h"
#endif

#endif //__DRIVERS_H

