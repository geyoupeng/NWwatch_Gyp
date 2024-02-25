#include "drivers.h"
#include "usart.h"
#include <stdio.h>
//重写 printf底层函数 可以使用printf直接向串口打印输出
int fputc(int c, FILE * f){
    unsigned char ch = c;
	HAL_UART_Transmit(&huart1, &ch, 1, 0xff);
	return c;
}

