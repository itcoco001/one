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

//���Ҳ��Խӿ�
void JTAG_SW_Config(void)
{
	//�ر�JTAG�ӿڣ�����SW�ӿ�--PA15��PB3��PB4����
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

// ����ģ�飺�����л�
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
	LED_Config();//LED�Ƴ�ʼ��
	BEEP_Config();//��������ʼ��
	KEY_Config();
	USART1_Config();//����1��ʼ��
	printf("I Love CHINA\r\n");
	printf("�Ұ��й�\r\n");
	LCD_Init();//LCD����ʼ��
	LCD_Fill(0,0,128,160,RED);
	LCD_ShowChinese(0,0,"֣�ݹ��̼���ѧԺ",BLACK,WHITE,16,0);
	LCD_ShowString(56,16,"16",BLACK,WHITE,16,0);
	LCD_ShowChinese(0,32,"�߸���",BLACK,WHITE,16,0);
	LCD_ShowChinese(64,32,"������",BLACK,WHITE,16,0);
	LCD_ShowChinese(0,48,"���Ǵ�",BLACK,WHITE,16,0);
	LCD_ShowChinese(64,48,"���ٽ�",BLACK,WHITE,16,0);
	LCD_ShowChinese(0,128,"��ǰģʽ��",BLACK,WHITE,16,0);
	BLE_Config();//������ʼ��
	Motor_Config();//�����ʼ��
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
				case 4:							//ǰ��
					Motor_LeftFrontControl(525);
					Motor_LeftBackControl(525);
					Motor_RightFrontControl(525);
					Motor_RightBackControl(550);
					if(sr04_len < 45){		//��ת
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
						LCD_ShowChinese(80, 128, "����", BLACK, WHITE, 16, 0);
					break;
					case 2:
						LCD_ShowChinese(80, 128, "ѭ��", BLACK, WHITE, 16, 0);
					break;
					case 3:
						LCD_ShowChinese(80, 128, "���", BLACK, WHITE, 16, 0);
					break;
					case 4:
						LCD_ShowChinese(80, 128, "����", BLACK, WHITE, 16, 0);	
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








			
