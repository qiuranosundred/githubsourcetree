/***************************************************************************//**
 * @file W25Qxx.c
 * @brief W25Qxx驱动
 * @note    4Kbytes为一个Sector
 * 			16个扇区为1个Block
 * 			W25Q128 容量为16M字节,共有128个Block,8192个Sector
 * @author Yu
 * @email yujingpeng@wivor.com
 * @version 1.0
 * @date 2019-05
 * @license WIVOR INDUSTRY
*******************************************************************************/
#include "W25Qxx.h"
#include "setting.h"

#if __RTOS
#include "cmsis_os.h"

#define  W25QXX_CS1()  HAL_GPIO_WritePin(CS_FLASH_GPIO_Port,CS_FLASH_Pin,GPIO_PIN_SET);
#define  W25QXX_CS0()  HAL_GPIO_WritePin(CS_FLASH_GPIO_Port,CS_FLASH_Pin,GPIO_PIN_RESET);

#define LockFlash() 	vTaskSuspendAll()
#define UnlokFlash() 	xTaskResumeAll()
#else
#define  W25QXX_CS1()  HAL_GPIO_WritePin(CS_FLASH_GPIO_Port,CS_FLASH_Pin,GPIO_PIN_SET)
#define  W25QXX_CS0()  HAL_GPIO_WritePin(CS_FLASH_GPIO_Port,CS_FLASH_Pin,GPIO_PIN_RESET)  //W25QXX的片选信号

#endif

uint16_t W25QXX_TYPE;		//定义W25QXX芯片型号
uint16_t Flash_Size;		//FLASH的容量（KB）

extern SPI_HandleTypeDef FLASH_HSPI;

/***************************************************************************//**
 * @brief 	SPI写入并读取一个字节
 * @param	IIC_time 延迟时间(2.5us)
*******************************************************************************/
unsigned char SPI_ReadWriteByte(unsigned char date)
{
    unsigned char Rxdata;
    HAL_SPI_TransmitReceive(&FLASH_HSPI,&date,&Rxdata,1,1000);
    return Rxdata;
}

/***************************************************************************//**
 * @brief 	初始化W25Qxx
 * @param	初始化W25Qxx FLASH并读取FLASH容量
*******************************************************************************/
void W25QXX_Init(void)
{
	Flash_Size=0;
	W25QXX_TYPE=W25QXX_ReadID();//读取FLASH ID.
    switch(W25QXX_TYPE)
    {
    case 0xEF13: Flash_Size=1024;break;
    case 0xEF14: Flash_Size=1024*2;break;
    case 0xEF15: Flash_Size=1024*4;break;
    case 0xEF16: Flash_Size=1024*8;break;
    case 0xEF17: Flash_Size=1024*16;break;
    }
}

/***************************************************************************//**
 * @brief 	读取W25QXX的状态寄存器
 * @note	BIT7  6   5   4   3   2   1   0
 * 			SPR   RV  TB BP2 BP1 BP0 WEL BUSY
 * 			SPR:默认0,状态寄存器保护位,配合WP使用
 * 			TB,BP2,BP1,BP0:FLASH区域写保护设置
 * 			WEL:写使能锁定
 * 			BUSY:忙标记位(1,忙;0,空闲)
 * 			 默认:0x00
 * @return	状态寄存器值
*******************************************************************************/
uint8_t W25QXX_ReadSR(void)
{
	uint8_t byte=0;
	LockFlash();
	W25QXX_CS0();                            //使能器件
	SPI_ReadWriteByte(W25X_ReadStatusReg); //发送读取状态寄存器命令
	byte=SPI_ReadWriteByte(0Xff);          //读取一个字节
	W25QXX_CS1();
	UnlokFlash();                            //取消片选
	return byte;
}

