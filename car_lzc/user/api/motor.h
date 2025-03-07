#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f10x.h"
#include "delay.h"

#define MAX_SPEED	900

//������
typedef enum{
	CAR_DIR_STOP = 0,
	CAR_DIR_FRONT = 1,
	CAR_DIR_LEFT = 2,
	CAR_DIR_RIGHT = 3,
	CAR_DIR_BACK = 4,
}CarDIR_Typedef;

//С�����ƽṹ��
typedef struct {
	uint16_t speed;
	CarDIR_Typedef dir;
}CarControl_Typedef;

extern CarControl_Typedef car;

/*
												A					B
	��ǰ		IA1  IB1		TIM4CH3		TIM4CH4		
	�Һ�		IA2  IB2		TIM3CH3		TIM3CH4		
	���		IA3  IB3		TIM4CH1		TIM4CH2		
	��ǰ		IA4  IB4		TIM3CH1		TIM3CH2		
*/

//��ǰ��-ǰ��														A												B																		
#define Motor_Front_Right_F(speed) 	(TIM_SetCompare3(TIM4, speed), TIM_SetCompare4(TIM4, 0))
//��ǰ��-����														A												B																	
#define Motor_Front_Right_B(speed) 	(TIM_SetCompare3(TIM4, 0), TIM_SetCompare4(TIM4, speed))
//�Һ���-ǰ��														A												B																		
#define Motor_Back_Right_F(speed) 	(TIM_SetCompare3(TIM3, speed), TIM_SetCompare4(TIM3, 0))
//�Һ���-����														A												B																	
#define Motor_Back_Right_B(speed) 	(TIM_SetCompare3(TIM3, 0), TIM_SetCompare4(TIM3, speed))
																						
//��ǰ��-ǰ��														A												B																		
#define Motor_Front_Left_F(speed) 	(TIM_SetCompare1(TIM3, speed), TIM_SetCompare2(TIM3, 0))
//��ǰ��-����														A												B																	
#define Motor_Front_Left_B(speed) 	(TIM_SetCompare1(TIM3, 0), TIM_SetCompare2(TIM3, speed))
//�����-ǰ��														A												B																		
#define Motor_Back_Left_F(speed) 	(TIM_SetCompare1(TIM4, speed), TIM_SetCompare2(TIM4, 0))
//�����-����														A												B																	
#define Motor_Back_Left_B(speed) 	(TIM_SetCompare1(TIM4, 0), TIM_SetCompare2(TIM4, speed))


//����ֹͣ
#define Motor_Stop()		(Motor_Front_Right_F(0), Motor_Back_Right_F(0),\
												 Motor_Front_Left_F(0), Motor_Back_Left_F(0))
												 
#define Motor_Front(speed)	(Motor_Front_Right_F(speed), Motor_Back_Right_F(speed),\
														 Motor_Back_Left_F(speed), Motor_Front_Left_F(speed))

#define Motor_Back(speed)		(Motor_Front_Right_B(speed), Motor_Back_Right_B(speed),\
														 Motor_Back_Left_B(speed), Motor_Front_Left_B(speed))
														 
#define Motor_Right(speed)	(Motor_Front_Right_B(speed), Motor_Back_Right_B(speed),\
														 Motor_Back_Left_F(speed), Motor_Front_Left_F(speed))

#define Motor_Left(speed)		(Motor_Front_Right_F(speed), Motor_Back_Right_F(speed),\
														 Motor_Back_Left_B(speed), Motor_Front_Left_B(speed))

void Motor_Config(void);
void Car_MotorControl(CarControl_Typedef c);


void Motor_RightFrontControl(int speed);
void Motor_RightBackControl(int speed);
void Motor_LeftFrontControl(int speed);
void Motor_LeftBackControl(int speed);


#endif

