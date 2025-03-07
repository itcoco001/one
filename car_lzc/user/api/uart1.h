#ifndef _UART1_H_
#define _UART1_H_

#include "stm32f10x.h"
#include "stdio.h"

struct UART_RX
{
	u8  buff[16];//接收缓冲区
	u32 count;   //保存接收到数据的个数
	u8  over_flag;//接收完成标志
};
extern struct UART_RX  usart1_rx;

void USART1_Config(void);
void USART1_Recive(void);
void USART1_DataAnlay(void);
#endif
