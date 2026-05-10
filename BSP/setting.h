#ifndef __setting_H
#define __setting_H

#define Display(x,y,str,size) LCD_ShowString(x,y,str,size)
#define Clear(color) LCD_Fill(0,0,LCD_W,LCD_H,color);


#define OLED_HSPI hspi2
#define SIZE096


#define FLASH_HSPI hspi2

/********************************************************************************
  *����ģ��ӿ�
  *  Wireless Model Port
  ******************************************************************************/
#define WIFI_USART	USART1
#define WIFI_HUART 	huart1
//#define BS_USART	USART2
//#define BS_HUART 	huart2



/********************************************************************************
  *USB�����ӳ�
  *  USB Translate Delay
  ******************************************************************************/
#define HID_TRANS_DELAY 1000000

/**
  ******************************************************************************
  *ģ��
  *  Support Model
  ******************************************************************************/
#define MODEL_WIFI 0
#define MODEL_RFID 0
#define FRAM_SUPPORT 1
#define Warning_LED_MODE 0
#

#define VERSION "V2.0.0"
#endif
