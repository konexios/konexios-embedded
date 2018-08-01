/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/mqtt.h"
#include <config.h>
#include <mqtt/client/client.h>
#include <arrow/telemetry.h>
#include <data/property.h>
#include <arrow/events.h>
#include <debug.h>

#define MQTT_DBG(...)

#define USE_STATIC
#include <data/chunk.h>

#if defined(NO_EVENTS)
  #define MQTT_CLEAN_SESSION 1
#else
  #define MQTT_CLEAN_SESSION 0
#endif

static mqtt_env_t *__mqtt_channels = NULL;

static arrow_mqtt_delivery_callback_t base_event_callbacks = {
    {0},
    process_event_init,
    process_event,
    process_event_finish,
    {NULL}
};

static arrow_mqtt_delivery_callback_t http_event_callbacks = {
    {0},
    process_http_init,
    process_http,
    process_http_finish,
    {NULL}
};

#if defined(STATIC_MQTT_ENV)
  static uint8_t telemetry_recvbuf[MQTT_RECVBUF_LEN + 1];
  static uint8_t telemetry_buf[MQTT_BUF_LEN + 1];
  static mqtt_env_t static_telemetry_channel;
  #if !defined(NO_EVENTS)
    #if defined(MQTT_TWO_CHANNEL)
      static uint8_t command_recvbuf[MQTT_RECVBUF_LEN + 1];
      static uint8_t command_buf[MQTT_BUF_LEN + 1];
      static mqtt_env_t static_command_channel;
      static mqtt_env_t *static_command_channel_ptr = &static_command_channel;
    #else
      static mqtt_env_t *static_command_channel_ptr = &static_telemetry_channel;
      static uint8_t *command_recvbuf = telemetry_recvbuf;
      static uint8_t *command_buf = telemetry_buf;
    #endif
  #endif
#endif

extern mqtt_driver_t iot_driver;
#if defined(__IBM__)
  extern mqtt_driver_t ibm_driver;
#endif
#if defined(__AZURE__)
  extern mqtt_driver_t azure_driver;
#endif

static void data_prep(MQTTPacket_connectData *data) {
  MQTTPacket_connectData d = MQTTPacket_connectData_initializer;
  *data = d;
  data->cleansession = MQTT_CLEAN_SESSION;
  data->keepAliveInterval = 10;
}

static int _mqtt_init_common(mqtt_env_t *env) {
  property_init(&env->p_topic);
  property_init(&env->s_topic);
#if defined(HTTP_VIA_MQTT)
  property_init(&env->p_api_topic);
  property_init(&env->s_api_topic);
#endif
  property_init(&env->username);
  property_init(&env->addr);
  data_prep(&env->data);
#if !defined(STATIC_MQTT_ENV)
    env->buf.size = MQTT_BUF_LEN;
    env->buf.buf = (unsigned char*)malloc(env->buf.size+1);
    env->readbuf.size = MQTT_RECVBUF_LEN;
    env->readbuf.buf = (unsigned char*)malloc(env->readbuf.size+1);
#endif
  env->timeout = DEFAULT_MQTT_TIMEOUT;
  env->port = MQTT_PORT;
  env->init = 0;
  arrow_linked_list_init(env);
  return 0;
}

static int _mqtt_deinit_common(mqtt_env_t *env) {
#if defined(MQTT_TASK)
  arrow_mutex_deinit(env->client.mutex);
#endif
  property_free(&env->p_topic);
  property_free(&env->s_topic);
#if defined(HTTP_VIA_MQTT)
  property_free(&env->p_api_topic);
  property_free(&env->s_api_topic);
#endif
  property_free(&env->username);
  property_free(&env->addr);
#if !defined(STATIC_MQTT_ENV)
  free(env->buf.buf);
  free(env->readbuf.buf);
#endif
  env->buf.size = 0;
  env->readbuf.size = 0;
  return 0;
}

static int _mqtt_env_is_init(mqtt_env_t *env, uint32_t init_mask) {
  int ret = 0;
  if ( env->init & init_mask ) {
    ret = 1;
  }
  return ret;
}

