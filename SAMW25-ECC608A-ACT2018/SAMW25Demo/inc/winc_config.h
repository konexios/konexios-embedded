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
#ifndef __WINC1500_INIT_H__
#define __WINC1500_INIT_H__

#include "defs_colors.h"

#ifndef printf
int printf ( const char * format, ... );
#endif

#define CONF_WINC_PIN_CHIP_SELECT		WINC_PIN_CHIP_SELECT
#define CONF_WINC_PIN_WAKE				WINC_PIN_WAKE
#define CONF_WINC_PIN_RESET				WINC_PIN_RESET
#define CONF_WINC_PIN_ENABLE			WINC_PIN_EN

#ifndef CONF_MGMT
#define CONF_MGMT 0
#endif
#ifndef CONF_WINC_USE_SPI
#define CONF_WINC_USE_SPI 1
#endif
#ifndef CONF_PERIPH
#define CONF_PERIPH 1
#endif
#define CONF_WINC_DEBUG (1)
#define CONF_WINC_PRINTF(...) do{RED();printf(__VA_ARGS__);DEFAULT();}while(0)

#endif

// EOF
