#include "drivers.h"
#include "common.h"
//#include "i2c.h"
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#define OLEDADD	(0x78)

static volatile uint8_t OLEDDisplayBuf[8][128] = {0};
void OledShowImage(int x,uint8_t y,uint8_t Width,uint8_t Heigth,const uint8_t *image,int16_t offset);
void OledUpdate(void);
void OledClear(void);

#if 0
/**
  * 函    数：OLED发送命令
  * 参    数：cmd 要发送的命令
  * 返 回 值：0 写入成功
  */
int OledSendCmd(uint8_t cmd){
	uint8_t data[] = {0x00,cmd};
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1,0x78, data, 2, 0xFF);
	if(status != HAL_OK){
		LogDebug("%x Send Failed\r\n",cmd);
		return -1;
	}

	return 0;
}

/**
  * 函    数：OLED发送数据
  * 参    数：Data 要写入的数据
  * 参    数：count 要写入的数据的个数
  * 返 回 值：0 写入成功
  */
static int OledSendData(uint8_t *Data,uint8_t count){
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, 0x78,0x40,1 ,Data,count, 0xff);
	//HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, (unsigned short)OLEDADD, data, count+1, 0xFF);
	if(status != HAL_OK){
		LogDebug("Send Failed\r\n");
		return -1;
	}

	return 0;
}
#endif
/**
  * 函    数：OLED初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：使用前，需要调用此初始化函数
  */
void OledInit(){
	OLED_GPIO_Init();

	//关闭显示
	OledSendCmd(0xAE);

	//设置时钟分频
	OledSendCmd(0xD5);
	OledSendCmd(0x80);

	//设置多路复用率
	OledSendCmd(0xA8);
	OledSendCmd(0x3F);

	//设置显示偏差
	OledSendCmd(0xD3);
	OledSendCmd(0x00);

	//设置开始行
	OledSendCmd(0x40);

	//设置左右方向，0xA1正常，0xA0左右反置
	OledSendCmd(0xA1);

	//设置上下方向，0xC8正常，0xC0上下反置
	OledSendCmd(0xC8);

	//设置COM引脚硬件配置
	OledSendCmd(0xDA);
	OledSendCmd(0x12);

	//设置对比度
	OledSendCmd(0x81);
	OledSendCmd(0xCF);

	//设置预充电周期
	OledSendCmd(0xD9);
	OledSendCmd(0xF1);

	//设置VCOMH取消选择级别
	OledSendCmd(0xDB);
	OledSendCmd(0x30);

	//屏幕完全显示
	OledSendCmd(0xA4);

	//正常模式 黑底白字
	OledSendCmd(0xA6);
	
	//使能电荷泵 内部的升压电路
	OledSendCmd(0x8D);
	OledSendCmd(0x14);
	
	//打开显示
	OledSendCmd(0xAF);
    
    OledClear();				//清空显存数组
	OledUpdate();				//更新显示，清屏，防止初始化后未显示内容时花屏
	LogDebug("OLED Init Success\r\n");
}
/**
  * 函    数：OLED设置光标显示
  * 参    数：Page 显示行地址
  * 参    数：X 	  显示列地址
  * 返 回 值：无
  */
void OledSetCursor(uint8_t Page,uint8_t X){
	if(Page>7){
		LogDebug("OledSetCursor Page is %d \r\n",Page);
		return;
	}
	//设置行地址
	OledSendCmd(0xB0|Page);

	//设置列地址 高四位
	OledSendCmd(0x10|((X&0xF0)>>4) );

	//设置列地址 低四位
	OledSendCmd(0x00|(X&0x0F));
}

/**
  * 函    数：将OLED缓存数据发送给OLED 更新显示的内容
  * 参    数：无
  * 返 回 值：无
  */
void OledUpdate(void){
	for(uint8_t j = 0;j < 8;j++){
		OledSetCursor(j,0);
		OledSendData((uint8_t *)OLEDDisplayBuf[j],128);
	}
}

/**
  * 函    数：将OLED缓存数据清零
  * 参    数：无
  * 返 回 值：无
  */
void OledClear(void){
	for(uint8_t i = 0;i < 8;i++){
		for(uint8_t j = 0;j < 128;j++){
			OLEDDisplayBuf[i][j] = 0x00;
		}
	}
}

