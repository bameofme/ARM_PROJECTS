/**
	*Using DS18B20.h and DS18B20.c to be able to read one-wire sensor
	*This is an example about reading DS18B20 - reading only one sensor 
	*To be able to read more sensor, it must be modified
	*Followed the instruction in DS18B20 datasheet 	-	https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf
	*Written by Karl Lunt		- 2015									- https://www.seanet.com/~karllunt/onewire_stm32f4.html
	*Modified by Nhut Tran 	- 2018
	*/


#ifndef DS18B20_H
#define	DS18B20_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
//#include "tm_stm32f4_timer_properties.h"
//#include "tm_stm32f4_delay.h"
#include "LiquidCrystal_I2C.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Delay_FUNC.h"

/* How many sensors we are expecting on 1wire bus? */
#define EXPECTING_SENSORS    1


/*
 *  Specify the port and pin used for 1-wire comms
 */
#define ONEWIRE_PIN_NUM				0
#define ONEWIRE_PIN_MASK      (1<<ONEWIRE_PIN_NUM)
#define ONEWIRE_PORT					GPIOC
#define ONEWIRE_CLK						RCC_AHB1Periph_GPIOC  


/*
 *  The following macros collapse direct accesses of the GPIO registers into
 *  single commands.  Refer to stm32f4xx_gpio.c and the STM32F4xx Reference
 *  Manual (GPIO chapter) for details.
 */
#define  ONEWIRE_INPUT_READ				ONEWIRE_PORT->IDR&ONEWIRE_PIN_MASK
#define  ONEWIRE_OUTPUT_HIGH			ONEWIRE_PORT->BSRRL=ONEWIRE_PIN_MASK
#define  ONEWIRE_OUTPUT_LOW				ONEWIRE_PORT->BSRRH=ONEWIRE_PIN_MASK
#define  ONEWIRE_CONFIG_OUTPUT		ONEWIRE_PORT->MODER|=(GPIO_Mode_OUT<<(ONEWIRE_PIN_NUM*2))
#define  ONEWIRE_CONFIG_INPUT			ONEWIRE_PORT->MODER&=~(GPIO_MODER_MODER0<<(ONEWIRE_PIN_NUM*2))


#define  READ_ROM			0x33
#define  SKIP_ROM			0xcc
#define  READ_SCRATCHPAD	0xbe
#define  CONVERT_TEMP		0x44


/*
 *  Local functions
 */
void					PingOneWireNetwork(void);
void					OneWire_Init(void);
void					SendInitialization(void);
void					SendByte(uint8_t  val);
uint8_t				ReadByte(void);
void					ReportScratchpad(void);
void					StartConversion(void);
void					ReportTemperature(void);
void					ReportROM(void);
float					ReportTemperature_2(void);



#endif
