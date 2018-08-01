#if !defined(ARROW_STATE_H_)
#define ARROW_STATE_H_

#include <arrow/device.h>

typedef struct _state_pair_ {
    property_t name;
    int typetag;
#if defined(__cplusplus)
    _state_pair_(property_t n, int t) : name(n), typetag(t) {}
#endif
} arrow_state_pair_t;

#if defined(__cplusplus)
#define state_pr(x, y) arrow_state_pair_t((x), (y))
#else
#define state_pr(x, y) (arrow_state_pair_t) { .name=(x), .typetag=(y) }
#endif

// set a new device state with default value
void arrow_device_state_init(int n, ...);
void arrow_device_state_set_string(property_t name,
                                   property_t value);
void arrow_device_state_set_number(property_t name,
                                   int value);
void arrow_device_state_set_bool(property_t name,
                                 bool value);

void arrow_device_state_free(void);

// get device state information
int arrow_state_receive(arrow_device_t *device);
int arrow_post_state_request(arrow_device_t *device);
int arrow_post_state_update(arrow_device_t *device);

// allow to use
int arrow_state_mqtt_run(arrow_device_t *device);
int arrow_state_mqtt_is_running(void);
int arrow_state_mqtt_stop(void);

// DeviceStateRequest event handler
int ev_DeviceStateRequest(void *, JsonNode *_parameters);

int arrow_device_state_handler(JsonNode *states);

#endif // ARROW_STATE_H_
