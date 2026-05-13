#include "LCD.h"
#include "font.h"
#include "stdlib.h"
#include "spi.h"
#include "setting.h"
#include <stdio.h>
#define LCD_RST_Clr() 	HAL_GPIO_WritePin(LCD_RES_GPIO_Port,LCD_RES_Pin,GPIO_PIN_RESET)//RES
#define LCD_RST_Set() 	HAL_GPIO_WritePin(LCD_RES_GPIO_Port,LCD_RES_Pin,GPIO_PIN_SET)
#define LCD_DC_Clr() 	HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET)//DC
#define LCD_DC_Set() 	HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET)
#define LCD_BLK_ON()	HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_SET);
#define LCD_BLK_OFF()	HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_RESET);

#if __RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#define LockLcd() 	vTaskSuspendAll()
#define UnlockLcd()	xTaskResumeAll()

#else
#define LockLcd() 	__NOP()
#define UnlockLcd() __NOP()
#endif

#if __BMD36
	#define LCD_CS_Clr() 	__NOP()//CS
	#define LCD_CS_Set() 	__NOP()
	extern SPI_HandleTypeDef hspi3;
	#define LCD_HSPI hspi3
#elif __VMD
	#define LCD_CS_Clr() 	__NOP()//CS
	#define LCD_CS_Set() 	__NOP()
	extern SPI_HandleTypeDef hspi1;
	#define LCD_HSPI hspi1
#else
	#define LCD_CS_Clr() 	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET)//CS
	#define LCD_CS_Set() 	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET)
	extern SPI_HandleTypeDef hspi1;
	#define LCD_HSPI hspi1
#endif

uint16_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色

/***************************************************************************//**
 * @brief 	LCD写1B数据
*******************************************************************************/
void LCD_WR_DATA8(uint8_t da)
{
	LCD_CS_Clr();
	LCD_DC_Set();
	HAL_SPI_Transmit(&LCD_HSPI,&da,1,10);
	LCD_CS_Set();
}
/***************************************************************************//**
 * @brief 	LCD写数据
*******************************************************************************/
void LCD_WR_DATA(char* da,int len)
{
	LCD_CS_Clr();
	LCD_DC_Set();
	HAL_SPI_Transmit(&LCD_HSPI,da,len,10);
	LCD_CS_Set();
}

/***************************************************************************//**
 * @brief 	LCD写指令
*******************************************************************************/
void LCD_WR_REG(uint8_t da)
{
	LCD_CS_Clr();
	LCD_DC_Clr();
	HAL_SPI_Transmit(&LCD_HSPI,&da,1,10);
	LCD_CS_Set();
}

/***************************************************************************//**
 * @brief 	读取LCD ID
*******************************************************************************/
uint8_t Lcd_RDD_COLMOD(void)
{
	uint8_t COLMOD = 0;
	LCD_WR_REG(0x0c);
	LCD_CS_Clr();
	LCD_DC_Set();
	HAL_SPI_Receive(&LCD_HSPI,&COLMOD,1,10);
	return COLMOD ;
}

/***************************************************************************//**
 * @brief 	LCD初始化
*******************************************************************************/
void Lcd_Init(void)
{
    LCD_BLK_OFF();
    LCD_RST_Clr();
    HAL_Delay(20);
    LCD_RST_Set();
    HAL_Delay(20);

    LockLcd();

    LCD_WR_REG(0x11);     // Sleep exit
    HAL_Delay(120);

    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);

    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);

    LCD_WR_REG(0xB3);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);

    LCD_WR_REG(0xB4);     // Dot inversion
    LCD_WR_DATA8(0x03);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x04);

    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0xC0);

    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0x2A);

    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0xEE);

    LCD_WR_REG(0xC5);     // VCOM
    LCD_WR_DATA8(0x1A);

    /* ============== 关键修改区域 ============== */
    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0xC0);     // ← 先改回你原来的值（最可能适合横屏）

    /* ========================================== */

    LCD_WR_REG(0xE0);           // Gamma
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x22);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x30);
    LCD_WR_DATA8(0x25);
    LCD_WR_DATA8(0x2A);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x3A);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x13);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x16);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x2D);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x23);
    LCD_WR_DATA8(0x27);
    LCD_WR_DATA8(0x27);
    LCD_WR_DATA8(0x25);
    LCD_WR_DATA8(0x2D);
    LCD_WR_DATA8(0x3B);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x13);

    LCD_WR_REG(0x3A);   // RGB565
    LCD_WR_DATA8(0x05);

    LCD_WR_REG(0x29);   // Display on

    UnlockLcd();

    LCD_BLK_ON();

    /* 可选：初始化后清一次屏 */
    // LCD_Clear(BLACK);
}
/***************************************************************************//**
 * @brief 	设置显示地址
 * @param	x1 y1 x2 y2 坐标
*******************************************************************************/
//void Address_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
//{
//	x1+=2;x2+=2;
//	LCD_WR_REG(0x2a);
//	LCD_WR_DATA8(x1>>8);
//	LCD_WR_DATA8(x1);
//	LCD_WR_DATA8(x2>>8);
//	LCD_WR_DATA8(x2);
//
//	y1+=1;y2+=1;
//	LCD_WR_REG(0x2b);
//	LCD_WR_DATA8(y1>>8);
//	LCD_WR_DATA8(y1);
//	LCD_WR_DATA8(y2>>8);
//	LCD_WR_DATA8(y2);
//	LCD_WR_REG(0x2C);
//}

