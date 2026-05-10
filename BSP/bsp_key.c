#include "bsp_key.h"

Key_Typedef Key1;
Key_Typedef Key2;
Key_Typedef Key3;

void updateKeyTime(void)
{
	if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == PRESS)
	{
		Key1.Press_Time++;
	}
	else
	{
		if(Key1.Press_Time >= SHORT_PRESS_TIME)
		{
			Key1.Short_Press_Flag = 1;
		}
		Key1.Press_Time = 0;
	}

	if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == PRESS)
	{
		Key2.Press_Time++;
	}
	else
	{
		if(Key2.Press_Time >= SHORT_PRESS_TIME)
		{
			Key2.Short_Press_Flag = 1;
		}
		Key2.Press_Time = 0;
	}

	if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == PRESS)
	{
		Key3.Press_Time++;
	}
	else
	{
		if(Key3.Press_Time >= SHORT_PRESS_TIME)
		{
			Key3.Short_Press_Flag = 1;
		}
		Key3.Press_Time = 0;
	}
}
