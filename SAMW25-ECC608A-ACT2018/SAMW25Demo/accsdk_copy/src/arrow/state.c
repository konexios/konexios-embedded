#include "arrow/state.h"
#include <time/time.h>
#include <http/http_client.h>
#include <json/json.h>
#include <sys/mem.h>
#include <http/http_routine.h>
#include <arrow/events.h>
#include <debug.h>
#include <data/chunk.h>
#include <arrow/api/json/parse.h>
#include <stdarg.h>

typedef struct _state_list_ {
    property_t name;
    JsonTag tag;
    union {
        property_t _property;
        bool _bool;
        double _number;
    } value;
    timestamp_t ts;
    arrow_linked_list_head_node;
} arrow_state_list_t;

#if defined(STATIC_DEVICE_STATES)
# include <data/static_alloc.h>
static_object_pool_type(arrow_state_list_t, ARROW_MAX_DEVICE_STATES)
# define ALLOC static_allocator
# define FREE(p)  static_free(arrow_state_list_t, p)
#else
# define ALLOC alloc_type
# define FREE free
#endif

#define is_valid_tag(t) (( t & JSON_BOOL ) || ( t & JSON_STRING ) || ( t & JSON_NUMBER ))

static arrow_state_list_t *__state_list = NULL;
static property_t _device_hid = {0};
static timestamp_t _last_modify = {0};

typedef void(*_state_add_f_)(arrow_state_list_t *st, void *d);
typedef JsonNode*(*_state_json_f_)(arrow_state_list_t *st);
typedef int(*_state_parse_f_)(arrow_state_list_t *st, const char *);
typedef void(*_state_free_f_)(arrow_state_list_t *st);

typedef struct _state_handle_ {
    _state_add_f_   add;
    _state_json_f_  json;
    _state_parse_f_ parse;
    _state_free_f_  free;
} state_handle_t;

static int stateeq( arrow_state_list_t *sl, property_t name ) {
    if ( property_cmp(&sl->name, &name) == 0 ) return 0;
    return -1;
}

// id d is NULL add a default value
static void state_add_bool(arrow_state_list_t *st, void *d) {
    bool data = false;
    if ( d ) data = *((bool*)d);
    st->value._bool = data;
}

static JsonNode *state_value_bool_json(arrow_state_list_t *st) {
    JsonNode *j = json_mkbool(st->value._bool);
    return j;
}

static int state_value_bool_parse(arrow_state_list_t *st, const char *s) {
    if ( strcmp(s, "true") == 0 )
        st->value._bool = true;
    else
        st->value._bool = false;
    return 0;
}

static void state_add_number(arrow_state_list_t *st, void *d) {
    double data = 0.0;
    if ( d ) data = *((double*)d);
    st->value._number = data;
}

static JsonNode *state_value_number_json(arrow_state_list_t *st) {
    JsonNode *j = json_mknumber(st->value._number);
    return j;
}

static int state_value_number_parse(arrow_state_list_t *st, const char *s) {
    st->value._number = atof(s);
    return 0;
}

static void state_value_string_add(arrow_state_list_t *st, void *d) {
    if ( !d ) {
        st->value._property = p_null();
    } else {
        property_t *p = (property_t *)d;
        property_move(&st->value._property, p);
    }
}

static JsonNode *state_value_string_json(arrow_state_list_t *st) {
    JsonNode *j = json_mkstring(P_VALUE(st->value._property));
    return j;
}

static int state_value_string_parse(arrow_state_list_t *st, const char *s) {
    property_free(&st->value._property);
    property_copy(&st->value._property, p_stack(s));
    return 0;
}

static void state_value_string_free(arrow_state_list_t *st) {
    if ( !IS_EMPTY(st->value._property) ) {
        property_free(&st->value._property);
    }
}

state_handle_t state_adder[] = {
    {NULL, NULL},
    {state_add_bool, state_value_bool_json, state_value_bool_parse, NULL},
    {state_value_string_add, state_value_string_json, state_value_string_parse, state_value_string_free},
    {state_add_number, state_value_number_json, state_value_number_parse, NULL}
};

