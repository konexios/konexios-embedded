/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/events.h"

#if !defined(NO_EVENTS)

#include <arrow/device_command.h>
#include <arrow/state.h>

#if !defined(NO_RELEASE_UPDATE)
#include <arrow/software_release.h>
#endif

#if !defined(NO_SOFTWARE_UPDATE)
# include <arrow/software_update.h>
#endif

#if defined(DEVICE_STARTSTOP)
#include <arrow/device_startstop.h>
#endif

#include <debug.h>
#include <http/http_client.h>
#include <json/json.h>
#include <sys/mem.h>
#include <arrow/gateway_payload_sign.h>

#if defined(STATIC_MQTT_ENV)
#include <data/static_alloc.h>
static_object_pool_type(mqtt_event_t, ARROW_MAX_MQTT_COMMANDS)
static_object_pool_type(mqtt_api_event_t, ARROW_MAX_MQTT_COMMANDS)
#endif

#if defined(ARROW_THREAD)
#include <sys/mutex.h>
#define MQTT_EVENTS_QUEUE_LOCK      arrow_mutex_lock(_event_mutex)
#define MQTT_EVENTS_QUEUE_UNLOCK    arrow_mutex_unlock(_event_mutex)
#else
#define MQTT_EVENTS_QUEUE_LOCK
#define MQTT_EVENTS_QUEUE_UNLOCK
#endif

static void mqtt_event_base_init(mqtt_event_base_t *mq) {
    property_init(&mq->id);
    property_init(&mq->name);
    mq->encrypted = 0;
    mq->parameters = NULL;
}

static void mqtt_event_base_free(mqtt_event_base_t *mq) {
    property_free(&mq->id);
    property_free(&mq->name);
    if ( mq->parameters ) json_delete(mq->parameters);
}

static void mqtt_event_init(mqtt_event_t *mq) {
    mqtt_event_base_init(&mq->base);
    property_init(&mq->device_hid);
    arrow_linked_list_init(mq);
}

static void mqtt_event_free(mqtt_event_t *mq) {
  mqtt_event_base_free(&mq->base);
  property_free(&mq->device_hid);
}

static void mqtt_api_event_init(mqtt_api_event_t *mq) {
    mqtt_event_base_init(&mq->base);
    arrow_linked_list_init(mq);
}

static void mqtt_api_event_free(mqtt_api_event_t *mq) {
    mqtt_event_base_free(&mq->base);
}

typedef int (*submodule)(void *, JsonNode *);
typedef void (*module_init)();
typedef void (*module_deinit)();
typedef struct {
  char *name;
  submodule proc;
  module_init init;
  module_deinit deinit;
} sub_t;

sub_t sub_list[] = {
  { "ServerToGateway_DeviceCommand", ev_DeviceCommand, NULL, arrow_command_handler_free },
  { "SendCommand", ev_DeviceCommand, NULL, NULL },
  { "ServerToGateway_DeviceStateRequest", ev_DeviceStateRequest, NULL, NULL },
#if !defined(NO_SOFTWARE_UPDATE)
  { "ServerToGateway_GatewaySoftwareUpdate", ev_GatewaySoftwareUpdate, NULL, NULL },
#endif
#if !defined(NO_RELEASE_UPDATE)
  { "ServerToGateway_DeviceSoftwareRelease", ev_DeviceSoftwareRelease, NULL, NULL },
  { "ServerToGateway_GatewaySoftwareRelease", ev_DeviceSoftwareRelease, NULL, NULL },
#endif
#if defined(DEVICE_STARTSTOP)
    { "ServerToGateway_DeviceStart", ev_DeviceSoftwareRelease, NULL, NULL },
    { "ServerToGateway_DeviceStop", ev_DeviceSoftwareRelease, NULL, NULL },
#endif
    { NULL, NULL, NULL, NULL }
};

// checker

typedef int(*sign_checker)(const char *,
                           property_t hid,
                           property_t name,
                           int encrypted,
                           JsonNode *parameters);
struct check_signature_t {
  const char *version;
  sign_checker check;
};

