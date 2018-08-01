/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions
#define GPIO_PIN_FUNCTION_A		0
#define GPIO_PIN_FUNCTION_B		1
#define GPIO_PIN_FUNCTION_C		2
#define GPIO_PIN_FUNCTION_D		3
#define GPIO_PIN_FUNCTION_E		4
#define GPIO_PIN_FUNCTION_F		5
#define GPIO_PIN_FUNCTION_G		6
#define GPIO_PIN_FUNCTION_H		7

// I2C
#define PA08					GPIO(GPIO_PORTA, 8)
#define PA09					GPIO(GPIO_PORTA, 9)

// SPI
#define CONF_WINC_SPI_MOSI		GPIO(GPIO_PORTA, 12)
#define CONF_WINC_SPI_SCK		GPIO(GPIO_PORTA, 13)
#define WINC_PIN_CHIP_SELECT	GPIO(GPIO_PORTA, 14)
#define CONF_WINC_SPI_MISO		GPIO(GPIO_PORTA, 15)

// WINC1500
#define WINC_PIN_EN				GPIO(GPIO_PORTA, 28)
#define WINC_PIN_RESET			GPIO(GPIO_PORTA, 27)
#define WINC_PIN_WAKE			GPIO(GPIO_PORTB, 8)
#define CONF_WINC_EXT_INT_PIN	GPIO(GPIO_PORTB, 9)

// 
#define PA23					GPIO(GPIO_PORTA, 23)
#define PA22					GPIO(GPIO_PORTA, 22)
#define PB10					GPIO(GPIO_PORTB, 10)
#define PB11					GPIO(GPIO_PORTB, 11)
#define LED0					GPIO(GPIO_PORTA, 23)
#define SW0						GPIO(GPIO_PORTB, 23)

#endif // ATMEL_START_PINS_H_INCLUDED
