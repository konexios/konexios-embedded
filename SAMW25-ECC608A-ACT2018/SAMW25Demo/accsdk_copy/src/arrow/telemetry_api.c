#include "arrow/telemetry_api.h"
#include <data/find_by.h>
#include <arrow/telemetry.h>
#include <http/http_routine.h>
#include <debug.h>
#include <data/chunk.h>

#define URI_LEN sizeof(ARROW_API_TELEMETRY_ENDPOINT) + 50

int telemetry_response_data_list_init(telemetry_response_data_list_t *data, int size, int page, int tsize, int tpage) {
  data->size = size;
  data->page = page;
  data->totalPages = tpage;
  data->totalSize = tsize;
  data->data = NULL;
  return 0;
}

int telemetry_response_data_list_free(telemetry_response_data_list_t *data) {
    telemetry_data_info_t *tmp = NULL;
    arrow_linked_list_for_each_safe(tmp, data->data, telemetry_data_info_t) {
        if ( tmp->deviceHid ) free(tmp->deviceHid);
        if ( tmp->name ) free(tmp->name);
        if ( tmp->type ) free(tmp->type);
        free(tmp);
    }
    return 0;
}

int add_telemetry_data_info(telemetry_response_data_list_t *data,
                            const char *deviceHid,
                            const char *name,
                            const char *type,
                            time_t timestamp,
                            int flval) {
    telemetry_data_info_t *info = alloc_type(telemetry_data_info_t);
    arrow_linked_list_init(info);
    info->deviceHid = strdup(deviceHid);
    info->name = strdup(name);
    info->type = strdup(type);
    info->timestamp = timestamp;
    info->floatValue = flval;
    arrow_linked_list_add_node_last(data->data, telemetry_data_info_t, info);
    return 0;
}


typedef struct _device_telemetry {
  arrow_device_t *device;
  void *data;
  int count;
} device_telemetry_t;