void Address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	x1+=2;x2+=2;
	y1+=1;y2+=1;
    LCD_WR_REG(0x2a);
    LCD_WR_DATA8(x1 >> 8);
    LCD_WR_DATA8(x1);
    LCD_WR_DATA8(x2 >> 8);
    LCD_WR_DATA8(x2);

    LCD_WR_REG(0x2b);
    LCD_WR_DATA8(y1 >> 8);
    LCD_WR_DATA8(y1);
    LCD_WR_DATA8(y2 >> 8);
    LCD_WR_DATA8(y2);

    LCD_WR_REG(0x2C);   // 开始写显存
}
/***************************************************************************//**
 * @brief 	LCD清屏
 * @param	Color 清屏颜色
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint16_t i;
	uint8_t color[LCD_W*2];
	for(i=0;i<LCD_W;i++)
	{
		color[2*i]=Color;
		color[2*i+1]=Color>>8;
	}

	LockLcd();
	Address_set(0,0,LCD_W-1,LCD_H-1);
	for(i=0;i<LCD_H;i++){
		LCD_WR_DATA(color,2*LCD_W);
	}
	UnlockLcd();
}
/***************************************************************************//**
 * @brief 	在指定区域内填充指定颜色
 * @param	xstax ysta xend yend 坐标
 * @param	color 填充颜色
*******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t Color)
{
	uint16_t i;
	uint16_t Fill_W;
	uint8_t color[LCD_W*2];
	if(yend >= LCD_W)
		xend = LCD_W - 1;
	if(yend >= LCD_H)
		xend = LCD_H - 1;
	Fill_W = xend - xsta + 1;
	for(i=0;i<Fill_W;i++)
	{
		color[2*i]=Color;
		color[2*i+1]=Color>>8;
	}

	LockLcd();
	Address_set(xsta,ysta,xend,yend);      //设置光标位置
	for(i=ysta;i<=yend;i++)
	{
		LCD_WR_DATA(color,2*Fill_W);
	}
	UnlockLcd();
}

/***************************************************************************//**
 * @brief 	画点
 * @param	x y 坐标
*******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LockLcd();
	Address_set(x,y,x,y);//设置光标位置
	LCD_WR_DATA8(POINT_COLOR);
	LCD_WR_DATA8(POINT_COLOR>>8);
	UnlockLcd();
}
/***************************************************************************//**
 * @brief 	画线
 * @param	x1,y1 起点坐标
 * @param	x2,y2 终点坐标
*******************************************************************************/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平线
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//画线输出
	{
		LCD_DrawPoint(uRow,uCol);//画点
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/***************************************************************************//**
 * @brief 	画矩形
 * @param	x1,y1 起点坐标
 * @param	x2,y2 终点坐标
*******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

/***************************************************************************//**
 * @brief 	在指定位置画一个指定大小的圆
 * @param	(x,y) 中心点
 * @param	r 半径
*******************************************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3
		LCD_DrawPoint(x0+b,y0-a);             //0
		LCD_DrawPoint(x0-a,y0+b);             //1
		LCD_DrawPoint(x0-b,y0-a);             //7
		LCD_DrawPoint(x0-a,y0-b);             //2
		LCD_DrawPoint(x0+b,y0+a);             //4
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6
		LCD_DrawPoint(x0-b,y0+a);
		a++;
		//使用Bresenham算法画圆
		if(di<0)di +=4*a+6;
		else
		{
			di+=10+4*(a-b);
			b--;
		}
		LCD_DrawPoint(x0+a,y0+b);
	}
}


/***************************************************************************//**
 * @brief 	在指定位置显示一个字符
 * @note	使用固件区自带字库
 * @param	(x,y) 坐标
 * @param	num 字符ASCII码
 * @param	显示大小
*******************************************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size)
{
#if __RTOS
	uint16_t *date = (uint16_t*)pvPortMalloc(1024);
#else
	uint16_t date[512];
#endif
    uint8_t temp;
    uint8_t pos,t;
    if(x>LCD_W-size/2||y>LCD_H-size)return;
	num=num-' ';//得到偏移后的值
	if(size==16){
		for(pos=0;pos<16;pos++)		{
			temp=ACS2_1608[(uint16_t)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)			{
				if(temp&0x01)				{
					date[(pos*8+t)]=POINT_COLOR;
				}else				{
					date[(pos*8+t)]=BACK_COLOR;
				}
				temp>>=1;
			}
		}
		LockLcd();
		Address_set(x,y,x+8-1,y+16-1);      //设置光标位置
		LCD_WR_DATA(date,256);
		UnlockLcd();
	}
	if(size==32){
		for(pos=0;pos<64;pos++){
			temp=ACS2_3216[(uint16_t)num*64+pos];		 //调用3216字体
			for(t=0;t<8;t++){
				if(temp&0x01){
					date[(pos*8+t)]=POINT_COLOR;
				}else{
					date[(pos*8+t)]=BACK_COLOR;
				}
				temp>>=1;
			}
		}
		LockLcd();
		Address_set(x,y,x+16-1,y+32-1);      //设置光标位置
		LCD_WR_DATA(date,1024);
		UnlockLcd();
	}
#if __RTOS
	vPortFree(date);
#endif
}
//m^n函数
unsigned long NumPow(uint8_t m,uint8_t n)
{
	unsigned long result=1;
	while(n--)result*=m;
	return result;
}

/***************************************************************************//**
 * @brief 	显示数字
 * @note	使用固件区自带字库,字体大小:16
 * @param	(x,y) 坐标
 * @param	len 数字的位数
 * @param	num 数值(0~999999);
*******************************************************************************/
void LCD_ShowNum(uint16_t x,uint16_t y,unsigned long num,uint8_t len)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	num=(uint16_t)num;
	for(t=0;t<len;t++)
	{
		temp=(num/NumPow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+16*t,y,' ',16);
				continue;
			}else enshow=1;

		}
		LCD_ShowChar(x+8*t,y,temp+16+' ',16);
	}
}

