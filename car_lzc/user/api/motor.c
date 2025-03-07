#include "motor.h"
/*
IA1 -- PB8
IB1 -- PB9
*/
void Motor_Config(void)
{
	//1.��عܽ��������ģʽ(�����������)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//2.PWM�����ڣ�1ms -- ��ʱ��4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//��������ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;//��Ƶ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	//3.��Ӧ��ͨ������PWMģʽ
	TIM_OCInitTypeDef TIM_OCInitStruct = {0};
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;//�ȽϼĴ���������ռ�ձȵģ�
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);
	TIM_OC4Init(TIM4,&TIM_OCInitStruct);
	
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);
	
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);
	
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
	//4.��ʱ��ʹ��
	TIM_Cmd(TIM4,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

//IA4 IB4		TIM3 CH1	2		PA5	6
//IA2 IB2		TIM3 CH3	4		PB0	1
//IA3 IB3		TIM4 CH1	2		PB6	7
//IA1 IB1		TIM4 CH3	4		PB8	9

CarControl_Typedef car = {MAX_SPEED, CAR_DIR_STOP};

//С���л�����״̬�ĵ���߻Ḵλ
void Car_MotorControl(CarControl_Typedef c)
{
	if(c.speed > MAX_SPEED)	c.speed = MAX_SPEED;
	if(c.dir == CAR_DIR_STOP) {
		Motor_Stop();
	}
	else if(c.dir == CAR_DIR_FRONT) {
		Motor_Front(c.speed);
	}
	else if(c.dir == CAR_DIR_BACK) {
		Motor_Back(c.speed);
	}
	else if(c.dir == CAR_DIR_LEFT) {
		Motor_Left(c.speed);
	}
	else if(c.dir == CAR_DIR_RIGHT) {
		Motor_Right(c.speed);
	}
}
/**
  ******************************************************************************
  * ���´�����Ը���ϸ�µĿ���С�����
  ******************************************************************************
  */

//��ǰ���������
//��������ٶ� >=0 ��ת������ת
void Motor_RightFrontControl(int speed)
{
	if(speed >= 0) {
		Motor_Front_Right_F(speed);
	}
	else {
		Motor_Front_Right_B(-speed);
	}
}

//�Һ󷽵������
void Motor_RightBackControl(int speed)
{
	if(speed >= 0) {
		Motor_Back_Right_F(speed);
	}
	else {
		Motor_Back_Right_B(-speed);
	}
}

//��ǰ���������
void Motor_LeftFrontControl(int speed)
{
	if(speed >= 0) {
		Motor_Front_Left_F(speed);
	}
	else {
		Motor_Front_Left_B(-speed);
	}
}

//��󷽵������
void Motor_LeftBackControl(int speed)
{
	if(speed >= 0) {
		Motor_Back_Left_F(speed);
	}
	else {
		Motor_Back_Left_B(-speed);
	}
}

