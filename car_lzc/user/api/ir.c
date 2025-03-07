#include "ir.h"
#include "motor.h"
/*
PC7 - 11配置为浮空输入
*/
void IR_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}
#include "stdio.h"
void IR_GetLineAndMove(void)
{
//	printf("%#x\r\n",IRVALUE);
	switch(IRVALUE)
	{
		case 0x00:			// 没有寻到线
			Motor_LeftFrontControl(500);
			Motor_LeftBackControl(500);
			Motor_RightFrontControl(550);
			Motor_RightBackControl(550);
			break;
		case 0x1F:			// 00000寻到起始点或者终点
			Motor_LeftFrontControl(0);
			Motor_LeftBackControl(0);
			Motor_RightFrontControl(0);
			Motor_RightBackControl(0);
		break;
		case 0x04:			// 11011 直行 00100
			Motor_LeftFrontControl(500);
			Motor_LeftBackControl(500);
			Motor_RightFrontControl(550);
			Motor_RightBackControl(550);
		break;
		case 0x0C:			// 10011 左小 01100
			Motor_LeftFrontControl(50);
			Motor_LeftBackControl(50);
			Motor_RightFrontControl(700);
			Motor_RightBackControl(750);
		break;
		case 0x08:			// 10111 左小 01000
			Motor_LeftFrontControl(50);
			Motor_LeftBackControl(50);
			Motor_RightFrontControl(700);
			Motor_RightBackControl(750);
		break;
		case 0x18:			// 00111 左大 11000
			Motor_LeftFrontControl(-150);
			Motor_LeftBackControl(-50);
			Motor_RightFrontControl(800);
			Motor_RightBackControl(800);
		break;
		case 0x10:			// 01111 左大 10000
			Motor_LeftFrontControl(-150);
			Motor_LeftBackControl(-50);
			Motor_RightFrontControl(800);
			Motor_RightBackControl(800);
		break;
		case 0x06:			// 11001 右小 00110
			Motor_LeftFrontControl(650);
			Motor_LeftBackControl(600);
			Motor_RightFrontControl(300);
			Motor_RightBackControl(300);
		break;
		case 0x02:			// 11101 右小 00010
			Motor_LeftFrontControl(650);
			Motor_LeftBackControl(600);
			Motor_RightFrontControl(300);
			Motor_RightBackControl(300);
		break;
		case 0x03:			// 11100 右大 00011
			Motor_LeftFrontControl(800);
			Motor_LeftBackControl(800);
			Motor_RightFrontControl(200);
			Motor_RightBackControl(200);
		break;
		case 0x01:			// 11110 右大 00001
			Motor_LeftFrontControl(800);
			Motor_LeftBackControl(800);
			Motor_RightFrontControl(200);
			Motor_RightBackControl(200);
		break;
//		case 0x1F://没有寻到线
//		case 0x00://寻到起始线和终止线
//				Motor_RightFrontControl(0);//右前轮
//				Motor_RightBackControl(0);//右后轮
//				Motor_LeftFrontControl(0);//左前轮
//				Motor_LeftBackControl(0);//左后轮
//			break;
//		case 0x04://直行
//				Motor_RightFrontControl(500);
//				Motor_RightBackControl(500);
//				Motor_LeftFrontControl(500);
//				Motor_LeftBackControl(500);
//			break;
//		case 0x0C://左转（小角度）
//		case 0x08:
//				Motor_RightFrontControl(500);
//				Motor_RightBackControl(500);
//				Motor_LeftFrontControl(250);
//				Motor_LeftBackControl(250);
//			break;
//		case 0x18://左转（大角度）
//		case 0x10:
//				Motor_RightFrontControl(500);
//				Motor_RightBackControl(500);
//				Motor_LeftFrontControl(-250);
//				Motor_LeftBackControl(-250);
//			break;
//		case 0x06://右转（小角度）
//		case 0x02:
//				Motor_RightFrontControl(250);
//				Motor_RightBackControl(250);
//				Motor_LeftFrontControl(500);
//				Motor_LeftBackControl(500);
//			break;
//		case 0x03://右转（大角度）
//		case 0x01:
//				Motor_RightFrontControl(-250);
//				Motor_RightBackControl(-250);
//				Motor_LeftFrontControl(500);
//				Motor_LeftBackControl(500);
//			break;
	}
}