void OledClearArea(uint8_t x,uint8_t y,uint8_t w){
	for(uint8_t i = 0;i < w;i++){
		OLEDDisplayBuf[y/8][x+i] = 0x00;
	}
}

/**
  * 函    数：OLED显示字符
  * 参    数：x 字符显示列
  * 参    数：y 字符显示行
  * 参    数：ch 显示字符
  * 参    数：FontSize 显示字体
  * 返 回 值：无
  */
void OledShowChar(uint8_t x,uint8_t y,char ch,uint8_t FontSize){
	if(FontSize == OLED_8X8){
		OledShowImage(x,y,8,8,OLED_F8X8[ch-' '],0);
	}else if(FontSize == OLED_16X8){
		OledShowImage(x,y,8,16,OLED_F16X8[ch-' '],0);
	}else if(FontSize == OLED_24X16){
		OledShowImage(x,y,16,24,OLED_F24X16[ch],0);
	}else if(FontSize == OLED_32X16){
		OledShowImage(x,y,16,32,OLED_F32X16[ch],0);
	}
}

/**
  * 函    数：OLED显示字符串
  * 参    数：x 字符显示列
  * 参    数：y 字符显示行
  * 参    数：str 显示字符串
  * 参    数：FontSize 显示字体
  * 返 回 值：无
  */
void OledShowStr(uint8_t x,uint8_t y,char *str,uint8_t FontSize){
	for(uint8_t i = 0;str[i] != '\0';i++ ){
		OledShowChar(x+i*8,y,str[i],FontSize);
	}
}

/**
  * 函    数：次方函数
  * 参    数：x 底数
  * 参    数：y 指数
  * 返 回 值：无
  */
static int OledPow(uint32_t x,uint32_t y){
	uint32_t res = 1;
	while(y--){
		res *= x;
	}
	return res;
}

/**
  * 函    数：OLED显示数字
  * 参    数：x 字符显示列
  * 参    数：y 字符显示行
  * 参    数：number 显示数字
  * 参    数：length 显示数字长度
  * 参    数：FontSize 显示字体
  *  uint8_t num = number/OledPow(10,length-i-1)%10+'0'; 从高位到地位提取每个数字
  */
void OledShowNum(uint8_t x,uint8_t y,uint32_t number,uint8_t length,uint8_t FontSize){
	for(uint8_t i = 0;i < length;i++ ){
		uint8_t num = number/OledPow(10,length-i-1)%10+'0';
		OledShowChar(x+i*8,y,num,FontSize);
	}
}

/**
  * 函    数：OLED显示有符号数字
  * 参    数：x 字符显示列
  * 参    数：y 字符显示行
  * 参    数：number 显示数字
  * 参    数：length 显示数字长度
  * 参    数：FontSize 显示字体
  *  uint8_t num = number/OledPow(10,length-i-1)%10+'0'; 从高位到地位提取每个数字
  */
void OledShowSignedNum(uint8_t x,uint8_t y,int32_t number,uint8_t length,uint8_t FontSize){
    if(number>0){
        OledShowChar(x,y,'+',FontSize);
        number = number;
    }else{
        OledShowChar(x,y,'-',FontSize);
        number = -number;
    }
	OledShowNum(x+8,y,number,length,FontSize);
}

/**
  * 函    数：OLED显示中文
  * 参    数：x 字符显示列
  * 参    数：y 字符显示行
  * 参    数：chinese 显示中文
  */
void OledShowChinese(uint8_t x,uint8_t y,char *chinese){
	uint8_t pchinese = 0;
	uint8_t pindex;

	char singlechinese[OLED_CHN_CHAR_WIDTH+1] = {0};
	for(uint8_t i = 0; chinese[i] != '\0'; i++){
		singlechinese[pchinese] = chinese[i];
		pchinese++;
	
		if(pchinese >= OLED_CHN_CHAR_WIDTH){
			pchinese = 0;
			for(pindex = 0;strcmp(OLED_CF16X16[pindex].index,"") != 0;pindex++){
				if(strcmp(OLED_CF16X16[pindex].index,singlechinese) == 0){
					break;
				}
			}
			OledShowImage(x+((i+1)/OLED_CHN_CHAR_WIDTH-1)*16,y,16,16,OLED_CF16X16[pindex].Data,0);	
		}

	}
}

