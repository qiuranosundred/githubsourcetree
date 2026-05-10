#include "stm32f4xx_hal.h"
#include "FM24CL64.h"
#include "gpio.h"

// SCL = PB6
// SDA = PB7

#define SDA_IN()		SDA2_GPIO_Port->MODER&=~(3<<(7*2));SDA2_GPIO_Port->MODER|=0<<7*2;
#define SDA_OUT()		SDA2_GPIO_Port->MODER&=~(3<<(7*2));SDA2_GPIO_Port->MODER|=1<<7*2;
#define IIC_SCL_0()		SCL2_GPIO_Port->BSRR = (uint32_t)SCL2_Pin << 16U
#define IIC_SCL_1()		SCL2_GPIO_Port->BSRR = SCL2_Pin
#define IIC_SDA_0() 	SDA2_GPIO_Port->BSRR = (uint32_t)SDA2_Pin << 16U
#define IIC_SDA_1()		SDA2_GPIO_Port->BSRR = SDA2_Pin
#define READ_SDA		(SDA2_GPIO_Port->IDR & SDA2_Pin)


/***************************************************************************//**
 * @brief 	2.5us延迟
 * @param	IIC_time 延迟时间(2.5us)
*******************************************************************************/
volatile void De_us(uint32_t IIC_time)
{
	uint16_t i=0,j=0;
	uint16_t IIC_usclock=30;
	while(i<IIC_time)
	{
		j=0;
		while(j<IIC_usclock)
		{
			j++;
		}
		i++;
	}
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_1();
	IIC_SCL_1();
	De_us(3);
 	IIC_SDA_0();//START:when CLK is high,DATA change form high to low
	De_us(3);
	IIC_SCL_0();//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_1();
	IIC_SDA_0();//STOP:when CLK is high DATA change form low to high
 	De_us(3);
	IIC_SDA_1();//发送I2C总线结束信号
	De_us(3);
	IIC_SCL_1();
}
//等待应答信号到来
//返回值：1，接收应答失败
//      0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入
	IIC_SDA_1();De_us(3);
	IIC_SCL_1();De_us(3);
	while(READ_SDA)
	{
		De_us(3);
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0();//时钟输出0
	return 0;
}
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_0();
	SDA_OUT();
	IIC_SDA_0();
	De_us(3);
	IIC_SCL_1();
	De_us(3);
	IIC_SCL_0();
}
//不产生ACK应答
void IIC_NAck(void)
{
	IIC_SCL_0();
	SDA_OUT();
	IIC_SDA_1();
	De_us(3);
	IIC_SCL_1();
	De_us(3);
	IIC_SCL_0();
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();//SDA设置为输出
    IIC_SCL_0();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        if((txd&0x80)>>7) IIC_SDA_1();
        else IIC_SDA_0();
        txd<<=1;
		IIC_SCL_1();
		De_us(3);
		IIC_SCL_0();
		De_us(3);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_0();
        De_us(3);
		IIC_SCL_1();
        receive<<=1;
        De_us(3);
        if(READ_SDA)receive++;
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}

