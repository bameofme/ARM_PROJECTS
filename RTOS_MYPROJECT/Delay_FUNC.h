#ifndef DELAY_FUNC_H
#define	DELAY_FUNC_H

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stdio.h"

void DELAY_Init 	(void);
void Delay_us			(uint32_t us);
void Delay_ms			(uint32_t ms);
void Delay				(uint32_t s);





#endif