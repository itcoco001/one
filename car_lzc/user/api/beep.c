#include "beep.h"

void BEEP_Config(void)
{
	//1.打开时钟A端口 BEEP -- PA15
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//2.PA15配置为通用推挽输出模式50MHz
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	BEEP_OFF();
}

