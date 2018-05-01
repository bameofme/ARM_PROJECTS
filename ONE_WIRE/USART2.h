#ifndef USART2_H
#define USART2_H

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void 				USART2_Init		(void);
extern int  SER_GetChar   (void);
extern int  SER_PutChar   (int c);


#endif