static void _mqtt_env_set_init(mqtt_env_t *env, uint32_t init_mask) {
  env->init |= init_mask;
}

static void _mqtt_env_unset_init(mqtt_env_t *env, uint32_t init_mask) {
  env->init &= ~init_mask;
}

static int _mqtt_env_connect(mqtt_env_t *env) {
  int ret = -1;
  NetworkInit(&env->net);
  ret = NetworkConnect(&env->net,
                       P_VALUE(env->addr),
                       env->port);
  DBG("MQTT Connecting %s %d", P_VALUE(env->addr), env->port);
  if ( ret < 0 ) {
    DBG("MQTT Connecting fail %s %d [%d]",
        P_VALUE(env->addr),
        env->port,
        ret);
    return -1;
  }
  MQTTClientInit(&env->client,
                 &env->net,
                 env->timeout,
                 env->buf.buf, env->buf.size,
                 env->readbuf.buf, env->readbuf.size);
  ret = MQTTConnect(&env->client, &env->data);
  if ( ret != MQTT_SUCCESS ) {
    DBG("MQTT Connect fail %d", ret);
    NetworkDisconnect(&env->net);
    return -1;
  }
  _mqtt_env_set_init(env, MQTT_CLIENT_INIT);
  return 0;
}

static int _mqtt_env_close(mqtt_env_t *env) {
  MQTTDisconnect(&env->client);
  NetworkDisconnect(&env->net);
  _mqtt_env_unset_init(env, MQTT_CLIENT_INIT);
  _mqtt_env_unset_init(env, MQTT_COMMANDS_INIT);
  return 0;
}

static int _mqtt_env_free(mqtt_env_t *env) {
  if ( _mqtt_env_is_init(env, MQTT_CLIENT_INIT) ) {
    _mqtt_env_close(env);
  }
  _mqtt_deinit_common(env);
  _mqtt_env_unset_init(env, 0xffffffff);
  return 0;
}

static int mqttchannelseq( mqtt_env_t *ch, uint32_t num ) {
  if ( ch->mask == num ) {
    return 0;
  }
  return -1;
}

static uint32_t get_telemetry_mask() {
  int mqttmask = ACN_num;
#if defined(__IBM__)
  mqttmask = IBM_num;
#elif defined(__AZURE__)
  mqttmask = Azure_num;
#endif
  return mqttmask;
}

static mqtt_env_t *get_telemetry_env() {
  int mqttmask = get_telemetry_mask();
  mqtt_env_t *tmp = NULL;
  linked_list_find_node(tmp,
                        __mqtt_channels,
                        mqtt_env_t,
                        mqttchannelseq,
                        mqttmask );
  if ( !tmp ) {
#if defined(STATIC_MQTT_ENV)
    tmp = &static_telemetry_channel;
#else
    tmp = (mqtt_env_t *)calloc(1, sizeof(mqtt_env_t));
#endif
    _mqtt_init_common(tmp);
#if defined(STATIC_MQTT_ENV)
    tmp->buf.size = sizeof(telemetry_buf) - 1;
    tmp->buf.buf = (unsigned char *)telemetry_buf;
    tmp->readbuf.size = sizeof(telemetry_recvbuf) - 1;
    tmp->readbuf.buf = (unsigned char *)telemetry_recvbuf;
#endif
    tmp->mask = mqttmask;
    arrow_linked_list_add_node_last(__mqtt_channels,
                                    mqtt_env_t,
                                    tmp);
  }
  return tmp;
}

static mqtt_driver_t *get_telemetry_driver(uint32_t mqttmask) {
  mqtt_driver_t *drv = NULL;
  switch(mqttmask) {
    case ACN_num:
      drv = &iot_driver;
      break;
#if defined(__IBM__)
    case IBM_num:
      drv = &ibm_driver;
      break;
#endif
#if defined(__AZURE__)
    case Azure_num:
      drv = &azure_driver;
      break;
#endif
    default:
      drv = NULL;
  }
  return drv;
}

