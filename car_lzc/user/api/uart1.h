#ifndef _UART1_H_
#define _UART1_H_

#include "stm32f10x.h"
#include "stdio.h"

struct UART_RX
{
	u8  buff[16];//���ջ�����
	u32 count;   //������յ����ݵĸ���
	u8  over_flag;//������ɱ�־
};
extern struct UART_RX  usart1_rx;

void USART1_Config(void);
void USART1_Recive(void);
void USART1_DataAnlay(void);
#endif
