#include "ble.h"
#include "led.h"
#include "beep.h"
#include "motor.h"
struct BLE_MESSAGE ble_data = {0};

void BLE_Config(void)
{
	#if UART2_UART5 == 1
	//串口初始化2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//PA2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//2.配置USART2 -- 1+8+0+1  波特率：9600
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 9600;//波特率
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//全双工
	USART_InitStruct.USART_Parity = USART_Parity_No;//不使用奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据长度为8
	USART_Init(USART2,&USART_InitStruct);
	//配置串口接收中断
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;//使用串口1通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//占先优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//次级优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);//使能
	//打开串口的接收中断和检测总线空闲中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	//3.使能串口2
	USART_Cmd(USART2,ENABLE);
	#else
	//串口初始化5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//PC12
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	//PD2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	//2.配置UART5 -- 1+8+0+1  波特率：9600
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 9600;//波特率
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//全双工
	USART_InitStruct.USART_Parity = USART_Parity_No;//不使用奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据长度为8
	USART_Init(UART5,&USART_InitStruct);
	//配置串口接收中断
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;//使用串口1通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//占先优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//次级优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);//使能
	//打开串口的接收中断和检测总线空闲中断
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);
	//3.使能串口5
	USART_Cmd(UART5,ENABLE);
	#endif
}
#if UART2_UART5 == 1
//编写串口2接收中断服务函数
void USART2_IRQHandler(void)
{
	u8 data = 0;
	//判断是否是串口接收中断
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除标志位
		ble_data.buff[ble_data.count++] = USART2->DR;//保存数据,记录当前接收了多少个字节数据
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET){
		data = USART2->DR;//清除空闲标志
		ble_data.over_flag = 1;
	}
}
#else
//编写串口5接收中断服务函数
void UART5_IRQHandler(void)
{
	u8 data = 0;
	//判断是否是串口接收中断
	if(USART_GetITStatus(UART5,USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);//清除标志位
		ble_data.buff[ble_data.count++] = UART5->DR;//保存数据,记录当前接收了多少个字节数据
		USART1->DR = UART5->DR;
	}
	if(USART_GetITStatus(UART5,USART_IT_IDLE) == SET){
		data = UART5->DR;//清除空闲标志
		ble_data.over_flag = 1;
	}
}
#endif

//编写蓝牙数据的解析 X(设备号) X(功能码)
void BLE_DataAnlay(void)
{
	if(ble_data.over_flag == 0)//是否收到数据
		return ;
//	if(ble_data.count < 2)//是否收到的数据个数大于2
//		return ;
	u8 no,func;
	no = ble_data.buff[0];//设备号
	func = ble_data.buff[1];//功能码
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
		case 5:		// 停止
			Motor_Front_Left_F(0);
			Motor_Back_Left_F(0);
			Motor_Front_Right_F(0);
			Motor_Back_Right_F(0);
		break;
		case 6:		// 前进
			Motor_LeftFrontControl(600);
			Motor_LeftBackControl(600);
			Motor_RightFrontControl(650);
			Motor_RightBackControl(650);
		break;
		case 7:		// 后退
			Motor_LeftFrontControl(-600);
			Motor_LeftBackControl(-600);
			Motor_RightFrontControl(-650);
			Motor_RightBackControl(-650);
		break;
		case 8:		// 左转
			Motor_LeftFrontControl(50);
			Motor_LeftBackControl(50);
			Motor_RightFrontControl(850);
			Motor_RightBackControl(800);
		break;
		case 9:		// 右转
			Motor_LeftFrontControl(750);
			Motor_LeftBackControl(700);
			Motor_RightFrontControl(150);
			Motor_RightBackControl(150);
		break;
	}
	//清零，从头开始
	ble_data.count = 0;
	ble_data.over_flag = 0;
}
