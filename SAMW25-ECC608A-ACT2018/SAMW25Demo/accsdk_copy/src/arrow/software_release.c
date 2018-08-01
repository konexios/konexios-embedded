/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/software_release.h"
#include <http/http_routine.h>
#include <debug.h>
#include <sys/watchdog.h>
#include <sys/reboot.h>
#include <ssl/md5sum.h>
#include <arrow/utf8.h>
#include <time/time.h>
#include <data/chunk.h>

#if defined(NO_RELEASE_UPDATE)
typedef void __dummy__;
#else
#define URI_LEN sizeof(ARROW_API_SOFTWARE_RELEASE_ENDPOINT) + 200

static __release_cb  __release = NULL;
static __download_init_cb __download_init = NULL;
static __download_payload_cb  __payload = NULL;
static __download_complete_cb __download = NULL;

static property_t serialize_software_trans(const char *hid, release_sched_t *rs) {
  JsonNode *_main = json_mkobject();
  json_append_member(_main, p_const("objectHid"), json_mkstring(hid));
  json_append_member(_main, p_const("softwareReleaseScheduleHid"), json_mkstring(rs->schedule_hid));
  json_append_member(_main, p_const("toSoftwareReleaseHid"), json_mkstring(rs->release_hid));
  property_t tmp = json_encode_property(_main);
  json_delete(_main);
  return tmp;
}

typedef struct _gateway_software_sched_ {
  arrow_gateway_t *gate;
  release_sched_t *rs;
} gateway_software_sched_t;