int mqtt_telemetry_connect(arrow_gateway_t *gateway,
                           arrow_device_t *device,
                           arrow_gateway_config_t *config) {
  int mqttmask = get_telemetry_mask();
  mqtt_driver_t *drv = get_telemetry_driver(mqttmask);
  i_args args = { device, gateway, config };
  mqtt_env_t *tmp = get_telemetry_env();
  if ( !tmp ) {
    DBG("Telemetry memory error");
    return -1;
  }
  if ( ! _mqtt_env_is_init(tmp, MQTT_COMMON_INIT) ) {
    drv->common_init(tmp, &args);
    _mqtt_env_set_init(tmp, MQTT_COMMON_INIT);
  }
  if ( ! _mqtt_env_is_init(tmp, MQTT_TELEMETRY_INIT) ) {
    int ret = drv->telemetry_init(tmp, &args);
    if ( ret < 0 ) {
      DBG("MQTT telemetry setting fail");
      return ret;
    }
#if defined(HTTP_VIA_MQTT)
    ret = drv->api_publish_init(tmp, &args);
    if ( ret < 0 ) {
      DBG("MQTT API publish setting fail");
      return ret;
    }
#endif
    _mqtt_env_set_init(tmp, MQTT_TELEMETRY_INIT);
  }
  if ( !_mqtt_env_is_init(tmp, MQTT_CLIENT_INIT) ) {
    int ret = _mqtt_env_connect(tmp);
    if ( ret < 0 ) {
      return ret;
    }
  }
  return 0;
}

int mqtt_telemetry_disconnect(void) {
  mqtt_env_t *tmp = get_telemetry_env();
  if ( !tmp ) {
    return -1;
  }
  if ( ! _mqtt_env_is_init(tmp,  MQTT_SUBSCRIBE_INIT) ) {
    _mqtt_env_close(tmp);
  } else {
    _mqtt_env_unset_init(tmp, MQTT_TELEMETRY_INIT);
  }
  return 0;
}

int mqtt_telemetry_terminate(void) {
  mqtt_env_t *tmp = get_telemetry_env();
  if ( !tmp ) {
    return -1;
  }
  if ( ! _mqtt_env_is_init(tmp,  MQTT_SUBSCRIBE_INIT) ) {
    arrow_linked_list_del_node(__mqtt_channels, mqtt_env_t, tmp);
    _mqtt_env_free(tmp);
#if !defined(STATIC_MQTT_ENV)
    free(tmp);
#endif
  } else {
    _mqtt_env_unset_init(tmp, MQTT_TELEMETRY_INIT);
  }
  return 0;
}

static JsonNode *mqtt_pub_pay = NULL;
static json_encode_machine_t em;


int p_init() {
    int len = json_size(mqtt_pub_pay);
    if ( json_encode_init(&em, mqtt_pub_pay) < 0 ) {
        return -1;
    }
    return len;
}

int p_part(char *ptr, int len) {
    int r = json_encode_part(&em, ptr, len);
    return r;
}

int p_fin() {
    int r = json_encode_fin(&em);
    return r;
}

mqtt_payload_drive_t mqtt_json_drive = {
    p_init,
    p_part,
    p_fin
};

int mqtt_publish(arrow_device_t *device, void *d) {
    MQTTMessage msg = {MQTT_QOS, MQTT_RETAINED, MQTT_DUP, 0, NULL, 0};
    int ret = -1;
    mqtt_env_t *tmp = get_telemetry_env();
    if ( tmp ) {
        mqtt_pub_pay = telemetry_serialize_json(device, d);
        ret = MQTTPublish_part(&tmp->client,
                               P_VALUE(tmp->p_topic),
                               &msg,
                               &mqtt_json_drive);
        json_delete(mqtt_pub_pay);
    }
    return ret;
}

#if defined(HTTP_VIA_MQTT)
int mqtt_api_publish(JsonNode *data) {
  MQTTMessage msg = {MQTT_QOS, MQTT_RETAINED, MQTT_DUP, 0, NULL, 0};
  int ret = -1;
  mqtt_env_t *tmp = get_telemetry_env();
  if ( tmp ) {
    if ( !data ) {
      return -1;
    }
    mqtt_pub_pay = data;
    msg.payload = (void *)data;
    msg.payloadlen = json_size(data);
    MQTT_DBG("[%d][%s]", msg.payloadlen, msg.payload);
    ret = MQTTPublish_part(&tmp->client,
                      P_VALUE(tmp->p_api_topic),
                      &msg,
                           &mqtt_json_drive);
  }
  return ret;
}
#endif

