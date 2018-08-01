#if !defined(ARROW_DEVICE_STARTSTOP_H_) && defined(DEVICE_STARTSTOP)
#define ARROW_DEVICE_STARTSTOP_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <json/json.h>

//typedef int (*fp)(const char *);

// Add a new command handler ( set the callback )
//int arrow_device_start_handler_set(fp callback);

// DeviceCommand event handler
int ev_DeviceStart(void *ev, JsonNode *node);
int ev_DeviceStop(void *ev, JsonNode *node);

// send the answer by DeviceCommand: failed, received or succeeded (cmd_type)
//int arrow_send_event_ans(const char *hid, cmd_type ev, const char *payload);

#if defined(__cplusplus)
}
#endif

#endif // ARROW_DEVICE_STARTSTOP_H_
