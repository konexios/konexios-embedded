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
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_ext_irq.h>
#include <hal_flash.h>
#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_usart_sync.h>

#include <hal_delay.h>

extern struct i2c_m_sync_desc       I2C_0;
extern struct spi_m_sync_descriptor SPI_0;

extern struct usart_sync_descriptor USART0;

extern struct flash_descriptor FLASH0;

void I2C_0_CLOCK_init(void);
void I2C_0_init(void);
void I2C_0_PORT_init(void);

void SPI_0_PORT_init(void);
void SPI_0_CLOCK_init(void);
void SPI_0_init(void);

void USART0_PORT_init(void);
void USART0_CLOCK_init(void);
void USART0_init(void);

void delay_driver_init(void);

void drivers_init(void);

void driver_LED(bool en);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
