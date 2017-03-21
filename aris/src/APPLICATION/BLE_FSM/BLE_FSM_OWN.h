#ifndef BLE_FSM_OWN_H
#define BLE_FSM_OWN_H

#include "BLE_FSM.h"
#include "sens_type.h"

typedef enum BLE_FSM_STATE_{
    BLE_IDLE_STATE=0,
    BLE_REFRESH_DATA_STATE,
    BLE_WAIT_ACK_STATE,
    BLE_DELAY_STATE,
} BLE_FSM_STATE;

struct BLE_FSM_
{
    BLE_FSM_STATE current_state;
    uint32_t counter;
    sens_type_t selected_sensor;
    sens_type_t last_sensor;
    uint32_t delay_time_ms;
    uint32_t wait_ack_timeout_ms;
#ifndef SKIP_INTEGRITY_CHECK
    BLE_FSM* self;
#endif
};

void BLE_FSM_build( BLE_FSM* this_,
                    uint32_t delay_time_ms,
                    uint32_t wait_ack_timeout_ms );
bool BLE_FSM_destroy( BLE_FSM* this_ );

#endif