static void _telemetry_init(http_request_t *request, void *arg) {
  device_telemetry_t *dt = (device_telemetry_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/devices/%s", ARROW_API_TELEMETRY_ENDPOINT, P_VALUE(dt->device->hid));
  http_request_init(request, POST, uri);
  FREE_CHUNK(uri);
  request->is_chunked = 1;
  http_request_set_payload(request, telemetry_serialize(dt->device, dt->data));
}

int arrow_send_telemetry(arrow_device_t *device, void *d) {
  device_telemetry_t dt = {device, d, 1};
  STD_ROUTINE(_telemetry_init, &dt,
              NULL, NULL,
              "Arrow Telemetry send failed...");
}

static void _telemetry_batch_init(http_request_t *request, void *arg) {
  device_telemetry_t *dt = (device_telemetry_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/devices/%s/batch", ARROW_API_TELEMETRY_ENDPOINT, P_VALUE(dt->device->hid));
  http_request_init(request, POST, uri);
  FREE_CHUNK(uri);
  request->is_chunked = 1;
  int i = 0;
  char *_main = NULL;
  for( i = 0; i < dt->count; i++ ) {
    property_t tmp = telemetry_serialize(dt->device, (int*)dt->data + i);
    if ( !_main ) {
      _main = (char*)malloc(property_size(&tmp) * ( dt->count + 1 ));
      strcpy(_main, "[");
    }
    strcat(_main, tmp.value);
    if ( i != dt->count - 1 ) strcat(_main, ",");
    else strcat(_main, "]");
    property_free(&tmp);
  }
  DBG("main: %s", _main);
  http_request_set_payload(request, p_heap(_main));
}

int arrow_telemetry_batch_create(arrow_device_t *device, void *data, int size) {
  device_telemetry_t dt = {device, data, size};
  STD_ROUTINE(_telemetry_batch_init, &dt,
              NULL, NULL,
              "Arrow Telemetry send failed...");
}

typedef struct _telemetry_hid_ {
  find_by_t *params;
  const char *hid;
} telemetry_hid_t;

static void _telemetry_find_by_application_hid_init(http_request_t *request, void *arg) {
  telemetry_hid_t *appl = (telemetry_hid_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/applications/%s", ARROW_API_TELEMETRY_ENDPOINT, appl->hid);
  http_request_init(request, GET, uri);
  FREE_CHUNK(uri);
  http_request_set_findby(request, appl->params);
}

static int _telemetry_find_by_application_hid_proc(http_response_t *response, void *arg) {
  SSP_PARAMETER_NOT_USED(arg);
  DBG("telem appl: %s", P_VALUE(response->payload));
  return 0;
}

int arrow_telemetry_find_by_application_hid(const char *hid, int n, ...) {
  find_by_t *params = NULL;
  find_by_collect(params, n);
  telemetry_hid_t app = {params, hid};
  STD_ROUTINE(_telemetry_find_by_application_hid_init, &app,
              _telemetry_find_by_application_hid_proc, NULL,
              "Arrow Telemetry find by failed...");
}

static void _telemetry_find_by_device_hid_init(http_request_t *request, void *arg) {
  telemetry_hid_t *appl = (telemetry_hid_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/devices/%s", ARROW_API_TELEMETRY_ENDPOINT, appl->hid);
  http_request_init(request, GET, uri);
  FREE_CHUNK(uri);
  http_request_set_findby(request, appl->params);
}

static int _telemetry_find_by_device_hid_proc(http_response_t *response, void *arg) {
  telemetry_response_data_list_t* t = (telemetry_response_data_list_t*)arg;
  if ( response->m_httpResponseCode != 200 ) return -1;
  DBG("telem appl: %s", P_VALUE(response->payload));
  JsonNode *_main = json_decode(P_VALUE(response->payload));
  JsonNode *size  = json_find_member(_main, p_const("size"));
  if ( !size ) return -1;
  JsonNode *page  = json_find_member(_main, p_const("page"));
  if ( !page ) return -1;
  JsonNode *totalSize  = json_find_member(_main, p_const("totalSize"));
  if ( !totalSize ) return -1;
  JsonNode *totalPages  = json_find_member(_main, p_const("totalPages"));
  if ( !totalPages ) return -1;
  telemetry_response_data_list_init(t, json_number(size),
                                    json_number(page),
                                    json_number(totalSize),
                                    json_number(totalPages));
  JsonNode *data  = json_find_member(_main, p_const("data"));
  if ( !data ) return 0;
  int i = 0;
  for ( i = 0; i < t->size; i++ ) {
    JsonNode *info = json_find_element(data, i);
    if ( !info ) goto parse_error;

    JsonNode *deviceHid = json_find_member(info, p_const("deviceHid"));
    if ( !deviceHid ) goto parse_error;
    JsonNode *name = json_find_member(info, p_const("name"));
    if ( !name ) goto parse_error;
    JsonNode *type = json_find_member(info, p_const("type"));
    if ( !type ) goto parse_error;
    JsonNode *timestamp = json_find_member(info, p_const("timestamp"));
    if ( !timestamp ) goto parse_error;
    JsonNode *floatValue = json_find_member(info, p_const("floatValue"));
    if ( !floatValue ) goto parse_error;

    add_telemetry_data_info(t,
                            deviceHid->string_,
                            name->string_,
                            type->string_,
                            json_number(timestamp),
                            json_number(floatValue));
  }
  json_delete(_main);
  return 0;

parse_error:
  DBG("parse error");
  json_delete(_main);
  telemetry_response_data_list_free(t);
  return -1;
}

int arrow_telemetry_find_by_device_hid(const char *hid,
                                       telemetry_response_data_list_t *data,
                                       int n, ...) {
  find_by_t *params = NULL;
  find_by_collect(params, n);
  telemetry_hid_t app = {params, hid};
  STD_ROUTINE(_telemetry_find_by_device_hid_init, &app,
              _telemetry_find_by_device_hid_proc, data,
              "Arrow Telemetry find by failed...");
}

static void _telemetry_find_by_node_hid_init(http_request_t *request, void *arg) {
  telemetry_hid_t *appl = (telemetry_hid_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/nodes/%s", ARROW_API_TELEMETRY_ENDPOINT, appl->hid);
  http_request_init(request, GET, uri);
  FREE_CHUNK(uri);
  http_request_set_findby(request, appl->params);
}

static int _telemetry_find_by_node_hid_proc(http_response_t *response, void *arg) {
  SSP_PARAMETER_NOT_USED(arg);
  DBG("telem appl: %s", P_VALUE(response->payload));
  return 0;
}

int arrow_telemetry_find_by_node_hid(const char *hid, int n, ...) {
  find_by_t *params = NULL;
  find_by_collect(params, n);
  telemetry_hid_t app = {params, hid};
  STD_ROUTINE(_telemetry_find_by_node_hid_init, &app,
              _telemetry_find_by_node_hid_proc, NULL,
              "Arrow Telemetry find by failed...");
}
