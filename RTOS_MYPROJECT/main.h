/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

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
void displayFunction( char *str);
void driving_Fan(const volatile float *temperature);

SemaphoreHandle_t xSemaphore = NULL;
TaskHandle_t xTask4 = NULL;



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
