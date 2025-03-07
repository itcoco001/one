#include "key.h"
#include "delay.h"

void KEY_Config(void)
{
	//1.打开时钟A端口 KEY -- PA0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//2.PA0配置为浮空输入
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/*
按键确实按下，返回1
没有按下，返回0
*/
//u8 Key_GetValue(void)
//{
//	if(KEY1 == 0){
//		Delay_ms(20);//消抖
//		if(KEY1 == 0){
//			while(KEY1 == 0);//松手检测
//			return 1;
//		}
//	}
//	return 0;
//}
u8 Key_GetValue(void)
{
	static u32 count = 0;
	if(KEY1 == 0) count++;
	else if(count > 50){
		count = 0;
		return 2;
	}else if(count > 15){
		count = 0;
		return 1;
	}else
		count = 0;
	
	return 0;
}







