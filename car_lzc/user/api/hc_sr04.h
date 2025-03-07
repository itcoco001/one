#ifndef _SR04_H_
#define _SR04_H_

#include "stm32f10x.h"

#define TRIG_Port  GPIOB
#define TRIG_Pin   GPIO_Pin_14

#define ECHO_Port  GPIOC
#define ECHO_Pin   GPIO_Pin_6

#define TRIG_L (GPIO_WriteBit(TRIG_Port,TRIG_Pin,(BitAction)0))
#define TRIG_H (GPIO_WriteBit(TRIG_Port,TRIG_Pin,(BitAction)1))

#define ECHO   (GPIO_ReadInputDataBit(ECHO_Port,ECHO_Pin))

extern u32 sr04_len;

void SR04_Config(void);
void SR04_GetLen(void);
#endif
