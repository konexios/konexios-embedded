/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_MQTT_CLIENT_DELIVERY_H_)
#define ACN_SDK_C_MQTT_CLIENT_DELIVERY_H_

#include <MQTTClient.h>
#include <data/property.h>
#include <data/linkedlist.h>

typedef int(*arrow_mqtt_delivery_init)(int);
typedef int(*arrow_mqtt_delivery_process)(const char *,int);
typedef int(*arrow_mqtt_delivery_close)(void);

typedef struct arrow_mqtt_delivery_callback_ {
    property_t topic;
    arrow_mqtt_delivery_init init;
    arrow_mqtt_delivery_process process;
    arrow_mqtt_delivery_close done;
    arrow_linked_list_head_node;
} arrow_mqtt_delivery_callback_t;

int arrow_mqtt_client_delivery_message_reg(arrow_mqtt_delivery_callback_t *dc);
int arrow_mqtt_client_delivery_message_init(MQTTClient *c, MQTTString *topicName, MQTTMessage *message);
int arrow_mqtt_client_delivery_message_process(MQTTClient *c, MQTTString *topicName, MQTTMessage *message);
int arrow_mqtt_client_delivery_message_done(MQTTClient *c, MQTTString *topicName, MQTTMessage *message);

#endif