/***************************************************************************//**
 * @brief 	写W25QXX状态寄存器
 * @note	只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
 * @param	sr 状态寄存器值
*******************************************************************************/
void W25QXX_Write_SR(uint8_t sr)
{
	LockFlash();
	W25QXX_CS0();                            //使能器件
	SPI_ReadWriteByte(W25X_WriteStatusReg);	//发送写取状态寄存器命令
	SPI_ReadWriteByte(sr);               	//写入一个字节
	W25QXX_CS1();							//取消片选
	UnlokFlash();                           //取消片选
}

/***************************************************************************//**
 * @brief 	W25QXX写使能
 * @note	将WEL置位
*******************************************************************************/
void W25QXX_Write_Enable(void)
{
	LockFlash();
	W25QXX_CS0();                          	//使能器件
    SPI_ReadWriteByte(W25X_WriteEnable); 	//发送写使能
	W25QXX_CS1();
	UnlokFlash();                           //取消片选
}

/***************************************************************************//**
 * @brief 	W25QXX写禁止
 * @note	将WEL清零
*******************************************************************************/
void W25QXX_Write_Disable(void)
{
	LockFlash();
	W25QXX_CS0();                           //使能器件
    SPI_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令
	W25QXX_CS1();
	UnlokFlash();                           //取消片选
}

/***************************************************************************//**
 * @brief 	读取芯片ID
 * @note	0XEF13,表示芯片型号为W25Q80
 * 			0XEF14,表示芯片型号为W25Q16
 * 			0XEF15,表示芯片型号为W25Q32
 * 			0XEF16,表示芯片型号为W25Q64
 * 			0XEF17,表示芯片型号为W25Q128
 * @return Temp 芯片ID
*******************************************************************************/
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;
	LockFlash();
	W25QXX_CS0();
	SPI_ReadWriteByte(0x90);//发送读取ID命令
	SPI_ReadWriteByte(0x00);
	SPI_ReadWriteByte(0x00);
	SPI_ReadWriteByte(0x00);
	Temp|=SPI_ReadWriteByte(0xFF)<<8;
	Temp|=SPI_ReadWriteByte(0xFF);
	W25QXX_CS1();
	UnlokFlash();
	return Temp;
}

/***************************************************************************//**
 * @brief 	读取SPI FLASH
 * @param	pBuffer 数据存储区
 * @param	ReadAddr 开始读取的地址(24bit)
 * @param	NumByteToRead 要读取的字节数(最大65535)
*******************************************************************************/
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
 	uint8_t W25QR[4]={W25X_ReadData,(uint8_t)((ReadAddr)>>16),(uint8_t)((ReadAddr)>>8),(uint8_t)ReadAddr};
	LockFlash();
	W25QXX_CS0();                            	//使能器件
	HAL_SPI_Transmit(&FLASH_HSPI,&W25QR,4,10);
	HAL_SPI_Receive(&FLASH_HSPI,pBuffer,NumByteToRead,100);
	W25QXX_CS1();
	UnlokFlash();

}

/***************************************************************************//**
 * @brief 	按页写入数据
 * @note	SPI在一页(0~65535)内写入少于256个字节的数据
 * @param	pBuffer 数据存储区
 * @param	NumByteToWrite 要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
*******************************************************************************/
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint8_t W25QR[4]={W25X_PageProgram,(uint8_t)((WriteAddr)>>16),(uint8_t)((WriteAddr)>>8),(uint8_t)WriteAddr};
    W25QXX_Write_Enable();                  	//SET WEL
	LockFlash();
	W25QXX_CS0();                            	//使能器件
	HAL_SPI_Transmit(&FLASH_HSPI,&W25QR,4,10);
	HAL_SPI_Transmit(&FLASH_HSPI,pBuffer,NumByteToWrite,10);
	W25QXX_CS1();
	UnlokFlash();                            	//取消片选
	W25QXX_Wait_Busy();					   		//等待写入结束
}

