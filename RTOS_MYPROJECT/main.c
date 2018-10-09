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
	
	//Relevant to this: https://www.freertos.org/RTOS-Cortex-M3-M4.html
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
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
//	LCDI2C_noBacklight();
//	Delay_ms(500);
//  LCDI2C_backlight();	
//  LCDI2C_clear();	
	
	LCDI2C_setCursor(2,0);
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
	xButton_Semaphore = xSemaphoreCreateBinary();
	
	if(xSemaphore != NULL)
	{		
		xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
						"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
						200,		/* Stack depth in words. 200 * 4  */
						NULL,		/* We are not using the task parameter. */
						4,			/* This task will run at priority 1. */
						NULL );		/* We are not using the task handle. */

		/* Create the other task in exactly the same way. */
		xTaskCreate( vTask2, "Task 2", 200, NULL, 1, NULL );
		xTaskCreate( vTask3, "Task 3", 200, NULL, 5, NULL );
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
		if(xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ) == pdTRUE)
		{
			
			#ifdef DEBUG_BY_TICK
				printf("\r\rTich Count Task 1: %d\n", xTaskGetTickCount());
			#endif
			
			printf( "%s\n",pcTaskName );
			readingFunction();
			
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
const char 								*pcTaskName = "Task 2 is running\r\n";
volatile unsigned long 		ul;
char 											str_buf[7];
const 										TickType_t xDelay = 350 / portTICK_PERIOD_MS;
char											*mode[] = {"Auto Mode", "User Mode"};
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		if(reading_Temp != 0 && xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY )==pdTRUE)
		{
			#ifdef DEBUG_BY_TICK
				printf("\r\rTich Count Task 2: %d\n", xTaskGetTickCount());
			#endif			

	  	printf( "%s\n",pcTaskName );
			sprintf(str_buf, "%0.2f", reading_Temp);
			if(user_Mode != 0)								//Enter User_Mode
			{
				displayFunction(str_buf, mode[1]);		
			}
			else
			{
				displayFunction(str_buf, mode[0]);
			}
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
const TickType_t xDelay = 150 / portTICK_PERIOD_MS;

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
			
			/*Checking if *flag* from the interrupt function is SET.
				It would delay from amount of time because of the unstable of the button.
				Then reset the flag for the next time.
				If the flag is SET -> switching modes (Auto_Mode and User_Mode)
				Uncomment to used the below codes. This was the early version of switing mode \
				by seting/reseting flag variable*/
			/*
			if(flag == SET)
			{
				vTaskDelay(150/portTICK_PERIOD_MS);
				flag = RESET;
				user_Mode = ~user_Mode;
			}
			*/
			
			/*Checking if *xButton_Semaphore* is Giving by external interrupt button.
				It would delay from amount of time because of the unstable of the button.
				Then switching modes (Auto_Mode and User_Mode)*/
			if(xSemaphoreTake( xButton_Semaphore, ( TickType_t ) 0 ))
			{
				user_Mode = ~user_Mode;
			}
			
			//Enter the user_Mode or Auto_mode by checking the user_Mode variable
			//The user_Mode would driving the FAN speed manually by changing the potentiometer - from the ADC
			//The auto_Mode would driving the FAN speed automatically by its algorithms according to \
			the temperature data from sensor 
			if(user_Mode != 0)								//Enter User_Mode
			{
				LED_ON();
				printf("Enter User Mode\r\n\n");				
				if(xTask4 == NULL)
				{
					xTaskCreate( vTask4, "Task 4", 200, NULL, 3, &xTask4 );
				}
			}			
			else if(user_Mode == 0)						//Enter Auto_Mode
			{				
				LED_OFF();
				if( xTask4 != NULL)
				{					
					vTaskDelete(xTask4);
					xTask4 = NULL;
				}				
				auto_Fan(&reading_Temp);
				printf( "FAN speed: %d%%\r\n\n",100 - (TIM5->CCR1 *100) /65535);				//Print the FAN's speed	
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
	uint16_t									reading_ADC;			
	uint16_t									fan_Speed;
	for( ;; )
	{
		
		if(xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY )==pdTRUE)
		{
			#ifdef DEBUG_BY_TICK
				printf("\r\rTich Count Task 4: %d\n", xTaskGetTickCount());
			#endif
			
			printf( "%s\n",pcTaskName );
			reading_ADC = ADC_Read()  ;	
			reading_ADC = (reading_ADC * 100) / 0x0FFF; //Caculating percentage of reading value in 12-bits value 
			printf("FAN's speed: ");
			printf("%d%%\r\n\n",reading_ADC);						//Monitor in percentage
			fan_Speed = 100 - reading_ADC;							//Calculating fan_Speed to duty-cycle
			driving_Fan(&fan_Speed);										//Driving motor - FAN
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


void auto_Fan(const volatile float *temperature)
{
	uint16_t speed_Value;
	if(*temperature > 20 && *temperature <= 24)
	{
		speed_Value = 75;																		//		TIM5->CCR1 = 75 * 65535 / 100;  // 75% Duty cycle
		driving_Fan(&speed_Value);
	}
	else if(*temperature > 24 && *temperature <= 27)
	{
		speed_Value = 40;																		//		TIM5->CCR1 = 40 * 65535 / 100;  // 40% Duty cycle
		driving_Fan(&speed_Value);
	}
	else if(*temperature > 27)
	{
		speed_Value = 10;																		//		TIM5->CCR1 = 10 * 65535 / 100;  // 10% Duty cycle
		driving_Fan(&speed_Value);
	}	
}

void driving_Fan(const uint16_t *value)
{
	TIM5->CCR1 = *value * 65535 / 100;
}


void displayFunction(char *str, char *mode)
{
	LCDI2C_setCursor(6,1);		
	LCDI2C_write_String("    ");
	LCDI2C_setCursor(6,1);
	LCDI2C_write_String(mode);
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
		
		//flag = SET; 			//another way for changing system mode
		
		//Giving Semaphore from ISR to be able to change system mode
		xSemaphoreGiveFromISR(xButton_Semaphore, NULL);
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}




void taskSwitchIn_Debugger(char *taskName)
{
	LED_ON();
}

void taskSwitchOut_Debugger(char *taskName)
{
	LED_OFF();
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


void vApplicationStackOverflowHook(void)
{
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
