/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ACN_SDK_C_MQTT_CONFIG_H_
#define ACN_SDK_C_MQTT_CONFIG_H_

#if !defined(DEFAULT_MQTT_TIMEOUT)
# define DEFAULT_MQTT_TIMEOUT 10000
#endif

#if defined(__IBM__) || \
  ( defined(__AZURE__) || !defined(DEV_ENV) )
# define MQTT_CIPHER
#endif

#if defined(__IBM__) || defined(__AZURE__)
#define MQTT_TWO_CHANNEL
#endif

#if !defined(MQTT_BUF_LEN)
#define MQTT_BUF_LEN 400
#endif

#if !defined(MQTT_RECVBUF_LEN)
#define MQTT_RECVBUF_LEN 1200
#endif

#if defined(ARROW_THREAD) &&  \
    !defined(__IBM__) && \
    !defined(__AZURE__)
# warning "There is only one possible MQTT connection"
# undef ARROW_THREAD
#endif

//#define MQTT_TASK

#if defined(MQTT_CIPHER)
#  define MQTT_SCH "tls"
#  define MQTT_PORT 8883
#else
# define MQTT_SCH "tcp"
# define MQTT_PORT 1883
#endif

#if !defined(S_TOP_NAME)
#define S_TOP_NAME "krs/cmd/stg/"
#endif
#if !defined(P_TOP_NAME)
#define P_TOP_NAME "krs.tel.gts."
#endif

#if !defined(PX_TOP_NAME)
#define PX_TOP_NAME "krs.api.gts."
#endif
#if !defined(SX_TOP_NAME)
#define SX_TOP_NAME "krs/api/stg/"
#endif

#if !defined(MQTT_COMMAND_ADDR)
# if defined(DEV_ENV)
#  define MQTT_COMMAND_ADDR "pgsdev01.arrowconnect.io"
# else
#  define MQTT_COMMAND_ADDR "mqtt-a01.arrowconnect.io"
# endif
#endif

#if !defined(VHOST)
# if defined(DEV_ENV)
#  define VHOST "/themis.dev:"
# else
#  define VHOST "/pegasus:"
# endif
#endif

#if defined(__IBM__)
#  define MQTT_TELEMETRY_ADDR ".messaging.internetofthings.ibmcloud.com"
#elif defined(__AZURE__)
#  define MQTT_TELEMETRY_ADDR "pgshubdev01.azure-devices.net"
#  define VHOST "iothubowner"
#else
# if !defined(MQTT_TELEMETRY_ADDR)
#  define MQTT_TELEMETRY_ADDR MQTT_COMMAND_ADDR
# endif
#endif



//#define MQTTCLIENT_PLATFORM_HEADER network.h

#define ARROW_MQTT_URL MQTT_SCH "://" MQTT_ADDR ":" #MQTT_PORT

#if !defined(MQTT_QOS)
#define MQTT_QOS        1
#endif
#if !defined(MQTT_RETAINED)
#define MQTT_RETAINED   0
#endif
#if !defined(MQTT_DUP)
#define MQTT_DUP        0
#endif

#endif // ACN_SDK_C_MQTT_CONFIG_H_
