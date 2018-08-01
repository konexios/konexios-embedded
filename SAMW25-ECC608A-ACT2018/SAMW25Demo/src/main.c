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
#include "defs.h"

#include "FreeRTOS.h"
#include "task.h"
#include "system_tasks.h"
#include "driver_init.h"

#include "system.h"
#include "winc_main.h"
#include "console.h"
#include "crypto.h"
#include "ac.h"
#include "sdk.h"

// Types and definitions
// ---------------------------------------------------------------------------

// Variables
// ---------------------------------------------------------------------------

// Local prototypes
// ---------------------------------------------------------------------------


// Public Functions
// ---------------------------------------------------------------------------
int main(void)
{
	// Initialize the board
	system_init();
	drivers_init();
		
	// Setup SDK related code
	sdk_crypt_init();
	sdk_socket_init();
	
	// Enable USART and I2C0
	usart_sync_enable(&USART0);
	i2c_m_sync_enable(&I2C_0);
	
	printf("---System Restart---\n");

	// Crypto chip init
	crypto_init(ATECC608A);

	// Wifi init
	winc_init();
	
	// Init arrow connect source
	ac_init();

	// Load the default variables
	system_loadvars();

	// Print welcome and help menu
	console_init();
		
	// Start the tasks
	systasks_init();

    // Start scheduler (does this return?)
	vTaskStartScheduler();

	// Should never get here

	return 0;
}

// Private functions
// ---------------------------------------------------------------------------



// EOF
