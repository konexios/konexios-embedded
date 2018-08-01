/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow/mqtt.h>
#include <arrow/sign.h>
#include <debug.h>

#define USE_STATIC
#include <data/chunk.h>

#define USERNAME_LEN 80
#define S_TOP_LEN sizeof(S_TOP_NAME) + 66
#define P_TOP_LEN sizeof(P_TOP_NAME) + 66

typedef struct _iot_args {
    arrow_gateway_t *gateway;
} iot_args;

static int mqtt_common_init_iot(
        mqtt_env_t *env,
        i_args *args) {
    CREATE_CHUNK(username, USERNAME_LEN);

    int ret = snprintf(username,
                       USERNAME_LEN,
                       VHOST "%s",
                       P_VALUE(args->gateway->hid));
    if ( ret < 0 ) return -1;
    username[ret] = 0x0;
    property_copy(&env->username, p_stack(username));
    DBG("qmtt.username %s", username);

    env->data.clientID.cstring = P_VALUE(args->gateway->hid);
    env->data.username.cstring = P_VALUE(env->username);
    env->data.password.cstring = (char*)get_api_key();
    property_copy(&env->addr, p_const(MQTT_COMMAND_ADDR));
    FREE_CHUNK(username);

    return 0;
}

#if defined(HTTP_VIA_MQTT)
static int mqtt_api_publish_init_iot(
        mqtt_env_t *env,
        i_args *args) {
  CREATE_CHUNK(p_topic, P_TOP_LEN);

  int ret = snprintf(p_topic,
                     P_TOP_LEN,
                     "%s%s",
                     PX_TOP_NAME,
                     P_VALUE(args->gateway->hid));
  if ( ret < 0 ) return -1;
  p_topic[ret] = 0x0;
  property_copy(&env->p_api_topic, p_stack(p_topic));

  FREE_CHUNK(p_topic);
  return 0;
}

static int mqtt_api_subscribe_init_iot(
        mqtt_env_t *env,
        i_args *args) {
    CREATE_CHUNK(s_topic, S_TOP_LEN);
    // API topic
    int ret = snprintf(s_topic,
                       S_TOP_LEN,
                       "%s%s",
                       SX_TOP_NAME,
                       P_VALUE(args->gateway->hid));
    if ( ret < 0 ) return -1;
    s_topic[ret] = 0x0;
    property_copy(&env->s_api_topic, p_stack(s_topic));
    DBG("sub %s",  s_topic);

    FREE_CHUNK(s_topic);
    return 0;
}
#endif

static int mqtt_telemetry_init_iot(
        mqtt_env_t *env,
        i_args *args) {
  CREATE_CHUNK(p_topic, P_TOP_LEN);

  int ret = snprintf(p_topic,
                     P_TOP_LEN,
                     "%s%s",
                     P_TOP_NAME,
                     P_VALUE(args->gateway->hid));
  if ( ret < 0 ) return -1;
  p_topic[ret] = 0x0;
  property_copy(&env->p_topic, p_stack(p_topic));

  FREE_CHUNK(p_topic);
  return 0;
}

static int mqtt_subscribe_init_iot(
        mqtt_env_t *env,
        i_args *args) {
    CREATE_CHUNK(s_topic, S_TOP_LEN);
    int ret = snprintf(s_topic,
                       S_TOP_LEN,
                       "%s%s",
                       S_TOP_NAME,
                       P_VALUE(args->gateway->hid));
    if ( ret < 0 ) return -1;
    s_topic[ret] = 0x0;
    property_copy(&env->s_topic, p_stack(s_topic));
    DBG("sub %s",  s_topic);

    FREE_CHUNK(s_topic);
    return 0;
}

mqtt_driver_t iot_driver = {
#if defined(HTTP_VIA_MQTT)
    mqtt_api_publish_init_iot,
    mqtt_api_subscribe_init_iot,
#endif
    mqtt_telemetry_init_iot,
    mqtt_subscribe_init_iot,
    mqtt_common_init_iot
};
