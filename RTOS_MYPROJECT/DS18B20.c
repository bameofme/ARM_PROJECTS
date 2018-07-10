#include <DS18B20.h>

uint8_t						reading_pad[9];
uint8_t						reading_rom[8];


/**Ping to One-wire device to detect whether any device on bus
	*Sending Reset pulse 
	*Reading Input - Input is low then there is the device on bus and otherwise is not 
	*/
void  PingOneWireNetwork(void)
{
	uint8_t				response;

	printf("\n\rSending initialization pulses...");

	response = 0;

	ONEWIRE_CONFIG_INPUT;
	Delay_us(100);

	while (1)
	{
		SendInitialization();
		response = ONEWIRE_INPUT_READ;		// device pulls line low in response
		Delay_us(420);

		if (response == 0)  break;
	}
	printf("response detected.");
}



/**
	*Requesting to read temperature data from DS1820
	*Initilization by sending reset pulse 
	*Sending ROM commnad (SKIP ROM)
	*Sending DS18B20 command (CONVERT T)
	*/
void  StartConversion(void)
{
	printf("\n\rSending command to start conversion.");
	SendInitialization();
	Delay_us(100);
	SendByte(SKIP_ROM);
	SendByte(CONVERT_TEMP);
}



/**
	*Reading temperature data from DS1820 - Use this function after Convert T command
	*Initilization by sending reset pulse 
	*Sending ROM commnad (SKIP ROM)
	*Sending DS18B20 command (READ_SCRATCHPAD)
	*Combinating first bytes - two temperature data bytes
	*Converting data bytes to temperature data by divided 16
	*Print to USART terminal
	*Sending data to LCD display
	*/
void  ReportTemperature(void)
{
	uint32_t			val;
	float					t;
	uint8_t				n;
	char 					str_buf[7];
	
	SendInitialization();
	Delay_us(100);
	SendByte(SKIP_ROM);
	SendByte(READ_SCRATCHPAD);
	for (n=0; n<9; n++)
	{
		reading_pad[n] = ReadByte();
	}
	val = (reading_pad[1] <<8) | reading_pad[0];			//Combine two bytes 
	t = (float)val/16;																//Converting byte value into temperature data 
	printf("\n\r 2 byte of temperature");
	printf("\n\r     %02x %02x", reading_pad[1], reading_pad[0]);
	printf("\n\rTemperature is: %0.2f degrees C\r\n\n", t);
	
	//Using sprint to convert int value to string
	sprintf(str_buf, "%0.2f", t);	
	
	//Using this to print the value to LCD-display	
	LCDI2C_setCursor(13,2);		
	LCDI2C_write_String("       ");
	
	LCDI2C_setCursor(13,2);		
	LCDI2C_write_String(str_buf);
	
	Delay_ms(5000);
}

float ReportTemperature_2(void)
{
	uint32_t			val;
	float					temp;
	uint8_t				n;
	char 					str_buf[7];
	
	SendInitialization();
	Delay_us(100);
	SendByte(SKIP_ROM);
	SendByte(READ_SCRATCHPAD);
	for (n=0; n<9; n++)
	{
		reading_pad[n] = ReadByte();
	}
	val = (reading_pad[1] <<8) | reading_pad[0];			//Combine two bytes 
	temp = (float)val/16;																//Converting byte value into temperature data 
	printf("\n\r 2 byte of temperature");
	printf("\n\r     %02x %02x", reading_pad[1], reading_pad[0]);
	printf("\n\rTemperature is: %0.2f degrees C\r\n\n", temp);
	
//	//Using sprint to convert int value to string
//	sprintf(str_buf, "%0.2f", temp);	
//	
//	//Using this to print the value to LCD-display	
//	LCDI2C_setCursor(13,2);		
//	LCDI2C_write_String("       ");
//	
//	LCDI2C_setCursor(13,2);		
//	LCDI2C_write_String(str_buf);
//	
	Delay_ms(5000);
	return temp;
}


