#include "hc_sr04.h"
#include "delay.h"
#include "stdio.h"
u32 sr04_len = 0;//��¼��������ǰ�����ľ��� -- ��λΪ��cm

/*
TRIG -- PB14 -- ͨ���������ģʽ
ECHO -- PC6  -- ��������ģʽ
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

//�������������ľ��� ���ؾ��룬 0--û�м�⵽����  >0 ��⵽����
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
  ״̬1��TRIG�ܽ����TTL��ƽ���ϵ�Ϊ�͵�ƽ����10us�ĸߵ�ƽ����Ϊ�͵�ƽ
  ״̬2���ȴ���⵽����
  ״̬3����ʱ60ms���� -- 100ms
*/
void SR04_GetLen(void)
{
	static u8 state = 1;//1-״̬1  2-״̬2 3-״̬3
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
			if(timeout == 0){//��ʱ���
				state = 1;
				break;
			}
			if(ECHO == 1) count++;
			else if(count >1){
				sr04_len = 1.7*count;
				count = 0;
				state = 3;
				time_sr04[1] = 1000;//��ʱ100ms
			}	
			break;
		case 3:
			//��Ϊ״̬1
			state = 1;
			time_sr04[1] = 1;
			break;
	}
}