/**
  * 函    数：OLED显示图像
  * 参    数：x 字符显示列
  * 参    数：y 字符显示行
  * 参    数：Width 显示图像的宽度
  * 参    数：Heigth 显示图像的高度
  * 参    数：image 显示图像的数据·
  * 返 回 值：无
  */
void OledShowImage1(uint8_t x,uint8_t y,uint8_t Width,uint8_t Heigth,const uint8_t *image){
	//参数 检查
	if(x > 127 || y/8 >7){
		LogDebug("OledShowImage x is %d y is %d\r\n",x,y);
		return;
	}

	//遍历图像设计的相关页
	//(Heigth-1)/8+1 是Heigth/8向上取整
	for(uint8_t j = 0;j<((Heigth-1)/8+1);j++){

		for(uint8_t i = 0;i<Width;i++){
			
			if( x+i>127){
				LogDebug("OledShowImage x is %d y is %d\r\n",x,y);
				break;
			}
			if( y/8+j>64 ){
				LogDebug("OledShowImage x is %d y is %d\r\n",x,y);
				return;
			}
			
			//更新显存中当前页的内容
			OLEDDisplayBuf[y/8+j][x+i] |= image[j*Width+i] << (y % 8);

			//如果超出边界则退出
			if(y/8+j+1 > 7){
				continue;
			}
			
			//更新显存中下一页的内容
			OLEDDisplayBuf[y/8+j+1][x+i] |= image[j*Width+i] >> (8 - (y % 8));
		}

	}
}

/**
  * 函    数：OLED显示图像
  * 参    数：x 字符显示列
  * 参    数：y 字符显示行
  * 参    数：Width 显示图像的宽度
  * 参    数：Heigth 显示图像的高度
  * 参    数：image 显示图像的数据·
  * 返 回 值：无
  */
void OledShowImage(int x,uint8_t y,uint8_t Width,uint8_t Heigth,const uint8_t *image,int16_t offset){
    
    int X = 0;
    uint8_t YY = y + animotion.offsetY;
	uint8_t YYY = YY - offset;
	
    for(uint8_t j = 0;j < Heigth;j++){
        for(uint8_t i = 0;i < Width;i++){
			X = x + i;
			if((YYY<=63) && (YYY >= YY) && (YYY < (YY+Heigth)) && (X >= 0) && (X <= 127) ){
				uint8_t curbit = (image[j/8*Width+i] & (0x01 << (j%8))) >> (j%8);
                curbit = (curbit << (YYY%8));
	            OLEDDisplayBuf[YYY/8][X] |= curbit;
			}
        }
        YYY++;
    }
}

/**
  * 函    数：OLED格式化打印函数
  * 参    数：x 字符显示列
  * 参    数：y 字符显示行
  * 参    数：FontSize 显示字符的字体
  * 参    数：format 显示的字符串
  * 参    数：... 变参·
  * 返 回 值：无
  */
void OledPrintf(uint8_t x,uint8_t y,uint8_t FontSize,char *format,...){
    char str[30] = {0};
    va_list ap;
    va_start(ap,format);
    vsprintf(str,format,ap);
    va_end(ap);
    OledShowStr(x,y,str,FontSize);
}

/**
  * 函    数：OLED反转函数
  * 功能	   ：OLED的显示从黑底白字->白底黑字 or 白底黑字->黑底白字
  */
void OledReverse(void){
    for(uint8_t j = 0;j < 8;j++){
        for(uint8_t i = 0;i < 128;i++){
            OLEDDisplayBuf[j][i] ^= 0xFF; 
        }
    }
}

/**
  * 函    数：OLED局部反转反转函数
  * 功    能：OLED的显示从黑底白字->白底黑字 or 白底黑字->黑底白字
  */
void OledReverseArea(uint8_t x,uint8_t y,uint8_t Width,uint8_t Heigth){
    //参数检查
    if(x > 127 || y > 63){
        LogDebug("OledReverseArea x is %d y is %d",x,y);
		return;
    }
    if( (x+Width) > 128 )   {Width = 128 - x;}
    if( (y+Heigth) > 64 )   {Heigth = 64 - y;}
    
    for(uint8_t j = y;j < (y+Heigth);j++){
        for(uint8_t i = x;i < (x+Width);i++){
            OLEDDisplayBuf[j/8][i] ^= (0x01 << (j%8)); 
        }
    }
}

/**
  * 函    数：OLED画点
  * 参    数：x 横坐标
  * 参    数：y 纵坐标
  */
