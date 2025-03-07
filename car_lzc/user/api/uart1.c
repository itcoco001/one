#include "uart1.h"
#include "beep.h"
#include "led.h"
struct UART_RX  usart1_rx = {0};

/*
PA9  -- �����������
PA10 -- ��������
*/
void USART1_Config(void)
{
	//1.��ʱ��A�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//PA9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA10
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//2.����USART1 -- 1+8+0+1  �����ʣ�9600
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 9600;//������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ȫ˫��
	USART_InitStruct.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//���ݳ���Ϊ8
	USART_Init(USART1,&USART_InitStruct);
	//���ô��ڽ����ж�
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//ʹ�ô���1ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//ռ�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//�μ����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);//ʹ��
	//�򿪴��ڵĽ����жϺͼ�����߿����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	//3.ʹ�ܴ���1
	USART_Cmd(USART1,ENABLE);
}

//�жϷ�����
void USART1_IRQHandler(void)
{
	u8 data = 0;
	//�ж��Ƿ��Ǵ��ڽ����ж�
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//�����־λ
		usart1_rx.buff[usart1_rx.count++] = USART1->DR;//��������,��¼��ǰ�����˶��ٸ��ֽ�����
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET){
		data = USART1->DR;//������б�־
		usart1_rx.over_flag = 1;
	}
}

int fputc(int c,FILE *fp)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);//�ȴ��ϴ����ݷ������
	USART_SendData(USART1,c);//�����µ�����
	
	return c;
}

void USART1_DataAnlay(void)
{
	if(usart1_rx.over_flag == 0)//�Ƿ��յ�����
		return ;
	if(usart1_rx.count < 2)//�Ƿ��յ������ݸ�������2
		return ;
	u8 no,func;
	no = usart1_rx.buff[0];//�豸��
	func = usart1_rx.buff[1];//������
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
	}
	//���㣬��ͷ��ʼ
	usart1_rx.count = 0;
	usart1_rx.over_flag = 0;
}
//void USART1_Recive(void)
//{
//	u8 data = 0;
//	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);//�����ȴ����ݵ���
//	data = USART_ReceiveData(USART1);
//	switch(data)
//	{
//		case 0:BEEP_OFF();break;
//		case 1:BEEP_ON(); break;
//	}
//}







