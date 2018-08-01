/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/device_command.h"
#if !defined(NO_EVENTS)
#include <http/http_routine.h>
#include <json/json.h>
#include <sys/mem.h>
#include <arrow/events.h>
#include <time/time.h>
#include <debug.h>

#include <data/chunk.h>

#if defined(STATIC_DEVICE_COMMANDS)
# include <data/static_alloc.h>
static_object_pool_type(cmd_handler, ARROW_MAX_DEVICE_COMMANDS)
# define ALLOC static_allocator
# define FREE(p)  static_free(cmd_handler, p)
#else
# define ALLOC alloc_type
# define FREE free
#endif

static cmd_handler *__handlers = NULL;

// handlers
void arrow_command_init(void) {
    __handlers = NULL;
}

int has_cmd_handler(void) {
	if ( __handlers ) return 0;
	return -1;
}

int arrow_command_handler_add(const char *name, __cmd_cb callback) {
    cmd_handler *h = ALLOC(cmd_handler);
    if ( !h ) return -1;
    property_copy(&h->name, p_const(name));
    h->callback = callback;
    arrow_linked_list_init(h);
    arrow_linked_list_add_node_last(__handlers, cmd_handler, h);
    return 0;
}

void arrow_command_handler_free(void) {
  cmd_handler *curr = NULL;
  arrow_linked_list_for_each_safe ( curr, __handlers , cmd_handler ) {
      property_free(&curr->name);
      FREE(curr);
  }
}

// events
static void form_evetns_url(property_t hid, cmd_type ev, char *uri) {
    strcpy(uri, ARROW_API_EVENTS_ENDPOINT);
    strcat(uri, "/");
    strcat(uri, P_VALUE(hid));
    switch(ev) {
        case failed:    strcat(uri, "/failed"); break;
        case received:  strcat(uri, "/received"); break;
        case succeeded: strcat(uri, "/succeeded"); break;
    }
}

typedef struct _event_data {
    property_t hid;
	cmd_type ev;
    property_t payload;
} event_data_t;

static void _event_ans_init(http_request_t *request, void *arg) {
    CREATE_CHUNK(uri, sizeof(ARROW_API_EVENTS_ENDPOINT) + 100);
    event_data_t *data = (event_data_t *)arg;
    form_evetns_url(data->hid, data->ev, uri);
    http_request_init(request, PUT, uri);
    FREE_CHUNK(uri);
    if ( !IS_EMPTY(data->payload) ) {
      http_request_set_payload(request, data->payload);
	}
}

int arrow_send_event_ans(property_t hid, cmd_type ev, property_t payload) {
    event_data_t edata = {p_null(), ev, p_null()};
    property_weak_copy(&edata.hid, hid);
    property_weak_copy(&edata.payload, payload);
    STD_ROUTINE(_event_ans_init, &edata,
                NULL, NULL,
                "Arrow Event answer failed...");
}

static int cmdeq( cmd_handler *s, property_t name ) {
    if ( property_cmp(&s->name, &name) == 0 ) return 0;
    return -1;
}

int ev_DeviceCommand(void *_ev, JsonNode *_parameters) {
  int ret = -1;
  JsonNode *_error = NULL;
  mqtt_event_t *ev = (mqtt_event_t *)_ev;
  int retry = 0;
  http_session_close_set(current_client(), false);
#if defined(HTTP_VIA_MQTT)
  http_session_set_protocol(current_client(), 1);
#endif
  while( arrow_send_event_ans(ev->base.id, received, p_null()) < 0 ) {
      RETRY_UP(retry, {
                   DBG("Max retry %d", retry);
                   return -2;
               });
      msleep(ARROW_RETRY_DELAY);
  }
  DBG("start device command processing");

  JsonNode *tmp = json_find_member(_parameters, p_const("deviceHid"));
  if ( !tmp || tmp->tag != JSON_STRING ) {
      _error = json_mkobject();
      json_append_member(_error, p_const("error"),
                         json_mkstring("There is no device HID"));
      goto device_command_done;
  }

  // FIXME workaround actually
  JsonNode *cmd = json_find_member(_parameters, p_const("command"));
  if ( !cmd ) cmd = json_find_member(_parameters, p_const("Command"));
  if ( !cmd || cmd->tag != JSON_STRING ) {
      _error = json_mkobject();
      json_append_member(_error, p_const("error"),
                         json_mkstring("There is no command"));
      goto device_command_done;
  }
  DBG("ev cmd: %s", cmd->string_);

  // FIXME workaround actually
  JsonNode *pay = json_find_member(_parameters, p_const("payload"));
  if ( !pay ) pay = json_find_member(_parameters, p_const("Payload"));
  if ( !pay || pay->tag != JSON_STRING ) {
      _error = json_mkobject();
      json_append_member(_error, p_const("error"),
                         json_mkstring("There is no payload"));
      goto device_command_done;
  }
//  DBG("ev msg: %s", pay->string_);

  cmd_handler *cmd_h = NULL;
  linked_list_find_node ( cmd_h, __handlers, cmd_handler, cmdeq, p_stack(cmd->string_) );
  if ( cmd_h ) {
    ret = cmd_h->callback(pay->string_);
    if ( ret < 0 ) {
      _error = json_mkobject();
      json_append_member(_error, p_const("error"),
                         json_mkstring("Something went wrong"));
    }
  } else {
    DBG("There is no handler");
    _error = json_mkobject();
    json_append_member(_error, p_const("error"),
                       json_mkstring("There is no a command handler"));
  }
  // close session after next request
device_command_done:
  http_session_close_set(current_client(), true);
  RETRY_CR(retry);
  if ( _error ) {
    property_t _error_prop = json_encode_property(_error);
    DBG("error string: %s", P_VALUE(_error_prop));
    while ( arrow_send_event_ans(ev->base.id, failed, _error_prop) < 0 ) {
        RETRY_UP(retry, {return -2;});
        msleep(ARROW_RETRY_DELAY);
    }
    property_free(&_error_prop);
    json_delete(_error);
  } else {
    while ( arrow_send_event_ans(ev->base.id, succeeded, p_null()) < 0 ) {
        RETRY_UP(retry, {return -2;});
        msleep(ARROW_RETRY_DELAY);
    }
  }
  http_session_set_protocol(current_client(), api_via_http);
  return 0;
}
#else
typedef void __dummy;
#endif