/***************************************************************************//**
 * @brief 	显示字符串
 * @note	使用固件区自带字库
 * @param	x,y 起点坐标
 * @param	*p 字符串起始地址
 * @param	size 字体大小
*******************************************************************************/
void LCD_ShowString(uint16_t x,uint16_t y,const char *p,uint8_t size)
{
	LockLcd();
    while(*p!='\0')
    {
        if(x>LCD_W-size/2)
        {
        	x=0;
        	y+=size;
        }
        if(y>LCD_H-size) break;
        LCD_ShowChar(x,y,*p,size);
        x+=size/2;
        p++;
    }
	UnlockLcd();
}

/***************************************************************************//**
 * @brief 	显示一个16号大小的GBK文字
 * @note	使用FLASH外置字库
 * @param	x,y 起点坐标
 * @param	qh GBK文字高8位
 * @param	wh GBK文字低8位
 * @param	gray 反色 0：不反色 1：反色
 * @param	color 字体颜色
*******************************************************************************/
void LCD_ShowZh16(uint8_t x,uint8_t y,char qh ,char wh ,uint32_t font ,uint8_t gray,uint16_t color)
{
	char t,i;
	uint8_t count_max = (qh?16:8);
	uint8_t zh[32];
	static uint16_t data[128] = {0};
	uint32_t offset;
	if(qh>0) offset = (94*(qh-0xa1)+(wh-0xa1))*32;
	else offset = wh*32;
	W25QXX_Read(zh,font + offset ,32);
	for(t=0;t<count_max;t++){
		uint8_t xorD = 0x01;
		for(i=0;i<8;i++){
			if(((gray?zh[t]:(~zh[t]))&xorD)>0){
		 		data[count_max*i+t] = color;
			}
			else{
		 		data[count_max*i+t] = BACK_COLOR;
			}
			xorD = xorD<<1;
		}
	}
	LockLcd();
	Address_set(x,y,x+count_max-1,y+8-1);      //设置光标位置
	LCD_WR_DATA(data,(qh?256:128));
	UnlockLcd();
	for(t=0;t<count_max;t++){
		uint8_t xorD = 0x01;
		for(i=0;i<8;i++){
			if(((gray?zh[16+t]:(~zh[16+t]))&xorD)>0){
		 		data[count_max*i+t] = color;
			}
			else{
		 		data[count_max*i+t] = BACK_COLOR;
			}
			xorD = xorD<<1;
		}
	}
	LockLcd();
	Address_set(x,y+8,x+count_max-1,y+16-1);      //设置光标位置
	LCD_WR_DATA(data,(qh?256:128));
	UnlockLcd();
}

