#ifndef SENS_TYPE_H
#define SENS_TYPE_H

typedef enum
{
    BLE_FSM_SENS_NONE=0,
    BLE_FSM_SENS_TEMP,
    BLE_FSM_SENS_HUM,
    BLE_FSM_SENS_ACCX,
    BLE_FSM_SENS_ACCY,
    BLE_FSM_SENS_ACCZ,
    BLE_FSM_SENS_GYROX,
    BLE_FSM_SENS_GYROY,
    /* --- */
    BLE_FSM_SENS_NUM,
} sens_type_t;

#endif
