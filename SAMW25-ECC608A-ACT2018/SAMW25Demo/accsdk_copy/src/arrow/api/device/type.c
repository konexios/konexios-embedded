/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/api/device/type.h"
#include <http/http_routine.h>
#include <debug.h>
#include <stdarg.h>
#include <sys/mem.h>
#include <data/chunk.h>

#define URI_LEN sizeof(ARROW_API_DEVICE_ENDPOINT) + 50

void device_type_init(device_type_t *dev, int enable, const char *name, const char *dec) {
  dev->enabled = enable;
  dev->description = strdup(dec);
  dev->name = strdup(name);
  dev->telemetries = NULL;
}

device_type_telemetry_t *device_type_add_telemetry(device_type_t *dev, const char *name, const char *type, const char *desc) {
  device_type_telemetry_t *telemetry;
  telemetry = alloc_type(device_type_telemetry_t);
  telemetry->description = strdup(desc);
  telemetry->name = strdup(name);
  telemetry->type = strdup(type);
  telemetry->variables = NULL;
  arrow_linked_list_init(telemetry);
  arrow_linked_list_add_node_last ( dev->telemetries, device_type_telemetry_t, telemetry );
  return telemetry;
}

void device_type_free(device_type_t *dev) {
  if (dev->description) free(dev->description);
  if (dev->name) free(dev->name);
  device_type_telemetry_t *telemetry = NULL;
  arrow_linked_list_for_each_safe(telemetry, dev->telemetries, device_type_telemetry_t) {
      if ( telemetry->description ) free(telemetry->description);
      if ( telemetry->name ) free(telemetry->name);
      if ( telemetry->type ) free(telemetry->type);
      property_map_clear(&telemetry->variables);
      free(telemetry);
  }
}

int device_type_add_telemetry_variables(device_type_telemetry_t *tel, const char *key, const char *value) {
    return property_map_add(&tel->variables, p_stack(key), p_stack(value));
}

static void _device_type_list_init(http_request_t *request, void *arg) {
  SSP_PARAMETER_NOT_USED(arg);
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/types", ARROW_API_DEVICE_ENDPOINT);
  http_request_init(request, GET, uri);
  FREE_CHUNK(uri);
}

static int _device_type_list_proc(http_response_t *response, void *arg) {
  SSP_PARAMETER_NOT_USED(arg);
  if ( response->m_httpResponseCode == 200 ) {
    DBG("types[%s]", P_VALUE(response->payload));
  } else return -1;
  return 0;
}


int arrow_device_type_list(void) {
  STD_ROUTINE(_device_type_list_init, NULL, _device_type_list_proc, NULL, "Device Type list");
}

static property_t device_type_serialize(device_type_t *dev) {
  JsonNode *_main = json_mkobject();
  json_append_member(_main, p_const("description"), json_mkstring(dev->description));
  json_append_member(_main, p_const("enabled"), json_mkbool(dev->enabled));
  json_append_member(_main, p_const("name"), json_mkstring(dev->name));
  JsonNode *tls = json_mkarray();
  device_type_telemetry_t *t = NULL;
  arrow_linked_list_for_each( t, dev->telemetries, device_type_telemetry_t ) {
    JsonNode *tl_element = json_mkobject();
    JsonNode *var_array = json_mkarray();
    property_map_t *var = NULL;
    arrow_linked_list_for_each ( var, t->variables, property_map_t ) {
        JsonNode *tl_variables = json_mkobject();
        json_append_member(tl_element, p_const("description"), json_mkstring(P_VALUE(var->key)));
        json_append_member(tl_element, p_const("description"), json_mkstring(P_VALUE(var->value)));
        json_append_element(var_array, tl_variables);
    }
    json_append_member(tl_element, p_const("variables"), var_array);
    json_append_member(tl_element, p_const("description"), json_mkstring(t->description));
    json_append_member(tl_element, p_const("name"), json_mkstring(t->name));
    json_append_member(tl_element, p_const("type"), json_mkstring(t->type));
    json_append_element(tls, tl_element);
  }
  json_append_member(_main, p_const("telemetries"), tls);
  property_t payload = json_encode_property(_main);
  DBG("type pay: [%s]", P_VALUE(payload));
  json_delete(_main);
  return payload;
}

static void _device_type_create_init(http_request_t *request, void *arg) {
  device_type_t *dev_type = (device_type_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/types", ARROW_API_DEVICE_ENDPOINT);
  http_request_init(request, POST, uri);
  FREE_CHUNK(uri);
  http_request_set_payload(request, device_type_serialize(dev_type));
}

int arrow_device_type_create(device_type_t *dev_type) {
  STD_ROUTINE(_device_type_create_init, dev_type, NULL, NULL,"Device Type create");
}

typedef struct _device_device_type_ {
  arrow_device_t *dev;
  device_type_t *type;
} device_device_type_t;


static void _device_type_update_init(http_request_t *request, void *arg) {
  device_device_type_t *ddt = (device_device_type_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/types/%s", ARROW_API_DEVICE_ENDPOINT,
           P_VALUE(ddt->dev->hid));
  http_request_init(request, PUT, uri);
  FREE_CHUNK(uri);
  http_request_set_payload(request, device_type_serialize(ddt->type));
}

int arrow_device_type_update(arrow_device_t *dev, device_type_t *dev_type) {
  device_device_type_t ddt = { dev, dev_type };
  STD_ROUTINE(_device_type_update_init, &ddt, NULL, NULL, "Device Type update");
}