static int check_sign_1(const char *sign,
                        property_t hid,
                        property_t name,
                        int encrypted,
                        JsonNode *parameters) {
  char signature[65] = {0};
  int err = arrow_event_sign(signature,
                             hid,
                             P_VALUE(name),
                             encrypted,
                             parameters);

  if ( !err && strcmp(sign, signature) == 0 ) {
      return 0;
  }

  DBG("sing check failed { %s, %s }", sign, signature);
  return -1;
}

static struct check_signature_t checker_collection[] = {
  {"1", check_sign_1},
};

static int check_signature(const char *vers, const char *sing, mqtt_event_base_t *ev) {
  unsigned int i = 0;
  for ( i = 0; i< sizeof(checker_collection) / sizeof(struct check_signature_t); i++ ) {
    if ( strcmp(vers, checker_collection[i].version ) == 0 ) {
      DBG("check version %s", checker_collection[i].version);
      return checker_collection[i].check(
                  sing,
                  ev->id,
                  ev->name,
                  ev->encrypted,
                  ev->parameters);
    }
  }
  return -1;
}

static mqtt_event_t *__event_queue = NULL;
static mqtt_api_event_t *__api_event_queue = NULL;
#if defined(ARROW_THREAD)
static arrow_mutex *_event_mutex = NULL;
#endif

void arrow_mqtt_events_init(void) {
#if defined(ARROW_THREAD)
    arrow_mutex_init(&_event_mutex);
#endif
    int i = 0;
    for (i=0; i < (int)(sizeof(sub_list)/sizeof(sub_t)); i++) {
      if ( sub_list[i].init ) sub_list[i].init();
    }
}

int arrow_mqtt_has_events(void) {
    int ret = -1;
    MQTT_EVENTS_QUEUE_LOCK;
    if ( !__event_queue ) return 0;
    ret = 0;
    mqtt_event_t *tmp = NULL;
    arrow_linked_list_for_each(tmp, __event_queue, mqtt_event_t) {
        ret++;
    }
    MQTT_EVENTS_QUEUE_UNLOCK;
    return ret;
}

int arrow_mqtt_event_proc(void) {
    mqtt_event_t *tmp = NULL;
    tmp = __event_queue;
    if ( !tmp ) {
        return -1;
    }

    submodule current_processor = NULL;
    int i = 0;
    for (i=0; i < (int)(sizeof(sub_list)/sizeof(sub_t)); i++) {
      if ( sub_list[i].name && strcmp(sub_list[i].name, P_VALUE(tmp->base.name)) == 0 ) {
        current_processor = sub_list[i].proc;
      }
    }
    int ret = -1;
    if ( current_processor ) {
      ret = current_processor(tmp, tmp->base.parameters);
    } else {
      DBG("No event processor for %s", P_VALUE(tmp->base.name));
      goto mqtt_event_proc_error;
    }
mqtt_event_proc_error:
    MQTT_EVENTS_QUEUE_LOCK;
    arrow_linked_list_del_node_first(__event_queue, mqtt_event_t);
    MQTT_EVENTS_QUEUE_UNLOCK;
    mqtt_event_free(tmp);
#if defined(STATIC_MQTT_ENV)
    static_free(mqtt_event_t, tmp);
#else
    free(tmp);
#endif
    if ( __event_queue && !ret ) ret = 1;
    return ret;
}

int arrow_mqtt_api_has_events(void) {
    int ret = -1;
    MQTT_EVENTS_QUEUE_LOCK;
    if ( !__api_event_queue ) return 0;
    ret = 0;
    mqtt_api_event_t *tmp = NULL;
    arrow_linked_list_for_each(tmp, __api_event_queue, mqtt_api_event_t) {
        ret++;
    }
    MQTT_EVENTS_QUEUE_UNLOCK;
    return ret;
}