/***************************************************************************//**
 * @brief 	显示一个32号大小的GBK文字
 * @note	使用FLASH外置字库
 * @param	x,y 起点坐标
 * @param	qh GBK文字高8位
 * @param	wh GBK文字低8位
 * @param	gray 反色 0：不反色 1：反色
 * @param	color 字体颜色
*******************************************************************************/
static void LCD_ShowZh32(uint8_t x,uint8_t y,char qh ,char wh ,uint32_t font ,uint8_t gray,uint16_t color)
{
	char t,i;
	uint8_t count_max = (qh?32:16);
	uint8_t zh[128];
	static uint16_t data[1024] = {0};
	uint32_t offset;
	if(qh>0) offset = (94*(qh-0xa1)+(wh-0xa1))*128;
	else offset = wh*128;
	W25QXX_Read(zh,font + offset ,128);
	for(t=0;t<count_max;t++){
		uint8_t xorD = 0x01;
		for(i=0;i<8;i++){
			if(((gray?zh[t]:(~zh[t]))&xorD)>0){
		 		data[count_max*i+t] = color;
			}
			else{
		 		data[count_max*i+t] = BACK_COLOR;
			}
			xorD = xorD<<1;
		}
	}
	LockLcd();
	Address_set(x,y,x+count_max-1,y+8-1);      //设置光标位置
	LCD_WR_DATA(data,(qh?512:256));
	UnlockLcd();
	for(t=0;t<count_max;t++){
		uint8_t xorD = 0x01;
		for(i=0;i<8;i++){
			if(((gray?zh[32+t]:(~zh[32+t]))&xorD)>0){
		 		data[count_max*i+t] = color;
			}
			else{
		 		data[count_max*i+t] = BACK_COLOR;
			}
			xorD = xorD<<1;
		}
	}
	LockLcd();
	Address_set(x,y+8,x+count_max-1,y+16-1);      //设置光标位置
	LCD_WR_DATA(data,(qh?512:256));
	UnlockLcd();
	for(t=0;t<count_max;t++){
		uint8_t xorD = 0x01;
		for(i=0;i<8;i++){
			if(((gray?zh[64+t]:(~zh[64+t]))&xorD)>0){
		 		data[count_max*i+t] = color;
			}
			else{
		 		data[count_max*i+t] = BACK_COLOR;
			}
			xorD = xorD<<1;
		}
	}
	LockLcd();
	Address_set(x,y+16,x+count_max-1,y+24-1);      //设置光标位置
	LCD_WR_DATA(data,(qh?512:256));
	UnlockLcd();
	for(t=0;t<count_max;t++){
		uint8_t xorD = 0x01;
		for(i=0;i<8;i++){
			if(((gray?zh[96+t]:(~zh[96+t]))&xorD)>0){
		 		data[count_max*i+t] = color;
			}
			else{
		 		data[count_max*i+t] = BACK_COLOR;
			}
			xorD = xorD<<1;
		}
	}
	LockLcd();
	Address_set(x,y+24,x+count_max-1,y+36-1);      //设置光标位置
	LCD_WR_DATA(data,(qh?512:256));
	UnlockLcd();
}