void OledDrawPoint(uint8_t x,uint8_t y){
	//参数检查
    if(x > 127 || y > 63){
        LogDebug("OledDrawPoint x is %d y is %d",x,y);
		return;
    }

	OLEDDisplayBuf[y/8][x] |= 0x01 << (y%8);
}

/**
  * 函    数：OLED获取该点的值
  * 参    数：x 横坐标
  * 参    数：y 纵坐标
  * 返回值 ：（x，y）该点的值
  */
uint8_t OledGetPoint(uint8_t x,uint8_t y){
	//参数检查
    if(x > 127 || y > 63){
        LogDebug("OledGetPoint x is %d y is %d",x,y);
		return 0;
    }

	if(OLEDDisplayBuf[y/8][x] & 0x01 << (y%8)){
		return 1;
	}

	return 0;
}

/**
  * 函    数：OLED画线
  * 参    数：x0 起始点横坐标
  * 参    数：y0 起始点纵坐标
  * 参    数：x1 终点横坐标
  * 参    数：y1 终点纵坐标
  */
void OledDrawLine(uint8_t X0,uint8_t Y0,uint8_t X1,uint8_t Y1){
	
	int16_t x0 = X0,x1 = X1,y0 = Y0,y1 = Y1;
	uint8_t flag1 = 0 ,flag2 = 0;		//flag1  对y坐标取负值，使线段从第四象限变到第一象限
										//flag2  交换x，y坐标，使线段从45-90 变到 0-45
	//横线单独处理
	if(y0 == y1){
		
		//从左到右开始画线 让x0,y0作为起始点
		if(x0 > x1){
			int16_t tmp = x0;
			x0 = x1;
			x1 = tmp;
		}
		
		for(int16_t i = x0;i < x1;i++){
			OledDrawPoint(i,y0);
		}
	}else if(x0 == x1){		//竖线单独处理

		//从下到上开始画线 让x0,y0作为起始点
		if(y0 > y1){
			int16_t tmp = y0;
			y0 = y1;
			y1 = tmp;
		}
		
		for(int16_t i = y0;i < y1;i++){
			OledDrawPoint(x0,i);
		}

	}else{
		//使用bresenham算法 画直线

		//以起始点为原点，如果处于二三象限交换两点的位置 使其处于一四象限。
		if(x0 > x1){
			int16_t tmp = y0;y0 = y1;y1 = tmp;
			tmp = x0;x0 = x1;x1 = tmp;
		}

		//如果处于第二象限则 对y坐标进行取反 使其处于第一象限
		if(y0>y1){
			y0 = -y0;
			y1 = -y1;
			flag1 = 1;
		}

		if(y1-y0>x1-x0){
			//交换坐标的x，y坐标 使其处于 第一象限的第一部分
			int16_t tmp = y0;y0 = x0;x0 = tmp;
			tmp = y1;y1 = x1;x1 = tmp;
			flag2 = 1;
		}

		int16_t dx = x1-x0;
		int16_t dy = y1-y0;

		int16_t incrE = 2*dy;
		int16_t incrNE = 2*dy - 2*dx;
		int16_t d = 2*dy - dx;
		int16_t x = x0;
		int16_t y = y0;

		if(flag2 && flag1)	OledDrawPoint(y,-x);
		else if(flag2)	OledDrawPoint(y,x);
		else if(flag1)	OledDrawPoint(x,-y);
		else OledDrawPoint(x,y);

		while(x < x1){
			x++;

			if(d < 0){
				d = d +	incrE;			 // 下一个节点在本节点的东方
			}else{
				y++;
				d = d +	incrNE;			 // 下一个节点在本节点的东北方
			}

			if(flag2 && flag1)	OledDrawPoint(y,-x);
			else if(flag2)	OledDrawPoint(y,x);
			else if(flag1)	OledDrawPoint(x,-y);
			else OledDrawPoint(x,y);
		}
	}

}

/**
  * 函    数：OLED画线
  * 参    数：x 起始点横坐标
  * 参    数：y 起始点纵坐标
  * 参    数：Width 矩形的宽度
  * 参    数：Heigth 矩形的高度
  */
