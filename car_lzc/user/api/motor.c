#include "motor.h"
/*
IA1 -- PB8
IB1 -- PB9
*/
void Motor_Config(void)
{
	//1.相关管脚配置相关模式(复用推挽输出)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//2.PWM波周期：1ms -- 定时器4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//计数器阈值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;//分频
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	//3.相应的通道配置PWM模式
	TIM_OCInitTypeDef TIM_OCInitStruct = {0};
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;//比较寄存器（控制占空比的）
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);
	TIM_OC4Init(TIM4,&TIM_OCInitStruct);
	
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);
	
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);
	
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
	//4.定时器使能
	TIM_Cmd(TIM4,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

//IA4 IB4		TIM3 CH1	2		PA5	6
//IA2 IB2		TIM3 CH3	4		PB0	1
//IA3 IB3		TIM4 CH1	2		PB6	7
//IA1 IB1		TIM4 CH3	4		PB8	9

CarControl_Typedef car = {MAX_SPEED, CAR_DIR_STOP};

//小车切换运行状态耗电过高会复位
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
  * 以下代码可以更加细致的控制小车电机
  ******************************************************************************
  */

//右前方电机控制
//传参如果速度 >=0 正转，否则反转
void Motor_RightFrontControl(int speed)
{
	if(speed >= 0) {
		Motor_Front_Right_F(speed);
	}
	else {
		Motor_Front_Right_B(-speed);
	}
}

//右后方电机控制
void Motor_RightBackControl(int speed)
{
	if(speed >= 0) {
		Motor_Back_Right_F(speed);
	}
	else {
		Motor_Back_Right_B(-speed);
	}
}

//左前方电机控制
void Motor_LeftFrontControl(int speed)
{
	if(speed >= 0) {
		Motor_Front_Left_F(speed);
	}
	else {
		Motor_Front_Left_B(-speed);
	}
}

//左后方电机控制
void Motor_LeftBackControl(int speed)
{
	if(speed >= 0) {
		Motor_Back_Left_F(speed);
	}
	else {
		Motor_Back_Left_B(-speed);
	}
}