int arrow_mqtt_api_event_proc(http_response_t *res) {
    mqtt_api_event_t *tmp = NULL;
    int ret = -1;
    tmp = __api_event_queue;
    if ( !tmp ) {
        return -1;
    }

    JsonNode *_parameters = tmp->base.parameters;
    JsonNode *status = json_find_member(_parameters, p_const("status"));
    if ( !status ) {
        DBG("No HTTP status!");
        goto mqtt_api_error;
    }
    if ( strcmp(status->string_, "OK") != 0 ) {
        JsonNode *body = json_find_member(_parameters, p_const("payload"));
        if ( body ) {
            DBG("[%s]", body->string_);
        }
        DBG("Not OK");
        goto mqtt_api_error;
    }
    res->m_httpResponseCode = 200;

    JsonNode *payload = json_find_member(_parameters, p_const("payload"));
    if ( payload ) {
        http_response_add_payload(res, p_stack(payload));
    }
    ret = 0;

mqtt_api_error:
    MQTT_EVENTS_QUEUE_LOCK;
    arrow_linked_list_del_node_first(__api_event_queue, mqtt_api_event_t);
    MQTT_EVENTS_QUEUE_UNLOCK;
    mqtt_api_event_free(tmp);
#if defined(STATIC_MQTT_ENV)
    static_free(mqtt_api_event_t, tmp);
#else
    free(tmp);
#endif
    if ( __api_event_queue ) ret = 1;
    return ret;
}

static json_parse_machine_t sm_http;
static int api_mqtt_max_capacity = 0;


int arrow_mqtt_api_wait(int num) {
    api_mqtt_max_capacity = num;
    while ( arrow_mqtt_api_has_events() > api_mqtt_max_capacity ) {
        http_response_t dummy;
        arrow_mqtt_api_event_proc(&dummy);
    }
    return 0;
}


int process_http_init(int size) {
#if defined(ARROW_MAX_MQTT_COMMANDS)
    if (arrow_mqtt_api_has_events() >= api_mqtt_max_capacity)
        return -1;
#endif
    DBG("Static http memory size %d", json_static_memory_max_sector());
    DBG("need %d", size);
#if defined(STATIC_ACN)
    if ( size*(1) > json_static_memory_max_sector() - 512 ) {
        DBG("Not enough mem %d/%d", size, json_static_memory_max_sector());
        return -1;
    }
#endif
    return json_decode_init(&sm_http);
}

int process_http(const char *str, int len) {
    int r = json_decode_part(&sm_http, str, len);
    if ( r != len ) {
        DBG("http decode fail %d/%d", r, len);
        JsonNode *root = json_decode_finish(&sm_http);
        if ( root ) json_delete(root);
        return -1;
    }
    return 0;
}

static int get_event_base(mqtt_event_base_t *base,
                          JsonNode *_main,
                          property_t id,
                          property_t name) {
    if ( fill_string_from_json(_main,
                               id,
                               &base->id) < 0 ) {
      DBG("cannot find requestId");
      return -1;
    }

    if ( fill_string_from_json(_main,
                               name,
                               &base->name) < 0 ) {
      DBG("cannot find name");
      return -1;
    }

    JsonNode *_encrypted = json_find_member(_main,
                                            p_const("encrypted"));
    if ( !_encrypted ) return -1;
    base->encrypted = _encrypted->bool_;

    JsonNode *_parameters = json_find_member(_main,
                                             p_const("parameters"));
    if ( !_parameters ) return -1;
    json_remove_from_parent(_parameters);
    base->parameters = _parameters;
    return 0;
}

static int mqtt_event_sign_checker(JsonNode *_main, mqtt_event_base_t *base) {
    JsonNode *sign_version = json_find_member(_main, p_const("signatureVersion"));
    if ( sign_version ) {
  #if defined(DEBUG_MQTT_PROCESS_EVENT)
        DBG("signature vertsion: %s", sign_version->string_);
  #endif
      JsonNode *sign = json_find_member(_main, p_const("signature"));
      if ( !sign ) return -1;

      if ( check_signature(
               sign_version->string_,
               sign->string_,
               base ) < 0 ) {
        DBG("Alarm! signature is failed...");
        return -1;
      }
    }
    return 0;
}

