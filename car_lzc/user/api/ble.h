#ifndef _BLE_H_
#define _BLE_H_

#include "stm32f10x.h"

#define UART2_UART5  0//0 - ����2  1 - ����5  

struct BLE_MESSAGE
{
	u8  buff[16];//���ջ�����
	u32 count;   //������յ����ݵĸ���
	u8  over_flag;//������ɱ�־
};
void BLE_Config(void);
void BLE_DataAnlay(void);
#endif
