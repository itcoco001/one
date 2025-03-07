#include "delay.h"

extern u32 timeled;
extern u32 timekey;
extern u32 timebeep;
extern u32 timebz;
extern u32 timesteer;
u32 time_sr04[2] = {0,100};
//系统滴答定时器的中断函数
//1ms -- 0.1ms
void SysTick_Handler(void)
{
	timeled++;
	timekey++;
	timebeep++;
	timebz++;
	timesteer++;
	time_sr04[0]++;
}


//系统定时器初始化
void Systick_Init(uint32_t load)
{
	if(SysTick_Config(load) == 1)
	{
		while(1);
	}
}


void Delay_us(uint32_t time)
{
	while(time--) {
		delay_1us();
	}
}

void Delay_ms(uint32_t time)
{
	uint64_t t = time*1000;
	while(t--) {
		delay_1us();
	}
}






