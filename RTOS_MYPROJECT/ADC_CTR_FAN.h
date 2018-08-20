#ifndef ADC_CTR_FAN_H
#define ADC_CTR_FAN_H


#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_adc.h"

void Adc_Start_Init(void);
uint16_t ADC_Read(void);


#endif
