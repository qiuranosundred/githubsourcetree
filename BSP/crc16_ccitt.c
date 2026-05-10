#include "crc16_ccitt.h"

uint16_t crc16_ccitt(uint8_t *data, int len)
{
	uint16_t i;
    uint16_t crc = 0x0000;

    for (i = 0; i < len; ++i)
	{
        crc ^= data[i];
        for (int j = 0; j < 8; ++j)
		{
            if (crc & 1)
			{
                crc = (crc >> 1) ^ 0x8408;
            }
			else
			{
                crc = (crc >> 1);
            }
        }
    }
    return crc;
}

//使用例子
//int main()
//{
//    //uint8_t data[] = {0x33 ,0x03 ,0x02 ,0x91 ,0x00 ,0x04 ,0x07 ,0x1B ,0x54 ,0x00};//72 19
//    uint8_t data[] = {0x33 ,0x03 ,0x02 ,0x91 ,0x00 ,0x04 ,0x07 ,0x1B ,0xA4 ,0x00};//7A 65
//    int len = sizeof(data) / sizeof(data[0]);
//
//    uint16_t crc = crc16_ccitt(data, len);
//    printf("CRC16-CCITT校验值为: %04X\n", crc);
//
//    return 0;
//}