static void _gateway_software_releases_trans_init(http_request_t *request, void *arg) {
  gateway_software_sched_t *gs = (gateway_software_sched_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  strcpy(uri, ARROW_API_SOFTWARE_RELEASE_ENDPOINT);
  strcat(uri, "/gateways/upgrade");
  http_request_init(request, POST, uri);
  FREE_CHUNK(uri);
  http_request_set_payload(request, serialize_software_trans(P_VALUE(gs->gate->hid), gs->rs));
}

static int _gateway_software_releases_trans_proc(http_response_t *response, void *arg) {
  release_sched_t *rs = (release_sched_t *)arg;
  if ( IS_EMPTY(response->payload) )  return -1;
  JsonNode *_main = json_decode(P_VALUE(response->payload));
  JsonNode *hid = json_find_member(_main, p_const("hid"));
  if ( !hid ) return -1;
  property_copy(&rs->trans_hid, p_stack(hid->string_));
  return 0;
}

int arrow_gateway_software_releases_trans(arrow_gateway_t *gate, release_sched_t *rs) {
  gateway_software_sched_t sch = {gate, rs};
  P_CLEAR(rs->trans_hid);
  STD_ROUTINE(_gateway_software_releases_trans_init, &sch, _gateway_software_releases_trans_proc, NULL, "Software Trans fail");
}

typedef struct _device_software_sched_ {
  arrow_device_t *gate;
  release_sched_t *rs;
} device_software_sched_t;

static void _device_software_releases_trans_init(http_request_t *request, void *arg) {
  device_software_sched_t *gs = (device_software_sched_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  strcpy(uri, ARROW_API_SOFTWARE_RELEASE_ENDPOINT);
  strcat(uri, "/devices/upgrade");
  http_request_init(request, POST, uri);
  FREE_CHUNK(uri);
  http_request_set_payload(request, serialize_software_trans(P_VALUE(gs->gate->hid), gs->rs));
}

static int _device_software_releases_trans_proc(http_response_t *response, void *arg) {
  release_sched_t *rs = (release_sched_t *)arg;
  if ( IS_EMPTY(response->payload) )  return -1;
  JsonNode *_main = json_decode(P_VALUE(response->payload));
  JsonNode *hid = json_find_member(_main, p_const("hid"));
  if ( !hid ) return -1;
  property_copy(&rs->trans_hid, p_stack(hid->string_));
  return 0;
}


int arrow_device_software_releases_trans(arrow_device_t *dev, release_sched_t *rs) {
  device_software_sched_t sch = {dev, rs};
  P_CLEAR(rs->trans_hid);
  STD_ROUTINE(_device_software_releases_trans_init, &sch, _device_software_releases_trans_proc, NULL, "Software Trans fail");
}

typedef enum {
  received,
  success,
  fail
} state_enums;

typedef struct _ans_ {
  state_enums state;
  const char *hid;
  const char *error;
} ans_t;

static void _software_releases_ans_init(http_request_t *request, void *arg) {
  ans_t *ans = (ans_t *)arg;
  int n = 0;
  CREATE_CHUNK(uri, URI_LEN);
  switch(ans->state) {
    case received:
      n = snprintf(uri, URI_LEN, ARROW_API_SOFTWARE_RELEASE_ENDPOINT "/%s/received", ans->hid);
    break;
    case success:
      n = snprintf(uri, URI_LEN, ARROW_API_SOFTWARE_RELEASE_ENDPOINT "/%s/succeeded", ans->hid);
    break;
    case fail:
      n = snprintf(uri, URI_LEN, ARROW_API_SOFTWARE_RELEASE_ENDPOINT "/%s/failed", ans->hid);
    break;
  }
  if ( n < 0 ) return;
  uri[n] = 0x0;
  DBG("uri %s", uri);
  http_request_init(request, PUT, uri);
  FREE_CHUNK(uri);
  if ( ans->state == fail && ans->error ) {
    JsonNode *_error = json_mkobject();
    json_append_member(_error, p_const("error"), json_mkstring(ans->error));
    http_request_set_payload(request, json_encode_property(_error));
    json_delete(_error);
  }
}

int arrow_software_releases_trans_fail(const char *hid, const char *error) {
  ans_t ans = {fail, hid, error};
  STD_ROUTINE(_software_releases_ans_init, &ans, NULL, NULL, "Software Trans fail");
}

int arrow_software_releases_trans_received(const char *hid) {
  ans_t ans = {received, hid, NULL};
  STD_ROUTINE(_software_releases_ans_init, &ans, NULL, NULL, "Software Trans fail");
}

int arrow_software_releases_trans_success(const char *hid) {
  ans_t ans = {success, hid, NULL};
  STD_ROUTINE(_software_releases_ans_init, &ans, NULL, NULL, "Software Trans fail");
}

static void _software_releases_start_init(http_request_t *request, void *arg) {
  const char *hid = (const char *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  int n = snprintf(uri, URI_LEN, "%s/%s/start", ARROW_API_SOFTWARE_RELEASE_ENDPOINT, hid);
  if ( n < 0 ) return;
  uri[n] = 0x0;
  http_request_init(request, POST, uri);
  FREE_CHUNK(uri);
}

int arrow_software_releases_trans_start(const char *hid) {
  STD_ROUTINE(_software_releases_start_init, (void*)hid, NULL, NULL, "Software Trans start fail");
}

int ev_DeviceSoftwareRelease(void *_ev, JsonNode *_parameters) {
  SSP_PARAMETER_NOT_USED(_ev);
  int ret = -1;
  JsonNode *tmp = json_find_member(_parameters, p_const("softwareReleaseTransHid"));
  if ( !tmp || tmp->tag != JSON_STRING ) return -1;
  char *trans_hid = tmp->string_;
  wdt_feed();

  http_session_close_set(current_client(), false);
#if defined(HTTP_VIA_MQTT)
  http_session_set_protocol(current_client(), 1);
#endif
  int retry = 0;
  while( arrow_software_releases_trans_received(trans_hid) < 0) {
    RETRY_UP(retry, {return -2;});
    msleep(ARROW_RETRY_DELAY);
  }
  wdt_feed();
  tmp = json_find_member(_parameters, p_const("tempToken"));
  if ( !tmp || tmp->tag != JSON_STRING ) goto software_release_done;
  char *_token = tmp->string_;
  DBG("FW TOKEN: %s", _token);
  DBG("FW HID: %s", trans_hid);
  tmp = json_find_member(_parameters, p_const("fromSoftwareVersion"));
  if ( !tmp || tmp->tag != JSON_STRING ) goto software_release_done;
  char *_from = tmp->string_;
  tmp = json_find_member(_parameters, p_const("toSoftwareVersion"));
  if ( !tmp || tmp->tag != JSON_STRING ) goto software_release_done;
  char *_to = tmp->string_;
  tmp = json_find_member(_parameters, p_const("md5checksum"));
  if ( !tmp || tmp->tag != JSON_STRING ) goto software_release_done;
  char *_checksum = tmp->string_;
  wdt_feed();
  if ( strcmp( _from, GATEWAY_SOFTWARE_VERSION ) != 0 ) {
      DBG("Warning: wrong base version [%s != %s]", _from, GATEWAY_SOFTWARE_VERSION);
  }
  if ( __download_init ) {
      ret = __download_init(_checksum);
      if ( ret < 0 ) goto software_release_done;
  }
  http_session_set_protocol(current_client(), api_via_http);
  ret = arrow_software_release_download(_token, trans_hid, _checksum);
//#if defined(HTTP_VIA_MQTT)
//  http_session_set_protocol(current_client(), 1);
//#endif
  wdt_feed();
  SSP_PARAMETER_NOT_USED(_to);
software_release_done:
  // close session after next request
  http_session_close_set(current_client(), true);
  if ( ret < 0 ) {
      int retry = 0;
      wdt_feed();
      while ( arrow_software_releases_trans_fail(trans_hid, "failed") < 0 ) {
          RETRY_UP(retry, {return -2;});
          msleep(ARROW_RETRY_DELAY);
      }
      wdt_feed();
  } else {
      int retry = 0;
      wdt_feed();
      while ( arrow_software_releases_trans_success(trans_hid) < 0 ) {
          RETRY_UP(retry, {return -2;});
          msleep(ARROW_RETRY_DELAY);
      }
      reboot();
  }
  http_session_set_protocol(current_client(), api_via_http);
  return ret;
}

void create_release_schedule(release_sched_t *rs, const char *shed_hid, const char *rel_hid) {
  P_CLEAR(rs->trans_hid);
  rs->schedule_hid = shed_hid;
  rs->release_hid = rel_hid;
}

void free_release_schedule(release_sched_t *rs) {
  property_free(&rs->trans_hid);
}

int __attribute__((weak)) arrow_software_release(const char *token,
                                                 const char *chsum,
                                                 const char *from,
                                                 const char *to) {
  if ( __release ) return __release(token, chsum, from, to);
  return -1;
}

int arrow_software_release_set_cb(__release_cb cb) {
  __release = cb;
  return 0;
}

typedef struct _token_hid_ {
  const char *token;
  const char *hid;
} token_hid_t;


// set the callback for update file processing
int arrow_software_release_dowload_set_cb(
        __download_init_cb icb,
        __download_payload_cb pcb,
        __download_complete_cb ccb) {
    __download_init = icb;
    __payload = pcb;
    __download = ccb;
    return 0;
}

// this is a special payload handler for the OTA
int arrow_software_release_payload_handler(void *r,
                                           property_t payload) {
  http_response_t *res = (http_response_t *)r;
  int flag = FW_FIRST;
  if ( __payload ) {
      wdt_feed();
      if ( ! res->processed_payload_chunk ) {
          md5_chunk_init();
      } else
          flag |= FW_NEXT;
      md5_chunk(payload.value, property_size(&payload));
      return __payload(payload.value, property_size(&payload), flag);
  }
  return -1;
}

static void _software_releases_download_init(http_request_t *request, void *arg) {
  token_hid_t *th = (token_hid_t *)arg;
  CREATE_CHUNK(uri, URI_LEN);
  int n = snprintf(uri, URI_LEN, ARROW_API_SOFTWARE_RELEASE_ENDPOINT "/%s/%s/file", th->hid, th->token);
  if (n < 0) return;
  uri[n] = 0x0;
  http_request_init(request, GET, uri);
  request->_response_payload_meth._p_add_handler = arrow_software_release_payload_handler;
  FREE_CHUNK(uri);
  wdt_feed();
}

static int _software_releases_download_proc(http_response_t *response, void *arg) {
    SSP_PARAMETER_NOT_USED(response);
    SSP_PARAMETER_NOT_USED(arg);
    char *checksum = (char *)arg;
    wdt_feed();
    if ( response->m_httpResponseCode != 200 ) return -1;
    if ( __download ) {
        char hash[18];
        char hash_hex[34];
        int size = md5_chunk_hash(hash);
        hex_encode(hash_hex, hash, size);
        hash_hex[2*size] = 0x0;
        DBG("fw hash cmp {%s, %s}", checksum, hash_hex);
        if ( strncmp(hash_hex, checksum, 2*size) == 0 ) {
            return __download(FW_SUCCESS);
        } else {
            DBG("fw md5 checksum failed...");
            return __download(FW_MD5SUM);
        }
    }
    return -1;
}

int arrow_software_release_download(const char *token, const char *tr_hid, const char *checksum) {
  token_hid_t th = { token, tr_hid };
  STD_ROUTINE(_software_releases_download_init, &th, _software_releases_download_proc, (void*)checksum, "File download fail");
}

// schedules
int arrow_schedule_model_init(arrow_schedule_t *sch, int category, property_t sw_hid, property_t user_hid) {
    sch->device_category = category;
    property_init(&sch->software_release_hid);
    property_init(&sch->user_hid);
    property_copy(&sch->software_release_hid, sw_hid);
    property_copy(&sch->user_hid, user_hid);
    sch->_hids = NULL;
    return 0;
}

int arrow_schedule_model_add_object(arrow_schedule_t *sch, property_t hid) {
    struct object_hid_list *objhid = alloc_type(struct object_hid_list);
    arrow_linked_list_init(objhid);
    property_weak_copy(&objhid->hid, hid);
    arrow_linked_list_add_node_last(sch->_hids, struct object_hid_list, objhid);
    return 0;
}

int arrow_schedule_model_free(arrow_schedule_t *sch) {
    property_free(&sch->software_release_hid);
    property_free(&sch->user_hid);
    struct object_hid_list *tmp = NULL;
    arrow_linked_list_for_each_safe(tmp, sch->_hids, struct object_hid_list) {
        property_free(&tmp->hid);
        free(tmp);
    }
    sch->_hids = NULL;
    return 0;
}

static void _software_releases_schedule_start_init(http_request_t *request, void *arg) {
    arrow_schedule_t *sch = (arrow_schedule_t *)arg;
    CREATE_CHUNK(uri, URI_LEN);
    int n = snprintf(uri, URI_LEN, "%s/start", ARROW_API_SOFTWARE_RELEASE_SCHEDULE_ENDPOINT);
    if (n < 0) return;
    uri[n] = 0x0;
    http_request_init(request, POST, uri);
    http_request_add_header(request,
                            p_const("x-arrow-apikey"),
                            p_const(DEFAULT_API_KEY));
    JsonNode *_main = json_mkobject();
    if ( sch->device_category == schedule_GATEWAY ) {
        json_append_member(_main, p_const("deviceCategory"), json_mkstring("GATEWAY"));
    } else {
        json_append_member(_main, p_const("deviceCategory"), json_mkstring("DEVICE"));
    }
    json_append_member(_main, p_const("softwareReleaseHid"), json_mkstring(P_VALUE(sch->software_release_hid)));
    json_append_member(_main, p_const("userHid"), json_mkstring(P_VALUE(sch->user_hid)));
    JsonNode *hids = json_mkarray();
    struct object_hid_list *tmp = NULL;
    arrow_linked_list_for_each(tmp, sch->_hids, struct object_hid_list) {
        json_append_element(hids, json_mkstring(P_VALUE(tmp->hid)));
    }
    json_append_member(_main, p_const("objectHids"), hids);
    http_request_set_payload(request, json_encode_property(_main));
    json_delete(_main);
    FREE_CHUNK(uri);
    wdt_feed();
}

static int _software_releases_schedule_start_proc(http_response_t *response, void *arg) {
    SSP_PARAMETER_NOT_USED(arg);
    if ( response->m_httpResponseCode != 200 ) {
        DBG("ERROR(%d): %s", response->m_httpResponseCode, P_VALUE(response->payload));
        return -1;
    }
    return 0;
}

int arrow_software_releases_schedules_start(arrow_schedule_t *sch) {
    STD_ROUTINE(_software_releases_schedule_start_init, (void*)sch,
                _software_releases_schedule_start_proc, NULL, "Schedule fail");
}
#endif

