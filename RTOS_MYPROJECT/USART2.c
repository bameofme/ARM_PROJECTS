#include "USART2.h"



void USART2_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	USART_InitTypeDef USART_InitStructurel;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;	
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(GPIOA, &GPIO_InitStructure1);
	
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	
	USART_InitStructurel.USART_BaudRate = 115200ul;
	USART_InitStructurel.USART_WordLength = USART_WordLength_8b;
	USART_InitStructurel.USART_StopBits = USART_StopBits_1;
	USART_InitStructurel.USART_Parity = USART_Parity_No;
	USART_InitStructurel.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructurel.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructurel);
	
	USART_Cmd(USART2, ENABLE);
	
	
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int ch) {

#ifdef __DBG_ITM
  ITM_SendChar (ch & 0xFF);
#else
  while (!(USART2->SR & 0x0080));
  USART2->DR = (ch & 0xFF);
#endif

  return (ch);
}

/*----------------------------------------------------------------------------
  Read character from Serial Port
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USART2->SR & 0x0020)
    return (USART2->DR);
#endif

  return (-1);
}


