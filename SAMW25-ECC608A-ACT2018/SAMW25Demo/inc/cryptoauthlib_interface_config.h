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
/* Auto-generated config file cryptoauthlib_interface_config.h */
#ifndef CRYPTOAUTHLIB_INTERFACE_CONFIG_H
#define CRYPTOAUTHLIB_INTERFACE_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Basic configuration

// <o> I2C Address <0x0-0x3FF>
// <i> The I2C Address of ATECC508A
// <id> atecc508a_i2c_address
#ifndef CONF_ATECC508A_I2C_ADDRESS
#define CONF_ATECC508A_I2C_ADDRESS 0xc0
#endif

// <o> I2C Clock Speed (Hz) <1000-1000000>
// <i> The I2C SCK Clock Frequency
// <id> atecc508a_i2c_frequency
#ifndef CONF_ATECC508A_I2C_FREQUENCY
#define CONF_ATECC508A_I2C_FREQUENCY 400000
#endif

// <o> Wakeup Delay (us)<1-10000>
// <i> Microseconds of Wake High Delay(tWHI) to Data Communication
// <id> atecc508a_i2c_wakeup_delay
#ifndef CONF_ATECC508A_I2C_WAKEUP_DELAY
#define CONF_ATECC508A_I2C_WAKEUP_DELAY 800
#endif

// <o> Receive Retry <1-65535>
// <i> The number of retries to attempt for receiving bytes
// <id> atecc508a_i2c_rxretry
#ifndef CONF_ATECC508A_I2C_RXRETRY
#define CONF_ATECC508A_I2C_RXRETRY 20
#endif

// </h>

// <<< end of configuration section >>>

#endif // CRYPTOAUTHLIB_INTERFACE_CONFIG_H
