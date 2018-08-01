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

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

struct spi_m_sync_descriptor SPI_0;

struct i2c_m_sync_desc I2C_0;

struct usart_sync_descriptor USART0;

struct flash_descriptor FLASH0;

void EXTERNAL_IRQ_0_init(void)
{
	_gclk_enable_channel(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);

	// Set pin direction to input
	gpio_set_pin_direction(SW0, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SW0,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SW0, PINMUX_PB23A_EIC_EXTINT7);

	// Set pin direction to input
	gpio_set_pin_direction(CONF_WINC_EXT_INT_PIN, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(CONF_WINC_EXT_INT_PIN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(CONF_WINC_EXT_INT_PIN, PINMUX_PB09A_EIC_EXTINT9);

	ext_irq_init();
}

void I2C_0_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA08,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA08, PINMUX_PA08C_SERCOM0_PAD0);

	gpio_set_pin_pull_mode(PA09,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA09, PINMUX_PA09C_SERCOM0_PAD1);
}

void I2C_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
	_gclk_enable_channel(SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	i2c_m_sync_init(&I2C_0, SERCOM0);
	I2C_0_PORT_init();
}

void SPI_0_PORT_init(void)
{

	// Set pin direction to output
	gpio_set_pin_direction(CONF_WINC_SPI_MOSI, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(CONF_WINC_SPI_MOSI,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(CONF_WINC_SPI_MOSI, PINMUX_PA12C_SERCOM2_PAD0);

	// Set pin direction to output
	gpio_set_pin_direction(CONF_WINC_SPI_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(CONF_WINC_SPI_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(CONF_WINC_SPI_SCK, PINMUX_PA13C_SERCOM2_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(CONF_WINC_SPI_MISO, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(CONF_WINC_SPI_MISO,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(CONF_WINC_SPI_MISO, PINMUX_PA15C_SERCOM2_PAD3);
}

void SPI_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM2);
	_gclk_enable_channel(SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI_0, SERCOM2);
	SPI_0_PORT_init();
}

void USART0_PORT_init(void)
{

	gpio_set_pin_function(PB10, PINMUX_PB10D_SERCOM4_PAD2);
	gpio_set_pin_function(PB11, PINMUX_PB11D_SERCOM4_PAD3);
	
	//gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM3_PAD1);
	//gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM3_PAD0);
}

void USART0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM4);
	_gclk_enable_channel(SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC);
}

void USART0_init(void)
{
	USART0_CLOCK_init();
	usart_sync_init(&USART0, SERCOM4, (void *)NULL);
	USART0_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void drivers_init(void)
{
	init_mcu();

	// Set pin direction to output
	gpio_set_pin_direction(LED0, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(LED0,false);

	gpio_set_pin_function(LED0, GPIO_PIN_FUNCTION_OFF);

	EXTERNAL_IRQ_0_init();

	I2C_0_init();

	SPI_0_init();

	USART0_init();
	
	// FLASH init
	_pm_enable_bus_clock(PM_BUS_APBB, NVMCTRL);
	flash_init(&FLASH0, NVMCTRL);
	

	delay_driver_init();
}

void driver_LED(bool en)
{
	//printf("LED: %d\n",en);
	if(en) gpio_set_pin_level(LED0,false);
	else gpio_set_pin_level(LED0,true);
	return;
}
