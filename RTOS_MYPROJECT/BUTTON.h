#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void 			BUTTON_INIT		(void);
uint8_t 	BUTTON_STATE	(void);



#endif
