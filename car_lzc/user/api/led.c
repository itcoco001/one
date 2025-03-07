#include "led.h"

void LED_Config(void)
{
	//LED1 -- PC0
	//1.打开时钟C端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//2.PC0配置为通用推挽输出模式50MHz
	GPIO_InitTypeDef led={0};
	led.GPIO_Mode = GPIO_Mode_Out_PP;
	led.GPIO_Speed = GPIO_Speed_50MHz;
	
	led.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC,&led);
	led.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC,&led);
	led.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC,&led);
}
#include "delay.h"
void LED_HX(void)
{
	int i = 0;
	for(i=0;i<1000;i++){
		LEDx_ON(LED1_Port,LED1_Pin);
		Delay_us(i);
		LEDx_OFF(LED1_Port,LED1_Pin);
		Delay_us(1000-i);
	}
	for(i=0;i<1000;i++){
		LEDx_OFF(LED1_Port,LED1_Pin);
		Delay_us(i);
		LEDx_ON(LED1_Port,LED1_Pin);
		Delay_us(1000-i);
	}
}