/**
	*Requesting to read ROM data from DS1820
	*Initilization by sending reset pulse 
	*Sending ROM commnad (READ_ROM)
	*/
void  ReportROM(void)
{
	uint8_t					n;

	SendInitialization();
	Delay_us(100);
	SendByte(READ_ROM);
	for (n=0; n<8; n++)
	{
		reading_rom[n] = ReadByte();
	}
	printf("\n\rROM:  7  6  5  4  3  2  1  0");
	printf("\n\r     %02x %02x %02x %02x %02x %02x %02x %02x",
		reading_rom[7], reading_rom[6], reading_rom[5], reading_rom[4], reading_rom[3], reading_rom[2], reading_rom[1], reading_rom[0]);
}


/**
	*Requesting to read SCARTCHPAD data from DS1820 - where consists temperature data and other configuration value
	*Initilization by sending reset pulse 
	*Sending ROM commnad (SKIP_ROM)
	*Sending DS18B20 command (READ_SCRATCHPAD) - it will fetch all scratchpad data bytes
	*Sending reading data bytes to USART terminal
	*/
void  ReportScratchpad(void)
{
	uint8_t					n;

	SendInitialization();
	Delay_us(100);
	SendByte(SKIP_ROM);
	SendByte(READ_SCRATCHPAD);
	for (n=0; n<9; n++)
	{
		reading_pad[n] = ReadByte();
	}
	printf("\n\rScratchpad contains: %02x %02x %02x %02x %02x %02x %02x %02x %02x",
		reading_pad[0], reading_pad[1], reading_pad[2], reading_pad[3], reading_pad[4], reading_pad[5], reading_pad[6], reading_pad[7], reading_pad[8]);
}

/**
	*Sending reset pulse to DS18B20 device to initializate.
	*Pulling HIGH the DQ line in 500us
	*Pulling LOW	the DQ line in 500us - to send reset pulse
	*Releasing 		the DQ line
	*Then Reading the DQ line 
	*/
void  SendInitialization(void)
{
	ONEWIRE_OUTPUT_HIGH;			//pull to high 
	ONEWIRE_CONFIG_OUTPUT;
	Delay_us(500);

	ONEWIRE_OUTPUT_LOW;				//pull to low
	Delay_us(500);

	ONEWIRE_OUTPUT_HIGH;			//releasing bus
	ONEWIRE_CONFIG_INPUT;
	Delay_us(50);
}


void  SendByte(uint8_t  val)
{
	uint8_t				n;

	for (n=0; n<8; n++)
	{
		ONEWIRE_OUTPUT_LOW;
		ONEWIRE_CONFIG_OUTPUT;
		Delay_us(5);
		if (val & 1)  ONEWIRE_OUTPUT_HIGH;
		Delay_us(95);
		ONEWIRE_OUTPUT_HIGH;
		Delay_us(5);
		val = val >> 1;
	}
}


uint8_t  ReadByte(void)
{
	uint8_t				n;
	uint8_t				val;

	val = 0;
	for (n=0; n<8; n++)
	{
		val = val >> 1;
		ONEWIRE_OUTPUT_LOW;
		ONEWIRE_CONFIG_OUTPUT;
		Delay_us(5);
		ONEWIRE_OUTPUT_HIGH;
		ONEWIRE_CONFIG_INPUT;
		Delay_us(10);
		if (ONEWIRE_INPUT_READ)  {val |= 0x80;}
		Delay_us(35);
	} 
	return  val;
}


/*
 *  OneWire_Init      hardware-specific configuration of 1-wire I/O
 */
void  OneWire_Init(void)
{
	GPIO_InitTypeDef			GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(ONEWIRE_CLK, ENABLE);		// route the clocks

    GPIO_InitStructure.GPIO_Pin = ONEWIRE_PIN_MASK;				// select the pin to modify
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				// set the mode to output
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// set the I/O speed to 100 MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				// set the output type to open-drain
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			// set the pull-up to none
    GPIO_Init(ONEWIRE_PORT, &GPIO_InitStructure);				// do the init
}

