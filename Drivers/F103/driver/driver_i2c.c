#include "gpio.h"

/*引脚配置*********************/

/**
  * 函    数：OLED写SCL高低电平
  * 参    数：要写入SCL的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SCL时，此函数会被调用
  *           用户需要根据参数传入的值，将SCL置为高电平或者低电平
  *           当参数传入0时，置SCL为低电平，当参数传入1时，置SCL为高电平
  */
void OLED_W_SCL(uint8_t BitValue)
{
	/*根据BitValue的值，将SCL置高电平或者低电平*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, BitValue);
	
	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
	//...
}

/**
  * 函    数：OLED写SDA高低电平
  * 参    数：要写入SDA的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SDA时，此函数会被调用
  *           用户需要根据参数传入的值，将SDA置为高电平或者低电平
  *           当参数传入0时，置SDA为低电平，当参数传入1时，置SDA为高电平
  */
void OLED_W_SDA(uint8_t BitValue)
{
	/*根据BitValue的值，将SDA置高电平或者低电平*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, BitValue);
	
	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
	//...
}

/**
  * 函    数：OLED引脚初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：当上层函数需要初始化时，此函数会被调用
  *           用户需要将SCL和SDA引脚初始化为开漏模式，并释放引脚
  */
void OLED_GPIO_Init(void)
{
	uint32_t i, j;
	
	/*在初始化前，加入适量延时，待OLED供电稳定*/
	for (i = 0; i < 1000; i ++)
	{
		for (j = 0; j < 1000; j ++);
	}
	#if 0
	
	/*将SCL和SDA引脚初始化为开漏模式*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pins : PB8 PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	#endif
	/*释放SCL和SDA*/
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/*********************引脚配置*/


/*通信协议*********************/

/**
  * 函    数：I2C起始
  * 参    数：无
  * 返 回 值：无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);		//释放SDA，确保SDA为高电平
	OLED_W_SCL(1);		//释放SCL，确保SCL为高电平
	OLED_W_SDA(0);		//在SCL高电平期间，拉低SDA，产生起始信号
	OLED_W_SCL(0);		//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
  * 函    数：I2C终止
  * 参    数：无
  * 返 回 值：无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);		//拉低SDA，确保SDA为低电平
	OLED_W_SCL(1);		//释放SCL，使SCL呈现高电平
	OLED_W_SDA(1);		//在SCL高电平期间，释放SDA，产生终止信号
}

/**
  * 函    数：I2C发送一个字节
  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	/*循环8次，主机依次发送数据的每一位*/
	for (i = 0; i < 8; i++)
	{
		/*使用掩码的方式取出Byte的指定一位数据并写入到SDA线*/
		/*两个!的作用是，让所有非零的值变为1*/
		OLED_W_SDA(!!(Byte & (0x80 >> i)));
		OLED_W_SCL(1);	//释放SCL，从机在SCL高电平期间读取SDA
		OLED_W_SCL(0);	//拉低SCL，主机开始发送下一位数据
	}
	
	OLED_W_SCL(1);		//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/**
  * 函    数：OLED写命令
  * 参    数：Command 要写入的命令值，范围：0x00~0xFF
  * 返 回 值：无
  */
void OledSendCmd(uint8_t Command)
{
	OLED_I2C_Start();				//I2C起始
	OLED_I2C_SendByte(0x78);		//发送OLED的I2C从机地址
	OLED_I2C_SendByte(0x00);		//控制字节，给0x00，表示即将写命令
	OLED_I2C_SendByte(Command);		//写入指定的命令
	OLED_I2C_Stop();				//I2C终止
}

/**
  * 函    数：OLED写数据
  * 参    数：Data 要写入数据的起始地址
  * 参    数：Count 要写入数据的数量
  * 返 回 值：无
  */
void OledSendData(uint8_t *Data, uint8_t Count)
{
	uint8_t i;
	
	OLED_I2C_Start();				//I2C起始
	OLED_I2C_SendByte(0x78);		//发送OLED的I2C从机地址
	OLED_I2C_SendByte(0x40);		//控制字节，给0x40，表示即将写数量
	/*循环Count次，进行连续的数据写入*/
	for (i = 0; i < Count; i ++)
	{
		OLED_I2C_SendByte(Data[i]);	//依次发送Data的每一个数据
	}
	OLED_I2C_Stop();				//I2C终止
}

/*********************通信协议*/


/*硬件配置*********************/

/**
  * 函    数：OLED初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：使用前，需要调用此初始化函数
  */
void OLED_Init(void)
{
	OLED_GPIO_Init();			//先调用底层的端口初始化
	
	/*写入一系列的命令，对OLED进行初始化配置*/
	OledSendCmd(0xAE);	//设置显示开启/关闭，0xAE关闭，0xAF开启
	
	OledSendCmd(0xD5);	//设置显示时钟分频比/振荡器频率
	OledSendCmd(0x80);	//0x00~0xFF
	
	OledSendCmd(0xA8);	//设置多路复用率
	OledSendCmd(0x3F);	//0x0E~0x3F
	
	OledSendCmd(0xD3);	//设置显示偏移
	OledSendCmd(0x00);	//0x00~0x7F
	
	OledSendCmd(0x40);	//设置显示开始行，0x40~0x7F
	
	OledSendCmd(0xA1);	//设置左右方向，0xA1正常，0xA0左右反置
	
	OledSendCmd(0xC8);	//设置上下方向，0xC8正常，0xC0上下反置

	OledSendCmd(0xDA);	//设置COM引脚硬件配置
	OledSendCmd(0x12);
	
	OledSendCmd(0x81);	//设置对比度
	OledSendCmd(0xCF);	//0x00~0xFF

	OledSendCmd(0xD9);	//设置预充电周期
	OledSendCmd(0xF1);

	OledSendCmd(0xDB);	//设置VCOMH取消选择级别
	OledSendCmd(0x30);

	OledSendCmd(0xA4);	//设置整个显示打开/关闭

	OledSendCmd(0xA6);	//设置正常/反色显示，0xA6正常，0xA7反色

	OledSendCmd(0x8D);	//设置充电泵
	OledSendCmd(0x14);

	OledSendCmd(0xAF);	//开启显示
	
	//OLED_Clear();				//清空显存数组
	//OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
}
