#include "BUTTON.h"


void BUTTON_INIT()
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	
	/* Enable the GPIOA peripheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Configure MCO1 pin(PC.13) in alternate function */
  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_1 ;
  GPIO_InitStructure1.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_DOWN;  
  GPIO_Init(GPIOC, &GPIO_InitStructure1);
	
	/* Tell system that you will use PC1 for EXTI_Line1 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
	
	/* PD0 is connected to EXTI_Line1 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PD0 is connected to EXTI_Line1, which has EXTI1_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}


/*
*	Return Button_state 
	@ return 1 if button is pressed
	@	return 0 if button is not pressed
*/
uint8_t BUTTON_STATE()
{
	uint8_t VALUE = 0;
	uint32_t debount_time = 0;
	debount_time |= 100000;				//150ms
	uint32_t last_time = (TIM_GetCounter(TIM2) | debount_time) % 0xffffffff;
	
	
	if((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)) != 0)
	{
		while(TIM_GetCounter(TIM2) < debount_time);
		VALUE |= 1;
	}
	return (VALUE);	
}