void OledDrawRectangle(uint8_t x,uint8_t y,uint8_t Width,uint8_t Heigth,uint8_t IsFill){
	if(!IsFill){
		//不填充矩形的情况下
		//首先画出矩形的上下两条线
		for(uint8_t i = x;i<(x+Width);i++){
			OledDrawPoint(i, y);
			OledDrawPoint(i, y+Heigth);
		}

		//在画出矩形的左右两条线
		for(uint8_t i = y;i<y+Heigth;i++){
			OledDrawPoint(x, i);
			OledDrawPoint(x+Width, i);
		}
	}else{

		//填充矩形的情况下 
		for(uint8_t i = x;i<x+Width;i++){
			for(uint8_t j = y;j<y+Heigth;j++){
				OledDrawPoint(i, j);
			}
		}
	}
}

static uint8_t OledPnpoly(uint8_t nvert,uint8_t *vx,uint8_t *vy,uint8_t testx,uint8_t testy){
	uint8_t i = 0, j = 0;
	uint8_t c = 0;
	for(i = 0,j=(nvert-1);i<nvert;j = i++){
		if( ( (vy[i] > testy) != (vy[j] > testy) ) &&
			(testx < (vx[j] - vx[i])*(testy - vy[i])/(vy[j] - vy[i])+vx[i]) ){
			c = !c;
		}
	}
	return c;
}

void OledDrawTriangle(uint8_t X0,uint8_t Y0,uint8_t X1,uint8_t Y1,uint8_t X2,uint8_t Y2,uint8_t IsFilled){
	if(!IsFilled){
		OledDrawLine(X0, Y0, X1, Y1);
		OledDrawLine(X0, Y0, X2, Y2);
		OledDrawLine(X2, Y2, X1, Y1);
	}else{
        OledDrawLine(X0, Y0, X1, Y1);
		OledDrawLine(X0, Y0, X2, Y2);
		OledDrawLine(X2, Y2, X1, Y1);
		uint8_t minx = X0,miny = Y0;
		uint8_t maxx = X0,maxy = Y0;
		uint8_t vx[] = {X0,X1,X2};
		uint8_t vy[] = {Y0,Y1,Y2};
		//找出多边形中最小的x，y坐标
		if(X1 < minx)	{minx = X1;}
		if(X2 < minx)	{minx = X2;}
		if(Y1 < miny)	{miny = Y1;}
		if(Y2 < miny)	{miny = Y2;}

		//找出多边形中最大的x，y坐标
		if(X1 > maxx)	{maxx = X1;}
		if(X2 > maxx)	{maxx = X2;}
		if(Y1 > maxy)	{maxy = Y1;}
		if(Y2 > maxy)	{maxy = Y2;}
		for(uint8_t i = minx;i<maxx ;i++){
			for(uint8_t j = minx;j<maxy ;j++){

				if(OledPnpoly(3,vx,vy,i,j)){
					OledDrawPoint(i,j);
				}
			}
		}
	}
}

void OledDrawCircle(uint8_t X,uint8_t Y,uint8_t Radius,uint8_t Isfiled){
	int16_t x,y,d;

	d = 1 - Radius;
	x = 0;
	y = Radius;

	OledDrawPoint(X+x, Y+y);
	OledDrawPoint(X-x, Y-y);
	OledDrawPoint(X+y, Y+x);
	OledDrawPoint(X-y, Y-x);
	
    if(Isfiled){
        for(int16_t j = -y;j < y;j++){
            OledDrawPoint(X, Y + j);
        }

    }

	while(x < y){
		x++;
		if(d < 0){
			d += 2*x + 1; 
		}else{
			y--;
			d += 2*(x-y) + 1;
		}

		/*画每个八分之一圆弧的点*/
		OledDrawPoint(X + x, Y + y);
		OledDrawPoint(X + y, Y + x);
		OledDrawPoint(X - x, Y - y);
		OledDrawPoint(X - y, Y - x);
		OledDrawPoint(X + x, Y - y);
		OledDrawPoint(X + y, Y - x);
		OledDrawPoint(X - x, Y + y);
		OledDrawPoint(X - y, Y + x);

		if(Isfiled){
			for(int16_t j = -y;j < y;j++){
				OledDrawPoint(X + x, Y + j);
				OledDrawPoint(X - x, Y + j);
			}

			for(int16_t j = -x;j < x;j++){
				OledDrawPoint(X - y, Y + j);
				OledDrawPoint(X + y, Y + j);
			}
		}
	}
}
