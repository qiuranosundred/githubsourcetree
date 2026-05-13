#include "interrupt.h"


//extern SemaphoreHandle_t xSemaphore;
uint32_t text_aaa = 0;

uint8_t uart_buf;


//uint8_t receive_buf[RECEIVE_BUF_ROW_MAX];
//uint8_t receive_buf_len;

//__attribute__ (( section (".ccmram") ))
uint8_t receive_big_buf[RECEIVE_BIG_BUF_COLUMN_MAX][RECEIVE_BIG_BUF_ROW_MAX];
uint16_t receive_big_buf_len[RECEIVE_BIG_BUF_COLUMN_MAX];
uint16_t receive_big_buf_line = 0;

Uart_Status uart_status;
uint8_t uart_overtime = 0;
uint8_t data_received_flg = 0;

//锟斤拷时锟斤拷
uint32_t Lose_Data_Time = 0;
uint32_t Lose_Heart_Time = 0;
uint32_t Heart_Send_Count = 0;
uint32_t Upload_Short_Or_Long_Package_Timeout = 0;

uint32_t  Upload_Passive_Reply_Timeout = 0;//锟斤拷锟斤拷锟斤拷时锟剿筹拷 锟较达拷状态 确锟斤拷锟节凤拷锟斤拷时锟斤拷锟捷诧拷锟皆讹拷锟斤拷锟斤拷锟剿筹拷upload状态

uint32_t Sys_Save_Time =  0;
uint8_t Sys_Save_Flag = 0;

uint8_t Sys_Update_Time_Flag = 0;

uint32_t RealTime_Time = 0;
uint8_t RealTime_Flag = 0;

uint32_t UpdateTime_Count= 0;
uint8_t UpdateTime_Flag = 0;

///////////////////////////////////////////////////////////////////
#define TimerTypeDef_default {0 , 0 ,0}
TimerTypeDef workTick = TimerTypeDef_default;

uint8_t EER_Dis_time=0;//0.5S锟斤拷烁
///////////////////////////////////////////////////////////////////


//锟斤拷锟斤拷锟斤拷
uint32_t text_line_num = 0;
///
//uint8_t data_received_flg = 0; //锟斤拷锟秸憋拷志位锟斤拷锟斤拷锟斤拷欠锟斤拷锟斤拷锟斤拷锟斤拷戏锟�

//uint8_t overtime = 0; //锟斤拷时时锟斤拷

//uint16_t receive_buf_len[5] = {0}; //锟斤拷锟捷筹拷锟斤拷
//uint8_t receive_buf_line = 0; //锟斤拷锟斤拷锟斤拷锟斤拷

//
//uint16_t Heart_overtime = 0;
//uint8_t Heart_flg=1;
//uint8_t Heart_Beat[10] = {0xFF,0xFF,0x00,0x78,0xCF,0xFF,0x01,0xFF,0xFF,0xFF};


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART2)
//	{
//		uart_overtime = 0;
//		TIM7->CNT = 0;
//
//		if(uart_status != UART_DONE)
//		{
//			uart_overtime = 0;
//			if(uart_status == UART_IDLE)
//			{
//				uart_status = UART_RXING;
//
//				if(status != UPLOAD)//锟斤拷锟斤拷使锟斤拷一维锟斤拷锟斤拷锟�
//				{
//					receive_buf_len = 0;
//				}
//				else				//锟斤拷锟斤拷使锟矫讹拷维锟斤拷锟斤拷锟�
//				{
//					//receive_big_buf_len[receive_big_buf_line] = 0;
//				}
//			}
//			if(status != UPLOAD)//锟斤拷锟斤拷使锟斤拷一维锟斤拷锟斤拷锟�
//			{
//				receive_buf[receive_buf_len] = uart_buf;
//				receive_buf_len++;
//			}
//			else				//锟斤拷锟斤拷使锟矫讹拷维锟斤拷锟斤拷锟�
//			{
////				receive_big_buf[receive_big_buf_line][receive_big_buf_len[receive_big_buf_line]] = uart_buf;
////				receive_big_buf_len[receive_big_buf_line]++;
//			}
//		}
//
//		HAL_UART_Receive_IT(&huart1,&uart_buf,1);
//	}
//}


//锟斤拷锟节斤拷锟斤拷锟叫断回碉拷锟斤拷锟斤拷
//cccd锟叫讹拷锟斤拷锟斤拷

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART1)
//	{
//		Lose_Heart_Time= 0;
//		uart_overtime = 0;
//		Upload_Short_Or_Long_Package_Timeout = 0;
//		TIM7->CNT = 0;
//		data_received_flg = 1;
//
//
//		receive_big_buf[receive_big_buf_line][receive_big_buf_len[receive_big_buf_line]] = uart_buf;
//		receive_big_buf_len[receive_big_buf_line]++;
//
//		HAL_UART_Receive_IT(&huart1,&uart_buf,1);
//
//	}
//}





void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	  if (htim->Instance == TIM14)
	  {
	    HAL_IncTick();
	  }

    if(htim->Instance == TIM7)// 0.5ms
    {

    }

    if(htim->Instance == TIM6)// 10ms
    {

    }

    if(htim->Instance == TIM10)// 1ms
    {
    	lv_tick_inc(1); // 告诉LVGL：时间过了1ms
  	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{

}






