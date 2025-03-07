#include "wifi.h"
#include "stdio.h"
#include "string.h"

void WIFI_Config(void)
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//����IO
	GPIO_InitTypeDef GPIO_InitStructure={0};
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//����USART3
	USART_InitTypeDef USART_InitStructuer={0};
	USART_InitStructuer.USART_BaudRate = 9600;
	USART_InitStructuer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructuer.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructuer.USART_Parity = USART_Parity_No;
	USART_InitStructuer.USART_StopBits = USART_StopBits_1;
	USART_InitStructuer.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructuer);
	//ʹ�ܽ����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	//����NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);
}
__WIFI_TYPE wifi = {0};

void USART3_IRQHandler(void)
{
	uint8_t data = 0;
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
	{
		data = USART3->DR;
		USART1->DR = data;
		if(wifi.recvOver == 0) {
			wifi.recvData[wifi.recvCount++] = data;
		}
	}
	if(USART_GetFlagStatus(USART3, USART_FLAG_IDLE) == SET)
	{
		data = USART3->DR;
		wifi.recvOver = 1;
	}	
}
#include "motor.h"
#include "steer.h"
//����wifi�յ������� һ֡��Ϣ6���ֽ� [0]=0xAA [1]=0x55 
/*
֡ͷ[0][1]+С������[2]+�������[3]+С���ٶ�[4]+У���[5]
wifi.recvData[0~5]
*/
void WIFI_Analysis(void)
{
	uint8_t check = 0;
	if(wifi.recvOver == 0)	return;//�ж��Ƿ�������
	if(wifi.recvCount == 0) //�ж��Ƿ���յ�����
	{
		wifi.recvOver = 0;//�����־λ
		return ;
	}
	//�ж��Ƿ���չ�5���ֽ�
	if(wifi.recvCount < 5)
	{
		wifi.recvOver = 0;
		goto err;
	}
	//�ж�У���
	check = wifi.recvData[0]+wifi.recvData[1]+wifi.recvData[2]+wifi.recvData[3]+wifi.recvData[4];
	if(check != wifi.recvData[5])
	{
		goto err;
	}
	//��С�������˶�״̬
	switch(wifi.recvData[2])
	{
		case 0x00:Motor_Stop();break;
		case 0x01:Motor_Front(550);break;
		case 0x02:Motor_Back(600);break;
		case 0x03:Motor_Right(550);break;
		case 0x04:Motor_Left(550);break;
		default:Motor_Stop();break;
	}
	//����̨����״̬
	switch(wifi.recvData[3])
	{
		case 0x05:SteerDir = SteerStop;break;
		case 0x06:SteerDir = SteerLeft;break;
		case 0x07:SteerDir = SteerRight;break;
		case 0x08:SteerDir = SteerUp;break;
		case 0x09:SteerDir = SteerDown;break;
		default:SteerDir = SteerStop;break;
	}
	err:
		memset(wifi.recvData, 0, 256);
		wifi.recvCount = 0;
		wifi.recvOver = 0;
}