void LCD_ShowGBKStr(uint8_t x,uint8_t y,char *p,uint8_t size ,uint32_t font ,uint8_t gray,uint16_t color)
{
	unsigned char j=0;
	while (p[j]!='\0')
	{
		if(size==16)
		{
			if(p[j]<0xa0)
			{
				LCD_ShowZh16(x,y,0,p[j],font,gray,color);
				x+=8;
				j++;
			}
			else
			{
				LCD_ShowZh16(x,y,p[j],p[j+1],font + 0x2000,gray,color);
				x+=16;
				j+=2;
			}
			if(x>120){x=0;y+=16;}
		}
		else if(size==32)
		{
			if(p[j]<0xa0)
			{
				LCD_ShowZh32(x,y,0,p[j],font + 0x47080,gray,color);
				x+=16;
				j++;
			}
			else
			{
				LCD_ShowZh32(x,y,p[j],p[j+1],font + 0x4F080,gray,color);
				x+=32;
				j+=2;
			}
			if(x>111){x=0;y+=32;}
		}
		else
		{
			break;
		}
	}
}

/***************************************************************************//**
 * @brief 	显示一张图片
 * @note	颜色格式 RGB565
 * 			文件格式 {"W",宽[2],"H",高[2],图片数据}
 * @param	x,y 起点坐标
 * @param	offset 图片在FLASH中的地址
 * @param	Location 对齐方式
*******************************************************************************/
void LCD_ShowImg(uint8_t x,uint8_t y,uint32_t offset,uint8_t Location){
	LockLcd();
	uint8_t head[4] = {0};
	W25QXX_Read(head,offset ,4);
	if(head[0]=='W' && head[2]=='H'){
		uint16_t wight = head[1],count=0,height = head[3];
		static uint8_t data[LCD_W*2] = {0};
		while(count < height-1){
			W25QXX_Read(data,offset+count*wight*2+4 ,wight*2);
			LockLcd();
			if(Location == L_centered){
				Address_set(63-(wight/2),y+count,63-(wight/2)+wight-1,y+count);      //设置光标位置
			}else{
				Address_set(x,y+count,x+wight-1,y+count);      //设置光标位置
			}
			LCD_WR_DATA(data,wight*2);
			UnlockLcd();
			count++;
		}
	}
	UnlockLcd();
}

/***************************************************************************//**
 * @brief 	显示一个GBK字库的数字
 * @note	使用固件区自带字库,字体大小:16
 * @param	(x,y) 坐标
 * @param	len 数字的位数
 * @param	num 数值(0~999999);
 * @param	size 字体大小
 * @param	font 字体库在FLASH中的位置
 * @param	gray 是否反色
 * @param	color 字体颜色
*******************************************************************************/
void LCD_ShowGBKNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint32_t font,uint8_t gray,uint16_t color)
{
	LockLcd();
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/NumPow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(size==16)
					LCD_ShowZh16(x+(size/2)*t,y,0x00,0x30,font,gray,color);
				else if(size==32){
					LCD_ShowZh32(x+(size/2)*t,y,0x00,0x30,font + 0x47080,gray,color);
				}
				continue;
			}else enshow=1;

		}
		if(size==16)
			LCD_ShowZh16(x+(size/2)*t,y,0x00,temp+0x30,font,gray,color);
		else if(size==32)
			LCD_ShowZh32(x+(size/2)*t,y,0x00,temp+0x30,font + 0x47080,gray,color);
	}
	UnlockLcd();
}

/***************************************************************************//**
 * @brief 	按电池电量显示电池图标
*******************************************************************************/
void LCD_DrawBat(uint16_t bat_ad){
//	static bat_level = 5;
//	if(bat_ad<3230 && bat_level == 5) bat_level = 4;
//	if(bat_ad<3110 && bat_level == 4) bat_level = 3;
//	if(bat_ad<2990 && bat_level == 3) bat_level = 2;
//	if(bat_ad<2870 && bat_level == 2) bat_level = 1;
//	if(bat_ad<*Set_LowBt && bat_level == 1) bat_level = 0;
}

/***************************************************************************//**
 * @brief 	字符串格式化输出
 * @param	(x,y) 坐标
 * @param	fmt 字符串
 * @param	格式化参数
*******************************************************************************/
void debug(uint16_t x,uint16_t y,const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	uint8_t str_buf[30] = {0};
	vsprintf(str_buf,fmt,args);
	LCD_ShowString(x,y,str_buf,16);
	va_end(args);
}
