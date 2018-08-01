/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/device.h"
#include <sys/mem.h>
#include <config.h>

void arrow_device_init(arrow_device_t *dev) {
    dev->info = NULL;
    dev->prop = NULL;
    P_CLEAR(dev->name);
    P_CLEAR(dev->type);
    P_CLEAR(dev->uid);
    P_CLEAR(dev->gateway_hid);
    P_CLEAR(dev->hid);
    property_init(&dev->softwareName);
    property_init(&dev->softwareVersion);
#if defined(__IBM__)
    P_CLEAR(dev->eid);
#endif
}

void arrow_device_free(arrow_device_t *dev) {
  property_free(&dev->name);
  property_free(&dev->type);
  property_free(&dev->uid);
  property_free(&dev->gateway_hid);
  property_free(&dev->hid);
  property_free(&dev->softwareName);
  property_free(&dev->softwareVersion);
  if ( dev->info ) json_delete(dev->info);
  if ( dev->prop ) json_delete(dev->prop);
#if defined(__IBM__)
  P_FREE(dev->eid);
#endif
}

void arrow_device_add_info(arrow_device_t *dev, property_t key, const char *value) {
  if ( !dev->info) dev->info = json_mkobject();
  json_append_member(dev->info, key, json_mkstring(value));
}

void arrow_device_add_property(arrow_device_t *dev, property_t key, const char *value) {
  if ( !dev->prop ) dev->prop = json_mkobject();
  json_append_member(dev->prop, key, json_mkstring(value));
}

property_t arrow_device_serialize(arrow_device_t *dev) {
  JsonNode *_main = json_mkobject();
  json_append_member(_main, p_const("name"), json_mkstring(P_VALUE(dev->name)));
  json_append_member(_main, p_const("type"), json_mkstring(P_VALUE(dev->type)));
  json_append_member(_main, p_const("uid"), json_mkstring(P_VALUE(dev->uid)));
  json_append_member(_main, p_const("gatewayHid"), json_mkstring(P_VALUE(dev->gateway_hid)));
  json_append_member(_main, p_const("softwareName"), json_mkstring(P_VALUE(dev->softwareName)));
  json_append_member(_main, p_const("softwareVersion"), json_mkstring(P_VALUE(dev->softwareVersion)));
  if ( dev->info ) json_append_member(_main, p_const("info"), dev->info);
  if ( dev->prop ) json_append_member(_main, p_const("properties"), dev->prop);
  property_t dev_property = json_encode_property(_main);
  if ( dev->info ) json_remove_from(_main, dev->info);
  if ( dev->prop ) json_remove_from(_main, dev->prop);
  json_delete(_main);
  return dev_property;
}

int arrow_device_parse(arrow_device_t *dev, const char *str) {
    JsonNode *_main = json_decode(str);
    if ( !_main ) return -1;
    JsonNode *hid = json_find_member(_main, p_const("hid"));
    if ( !hid || hid->tag != JSON_STRING ) return -1;
    property_copy(&dev->hid, p_stack(hid->string_));
#if defined(__IBM__)
    JsonNode *eid = json_find_member(_main, p_const("externalId"));
    if ( !eid || eid->tag != JSON_STRING ) return -1;
    property_copy(&dev->eid, p_stack(eid->string_));
#endif
    json_delete(_main);
    return 0;
}

#if defined(STATIC_ACN)
static char static_device_uid[GATEWAY_UID_SIZE + sizeof(DEVICE_UID_SUFFIX)+2];
#endif

int arrow_prepare_device(arrow_gateway_t *gateway, arrow_device_t *device) {
  arrow_device_init(device);
  property_weak_copy(&device->gateway_hid, gateway->hid );
  property_copy(&device->name, p_const(DEVICE_NAME));
  property_copy(&device->type, p_const(DEVICE_TYPE));
  property_copy(&device->softwareName, p_const(DEVICE_SOFTWARE_NAME));
  property_copy(&device->softwareVersion, p_const(DEVICE_SOFTWARE_VERSION));
  if ( IS_EMPTY(gateway->uid) ) return -1;
#if defined(STATIC_ACN)
  char *uid = static_device_uid;
#else
  char *uid = (char*)malloc(P_SIZE(gateway->uid)+sizeof(DEVICE_UID_SUFFIX)+2);
#endif
  strcpy(uid, P_VALUE(gateway->uid) );
  strcat(uid, "-");
  strcat(uid, DEVICE_UID_SUFFIX);
#if defined(STATIC_ACN)
  property_t tmp = p_const(uid);
#else
  property_t tmp = p_heap(uid);
#endif
  property_move(&device->uid, &tmp);
  return 0;
}
