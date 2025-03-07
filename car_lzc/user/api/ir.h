#ifndef _IR_H_
#define _IR_H_

#include "stm32f10x.h"

#define OUT1 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7))
#define OUT2 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8))
#define OUT3 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))
#define OUT4 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10))
#define OUT5 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11))

//构成8位数据
#define IRVALUE ((OUT1<<0)|(OUT2<<1)|(OUT3<<2)|(OUT4<<3)|(OUT5<<4))

void IR_Config(void);
void IR_GetLineAndMove(void);
#endif
