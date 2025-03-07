#include "hc_sr04.h"
#include "delay.h"
#include "stdio.h"
u32 sr04_len = 0;//记录超声波当前测量的距离 -- 单位为：cm

/*
TRIG -- PB14 -- 通用推挽输出模式
ECHO -- PC6  -- 浮空输入模式
*/
void SR04_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	TRIG_L;
}

//测量超声波检测的距离 返回距离， 0--没有检测到物体  >0 检测到物体
//u32 SR04_GetLen(void)
//{
//	static u32 count = 0;
//	if(ECHO == 1) count++;
//	else if(count > 1){
//		//sr04_len = 340*((count*0.1)/1000)/2*100;
//		sr04_len = 1.7*count;
//		count = 0;
//		return sr04_len;
//	}else{
//		count = 0;
//	}
//	return 0;
//}
/*
  状态1：TRIG管脚输出TTL电平（上电为低电平），10us的高电平，变为低电平
  状态2：等待检测到物体
  状态3：延时60ms以上 -- 100ms
*/
void SR04_GetLen(void)
{
	static u8 state = 1;//1-状态1  2-状态2 3-状态3
	static u32 count = 0;
	static u32 timeout = 0;
	sr04_len = 0;
	switch(state)
	{
		case 1:
					 time_sr04[1] = 1;
					 TRIG_H;Delay_us(10);TRIG_L;
					 state = 2;
					 timeout = 500;//50ms
		case 2:
			timeout--;
			if(timeout == 0){//超时溢出
				state = 1;
				break;
			}
			if(ECHO == 1) count++;
			else if(count >1){
				sr04_len = 1.7*count;
				count = 0;
				state = 3;
				time_sr04[1] = 1000;//延时100ms
			}	
			break;
		case 3:
			//变为状态1
			state = 1;
			time_sr04[1] = 1;
			break;
	}
}









