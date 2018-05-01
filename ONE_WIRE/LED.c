#include <LED.h>


void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the GPIOA peripheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* Configure MCO1 pin(PA.5) in alternate function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void LED_ON()
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
}

void LED_OFF()
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
}


