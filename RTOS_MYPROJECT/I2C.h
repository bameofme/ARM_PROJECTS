#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "I2C.h"


void init_I2C1(void);
void I2C_StartTransmission(I2C_TypeDef* I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress);
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_ReadData(I2C_TypeDef* I2Cx);



#endif
