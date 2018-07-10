#include "I2C.h"


//для I2C
GPIO_InitTypeDef i2c_gpio;
I2C_InitTypeDef i2c;

void init_I2C1(void)
{
    // Enable periphrals clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		
		
		// I2C output pin
    i2c_gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    i2c_gpio.GPIO_Mode = GPIO_Mode_AF;
    i2c_gpio.GPIO_Speed = GPIO_Speed_50MHz;
		i2c_gpio.GPIO_OType =	GPIO_OType_OD;
		i2c_gpio.GPIO_PuPd 	=	GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &i2c_gpio);
		
		
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	
	
    // Setting value for I2C 
    i2c.I2C_ClockSpeed = 100000;						// 100kHz
    i2c.I2C_Mode = I2C_Mode_I2C;						
    i2c.I2C_DutyCycle = I2C_DutyCycle_2;		//50% Duty cycle 
    // Setting slave device address
    i2c.I2C_OwnAddress1 = 0x00;							
    i2c.I2C_Ack = I2C_Ack_Enable;
    i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &i2c);

    
		
    // Enable I2C
    I2C_Cmd(I2C1, ENABLE);
}

/*******************************************************************/
void I2C_StartTransmission(I2C_TypeDef* I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress)
{
    // flag status from register
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    // Generating I2C Start bit
    I2C_GenerateSTART(I2Cx, ENABLE);
    // Ждем пока взлетит нужный флаг
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    // Sending 7 bit slave address
    I2C_Send7bitAddress(I2Cx, slaveAddress<<1, transmissionDirection);
    // Checking transmittion direction 
    if(transmissionDirection== I2C_Direction_Transmitter)
    {
    	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    if(transmissionDirection== I2C_Direction_Receiver)
    {
			while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

/*******************************************************************/
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data)
{
    // Просто вызываем готоваую функцию из SPL и ждем, пока данные улетят
    I2C_SendData(I2Cx, data);
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}



/*******************************************************************/
uint8_t I2C_ReadData(I2C_TypeDef* I2Cx)
{
    // Тут картина похожа, как только данные пришли быстренько считываем их и возвращаем
    while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    uint8_t data;
    data = I2C_ReceiveData(I2Cx);
    return data;
}

