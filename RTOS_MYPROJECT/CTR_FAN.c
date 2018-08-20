#include "CTR_FAN.h"

void TIM_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	TIM_OCInitTypeDef          TIM_OCInitStructure;
	GPIO_InitTypeDef           GPIO_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0xF;  
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;   // 65535
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 0xFFFF;
  //TIM_OCStructInit(&TIM_OCInitStructure);
  
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);  
  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
 
  TIM_ARRPreloadConfig(TIM5, ENABLE);
 
  /* TIM1 enable counter */
  TIM_Cmd(TIM5, ENABLE);
//  TIM_CtrlPWMOutputs(TIM5, ENABLE);		
}




