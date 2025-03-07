#include "ble.h"
#include "led.h"
#include "beep.h"
#include "motor.h"
struct BLE_MESSAGE ble_data = {0};

void BLE_Config(void)
{
	#if UART2_UART5 == 1
	//���ڳ�ʼ��2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//PA2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//2.����USART2 -- 1+8+0+1  �����ʣ�9600
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 9600;//������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ȫ˫��
	USART_InitStruct.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//���ݳ���Ϊ8
	USART_Init(USART2,&USART_InitStruct);
	//���ô��ڽ����ж�
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;//ʹ�ô���1ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//ռ�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//�μ����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);//ʹ��
	//�򿪴��ڵĽ����жϺͼ�����߿����ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	//3.ʹ�ܴ���2
	USART_Cmd(USART2,ENABLE);
	#else
	//���ڳ�ʼ��5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//PC12
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	//PD2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	//2.����UART5 -- 1+8+0+1  �����ʣ�9600
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 9600;//������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ȫ˫��
	USART_InitStruct.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//���ݳ���Ϊ8
	USART_Init(UART5,&USART_InitStruct);
	//���ô��ڽ����ж�
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;//ʹ�ô���1ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//ռ�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//�μ����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);//ʹ��
	//�򿪴��ڵĽ����жϺͼ�����߿����ж�
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);
	//3.ʹ�ܴ���5
	USART_Cmd(UART5,ENABLE);
	#endif
}
#if UART2_UART5 == 1
//��д����2�����жϷ�����
void USART2_IRQHandler(void)
{
	u8 data = 0;
	//�ж��Ƿ��Ǵ��ڽ����ж�
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//�����־λ
		ble_data.buff[ble_data.count++] = USART2->DR;//��������,��¼��ǰ�����˶��ٸ��ֽ�����
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET){
		data = USART2->DR;//������б�־
		ble_data.over_flag = 1;
	}
}
#else
//��д����5�����жϷ�����
void UART5_IRQHandler(void)
{
	u8 data = 0;
	//�ж��Ƿ��Ǵ��ڽ����ж�
	if(USART_GetITStatus(UART5,USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);//�����־λ
		ble_data.buff[ble_data.count++] = UART5->DR;//��������,��¼��ǰ�����˶��ٸ��ֽ�����
		USART1->DR = UART5->DR;
	}
	if(USART_GetITStatus(UART5,USART_IT_IDLE) == SET){
		data = UART5->DR;//������б�־
		ble_data.over_flag = 1;
	}
}
#endif

//��д�������ݵĽ��� X(�豸��) X(������)
void BLE_DataAnlay(void)
{
	if(ble_data.over_flag == 0)//�Ƿ��յ�����
		return ;
//	if(ble_data.count < 2)//�Ƿ��յ������ݸ�������2
//		return ;
	u8 no,func;
	no = ble_data.buff[0];//�豸��
	func = ble_data.buff[1];//������
	//��������
	switch(no){
		case 1:
			if(func == 0) LEDx_OFF(LED2_Port,LED2_Pin);
			else if(func == 1) LEDx_ON(LED2_Port,LED2_Pin);
			break;
		case 2:
			if(func == 0) LEDx_OFF(LED3_Port,LED3_Pin);
			else if(func == 1) LEDx_ON(LED3_Port,LED3_Pin);
			break;
		case 3:
			if(func == 0) BEEP_OFF();
			else if(func == 1) BEEP_ON();
			break;	
		case 5:		// ֹͣ
			Motor_Front_Left_F(0);
			Motor_Back_Left_F(0);
			Motor_Front_Right_F(0);
			Motor_Back_Right_F(0);
		break;
		case 6:		// ǰ��
			Motor_LeftFrontControl(600);
			Motor_LeftBackControl(600);
			Motor_RightFrontControl(650);
			Motor_RightBackControl(650);
		break;
		case 7:		// ����
			Motor_LeftFrontControl(-600);
			Motor_LeftBackControl(-600);
			Motor_RightFrontControl(-650);
			Motor_RightBackControl(-650);
		break;
		case 8:		// ��ת
			Motor_LeftFrontControl(50);
			Motor_LeftBackControl(50);
			Motor_RightFrontControl(850);
			Motor_RightBackControl(800);
		break;
		case 9:		// ��ת
			Motor_LeftFrontControl(750);
			Motor_LeftBackControl(700);
			Motor_RightFrontControl(150);
			Motor_RightBackControl(150);
		break;
	}
	//���㣬��ͷ��ʼ
	ble_data.count = 0;
	ble_data.over_flag = 0;
}
