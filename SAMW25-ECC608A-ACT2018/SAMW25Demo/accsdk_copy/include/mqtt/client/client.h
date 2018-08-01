/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_MQTT_CLIENT_CLIENT_H_)
#define ACN_SDK_C_MQTT_CLIENT_CLIENT_H_

#include <mqtt/client/delivery.h>

#if !defined(MQTT_CLIENT_MAX_PAYLOAD_LEN)
#define MQTT_CLIENT_MAX_PAYLOAD_LEN 3000
#endif

#define MQTT_CLIENT_MAX_MSG_LEN ( MQTT_BUF_LEN + MQTT_CLIENT_MAX_PAYLOAD_LEN )

int arrow_mqtt_client_subscribe(MQTTClient *c,
                                enum QoS qos,
                                arrow_mqtt_delivery_callback_t *cb);

#endif
