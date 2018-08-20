/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	SystemInit();
	
	//SystemCoreClockConfigure(); 
	//SystemCoreClockUpdate();
	
  //SysTick_Config(SystemCoreClock / 1000);	
	
	DELAY_Init();	  
	LED_Init();
  BUTTON_INIT();
	USART2_Init();
	
	putchar(12);			//Clearing the terminal
	
	init_I2C1();
	Adc_Start_Init();

	//Uncomment this to search for Slave Address	
	/*uint8_t Device_address = 0;
	Device_address = LCDI2C_Searching_Address();
	printf("Slave address is: %02x \r\n", Device_address);*/
  
	
  /* HSI clock selected to output on MCO1 pin(PA8)*/
  //RCC_MCO1Config(RCC_MCO1Source_HSI, RCC_MCO1Div_3);
  
	//Init One-wire device
	OneWire_Init();
	
	LCDI2C_init(0x3F,20,4);
	LCDI2C_noBacklight();
	Delay_ms(500);
  LCDI2C_backlight();	
  LCDI2C_clear();	
	
	LCDI2C_setCursor(1,0);
	LCDI2C_write_String("Welcome To Thesis");
	LCDI2C_setCursor(1,2);	
	LCDI2C_write_String("Temperature: ");
	LCDI2C_setCursor(18,2);
	LCDI2C_write(0xDF);
	LCDI2C_setCursor(19,2);
	LCDI2C_write_String("C");	
	
	//Init Timer 
	TIM_PWM_Init();
	
	/* initialize the recorder */
	//vTraceEnable(TRC_INIT);	
	
	/* start the recorder */
	//vTraceEnable(TRC_START);
  //uiTraceStart();	
	
	xSemaphore = xSemaphoreCreateBinary();
	
	if(xSemaphore != NULL)
	{		
		xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
						"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
						200,		/* Stack depth in words. 120 * 4  */
						NULL,		/* We are not using the task parameter. */
						1,			/* This task will run at priority 1. */
						NULL );		/* We are not using the task handle. */

		/* Create the other task in exactly the same way. */
		xTaskCreate( vTask2, "Task 2", 200, NULL, 1, NULL );
		xTaskCreate( vTask3, "Task 3", 200, NULL, 1, NULL );
		/* lets create the binary semaphore dynamically */		
		
		/* lets make the semaphore token available for the first time */
		xSemaphoreGive( xSemaphore);

		/* Start the scheduler so our tasks start executing. */
		vTaskStartScheduler();
	}
}

/**
	*TASK 1
	*	
	*
	*
***/
//Execution time ~ 30 ticks 
void vTask1( void *pvParameters )
{
const char *pcTaskName = "Task 1 is running\r\n";
volatile unsigned long ul;
const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
TickType_t xLastWakeTime;
//const TickType_t xFrequency = 500;
	
	xLastWakeTime = xTaskGetTickCount();
	
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		#ifdef DelayUntil 
			vTaskDelayUntil( &xLastWakeTime, xDelay );
		#endif
		if(xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY )==pdTRUE)
		{
			
			#ifdef DEBUG_BY_TICK
				printf("\r\rTich Count Task 1: %d\n", xTaskGetTickCount());
			#endif
			
			printf( "%s\n",pcTaskName );
			//LED_ON();
			readingFunction();
			
//			vTaskDelayUntil( &xLastWakeTime, xDelay );
			#ifdef DEBUG_BY_TICK
				printf("\rTich Count END Task 1: %d\n", xTaskGetTickCount());
			#endif
			

			xSemaphoreGive( xSemaphore);
			#ifdef DelayOnly
				vTaskDelay( xDelay );		
			#endif
		}
		
	}
}
/*-----------------------------------------------------------*/
/**
	*TASK 2
	*	
	*
	*
***/
void vTask2( void *pvParameters )
{
const char *pcTaskName = "Task 2 is running\r\n";
volatile unsigned long ul;
char 					str_buf[7];
const TickType_t xDelay = 350 / portTICK_PERIOD_MS;
	
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		if(reading_Temp != 0 && xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY )==pdTRUE)
		{
			#ifdef DEBUG_BY_TICK
				printf("\r\rTich Count Task 2: %d\n", xTaskGetTickCount());
			#endif			

	  	printf( "%s\n",pcTaskName );
//			LED_OFF();
			sprintf(str_buf, "%0.2f", reading_Temp);
			displayFunction(str_buf);		
			
			#ifdef DEBUG_BY_TICK
				printf("\rTich Count END Task 2: %d\n", xTaskGetTickCount());
			#endif
			
			xSemaphoreGive( xSemaphore);
			vTaskDelay( xDelay );			
		}
	}
}

