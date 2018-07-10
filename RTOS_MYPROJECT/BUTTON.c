#include "BUTTON.h"


void BUTTON_INIT()
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	
	/* Enable the GPIOA peripheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
	/* Configure MCO1 pin(PC.13) in alternate function */
  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure1.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(GPIOC, &GPIO_InitStructure1);
	
}


/*
*	Return Button_state 
	@ return 1 if button is pressed
	@	return 0 if button is not pressed
*/
uint8_t BUTTON_STATE()
{
	uint8_t VALUE = 0;
	
	if((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)) == 0)
	{
		VALUE |= 1;
	}
	return (VALUE);	
}


