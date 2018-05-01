#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


void 	LED_Init		(void);
void 	LED_ON			(void);
void 	LED_OFF			(void);



#endif