/***************************************************************************//**
 * @brief 	无检验写SPI FLASH
 * @note	必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
 * 			具有自动换页功能
 * 			在指定地址开始写入指定长度的数据,但是要确保地址不越界!
 * @param	pBuffer 数据存储区
 * @param	WriteAddr 开始写入的地址(24bit)
 * @param	NumByteToWrite 要写入的字节数(最大65535)
*******************************************************************************/
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t pageremain;
	pageremain=256-WriteAddr%256; //单页剩余的字节数
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};
}

/***************************************************************************//**
 * @brief 	SPI FLASH 写扇区
 * @note	扇区大小(4096)
 * @param	pBuffer 数据存储区
 * @param	WriteAddr 开始写入的地址(24bit)
*******************************************************************************/
void W25QXX_WriteSecpos(uint8_t* pBuffer,uint32_t WriteAddr){
	W25QXX_Write_NoCheck(pBuffer,WriteAddr,4096);//写已经擦除了的,直接写入扇区剩余区间.
}

/***************************************************************************//**
 * @brief 	写SPI FLASH
 * @note	在指定地址开始写入指定长度的数据
 * 			该函数带擦除操作
 * @param	pBuffer 数据存储区
 * @param	WriteAddr 开始写入的地址(24bit)
 * @param	NumByteToWrite 要写入的字节数(最大65535)
*******************************************************************************/
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
 	uint16_t i;
	static uint8_t W25QXX_BUF[4096];
 	secpos=WriteAddr/4096;//扇区地址
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1)
	{
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0

		   	pBuffer+=secremain;  					//指针偏移
			WriteAddr+=secremain;					//写地址偏移
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}
	};
}

/***************************************************************************//**
 * @brief 	擦除整个芯片
 * @note	等待时间超长
*******************************************************************************/
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();                 	 	//SET WEL
    W25QXX_Wait_Busy();
  	LockFlash();
  	W25QXX_CS0();                            	//使能器件
    SPI_ReadWriteByte(W25X_ChipErase);        	//发送片擦除命令
	W25QXX_CS1();
	UnlokFlash();                            	//取消片选
	W25QXX_Wait_Busy();   				   		//等待芯片擦除结束
}

/***************************************************************************//**
 * @brief 	擦除一个扇区
 * @note	擦除一个扇区的最少时间:150ms
 * @param	Dst_Addr 扇区地址 根据实际容量设置
*******************************************************************************/
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  	//SET WEL
    W25QXX_Wait_Busy();
  	LockFlash();
  	W25QXX_CS0();                            	//使能器件
    SPI_ReadWriteByte(W25X_SectorErase);      	//发送扇区擦除指令
    SPI_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  	//发送24bit地址
    SPI_ReadWriteByte((uint8_t)((Dst_Addr)>>8));
    SPI_ReadWriteByte((uint8_t)Dst_Addr);
	W25QXX_CS1();
	UnlokFlash();                            	//取消片选
    W25QXX_Wait_Busy();   				   		//等待擦除完成
}

/***************************************************************************//**
 * @brief 	等待空闲
*******************************************************************************/
void W25QXX_Wait_Busy(void)
{
	while((W25QXX_ReadSR()&0x01)==0x01) 		// 等待BUSY位清空
	{
		HAL_Delay(2);
	}
}

/***************************************************************************//**
 * @brief 	进入掉电模式
*******************************************************************************/
void W25QXX_PowerDown(void)
{
  	LockFlash();
  	W25QXX_CS0();                           	//使能器件
    SPI_ReadWriteByte(W25X_PowerDown);        	//发送掉电命令
	W25QXX_CS1();
	UnlokFlash();                            	//取消片选
    HAL_Delay(1);                               //等待TPD
}

/***************************************************************************//**
 * @brief 	唤醒
*******************************************************************************/
void W25QXX_WAKEUP(void)
{
  	LockFlash();
  	W25QXX_CS0();                            	//使能器件
    SPI_ReadWriteByte(W25X_ReleasePowerDown);	//send W25X_PowerDown command 0xAB
	W25QXX_CS1();
	UnlokFlash();                            	//取消片选
    HAL_Delay(1);                            	//等待TRES1
}
