#include "ledcmd.h"
#include <json/json.h>
#include <unint.h>
#include <debug.h>


static uint32_t ledmask = 0;

bool is_blue_led_hold() {
    if ( ledmask & LED2_HOLD ) return true;
    return false;
}

bool is_orange_led_hold() {
    if ( ledmask & LED1_HOLD ) return true;
    return false;
}

static int _set_mask(JsonNode *led, uint32_t mask) {
    if ( led->tag != JSON_STRING ) return -1;
    if ( strcmp(led->string_, "hold") == 0 ) {
        ledmask |= mask;
    } else if ( strcmp(led->string_, "blink") == 0 ) {
        ledmask &= ~mask;
    } else {
        return -1;
    }
    return 0;
}

int led_ctrl(const char *str) {
    if (!str) return -1;
    DBG("led: [%s]", str);
    JsonNode *_main = json_decode(str);
    if ( !_main ) return -1;
    JsonNode *led = json_find_member(_main, "orange_led");
    if ( led ) {
        if ( _set_mask(led, LED1_HOLD) < 0 ) return -1;
    }
    led = json_find_member(_main, "blue_led");
    if ( led ) {
        if ( _set_mask(led, LED2_HOLD) < 0 ) return -1;
    }
    return 0;
}
