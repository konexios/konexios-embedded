/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/device_command.h"

#if defined(DEVICE_STARTSTOP)
#include <http/http_routine.h>
#include <json/json.h>
#include <sys/mem.h>
#include <arrow/events.h>
#include <time/time.h>
#include <debug.h>

int ev_DeviceStart(void *_ev, JsonNode *_parameters) {
  mqtt_event_t *ev = (mqtt_event_t *)_ev;
  SSP_PARAMETER_NOT_USED(ev);
  DBG("start device command processing");

  JsonNode *tmp = json_find_member(_parameters, "deviceHid");
  if ( !tmp || tmp->tag != JSON_STRING ) return -1;

  JsonNode *cmd = json_find_member(_parameters, "command");
  if ( !cmd || cmd->tag != JSON_STRING ) return -1;
  DBG("ev cmd: %s", cmd->string_);

  JsonNode *pay = json_find_member(_parameters, "payload");
  if ( !pay || pay->tag != JSON_STRING ) return -1;
  DBG("ev msg: %s", pay->string_);
  return 0;
}

int ev_DeviceStop(void *_ev, JsonNode *_parameters) {
  mqtt_event_t *ev = (mqtt_event_t *)_ev;
  SSP_PARAMETER_NOT_USED(ev);
  DBG("start device command processing");

  JsonNode *tmp = json_find_member(_parameters, "deviceHid");
  if ( !tmp || tmp->tag != JSON_STRING ) return -1;

  JsonNode *cmd = json_find_member(_parameters, "command");
  if ( !cmd || cmd->tag != JSON_STRING ) return -1;
  DBG("ev cmd: %s", cmd->string_);

  JsonNode *pay = json_find_member(_parameters, "payload");
  if ( !pay || pay->tag != JSON_STRING ) return -1;
  DBG("ev msg: %s", pay->string_);
  return 0;
}

#else
typedef void __dummy;
#endif
