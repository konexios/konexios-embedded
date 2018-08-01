/* Auto-generated config file cryptoauthlib_config.h */
#ifndef CRYPTOAUTHLIB_CONFIG_H
#define CRYPTOAUTHLIB_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Basic configuration

// <q> Debug Helper API
//  <i> Enable debug helper API
//  <id> debug_helper
#ifndef CONF_CRYPTOAUTHLIB_DEBUG_HELPER
#define CONF_CRYPTOAUTHLIB_DEBUG_HELPER 0
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
