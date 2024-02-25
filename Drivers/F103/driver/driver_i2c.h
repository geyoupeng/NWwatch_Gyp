#ifndef __DRIVER_I2C_H
#define __DRIVER_I2C_H
void OLED_GPIO_Init(void);
void OLED_WriteCommand(uint8_t Command);
void OledSendData(uint8_t *Data, uint8_t Count);
void OLED_Init(void);
#endif
