#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "main.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"

//#define RECEIVE_BUF_ROW_MAX			50

#define RECEIVE_BIG_BUF_COLUMN_MAX	20
#define RECEIVE_BIG_BUF_ROW_MAX		700


//cccd有定义
//typedef enum
//{
//	UART_IDLE = 0,
//	UART_RXING,
//	UART_DONE
//}Uart_Status;




//extern uint8_t receive_buf[RECEIVE_BUF_ROW_MAX];
//extern uint8_t receive_buf_len;

extern uint8_t uart_buf;
extern uint8_t data_received_flg;

extern uint8_t receive_big_buf[RECEIVE_BIG_BUF_COLUMN_MAX][RECEIVE_BIG_BUF_ROW_MAX];
extern uint16_t receive_big_buf_len[RECEIVE_BIG_BUF_COLUMN_MAX];
extern uint16_t receive_big_buf_line;

//extern Uart_Status uart_status;
extern uint8_t uart_overtime;

extern uint32_t Lose_Data_Time;
extern uint32_t Lose_Heart_Time;
extern uint32_t Heart_Send_Count;
extern uint32_t Upload_Short_Or_Long_Package_Timeout;
extern uint32_t  Upload_Passive_Reply_Timeout;

extern uint8_t Sys_Save_Flag;
extern uint8_t UpdateTime_Flag;
//extern uint8_t receive_buf[5][650]; //数据存储缓存
//extern uint16_t receive_buf_len[5]; //数据长度
//extern uint8_t receive_buf_line; //数据行数
//extern uint8_t data_received_flg;
//extern uint8_t Heart_flg;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif
