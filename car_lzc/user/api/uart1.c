#include "uart1.h"
#include "beep.h"
#include "led.h"
struct UART_RX  usart1_rx = {0};

/*
PA9  -- 复用推挽输出
PA10 -- 浮空输入
*/
void USART1_Config(void)
{
	//1.打开时钟A端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//PA9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA10
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//2.配置USART1 -- 1+8+0+1  波特率：9600
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 9600;//波特率
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//全双工
	USART_InitStruct.USART_Parity = USART_Parity_No;//不使用奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据长度为8
	USART_Init(USART1,&USART_InitStruct);
	//配置串口接收中断
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//使用串口1通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//占先优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//次级优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);//使能
	//打开串口的接收中断和检测总线空闲中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	//3.使能串口1
	USART_Cmd(USART1,ENABLE);
}

//中断服务函数
void USART1_IRQHandler(void)
{
	u8 data = 0;
	//判断是否是串口接收中断
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除标志位
		usart1_rx.buff[usart1_rx.count++] = USART1->DR;//保存数据,记录当前接收了多少个字节数据
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET){
		data = USART1->DR;//清除空闲标志
		usart1_rx.over_flag = 1;
	}
}

int fputc(int c,FILE *fp)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);//等待上次数据发送完成
	USART_SendData(USART1,c);//发送新的数据
	
	return c;
}

void USART1_DataAnlay(void)
{
	if(usart1_rx.over_flag == 0)//是否收到数据
		return ;
	if(usart1_rx.count < 2)//是否收到的数据个数大于2
		return ;
	u8 no,func;
	no = usart1_rx.buff[0];//设备号
	func = usart1_rx.buff[1];//功能码
	//解析数据
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
	//清零，从头开始
	usart1_rx.count = 0;
	usart1_rx.over_flag = 0;
}
//void USART1_Recive(void)
//{
//	u8 data = 0;
//	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);//阻塞等待数据到来
//	data = USART_ReceiveData(USART1);
//	switch(data)
//	{
//		case 0:BEEP_OFF();break;
//		case 1:BEEP_ON(); break;
//	}
//}







