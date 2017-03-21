#ifndef BLE_EVENT_H
#define BLE_EVENT_H

typedef enum
{
    BLE_EVENT_NONE = 0,
    BLE_EVENT_TURN_LED1_ON,     /* BLE turn LED1 ON */
    BLE_EVENT_TURN_LED1_OFF,    /* BLE turn LED1 OFF */
    BLE_EVENT_TURN_LED2_ON,     /* BLE turn LED2 ON */
    BLE_EVENT_TURN_LED2_OFF,    /* BLE turn LED2 OFF */
    BLE_EVENT_ACK,              /* BLE ack */
    BLE_EVENT_CONNECTED,        /* BLE Connected */
    BLE_EVENT_DISCONNECTED,     /* BLE disconnected */
} ble_event_t;

#endif
