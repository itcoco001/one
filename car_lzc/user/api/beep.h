#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f10x.h"

#define BEEP_Port  GPIOA
#define BEEP_Pin   GPIO_Pin_15

#define BEEP_ON()  		(GPIO_WriteBit(BEEP_Port,BEEP_Pin,1))
#define BEEP_OFF() 		(GPIO_WriteBit(BEEP_Port,BEEP_Pin,0))
#define BEEP_Toggle()	(BEEP_Port->ODR ^= BEEP_Pin) 

void BEEP_Config(void);
#endif
