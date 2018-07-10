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

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
//static void Delay(__IO uint32_t nTime);
/*
volatile uint32_t msTicks;  */                               // counts 1ms timeTicks
/*----------------------------------------------------------------------------
 * SysTick_Handler:
 *----------------------------------------------------------------------------*/
/* 
void SysTick_Handler(void) {
  msTicks++;
}
*/

/*----------------------------------------------------------------------------
 * Delay: delays a number of Systicks
 *----------------------------------------------------------------------------*/
/*
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}*/


/*
void SystemCoreClockConfigure(void) {

  RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock

  FLASH->ACR  = FLASH_ACR_PRFTEN;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_ICEN;                            // Instruction cache enable
  FLASH->ACR |= FLASH_ACR_DCEN;                            // Data cache enable
  FLASH->ACR |= FLASH_ACR_LATENCY_5WS;                     // Flash 5 wait state

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;                        // APB1 = HCLK/4
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;                        // APB2 = HCLK/2

  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL

  // PLL configuration:  VCO = HSI/M * N,  Sysclk = VCO/P
  RCC->PLLCFGR = ( 16ul                   |                // PLL_M =  16
                 (384ul <<  6)            |                // PLL_N = 384
                 (  3ul << 16)            |                // PLL_P =   8
                 (RCC_PLLCFGR_PLLSRC_HSI) |                // PLL_SRC = HSI
                 (  8ul << 24)             );              // PLL_Q =   8

  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
}

*/
/* Private functions ---------------------------------------------------------*/







/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	//float reading_Temp;
	uint32_t Delay_time;
	Delay_time = 0xC8;
	
	SystemInit();
	
	//TM_DELAY_Init();
	//SystemCoreClockConfigure(); 
	//SystemCoreClockUpdate();
	
  //SysTick_Config(SystemCoreClock / 1000);	
	
	DELAY_Init();	  
	LED_Init();
  BUTTON_INIT();
	USART2_Init();
	
	putchar(12);			//Clearing the terminal
	//printf("\n\rDallas DS18S20 demo\n\r");
	init_I2C1();
	
	//Uncomment this to search for Slave Address	
	/*uint8_t Device_address = 0;
	Device_address = LCDI2C_Searching_Address();
	printf("Slave address is: %02x \r\n", Device_address);*/
  
	
  /* HSI clock selected to output on MCO1 pin(PA8)*/
  RCC_MCO1Config(RCC_MCO1Source_HSI, RCC_MCO1Div_1);
  
	//Init One-wire device
	OneWire_Init();
	
	LCDI2C_init(0x3F,20,4);
//	for(int i = 0; i< 3; i++)
//	{
//    LCDI2C_backlight();
//    Delay_ms(500);
    LCDI2C_noBacklight();
    Delay_ms(500);
	//}
  LCDI2C_backlight();	
  LCDI2C_clear();	
	
	//Ping to One-wire device, reading ROM and report Scratchpad content 
	/*PingOneWireNetwork();
	ReportROM();
	ReportScratchpad();
	StartConversion();
	Delay(750000);
	ReportScratchpad();
	
  //Init LCD_I2C	
	LCDI2C_init(0x3F,20,4);
	for(int i = 0; i< 3; i++)
	{
    LCDI2C_backlight();
    Delay(50);
    LCDI2C_noBacklight();
    Delay(50);
	}
  LCDI2C_backlight();	
  LCDI2C_clear();	
	
	//Printing Welcome words
	LCDI2C_setCursor(1,0);
	LCDI2C_write_String("Welcome To Thesis");
	LCDI2C_setCursor(1,2);	
	LCDI2C_write_String("Temperature: ");
	
	while (1)
  {
		LED_ON();
		Delay(50000);
		LED_OFF();
		Delay(50000);
		if(BUTTON_STATE() & 1)
		{
			printf("Button is pressed \r\n");
			Delay_time = Delay_time - 0x0A;
			Delay(10);			
		}
		
		//Using Convert-T DS18B20 command to ask for temperature data
		StartConversion();
		Delay(500);
		
		//
		ReportTemperature();
		Delay(5000);
  }
	*/
		LCDI2C_setCursor(1,0);
		LCDI2C_write_String("Welcome To Thesis");
		LCDI2C_setCursor(1,2);	
		LCDI2C_write_String("Temperature: ");
	vTraceEnable(TRC_INIT);
	/* initialize the recorder */
	vTraceEnable(TRC_START);
	
	/* start the recorder */
  //uiTraceStart();
	
	
	xSemaphore = xSemaphoreCreateBinary();
	
	if(xSemaphore != NULL)
	{		
		xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
						"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
						120,		/* Stack depth in words. */
						NULL,		/* We are not using the task parameter. */
						1,			/* This task will run at priority 1. */
						NULL );		/* We are not using the task handle. */

		/* Create the other task in exactly the same way. */
		xTaskCreate( vTask2, "Task 2", 120, NULL, 1, NULL );
		
		/* lets create the binary semaphore dynamically */
		
		
		/* lets make the semaphore token available for the first time */
		//xSemaphoreGive( xSemaphore);

		/* Start the scheduler so our tasks start executing. */
		vTaskStartScheduler();
	}
}


void vTask1( void *pvParameters )
{
const char *pcTaskName = "Task 1 is running\r\n";
volatile unsigned long ul;
const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		/* lets make the sema un-available */
	
//		while(xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ))
//		{
	  printf( "%s\n",pcTaskName );
		LED_ON();
		/* lets make the sema available */
		StartConversion();
		//Delay_ms(500);
		vTaskDelay( xDelay );
		reading_Temp = ReportTemperature_2();
		//Delay_ms(5000);
		
		xSemaphoreGive( xSemaphore);
		vTaskDelay( xDelay * 10 );
		//}
		//vTaskDelay( xDelay );
		
		/* Delay for a period. */
//		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
//		{
//			/* This loop is just a very crude delay implementation.  There is
//			nothing to do in here.  Later exercises will replace this crude
//			loop with a proper delay/sleep function. */
//		}
	}
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
const char *pcTaskName = "Task 2 is running\r\n";
volatile unsigned long ul;
char 					str_buf[7];
const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		
//		LED_OFF();
//		LCDI2C_setCursor(1,0);
//		LCDI2C_write_String("Welcome To Thesis");
//		LCDI2C_setCursor(1,2);	
//		LCDI2C_write_String("Temperature: ");
		
		
		/* Print out the name of this task. */
		/* lets make the sema un-available */
		while(xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ))
		{
		LED_OFF();
		LCDI2C_setCursor(13,2);		
		LCDI2C_write_String("       ");	
		LCDI2C_setCursor(13,2);		
		sprintf(str_buf, "%0.2f", reading_Temp);
//		xSemaphoreGive( xSemaphore);
	  	printf( "%s\n",pcTaskName );
			printf("\rDisplay to LCD \n\n\r");
		
		/* lets make the sema available */
		//sprintf(str_buf, "%0.2f", reading_Temp);	
		
		
		LCDI2C_write_String(str_buf);
		 //vTaskDelay( xDelay );
		//Delay_ms(5000);
		
		//xSemaphoreGive( xSemaphore);
			vTaskDelay( xDelay *10 );
		}
//		vTaskDelay( xDelay );
		/* Delay for a period. */
//	for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
//		{
//			/* This loop is just a very crude delay implementation.  There is
//			nothing to do in here.  Later exercises will replace this crude
//			loop with a proper delay/sleep function. */
//		}
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
