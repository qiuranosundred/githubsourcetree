#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "setting.h"

#define LCD_W 128
#define LCD_H 160
#define Font_Con 0x100000
#define Marked	 0x00
#define Unmark	 0x01
#define L_left	0x0
#define L_centered 0x01
#define L_right 0x02

extern uint16_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void LCD_WR_DATA8(uint8_t da); //发送数据-8位参数
void LCD_WR_REG(uint8_t da);
void Address_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

uint8_t Lcd_RDD_COLMOD(void);
void Lcd_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_DrawPoint(uint16_t x,uint16_t y);//画点
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t Color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size);//显示一个字符
void LCD_ShowSMG(uint16_t x,uint16_t y,uint8_t num);//显示一个数码管字符
void LCD_ShowNum(uint16_t x,uint16_t y,unsigned long num,uint8_t len);//显示数字
void LCD_ShowString(uint16_t x,uint16_t y,const char *p,uint8_t size);//显示一个字符串,16字体

void LCD_DrawBat(uint16_t bat_ad);
void LCD_DrawSys(uint8_t num);
void LCD_DrawMain(uint8_t Title);
void LCD_DrawDemo();
void showhanzi(uint16_t x,uint16_t y,uint8_t index);
void debug(uint16_t x,uint16_t y,const char *fmt, ...);
//画笔颜色
#define WHITE				0xFFFF
#define BLACK				0x0000
#define BLUE				0x1F00
#define BRED				0X1FF8
#define GRED				0XE0FF
#define GBLUE				0XFF07
#define RED					0x00F8
#define MAGENTA				0x1FF8//品红
#define GREEN				0xE007
#define CYAN				0xFF7F//青色
#define YELLOW				0xE0FF
#define BROWN				0X40BC //棕色
#define BRRED				0X07FC //棕红色
#define GRAY				0x2421 //灰色

//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN		0X841F //浅绿色
#define LGRAY			0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#define IMG_LOGO 	 (0x400000+0)
#define IMG_LOWBAT 	 (0x400000+40976)
#define IMG_FORWARD 	 (0x400000+43872)
#define IMG_REVERSE 	 (0x400000+46768)
#define IMG_OVERTQ 	 (0x400000+49664)
#define IMG_SENSORERR 	 (0x400000+52560)
#define IMG_OVERTMP 	 (0x400000+55456)
#define IMG_WIFI_ONLINE_STATUE 	 (0x400000+58352)
#define IMG_READY 	 (0x400000+59008)
#define IMG_HIGH_TEMP 	 (0x400000+66400)
#define IMG_LOW_TEMP 	 (0x400000+66928)
#define IMG_MEDIUM_TEMP 	 (0x400000+67456)

#endif
