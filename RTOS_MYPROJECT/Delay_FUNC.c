#include "DELAY_FUNC.h"

void DELAY_Init()
{
	TIM_TimeBaseInitTypeDef timerInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timerInitStructure.TIM_Prescaler = 15; // counter rate is 1ms
	timerInitStructure.TIM_Period = 4294967295;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_ClockDivision = 0;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
}



void Delay_us(uint32_t us)
{
	__disable_irq();
	TIM_SetCounter(TIM2, 0);    // Make sure TIM2 Counter start from zero
	TIM_Cmd(TIM2, ENABLE);              // Enalbe TIM2
	while(TIM_GetCounter(TIM2) < us);   // Wait ms Miliseconds
	TIM_Cmd(TIM2, DISABLE);
	__enable_irq();
	
}


void Delay_ms(uint32_t ms)
{
	uint32_t temp = ms * 1000 ;
	__disable_irq();
	TIM_SetCounter(TIM2, 0);    // Make sure TIM2 Counter start from zero
	TIM_Cmd(TIM2, ENABLE);              // Enalbe TIM2
	while(TIM_GetCounter(TIM2) < temp);   // Wait ms Miliseconds
	TIM_Cmd(TIM2, DISABLE);
	__enable_irq();
}

void Delay_s(uint32_t s)
{
	uint32_t temp = s * 1000000 ;
	__disable_irq();
	TIM_SetCounter(TIM2, 0);    // Make sure TIM2 Counter start from zero
	TIM_Cmd(TIM2, ENABLE);              // Enalbe TIM2
	while(TIM_GetCounter(TIM2) < temp);   // Wait ms Miliseconds
	TIM_Cmd(TIM2, DISABLE);
	__enable_irq();
}
