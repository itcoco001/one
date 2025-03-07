#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "uart1.h"
#include "lcd.h"
#include "ble.h"
#include "motor.h"
#include "ir.h"
#include "hc_sr04.h"
#include "steer.h"
#include "wifi.h"
//extern const unsigned char gImage_pic1[40960];

//厂家测试接口
void JTAG_SW_Config(void)
{
	//关闭JTAG接口，开启SW接口--PA15、PB3、PB4可用
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}
u32 timeled = 0;
u32 timekey = 0;
u32 timebeep = 0;
u32 timebz=0;
u32 timesteer = 0;
u8 condition_met = 0;
u8 lcd_buff[64]="\0";

// 功能模块：条件切换
void Handle_Conditions(void)
{
    switch (condition_met % 5)
    {
    case 0:
			Motor_Stop();
			break;
    case 1:
			BLE_DataAnlay();
			break;
    case 2:
			IR_GetLineAndMove();
			break;
    case 3:
			WIFI_Analysis();
			break;
    }
}

int main(void)
{
	u8 key = 0;
	u8 count = 0;
	NVIC_SetPriorityGrouping(4);
	JTAG_SW_Config();
	Systick_Init(7200);
	LED_Config();//LED灯初始化
	BEEP_Config();//蜂鸣器初始化
	KEY_Config();
	USART1_Config();//串口1初始化
	printf("I Love CHINA\r\n");
	printf("我爱中国\r\n");
	LCD_Init();//LCD屏初始化
	LCD_Fill(0,0,128,160,RED);
	LCD_ShowChinese(0,0,"郑州工程技术学院",BLACK,WHITE,16,0);
	LCD_ShowString(56,16,"16",BLACK,WHITE,16,0);
	LCD_ShowChinese(0,32,"高富林",BLACK,WHITE,16,0);
	LCD_ShowChinese(64,32,"曾国庆",BLACK,WHITE,16,0);
	LCD_ShowChinese(0,48,"栗智聪",BLACK,WHITE,16,0);
	LCD_ShowChinese(64,48,"和少杰",BLACK,WHITE,16,0);
	LCD_ShowChinese(0,128,"当前模式：",BLACK,WHITE,16,0);
	BLE_Config();//蓝牙初始化
	Motor_Config();//电机初始化
	IR_Config();
	SR04_Config();
	Steer_Init();
	WIFI_Config();
	while (1)
  {
//		Handle_Conditions();
		if(time_sr04[0] >= time_sr04[1]){
			time_sr04[0] = 0;
			SR04_GetLen();
		}
		if(timebz >= 2000){
//				timebz++;
			timebz = 0;
			switch(condition_met % 5){
				case 4:							//前进
					Motor_LeftFrontControl(525);
					Motor_LeftBackControl(525);
					Motor_RightFrontControl(525);
					Motor_RightBackControl(550);
					if(sr04_len < 45){		//左转
						Motor_LeftFrontControl(-450);
						Motor_LeftBackControl(-450);
						Motor_RightFrontControl(825);
						Motor_RightBackControl(850);
					}
			}
		}
		//		WIFI_Analysis();
		if(timesteer >= 300){
			timesteer = 0;
			Steer_Control();
		}
		if(timekey > 100){
			timekey = 0;
			key = Key_GetValue();
			if(key == 1){
				count = 4;
				condition_met++;
				switch(condition_met % 5){
					case 0:
						LCD_Fill(80,128,112,144,RED);
					break;
					case 1:
						LCD_ShowChinese(80, 128, "蓝牙", BLACK, WHITE, 16, 0);
					break;
					case 2:
						LCD_ShowChinese(80, 128, "循迹", BLACK, WHITE, 16, 0);
					break;
					case 3:
						LCD_ShowChinese(80, 128, "监控", BLACK, WHITE, 16, 0);
					break;
					case 4:
						LCD_ShowChinese(80, 128, "避障", BLACK, WHITE, 16, 0);	
					break;					
				}
			}else if(key == 2){
				count = 8;
			}
		}
		if(timeled > 5000){
			timeled = 0;
			LEDx_Toggle(LED1_Port,LED1_Pin);
			printf("len = %d\r\n",sr04_len);
			sprintf((char *)lcd_buff,"len = %d  ",sr04_len);
			LCD_ShowString(0,80,lcd_buff,BLACK,WHITE,16,0);
		}
		if(timebeep > 3000){
			timebeep = 0;
			if(count > 0){
				count--;
				BEEP_Toggle();
			}
		}
		Handle_Conditions();
  }
}








			
