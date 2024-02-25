#include "spi.h"
#include "gpio.h"
#include <stdbool.h>
#define SPI1CS_ON	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
#define SPI1CS_OFF	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)

static volatile bool gSPI1TxCplt = 0;
static volatile bool gSPI1RxCplt = 0;

static void SPI1WaitTxCplt(void)
{
    while(gSPI1TxCplt == 0);
    gSPI1TxCplt = 0;
}

static void SPI1WaitRxCplt(void)
{
    while(gSPI1RxCplt == 0);
    gSPI1RxCplt = 0;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(SPI1 == hspi->Instance)
    {
        gSPI1TxCplt = 1;
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(SPI1 == hspi->Instance)
    {
        gSPI1RxCplt = 1;
    }
}

static int SpiWrite(unsigned char *wbuf, unsigned int length){
	HAL_StatusTypeDef status = HAL_SPI_Transmit_DMA(&hspi1, wbuf, length);
    if(HAL_OK != status)    return -1;
	SPI1WaitTxCplt();
    
    return length;
}

static int SpiRead(unsigned char *rbuf, unsigned int length){
	HAL_StatusTypeDef status = HAL_SPI_Receive_DMA(&hspi1, rbuf, length);
    if(HAL_OK != status)    return -1;
	SPI1WaitRxCplt();
    
    return length;
}

static unsigned int ReadID(void)
{    
    unsigned char cmd = 0x9F;   // W25X_JedecDeviceID
    unsigned char id[3] = {0};
    
    SPI1CS_ON;
    SpiWrite(&cmd,1);
    SpiRead(id, 3);
    SPI1CS_OFF;
    
    unsigned int ID = (id[0] << 16) | (id[1] << 8) | (id[2]);
    return ID;
}

void W25QInit(void){
    int ID = ReadID();
	if(ID != 0xEF4017)
		return;
	printf("ID is %d",ID);
}

static void W25QWriteEnable(void){
	unsigned char cmd = 0x06;   // W25X_JedecDeviceID
    
    SPI1CS_ON;
    SpiWrite(&cmd,1);
    SPI1CS_OFF;
}

static void WaitWrite(void){
	unsigned char cmd = 0x05;
	unsigned char status = 0xFF;

	SPI1CS_ON;
	int len = SpiWrite(&cmd, 1);
	if(len != 1)
	{
	 	SPI1CS_OFF;
		return;
	}
	while((status & 0x01) == 1)
	{
		len = SpiRead(&status, 1);
		if(len != 1)
		{
			SPI1CS_OFF;
			return;
		}
	}
	SPI1CS_OFF;
}

int W25QPageWrite(uint32_t addr,unsigned char *buf,unsigned int length){

	unsigned char data[4];
	data[0] = 0x02;
	data[1] = (addr &0xFF0000)>>16;
	data[2] = (addr &0x00FF00)>>8;
	data[3] = (addr &0x0000FF);

	W25QWriteEnable();
	SPI1CS_ON;
	int ret = SpiWrite(data,4);
	if(ret < 0){
		SPI1CS_OFF;
	}

	ret = SpiWrite(buf,length);
	if(ret < 0){
		SPI1CS_OFF;
	}
	SPI1CS_OFF;

	WaitWrite();
	return (int)length;
}

int W25QRead(uint32_t addr,unsigned char *buf,unsigned int length){

	unsigned char data[4];
    data[0] = 0x03;
    data[1] = (addr &0xFF0000)>>16;
    data[2] = (addr &0x00FF00)>>8;
    data[3] = (addr &0x0000FF);
    
    SPI1CS_ON;
    int ret = SpiWrite(data,4);
	if(ret < 0){
		SPI1CS_OFF;
	}
	
    ret = SpiRead(buf,length);
    if(ret != length){
		SPI1CS_OFF;
	}
    SPI1CS_OFF;
    return (int)length;
}

void W25QEraseSector(uint32_t addr){

	unsigned char data[4];
	data[0] = 0x20;
    data[1] = (addr &0xFF0000)>>16;
    data[2] = (addr &0x00FF00)>>8;
    data[3] = (addr &0x0000FF);
	
	W25QWriteEnable();
	SPI1CS_ON;
	int ret = SpiWrite(data,4);
	if(ret < 0){
		SPI1CS_OFF;
	}
    SPI1CS_OFF;
	WaitWrite();
                                                                             }

