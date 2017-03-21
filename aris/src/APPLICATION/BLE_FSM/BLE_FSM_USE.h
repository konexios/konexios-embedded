#ifndef BLE_FSM_USE_H
#define BLE_FSM_USE_H

#include "BLE_FSM.h"
#include "ble_event.h"
#include "sens_type.h"

sens_type_t BLE_FSM_run(    BLE_FSM* this_,
                            ble_event_t inEvent,
                            uint32_t time_elapsed_ms );

#endif
