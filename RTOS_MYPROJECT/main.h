/**
  ******************************************************************************
  * @file    ARM-Project/main.h
  * @author  Nhut Tran
  * @date    01-Ocotber-2018
  * @brief   Header file for main.c
  ******************************************************************************
  * @attention
  *	This file is based on the example file of ST company
	* This system would demonstrate the smart home system
	*	The system shall have:
	* 1. Data acquisition function to collect temperature
	*	2. Displaying function to display the data on LCD
	* 3. Driving fan(motor) according to temperature data in auto-mode
	* 4. Driving fan(motor) according to the changing input voltage from potentiometer
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


/* Define MACROS */




/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "LED.h"
#include "BUTTON.h"
#include "USART2.h"
#include <stdio.h>
//#include "stm32f4xx_i2c.h"
#include "I2C.h"
#include "LiquidCrystal_I2C.h"
#include "DS18B20.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "Delay_FUNC.h"
#include "CTR_FAN.h"
#include "ADC_CTR_FAN.h"

#if defined (HSI_VALUE)
	
	#undef 	HSI_VALUE
	#define HSI_VALUE			((uint32_t)16000000ul)
#endif


/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )

#define DelayUntil
//#define DEBUG_BY_TICK

/* The task functions prototype*/
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void vTask3( void *pvParameters );
void vTask4( void *pvParameters );

void checking_Button( void );
void readingFunction(	void );
void displayFunction( char *str, char *mode);
void auto_Fan(const volatile float *temperature);
void driving_Fan(const uint16_t *value);

SemaphoreHandle_t xSemaphore = NULL;
SemaphoreHandle_t xButton_Semaphore = NULL;
TaskHandle_t xTask4 = NULL;


FlagStatus flag; 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);


volatile float 						reading_Temp		= 0	;
static volatile uint8_t		user_Mode				= 0	;
volatile uint32_t					debug_Timing		= 0 ;
volatile uint8_t 					sensor_detected = 0xFF;



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
