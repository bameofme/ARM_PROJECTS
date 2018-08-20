#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"


void 			BUTTON_INIT		(void);
uint8_t 	BUTTON_STATE	(void);



#endif
