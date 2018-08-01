#include "arrow/software_update.h"
#include <debug.h>
#include <arrow/events.h>
#include <arrow/routine.h>
#include <arrow/api/gateway/gateway.h>
#include <time/time.h>
#include <sys/reboot.h>

#if defined(NO_SOFTWARE_UPDATE)
typedef void __dummy__;
#else
__update_cb __attribute__((weak)) __update;

int ev_GatewaySoftwareUpdate(void *_ev, JsonNode *_parameters) {
  SSP_PARAMETER_NOT_USED(_ev);
  DBG("start software update processing");
//  mqtt_event_t *ev = (mqtt_event_t *)_ev;
  JsonNode *tmp = json_find_member(_parameters, p_const("url"));
  if ( !tmp || tmp->tag != JSON_STRING ) return -1;
  DBG("update url: %s", tmp->string_);

  if ( arrow_gateway_software_update(tmp->string_) < 0 ) return -1;
  DBG("Reboot...");
  reboot();
  return 0;
}

int __attribute__((weak)) arrow_gateway_software_update(const char *url) {
  if ( __update ) return __update(url);
  return -1;
}

int arrow_gateway_software_update_set_cb(__update_cb cb) {
  __update = cb;
  return 0;
}
#endif
