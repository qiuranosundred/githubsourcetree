#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "main.h"

//按键按下与松开的电平
#define PRESS 0

#if PRESS
	#define LOOSEN 0
#else
	#define LOOSEN 1
#endif


#define SHORT_PRESS_TIME 10


typedef struct
{
	uint16_t Press_Time;
	uint8_t Short_Press_Flag;
	uint8_t text;

}Key_Typedef;

extern Key_Typedef Key1;
extern Key_Typedef Key2;
extern Key_Typedef Key3;

void updateKeyTime(void);//更新按键时间 放入定时器


#endif