/* FM24Cxx读数据函数*/
uint8_t FRAM_Read_32(uint8_t Slave_Add,uint16_t Address,uint8_t *pData, uint16_t Size)
{
    IIC_Start();
	IIC_Send_Byte(Slave_Add & 0xFE);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(Address>>8);//发送高地址
	IIC_Wait_Ack();
    IIC_Send_Byte(Address%256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(Slave_Add|0x01);//进入接收模式
	IIC_Wait_Ack();
	while(Size>1){
		*pData=IIC_Read_Byte(1);
		Size--;
		pData++;
	}
	(*pData)=IIC_Read_Byte(0);
    IIC_Stop();//产生一个停止条件
	return HAL_OK;
}

uint8_t FRAM_Write_32(uint8_t Slave_Add,uint16_t Address,uint8_t *pData, uint16_t Size)
{
    IIC_Start();
	IIC_Send_Byte(Slave_Add & 0xFE);//发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(Address>>8);//发送高地址
	IIC_Wait_Ack();
    IIC_Send_Byte(Address%256);//发送低地址
	IIC_Wait_Ack();
	while(Size>0){
		IIC_Send_Byte(*pData);
		IIC_Wait_Ack();
		Size--;
		pData++;
	}
    IIC_Stop();//产生一个停止条件
    return HAL_OK;
}

/*注意对普通变量使用 “&”获取变量地址，以保证浮点数，有符号数维持不变*/
void FRAM_Read(uint8_t TypeProgram, uint16_t Address, uint8_t *Data)
{
	uint8_t Slave_Add=0xA0;
	FRAM_Read_32(Slave_Add,Address,Data,TypeProgram);
}

/*将数据写入FM24CXX 芯片，使用指针读取数据，避免浮点数和有符号数被转换，
 *对普通变量使用 & 获取变量地址，程序将目标区域的数据当做8位无符号数直接存到FRAM*/
void FRAM_Program(uint8_t TypeProgram, uint16_t Address, uint8_t *Data)
{
	uint8_t Slave_Add=0xA0;
	FRAM_Write_32(Slave_Add,Address,Data,TypeProgram);
}

uint32_t CRC32_ForBytes(uint8_t* pData,uint32_t len){
	/***************************************************************************//**
	 * @brief 	计算8位数组的CRC32值
	 * @note	计算CRC写入
	 * @param	pData：计算的数组 len：数组长度
	 * @return  0:写入成功
	*******************************************************************************/
//	uint32_t u32Data[512]={0};
//	uint32_t i=0;
//	for(;i<len;i++){
//		u32Data[i] = *(pData+i);
//	}
//	return  HAL_CRC_Calculate(&hcrc, u32Data, len);
	return 0;
}


uint8_t DATA_Write_verify(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite){
	/***************************************************************************//**
	 * @brief 	FM24CL64 FRAM写入
	 * @note	同时将数据写入到FRAM与SPI FLASH
	 * @param	pBuffer：需要保存的数组; WriteAddr：保存地址; NumByteToWrite：写入长度
	 * @return  读取成功与否
	*******************************************************************************/
//	uint8_t verify_buf[512]={0};
//	uint8_t crcBuf[4] = {0};
//	uint32_t uCRCValue = CRC32_ForBytes(pBuffer,NumByteToWrite);
//	crcBuf[0] = (uCRCValue>>24) & 0xff;
//	crcBuf[1] = (uCRCValue>>16) & 0xff;
//	crcBuf[2] = (uCRCValue>>8) & 0xff;
//	crcBuf[3] = uCRCValue & 0xff;
//	memcpy(verify_buf,crcBuf,4);
//	memcpy(verify_buf+4,pBuffer,NumByteToWrite);
//
//	FRAM_Write_32(0xA0,WriteAddr,verify_buf,NumByteToWrite+4);
//	W25QXX_Write(verify_buf,WriteAddr,NumByteToWrite+4);
//
//	HAL_Delay(10);

	return HAL_OK;
}

uint8_t DATA_Read_verify(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead){
	/***************************************************************************//**
	 * @brief 	FM24CL64 FRAM读取
	 * @note	读取并进行CRC验证，读取FRAM失败后会写入到SPI FLASH
	 * @param	pBuffer：读取数据保存到的数组; ReadAddr：读取地址; NumByteToRead：读取长度
	 * @return  读取成功与否
	*******************************************************************************/
//	uint8_t crcBuf[4] = {0};
//
//	if(storage_location == LOCATION_I2C_FRAM){
//		FRAM_Read_32(0xA0,ReadAddr,crcBuf,4);
//		HAL_Delay(10);
//		FRAM_Read_32(0xA0,ReadAddr+4,pBuffer,NumByteToRead);
//	}else if(storage_location == LOCATION_SPI_FLASH){
//		W25QXX_Read(crcBuf,ReadAddr,4);
//		HAL_Delay(10);
//		W25QXX_Read(pBuffer,ReadAddr+4,NumByteToRead);
//	}
//
//	uint32_t uDestCRCValue = (uint32_t)crcBuf[0]*0x1000000 + (int32_t)crcBuf[1]*0x10000 + (int32_t)crcBuf[2]*0x100 + (int32_t)crcBuf[3];
//
//	if(CRC32_ForBytes(pBuffer,NumByteToRead) != uDestCRCValue){
//		if(storage_location == LOCATION_I2C_FRAM){
//			storage_location = LOCATION_SPI_FLASH;
//			W25QXX_Read(pBuffer,ReadAddr+4,NumByteToRead);
//			return HAL_OK;
//		}else if(storage_location == LOCATION_SPI_FLASH){
//			storage_location = LOCATION_I2C_FRAM;
//			FRAM_Read_32(0xA0,ReadAddr+4,pBuffer,NumByteToRead);
//			return HAL_OK;
//		}
//	}
	return HAL_OK;
}