int process_http_finish() {
    int ret = -1;
    DBG("start http msg processing");
    JsonNode *_main = json_decode_finish(&sm_http);
    if ( !_main ) {
        DBG("http payload decode failed");
        goto no_api_error;
    }

#if defined(STATIC_MQTT_ENV)
    mqtt_api_event_t *api_e = static_allocator(mqtt_api_event_t);
#else
  mqtt_api_event_t *api_e = alloc_type(mqtt_api_event_t);
#endif
  if ( !api_e ) {
      DBG("PROCESS API EVENT: not enough memory");
      goto no_api_error;
  }
  mqtt_api_event_init(api_e);

  if ( get_event_base(&api_e->base,
                      _main,
                      p_const("requestId"),
                      p_const("eventName") ) < 0 ) {
      DBG("MQTT base pack failed");
      goto error;
  }

    if ( mqtt_event_sign_checker(_main, &api_e->base) < 0 ) {
        goto error;
    }

    arrow_linked_list_add_node_last(__api_event_queue, mqtt_api_event_t, api_e);
    DBG("http queue size %d", arrow_mqtt_api_has_events());
    ret = 0;
error:
    if ( ret < 0 ) {
        mqtt_api_event_free(api_e);
  #if defined(STATIC_MQTT_ENV)
        static_free(mqtt_api_event_t, api_e);
  #else
        free(api_e);
  #endif
    }
no_api_error:
    if ( _main ) json_delete(_main);
    return ret;
}

static json_parse_machine_t sm;

int process_event_init(int size) {
#if defined(ARROW_MAX_MQTT_COMMANDS)
    if (arrow_mqtt_has_events() >= ARROW_MAX_MQTT_COMMANDS)
        return -1;
#endif
    DBG("Static memory size %d", json_static_memory_max_sector());
    DBG("need %d", size);
#if defined(STATIC_ACN)
    if ( size*(2) > json_static_memory_max_sector() - 1024 ) {
        DBG("Not enough mem %d/%d", size, json_static_memory_max_sector());
        return -1;
    }
#endif
    return json_decode_init(&sm);
}

int process_event(const char *str, int len) {
    int r = json_decode_part(&sm, str, len);
    if ( r != len ) {
        DBG("JSON decode fail %d/%d", r, len);
        JsonNode *root = json_decode_finish(&sm);
        if ( root ) json_delete(root);
        return -1;
    }
    return 0;
}


int process_event_finish() {
    int ret = -1;
    DBG("ev fin");
    JsonNode *_main = json_decode_finish(&sm);
    if ( !_main ) {
        DBG("event payload decode failed");
        goto no_event_error;
    }
#if defined(STATIC_MQTT_ENV)
    mqtt_event_t *mqtt_e = static_allocator(mqtt_event_t);
#else
  mqtt_event_t *mqtt_e = alloc_type(mqtt_event_t);
#endif
  if ( !mqtt_e ) {
      DBG("PROCESS EVENT: not enough memory");
      goto no_event_error;
  }
  mqtt_event_init(mqtt_e);

  if ( get_event_base(&mqtt_e->base,
                      _main,
                      p_const("hid"),
                      p_const("name") ) < 0 ) {
      DBG("MQTT event base pack failed");
      goto error;
  }

  if ( mqtt_event_sign_checker(_main, &mqtt_e->base) < 0 ) {
      goto error;
  }
  ret = 0;
  arrow_linked_list_add_node_last(__event_queue, mqtt_event_t, mqtt_e);

error:
  if ( ret < 0 ) {
      mqtt_event_free(mqtt_e);
#if defined(STATIC_MQTT_ENV)
      static_free(mqtt_event_t, mqtt_e);
#else
      free(mqtt_e);
#endif
  }
no_event_error:
  if ( _main ) json_delete(_main);
  return ret;
}

void arrow_mqtt_events_done() {
#if defined(ARROW_THREAD)
    arrow_mutex_deinit(_event_mutex);
#endif
    int i = 0;
    for (i=0; i < (int)(sizeof(sub_list)/sizeof(sub_t)); i++) {
      if ( sub_list[i].deinit ) sub_list[i].deinit();
    }
}
#else
typedef void __dummy;
#endif
