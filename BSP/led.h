#ifndef __LED_H
#define __LED_H

#include "main.h"


//三个状态 1用在（等待）WiFi配置       2用在参数配置            3用在维持心跳

#define LED1_FLAG {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);}//红

#define LED2_FLAG {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);}//全灭
#define LED3_FLAG {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);}//蓝

#endif
