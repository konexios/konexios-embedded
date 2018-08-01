/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ACN_SDK_C_CONFIG_H_
#define ACN_SDK_C_CONFIG_H_

#define xstr(s) str(s)
#define str(s) #s

#define SDK_VERSION 1.3.10

#if !defined(_KEYS_)
#include "private.h"
#endif

#if !defined(RINGBUFFER_SIZE)
#define RINGBUFFER_SIZE		    1024
#endif

#if defined(STATIC_ACN)
#define STATIC_MQTT_ENV
#define STATIC_HTTP_CLIENT
#define STATIC_SIGN
# if !defined(ARROW_MAX_MQTT_COMMANDS)
# define ARROW_MAX_MQTT_COMMANDS 10
# endif
#endif

#if (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)) || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(__MIPSEL__) || \
    defined(__XTENSA_EL__)
#define __LE_MODE__ 1
#elif (defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(__MIPSEB__) || \
    defined(__XTENSA_EB__)
#define __BE_MODE__ 1
#else
#warning "Undefined endian mode __LE_MODE__/__BE_MODE__ [little by default]"
#endif

#if !defined(__NO_STD__)
# define __USE_STD__
// for the std lib headers
# if !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
# endif
#endif

#if !defined(SSP_PARAMETER_NOT_USED)
# define SSP_PARAMETER_NOT_USED(x) (void)((x))
#endif

#if defined(STATIC_HTTP_CLIENT)
# define RING_BUFFER_ARRAY
#endif

// for AP mode only
#if !defined(MAIN_WLAN_SSID)
#  define MAIN_WLAN_SSID      "ACN_WIFI"
#endif

#include <config/ntp.h>
#include <config/api.h>
#include <config/mqtt.h>

/* telemetry configuration */
#define TELEMETRY_DEVICE_HID        "_|deviceHid"
#define TELEMETRY_TEMPERATURE       "f|temperature"
#define TELEMETRY_HUMIDITY          "f|humidity"
#define TELEMETRY_BAROMETER         "f|barometer"
#define TELEMETRY_ACCELEROMETER_XYZ "f3|accelerometerXYZ"
#define TELEMETRY_ACCELEROMETER_X   "f|accelerometerX"
#define TELEMETRY_ACCELEROMETER_Y   "f|accelerometerY"
#define TELEMETRY_ACCELEROMETER_Z   "f|accelerometerZ"
#define TELEMETRY_GYROMETER_XYZ     "f3|gyrometerXYZ"
#define TELEMETRY_GYROMETER_X       "f|gyrometerX"
#define TELEMETRY_GYROMETER_Y       "f|gyrometerY"
#define TELEMETRY_GYROMETER_Z       "f|gyrometerZ"
#define TELEMETRY_MAGNETOMETER_XYZ  "f3|magnetometerXYZ"
#define TELEMETRY_MAGNETOMETER_X    "f|magnetometerX"
#define TELEMETRY_MAGNETOMETER_Y    "f|magnetometerY"
#define TELEMETRY_MAGNETOMETER_Z    "f|magnetometerZ"
#if !defined(TELEMETRY_DELAY)
#define TELEMETRY_DELAY             5000
#endif
#if !defined(ARROW_MAX_RETRY)
#define ARROW_MAX_RETRY -1
#endif
#define RETRY_UP(r, overact) \
    if ( r >= 0 && ++r > ARROW_MAX_RETRY ) { overact; }
#define RETRY_CR(r) \
    r = 0;
#define TO_FAHRENHEIT(x) ((x)*1.8 + 32)


#endif /* ACN_SDK_C_CONFIG_H_ */
