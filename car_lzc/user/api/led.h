#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

#define LED1_Port  GPIOC
#define LED1_Pin   GPIO_Pin_0

#define LED2_Port  GPIOC
#define LED2_Pin   GPIO_Pin_1

#define LED3_Port  GPIOC
#define LED3_Pin   GPIO_Pin_2

#define LEDx_ON(port,pin)  		(GPIO_WriteBit(port,pin,0))
#define LEDx_OFF(port,pin) 		(GPIO_WriteBit(port,pin,1))
#define LEDx_Toggle(port,pin)	(port->ODR ^= pin) 

void LED_Config(void);
void LED_HX(void);
#endif