/*-----------------------------------------------------------*/
/**
	*TASK 3
	*	
	*
	*
***/
//Execution time ~ 5ticks
void vTask3( void *pvParameters )
{
const char *pcTaskName = "Task 3 is running\r\n";
volatile unsigned long ul;
const TickType_t xDelay = 50 / portTICK_PERIOD_MS;


	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		if(xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY )==pdTRUE)
		{
			#ifdef DEBUG_BY_TICK
				printf("\r\rTich Count Task 3: %d\n", xTaskGetTickCount());
			#endif			

			printf( "%s\n",pcTaskName );
//			checking_Button();
						
			if(user_Mode != 0)								//Enter User_Mode
			{
				LED_ON();
				printf("Enter User Mode\r\n\n");				
				if(xTask4 == NULL)
				{
					xTaskCreate( vTask4, "Task 4", 200, NULL, 1, &xTask4 );
				}
			}			
			else															//Enter Auto_Mode
			{				
				LED_OFF();
				if( xTask4 != NULL)
				{					
					vTaskDelete(xTask4);
					xTask4 = NULL;
				}
				
				driving_Fan(&reading_Temp);
				printf( "FAN speed: %d%%\r\n\n",TIM5->CCR1);				//Print the FAN's speed	
			}
			
			#ifdef DEBUG_BY_TICK
				printf("\rTich Count END Task 3: %d\n", xTaskGetTickCount());
			#endif			
			
			xSemaphoreGive( xSemaphore);
			vTaskDelay( xDelay );			
		}			
	}
}

/*-----------------------------------------------------------*/
/**
	*TASK 4
	*	
	*
	*
***/
void vTask4( void *pvParameters )
{
	const char 								*pcTaskName 				= "Task 4 is running\r\n";
	volatile unsigned long 		ul;
	const 										TickType_t xDelay 	= 100 / portTICK_PERIOD_MS;
	char 											adc_buf[7];
	volatile uint16_t					reading_ADC;			
	
	for( ;; )
	{
		
		if(xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY )==pdTRUE)
		{
			#ifdef DEBUG_BY_TICK
				printf("\r\rTich Count Task 4: %d\n", xTaskGetTickCount());
			#endif
			
			printf( "%s\n",pcTaskName );
			reading_ADC = ADC_Read() ;			
			printf("FAN's speed: ");
			//sprintf(adc_buf, "%u", reading_ADC);
			printf("%d%%\r\n\n",reading_ADC);
			
			#ifdef DEBUG_BY_TICK
				printf("\rTich Count END Task 4: %d\n", xTaskGetTickCount());
			#endif
			
			xSemaphoreGive( xSemaphore);
			vTaskDelay( xDelay );
		}
	}
}



void readingFunction()
{
	StartConversion();
	Delay_ms(1);
	reading_Temp = ReportTemperature_2();
}

void driving_Fan(const volatile float *temperature)
{
	if(*temperature > 15 && *temperature <= 20)
	{
		TIM5->CCR1 = 75 * 65535 / 100;  // 75% Duty cycle
	}
	else if(*temperature > 20 && *temperature <= 30)
	{
		TIM5->CCR1 = 40 * 65535 / 100;  // 40% Duty cycle
	}
	else if(*temperature > 30)
	{
		TIM5->CCR1 = 10 * 65535 / 100;  // 10% Duty cycle
	}	
}



void displayFunction(char *str)
{
	LCDI2C_setCursor(13,2);		
	LCDI2C_write_String("       ");	
	LCDI2C_setCursor(13,2);
	LCDI2C_write_String(str);
	LCDI2C_setCursor(18,2);
	LCDI2C_write(0xDF);
	LCDI2C_setCursor(19,2);
	LCDI2C_write_String("C");	
}

void checking_Button()
{
	if(BUTTON_STATE() & 1)
	{
		if(user_Mode == 0)
		{
			user_Mode = 1;
		}
		else
		{
			user_Mode = 0;
		}
	}	
}

/* Handle PC1 interrupt */
void EXTI1_IRQHandler(void) {
	/* Make sure that interrupt flag is set */
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		
		user_Mode = ~user_Mode;
//		LED_ON();
		for(uint32_t i = 0; i < 450000; i++)	//Delay ~196ms -- ~174ms with 400.000
		{
			__asm__("nop");											
		}
		/* Clear interrupt flag */
//		LED_OFF();
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}

void vApplicationGetIdleTaskMemory(void)
{
	
}

void vApplicationGetTimerTaskMemory(void)
{
	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
