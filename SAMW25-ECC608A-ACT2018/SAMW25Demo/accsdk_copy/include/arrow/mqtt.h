/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ARROW_MQTT_H_
#define ARROW_MQTT_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "gateway.h"
#include "device.h"
#include <data/linkedlist.h>
#include <MQTTClient.h>

enum _mqtt_mask_ {
    ACN_num = 1<<0,
    IBM_num = 1<<8,
    Azure_num = 1<<16
};

enum _mqtt_init {
    MQTT_COMMON_INIT = 1,
    MQTT_TELEMETRY_INIT = 1<<1,
    MQTT_SUBSCRIBE_INIT = 1<<2,
    MQTT_CLIENT_INIT    = 1<<3,
    MQTT_COMMANDS_INIT  = 1<<5
};

typedef struct _pstring {
    unsigned char *buf;
    size_t size;
} pstring;

typedef struct _mqtt_env_ {
    Network net;
    MQTTClient client;
    MQTTPacket_connectData data;
    pstring buf;
    pstring readbuf;
    property_t username;
    property_t s_topic;
    property_t p_topic;
#if defined(HTTP_VIA_MQTT)
    property_t s_api_topic;
    property_t p_api_topic;
#endif
    property_t addr;
    short port;
    short init;
    int timeout;
    uint32_t mask;
    arrow_linked_list_head_node;
} mqtt_env_t;

typedef struct _i_args {
    arrow_device_t *device;
    arrow_gateway_t *gateway;
    arrow_gateway_config_t *config;
} i_args;

typedef struct _mqtt_driver {
#if defined(HTTP_VIA_MQTT)
    int (*api_publish_init)(mqtt_env_t *env, i_args *arg);
    int (*api_subscribe_init)(mqtt_env_t *env, i_args *arg);
#endif
    int (*telemetry_init)(mqtt_env_t *env, i_args *arg);
    int (*commands_init)(mqtt_env_t *env, i_args *arg);
    int (*common_init)(mqtt_env_t *env, i_args *arg);
} mqtt_driver_t;

// Establishing MQTT connection depends used define:
// __IBM__ or __AZURE__ in private.h file
// if there is no define IoT connection will be used as default.
int mqtt_telemetry_connect(arrow_gateway_t *gateway,
                           arrow_device_t *device,
                           arrow_gateway_config_t *config);

int mqtt_is_telemetry_connect(void);

// Terminate MQTT connection
int mqtt_telemetry_disconnect(void);
int mqtt_telemetry_terminate(void);

// Send the telemetry data to the cloud
// there is extremely needed the telemetry_serialize function implementation to serealize 'data' correctly
int mqtt_publish(arrow_device_t *device, void *data);
int mqtt_api_publish(JsonNode *data);

#if !defined(NO_EVENTS)
int mqtt_subscribe_connect(arrow_gateway_t *gateway,
                           arrow_device_t *device,
                           arrow_gateway_config_t *config);

int mqtt_is_subscribe_connect(void);
int mqtt_subscribe_disconnect(void);
int mqtt_subscribe_terminate(void);

// Subscribe on MQTT events
// In this context this means that device can be controlled by
// command received from the cloud
int mqtt_subscribe(void);
#endif

// Wait some event from the cloud
// The user's command or software update command
int mqtt_yield(int timeout_ms);
int mqtt_receive(int timeout_ms);
// Terminate all connections
void mqtt_disconnect(void);
void mqtt_terminate(void);

#if defined(__cplusplus)
}
#endif

#endif /* ARROW_MQTT_H_ */
