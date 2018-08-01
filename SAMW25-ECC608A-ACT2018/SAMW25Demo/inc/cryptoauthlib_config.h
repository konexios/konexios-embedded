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
/* Auto-generated config file cryptoauthlib_config.h */
#ifndef CRYPTOAUTHLIB_CONFIG_H
#define CRYPTOAUTHLIB_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Basic configuration

// <q> Debug Helper API
//  <i> Enable debug helper API
//  <id> debug_helper
#ifndef CONF_CRYPTOAUTHLIB_DEBUG_HELPER
#define CONF_CRYPTOAUTHLIB_DEBUG_HELPER 1
#endif

// </h>

/* Enable debug helper function */
#if CONF_CRYPTOAUTHLIB_DEBUG_HELPER == 1
#ifndef ATCAPRINTF
#define ATCAPRINTF 1
#endif
#endif

/* Enable HAL I2C */
#ifndef ATCA_HAL_I2C
#define ATCA_HAL_I2C
#endif

// <<< end of configuration section >>>

#endif // CRYPTOAUTHLIB_CONFIG_H