int mqtt_is_telemetry_connect(void) {
  mqtt_env_t *tmp = get_telemetry_env();
  if ( tmp ) {
    if ( _mqtt_env_is_init(tmp, MQTT_CLIENT_INIT ) &&
         _mqtt_env_is_init(tmp, MQTT_TELEMETRY_INIT) ) {
      return 1;
    }
  }
  return 0;
}

void mqtt_disconnect(void) {
  mqtt_env_t *curr = NULL;
  arrow_linked_list_for_each ( curr, __mqtt_channels, mqtt_env_t ) {
    if ( _mqtt_env_is_init(curr, MQTT_CLIENT_INIT) ) {
      _mqtt_env_close(curr);
    }
  }
}

void mqtt_terminate(void) {
  mqtt_env_t *curr = NULL;
  arrow_linked_list_for_each_safe ( curr, __mqtt_channels, mqtt_env_t ) {
    if ( curr->init ) {
      _mqtt_env_free(curr);
    }
#if !defined(STATIC_MQTT_ENV)
    free(curr);
#endif
  }
  __mqtt_channels = NULL;
}

#if !defined(NO_EVENTS)
static mqtt_env_t *get_event_env() {
  mqtt_env_t *tmp = NULL;
  linked_list_find_node(tmp,
                        __mqtt_channels,
                        mqtt_env_t,
                        mqttchannelseq,
                        ACN_num );
  if ( !tmp ) {
#if defined(STATIC_MQTT_ENV)
    tmp = static_command_channel_ptr;
#else
    tmp = (mqtt_env_t *)calloc(1, sizeof(mqtt_env_t));
#endif
    _mqtt_init_common(tmp);
#if defined(STATIC_MQTT_ENV)
    tmp->buf.size = MQTT_BUF_LEN;
    tmp->buf.buf = (unsigned char *)command_buf;
    tmp->readbuf.size = MQTT_RECVBUF_LEN;
    tmp->readbuf.buf = (unsigned char *)command_recvbuf;
#endif
    tmp->mask = ACN_num;
    arrow_linked_list_add_node_last(__mqtt_channels,
                                    mqtt_env_t,
                                    tmp);
  }
  return tmp;
}

int mqtt_subscribe_connect(arrow_gateway_t *gateway,
                           arrow_device_t *device,
                           arrow_gateway_config_t *config) {
  mqtt_driver_t *drv = &iot_driver;
  i_args args = { device, gateway, config };
  mqtt_env_t *tmp = get_event_env();
  if ( !tmp ) {
    return -1;
  }
  if ( ! _mqtt_env_is_init(tmp, MQTT_COMMON_INIT) ) {
    drv->common_init(tmp, &args);
    _mqtt_env_set_init(tmp, MQTT_COMMON_INIT);
  }
  if ( ! _mqtt_env_is_init(tmp, MQTT_SUBSCRIBE_INIT) ) {
    int ret = drv->commands_init(tmp, &args);
    if ( ret < 0 ) {
      DBG("MQTT subscribe setting fail");
      return ret;
    }
#if defined(HTTP_VIA_MQTT)
    ret = drv->api_subscribe_init(tmp, &args);
    if ( ret < 0 ) {
      DBG("MQTT API subscribe setting fail");
      return ret;
    }
#endif
    _mqtt_env_set_init(tmp, MQTT_SUBSCRIBE_INIT);
  }
  property_weak_copy(&base_event_callbacks.topic, tmp->s_topic);
  if ( arrow_mqtt_client_delivery_message_reg(&base_event_callbacks) < 0 ) {
      DBG("MQTT handler fail");
  }

#if defined(HTTP_VIA_MQTT)
  property_weak_copy(&http_event_callbacks.topic, tmp->s_api_topic);
  if ( arrow_mqtt_client_delivery_message_reg(&http_event_callbacks) < 0 ) {
      DBG("MQTT API handler fail");
  }
#endif

  if ( ! _mqtt_env_is_init(tmp, MQTT_CLIENT_INIT) ) {
    int ret = _mqtt_env_connect(tmp);
    if ( ret < 0 ) {
      return -1;
    }
  }
  return 0;
}

