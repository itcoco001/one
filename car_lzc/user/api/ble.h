#ifndef _BLE_H_
#define _BLE_H_

#include "stm32f10x.h"

#define UART2_UART5  0//0 - 串口2  1 - 串口5  

struct BLE_MESSAGE
{
	u8  buff[16];//接收缓冲区
	u32 count;   //保存接收到数据的个数
	u8  over_flag;//接收完成标志
};
void BLE_Config(void);
void BLE_DataAnlay(void);
#endif