void arrow_device_state_list_init(arrow_state_list_t *st) {
    property_init(&st->name);
    st->value._property = p_null();
    memset(&st->ts, 0x0, sizeof(timestamp_t));
    arrow_linked_list_init(st);
}

void arrow_device_state_list_free(arrow_state_list_t *st) {
    property_free(&st->name);
    if ( is_valid_tag(st->tag) && state_adder[st->tag].free ) {
        state_adder[st->tag].free(st);
    }
}

int arrow_device_state_handler(JsonNode *_main) __attribute__((weak));

void arrow_device_state_add(property_t name,
                            JsonTag tag,
                            void *value) {
    arrow_state_list_t *dev_state = NULL;
    linked_list_find_node( dev_state, __state_list, arrow_state_list_t, stateeq, name );
    if ( dev_state ) {
        if ( is_valid_tag(tag) && state_adder[tag].add ) {
            state_adder[tag].add(dev_state, value);
            timestamp(&dev_state->ts);
            _last_modify = dev_state->ts;
        }
    }
}

void arrow_device_state_init(int n, ...) {
    va_list args;
    va_start(args, n);
    int i = 0;
    for (i=0; i < n; i++) {
      arrow_state_pair_t tmp;
      tmp = va_arg(args, arrow_state_pair_t);
      arrow_state_list_t *state = ALLOC(arrow_state_list_t);
      if ( !state ) {
          DBG("DEV STATES: out of memory!");
          return;
      }
      arrow_device_state_list_init(state);
      property_copy(&state->name, tmp.name);
      state->tag = tmp.typetag;
      if ( is_valid_tag(tmp.typetag) && state_adder[tmp.typetag].add ) {
          state_adder[tmp.typetag].add(state, NULL);
      }
      arrow_linked_list_add_node_last(__state_list, arrow_state_list_t, state);
    }
    va_end(args);
}

void arrow_device_state_set_string(property_t name,
                                   property_t value) {
    arrow_device_state_add(name, JSON_STRING, &value);
}

void arrow_device_state_set_number(property_t name,
                                   int value) {
    double tmp = (double)value;
    arrow_device_state_add(name, JSON_NUMBER, (void *)&tmp);
}

void arrow_device_state_set_bool(property_t name,
                                 bool value) {
    arrow_device_state_add(name, JSON_BOOL, (void *)&value);
}

void arrow_device_state_free(void) {
    arrow_state_list_t *tmp = NULL;
    arrow_linked_list_for_each_safe( tmp, __state_list , arrow_state_list_t ) {
        arrow_device_state_list_free(tmp);
        FREE(tmp);
    }
    __state_list = NULL;
}

int arrow_state_mqtt_is_running(void) {
  if ( !__state_list ) return -1;
  return 0;
}

int arrow_state_mqtt_stop(void) {
  if (__state_list) arrow_device_state_free();
  property_free(&_device_hid);
  return 0;
}

int arrow_state_mqtt_run(arrow_device_t *device) {
  if ( IS_EMPTY(_device_hid) ) {
    property_weak_copy(&_device_hid, device->hid);
  }
  return arrow_state_mqtt_is_running();
}

static void _state_get_init(http_request_t *request, void *arg) {
  arrow_device_t *device = (arrow_device_t *)arg;
  CREATE_CHUNK(uri, sizeof(ARROW_API_DEVICE_ENDPOINT) + P_SIZE(device->hid) + 10);
  strcpy(uri, ARROW_API_DEVICE_ENDPOINT);
  strcat(uri, "/");
  strcat(uri, P_VALUE(device->hid));
  strcat(uri, "/state");
  http_request_init(request, GET, uri);
  FREE_CHUNK(uri);
}

