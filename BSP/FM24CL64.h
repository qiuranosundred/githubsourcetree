#ifndef __FM24CL64_H
#define __FM24CL64_H

#include "stm32F4xx_hal.h"
#include "main.h"
#include "stm32F4xx_hal_def.h"

#define LOCATION_SPI_FLASH 1
#define LOCATION_I2C_FRAM 0

#define Max_Add 8192

#define FRAM_BYTE        	1  /*!< Program byte (8-bit) at a specified address           */
#define FRAM_HALFWORD    	2  /*!< Program a half-word (16-bit) at a specified address   */
#define FRAM_WORD    		4  /*!< Program a word (32-bit) at a specified address        */
#define FRAM_DOUBLEWORD  	8  /*!< Program a double word (64-bit) at a specified address */

uint8_t FRAM_Read_32(uint8_t Slave_Add,uint16_t Address,uint8_t *pData, uint16_t Size);
uint8_t FRAM_Write_32(uint8_t Slave_Add,uint16_t Address,uint8_t *pData, uint16_t Size);
void FRAM_Program(uint8_t TypeProgram, uint16_t Address, uint8_t *Data);
void FRAM_Read(uint8_t TypeData, uint16_t Address, uint8_t *Data);
uint8_t DATA_Read_verify(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
uint8_t DATA_Write_verify(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void DATA_Write_Type(uint8_t TypeProgram, uint16_t Address, uint8_t *Data);
void DATA_Read_Type(uint8_t TypeProgram, uint16_t Address, uint8_t *Data);
#endif /* __FM24CL64_H */
