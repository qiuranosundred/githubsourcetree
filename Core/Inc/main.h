/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "interrupt.h"
//#include "funcimp.h"
//#include "led.h"
//#include "table.h"
//#include "crc16_ccitt.h"
//#include "usart_redirect.h"

#include "lvgl.h"
#include "LCD.h"
//#include "string.h"
//#include "stdio.h"


//#include "setting.h"

//#include "menu.h"

//#include "CCCD.h"
//#include "view.h"

//#include "bsp_key.h"
//#include "Basic_interface.h"
//#include "menu.h"
//#include "bsp_wifi12.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY2_Pin GPIO_PIN_0
#define KEY2_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_1
#define KEY3_GPIO_Port GPIOA
#define MT_WIFI_DET_Pin GPIO_PIN_4
#define MT_WIFI_DET_GPIO_Port GPIOA
#define MATOU_CTS_Pin GPIO_PIN_5
#define MATOU_CTS_GPIO_Port GPIOA
#define SLEEP_IO_Pin GPIO_PIN_6
#define SLEEP_IO_GPIO_Port GPIOA
#define STATA_IO_Pin GPIO_PIN_7
#define STATA_IO_GPIO_Port GPIOA
#define LINK_IO_Pin GPIO_PIN_4
#define LINK_IO_GPIO_Port GPIOC
#define FAULT_IO_Pin GPIO_PIN_5
#define FAULT_IO_GPIO_Port GPIOC
#define IR_IO_Pin GPIO_PIN_0
#define IR_IO_GPIO_Port GPIOB
#define CS_FLASH_Pin GPIO_PIN_1
#define CS_FLASH_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_12
#define KEY1_GPIO_Port GPIOB
#define MATOU_RTS_Pin GPIO_PIN_13
#define MATOU_RTS_GPIO_Port GPIOB
#define LCD_RES_Pin GPIO_PIN_10
#define LCD_RES_GPIO_Port GPIOC
#define LCD_BLK_Pin GPIO_PIN_11
#define LCD_BLK_GPIO_Port GPIOC
#define LCD_DC_Pin GPIO_PIN_12
#define LCD_DC_GPIO_Port GPIOC
#define SCL2_Pin GPIO_PIN_6
#define SCL2_GPIO_Port GPIOB
#define SDA2_Pin GPIO_PIN_7
#define SDA2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef enum
{
	UART_IDLE = 0x0000 ,// 空闲状态
	UART_RXING = 0x0001,// 正在接收数据
	UART_DONE = 0x0002,// 接收完成
	UART_INC = 0x8000// 数据帧不完整(两字符间的空闲间隔大于1.5个字符时间)
}Uart_Status;


typedef struct
{
	uint32_t	datalen;
	uint8_t		tmpbuf;
	uint8_t		data[512];
	uint16_t	statue;
}UartTypeDef;

typedef struct
{
	uint32_t	c_min;
	uint32_t	c_s;
	uint32_t	c_ms;

}TimerTypeDef;
#define __RTOS 1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
