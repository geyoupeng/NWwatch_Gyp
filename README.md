# 开发环境：
```c
	本程序使用的所有外设均是 江协科大STM32教程所使用的外设。
	CPU：STM32F103C8T6
	0.96寸OLED I2C通信
	3个按键
	有源蜂鸣器
	LED
	W25Q64外部Flash
```
# 可实现的效果：
## 闹钟、手电筒、秒表计时、3D动画演示、游戏（赛车、破坏小球） 设置->日期调整、显示设置、版本信息查看

```
0.96寸OLED_			 ----> PB8	  SCL      PB9	  SDA（软件模拟I2C）
3个按键        			 ----> PB0、PA3、PA1	(CubeMX配置输入模式)
有源蜂鸣器       		 ----> PB1	(定时器三配置PWM输出）
LED		   		 ----> PB12、PB13
W25Q64外部Flash	 ----> PA5
					 PA6
					 PA7	  (SPI1默认引脚)	
					 PB11 CS
```
