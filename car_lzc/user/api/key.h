#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"

#define KEY1 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))

void KEY_Config(void);
u8 Key_GetValue(void);
#endif
