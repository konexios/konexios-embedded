/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ACN_SDK_C_API_CONFIG_H_
#define ACN_SDK_C_API_CONFIG_H_

#if !defined(DEFAULT_API_TIMEOUT)
# define DEFAULT_API_TIMEOUT 10000
#endif

#if !defined(DEV_ENV)
# define HTTP_CIPHER
#endif

/* cloud connectivity */
#if defined(HTTP_CIPHER)
# define ARROW_SCH "https"
# define ARROW_PORT 443
#else
# define ARROW_SCH "http"
# define ARROW_PORT 12001
#endif
#if defined(DEV_ENV)
# define ARROW_ADDR "pgsdev01.arrowconnect.io"
#else
# define ARROW_ADDR "api.arrowconnect.io"
#endif

#define ARROW_API_BASE_URL                  ARROW_SCH "://" ARROW_ADDR ":" xstr(ARROW_PORT)
#define ARROW_API_GATEWAY_ENDPOINT          ARROW_API_BASE_URL "/api/v1/kronos/gateways"
#define ARROW_API_DEVICE_ENDPOINT           ARROW_API_BASE_URL "/api/v1/kronos/devices"
#define ARROW_API_TELEMETRY_ENDPOINT        ARROW_API_BASE_URL "/api/v1/kronos/telemetries"
#define ARROW_API_EVENTS_ENDPOINT           ARROW_API_BASE_URL "/api/v1/core/events"
#define ARROW_API_ACCOUNT_ENDPOINT          ARROW_API_BASE_URL "/api/v1/kronos/accounts"
#define ARROW_API_NODE_ENDPOINT             ARROW_API_BASE_URL "/api/v1/kronos/nodes"
#define ARROW_API_NODE_TYPE_ENDPOINT        ARROW_API_BASE_URL "/api/v1/kronos/nodes/types"
#define ARROW_API_TESTSUITE_ENDPOINT        ARROW_API_BASE_URL "/api/v1/kronos/testsuite"
#define ARROW_API_SOFTWARE_RELEASE_ENDPOINT ARROW_API_BASE_URL "/api/v1/kronos/software/releases/transactions"
#define ARROW_API_SOFTWARE_RELEASE_SCHEDULE_ENDPOINT ARROW_API_BASE_URL "/api/v1/kronos/software/releases/schedules"

/* default gateway and device configuration */
/* default gateway configuration */
#if !defined(GATEWAY_UID_PREFIX)
# define GATEWAY_UID_PREFIX          "unknown"
#endif
#if !defined(GATEWAY_NAME)
#  define GATEWAY_NAME                GATEWAY_UID_PREFIX "-gateway"
#endif
#if !defined(GATEWAY_OS)
# define GATEWAY_OS                  "none"
#endif

#if !defined(GATEWAY_TYPE)
#define GATEWAY_TYPE                "Local"
#endif
#if !defined(GATEWAY_SOFTWARE_NAME)
#define GATEWAY_SOFTWARE_NAME       "eos"
#endif
#if !defined(GATEWAY_SOFTWARE_VERSION)
#define GATEWAY_SOFTWARE_VERSION    "0.1"
#endif

#if !defined(DEVICE_SOFTWARE_NAME)
#define DEVICE_SOFTWARE_NAME GATEWAY_SOFTWARE_NAME
#endif
#if !defined(DEVICE_SOFTWARE_VERSION)
#define DEVICE_SOFTWARE_VERSION GATEWAY_SOFTWARE_VERSION
#endif


/* device configuration */
#if !defined(DEVICE_NAME)
#define DEVICE_NAME                 "unknown"
#endif
#if !defined(DEVICE_TYPE)
#define DEVICE_TYPE                 "unknown"
#endif
#if !defined(DEVICE_UID_SUFFIX)
#define DEVICE_UID_SUFFIX           "dev"
#endif

#if !defined(ARROW_ERROR_DELAY)
#define ARROW_RETRY_DELAY 3000
#endif

#endif // ACN_SDK_C_API_CONFIG_H_