// payload example
// {"hid":"610c0cd8f213317152668fa7678cf6a51ee88742",
// "pri":"arw:krn:dev-sha:610c0cd8f213317152668fa7678cf6a51ee88742",
// "links":{},
// "deviceHid":"86241a5e939baa7da58faff3527eb021d06d5e9a",
// "states":{"delay":{"value":"0","timestamp":"2018-05-22T10:08:38.503Z"},"led":{"value":"false","timestamp":"2018-05-22T10:08:38.503Z"}}}
static int _state_get_proc(http_response_t *response, void *arg) {
    arrow_device_t *dev = (arrow_device_t *)arg;
    if ( response->m_httpResponseCode != 200 ) return -1;
    if ( !IS_EMPTY(response->payload) ) {
        DBG("[%s]", P_VALUE(response->payload));
    }
    JsonNode *_main = json_decode(P_VALUE(response->payload));
    if ( !_main ) return -1;
    JsonNode *dev_hid = json_find_member(_main, p_const("deviceHid"));
    if ( !dev_hid ) return -1;
    if ( strcmp(dev_hid->string_, P_VALUE(dev->hid)) != 0 ) return -1;
    JsonNode *states = json_find_member(_main, p_const("states"));
    if ( !states ) return -1;
    arrow_device_state_handler(states);
    json_delete(_main);
    return 0;
}

int arrow_state_receive(arrow_device_t *device) {
  STD_ROUTINE(_state_get_init, device, _state_get_proc, device, "State get failed...");
}

typedef enum {
  st_request,
  st_update
} _st_post_api;

typedef struct _post_dev_ {
  arrow_device_t *device;
  int post;
} post_dev_t;

static void _state_post_init(http_request_t *request, void *arg) {
  post_dev_t *pd = (post_dev_t *)arg;
  CREATE_CHUNK(uri, sizeof(ARROW_API_DEVICE_ENDPOINT) + P_SIZE(pd->device->hid) + 20);
  strcpy(uri, ARROW_API_DEVICE_ENDPOINT);
  strcat(uri, "/");
  strcat(uri, P_VALUE(pd->device->hid));
  strcat(uri, "/state/");
  switch( pd->post ) {
    case st_request:
      strcat(uri, "request");
    break;
    case st_update:
      strcat(uri, "update");
    break;
    default:
      FREE_CHUNK(uri);
      return;
  }
  FREE_CHUNK(uri);
  http_request_init(request, POST, uri);
  JsonNode *_main = NULL;
  JsonNode *_states = NULL;
  _main = json_mkobject();
  _states = json_mkobject();

  arrow_state_list_t *tmp = NULL;
  arrow_linked_list_for_each( tmp, __state_list , arrow_state_list_t ) {
      JsonNode *value = NULL;
      if ( is_valid_tag(tmp->tag) && state_adder[tmp->tag].json ) {
          value = state_adder[tmp->tag].json(tmp);
          json_append_member(_states, tmp->name, value);
      }
  }
  json_append_member(_main, p_const("states"), _states);

  if ( !timestamp_is_empty(&_last_modify) ) {
      char ts[30];
      timestamp_string(&_last_modify, ts);
      json_append_member(_main, p_const("timestamp"), json_mkstring(ts));
  }

  http_request_set_payload(request, json_encode_property(_main));
  json_delete(_main);
}

static int _arrow_post_state(arrow_device_t *device, _st_post_api post_type) {
  post_dev_t pd = {device, post_type};
  STD_ROUTINE(_state_post_init, &pd,
              NULL, NULL,
              "State post failed...");
}

int arrow_post_state_request(arrow_device_t *device) {
  return _arrow_post_state(device, st_request);
}

int arrow_post_state_update(arrow_device_t *device) {
  return _arrow_post_state(device, st_update);
}



typedef enum {
  st_received,
  st_complete,
  st_error
} _st_put_api;

typedef struct _put_dev_ {
  property_t device_hid;
  const char *trans_hid;
  int put_type;
} put_dev_t;