int mqtt_subscribe_disconnect(void) {
  mqtt_env_t *tmp = get_event_env();
  if ( !tmp ) {
    DBG("There is no sub channel");
    return -1;
  }
  if ( ! _mqtt_env_is_init(tmp, MQTT_TELEMETRY_INIT) ) {
    _mqtt_env_close(tmp);
  } else {
    _mqtt_env_unset_init(tmp, MQTT_SUBSCRIBE_INIT);
  }
  return 0;
}

int mqtt_subscribe_terminate(void) {
  mqtt_env_t *tmp = get_event_env();
  if ( !tmp ) {
    return -1;
  }
  if ( ! _mqtt_env_is_init(tmp, MQTT_TELEMETRY_INIT) ) {
    arrow_linked_list_del_node(__mqtt_channels, mqtt_env_t, tmp);
    _mqtt_env_free(tmp);
#if !defined(STATIC_MQTT_ENV)
    free(tmp);
#endif
  } else {
    _mqtt_env_unset_init(tmp, MQTT_SUBSCRIBE_INIT);
  }
  return 0;
}

int mqtt_subscribe(void) {
  mqtt_env_t *tmp = get_event_env();
  if ( tmp &&
       _mqtt_env_is_init(tmp, MQTT_SUBSCRIBE_INIT) &&
       !_mqtt_env_is_init(tmp, MQTT_COMMANDS_INIT) ) {

    DBG("Subscribing to %s", P_VALUE(tmp->s_topic));
    int rc = arrow_mqtt_client_subscribe(&tmp->client,
                           QOS2,
                           &base_event_callbacks);
    if ( rc < 0 ) {
      DBG("Subscribe failed %d\n", rc);
      return rc;
    }

#if defined(HTTP_VIA_MQTT)
    DBG("Subscribing API to %s", P_VALUE(tmp->s_api_topic));
    rc = arrow_mqtt_client_subscribe(&tmp->client,
                               QOS2,
                               &http_event_callbacks);
    if ( rc < 0 ) {
      DBG("Subscribe API failed %d\n", rc);
    }
#endif
    _mqtt_env_set_init(tmp, MQTT_COMMANDS_INIT);
  }
  return 0;
}

int mqtt_is_subscribe_connect(void) {
  mqtt_env_t *tmp = get_event_env();
  if ( tmp ) {
    if ( _mqtt_env_is_init(tmp, MQTT_CLIENT_INIT) ) {
      return 1;
    }
  }
  return 0;
}
#endif

int mqtt_yield(int timeout_ms) {
#if !defined(NO_EVENTS)
  int ret = -1;
  mqtt_env_t *tmp = get_event_env();
  if ( tmp &&
       _mqtt_env_is_init(tmp, MQTT_SUBSCRIBE_INIT) &&
       _mqtt_env_is_init(tmp, MQTT_CLIENT_INIT) ) {
    ret = MQTTYield(&tmp->client, timeout_ms);
    return ret;
  }
  return -1;
#else
  msleep(timeout_ms);
  return 0;
#endif
}

#if !defined(NO_EVENTS)
int mqtt_receive(int timeout_ms) {
  TimerInterval timer;
  TimerInit(&timer);
  TimerCountdownMS(&timer, (unsigned int)timeout_ms);

  int ret = -1;
  mqtt_env_t *tmp = get_event_env();
  if ( tmp &&
       _mqtt_env_is_init(tmp, MQTT_SUBSCRIBE_INIT) &&
       _mqtt_env_is_init(tmp, MQTT_CLIENT_INIT) ) {
    do {
      ret = MQTTYield(&tmp->client, TimerLeftMS(&timer));
    } while (!TimerIsExpired(&timer));
    return ret;
  }
  return -1;
}
#endif