int arrow_device_state_handler(JsonNode *_main) {
  JsonNode *tmp = NULL;
  json_foreach(tmp, _main) {
      arrow_state_list_t *dev_state = NULL;
      linked_list_find_node( dev_state, __state_list, arrow_state_list_t, stateeq, tmp->key );
      if ( !dev_state ) {
          DBG("No such state on device %s", P_VALUE(tmp->key));
          continue;
      }
      JsonNode *value = json_find_member(tmp, p_const("value"));
      if ( ! value ) continue;
      JsonNode *timestamp = json_find_member(tmp, p_const("timestamp"));
      timestamp_t ts = {0};
      timestamp_parse(&ts, timestamp->string_);
      if ( timestamp_less(&dev_state->ts, &ts) ) {
          dev_state->ts = ts;
          if ( is_valid_tag(dev_state->tag) && state_adder[dev_state->tag].parse ) {
              state_adder[dev_state->tag].parse(dev_state, value->string_);
          }
      }
  }
  timestamp(&_last_modify);
  json_delete(_main);
  return 0;
}

#if !defined(NO_EVENTS)
static void _state_put_init(http_request_t *request, void *arg) {
  put_dev_t *pd = (put_dev_t *)arg;
  JsonNode *_error = NULL;
  CREATE_CHUNK(uri, sizeof(ARROW_API_DEVICE_ENDPOINT) +
               property_size(&pd->device_hid) + strlen(pd->trans_hid) + 50);
  strcpy(uri, ARROW_API_DEVICE_ENDPOINT);
  strcat(uri, "/");
  strcat(uri, P_VALUE(pd->device_hid));
  strcat(uri, "/state/trans/");
  strcat(uri, pd->trans_hid);
  switch( pd->put_type ) {
    case st_received:
      strcat(uri, "/received");
    break;
    case st_complete:
      strcat(uri, "/succeeded");
    break;
    case st_error: {
      strcat(uri, "/error");
      _error = json_mkobject();
      json_append_member(_error, p_const("error"), json_mkstring("unknown"));
    }
    break;
    default:
      FREE_CHUNK(uri);
      return;
  }
  FREE_CHUNK(uri);
  http_request_init(request, PUT, uri);
  if ( _error ) {
    http_request_set_payload(request, json_encode_property(_error));
    json_delete(_error);
  }
}

static int arrow_device_state_answer(property_t device_hid, _st_put_api put_type, const char *trans_hid) {
    put_dev_t pd = {device_hid, trans_hid, put_type};
    STD_ROUTINE(_state_put_init, &pd, NULL, NULL, "State put failed...");
}

int ev_DeviceStateRequest(void *_ev, JsonNode *_parameters) {
  mqtt_event_t *ev = (mqtt_event_t *)_ev;
  SSP_PARAMETER_NOT_USED(ev);
  if ( IS_EMPTY(_device_hid) ) return -1;

  JsonNode *device_hid = json_find_member(_parameters, p_const("deviceHid"));
  if ( !device_hid ) {
      DBG("cannot find device HID");
      return -1;
  }

  JsonNode *trans_hid = json_find_member(_parameters, p_const("transHid"));
  if ( !trans_hid ) {
      DBG("cannot find trans HID");
      return -1;
  }

  JsonNode *payload = json_find_member(_parameters, p_const("payload"));
  if ( !payload ) {
      DBG("cannot find payload");
      return -1;
  }
  int retry = 0;

  while ( arrow_device_state_answer(_device_hid, st_received, trans_hid->string_) < 0 ) {
      RETRY_UP(retry, {return -2;});
      msleep(ARROW_RETRY_DELAY);
  }

  JsonNode *_states = json_decode(payload->string_);
  int ret = arrow_device_state_handler(_states);

  if ( ret < 0 ) {
    while ( arrow_device_state_answer(_device_hid, st_error, trans_hid->string_) < 0 ) {
        RETRY_UP(retry, {return -2;});
        msleep(ARROW_RETRY_DELAY);
    }
  } else {
    while ( arrow_device_state_answer(_device_hid, st_complete, trans_hid->string_) < 0 ) {
        RETRY_UP(retry, {return -2;});
        msleep(ARROW_RETRY_DELAY);
    }
  }
  return 0;
}
#endif
