/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ARROW_ROUTINE_H_
#define ARROW_ROUTINE_H_

#if defined(__cplusplus)
extern "C" {
#endif
#include <arrow/gateway.h>
#include <arrow/device.h>

typedef enum {
#if defined(VALGRIND_TEST)
    ROUTINE_TEST_DONE   = -1000,
#endif
    ROUTINE_SUCCESS               = 0,
    ROUTINE_RECEIVE_EVENT         = 1,
    ROUTINE_ERROR                 = -300,
    ROUTINE_NOT_INITIALIZE        = -301,
    ROUTINE_GET_TELEMETRY_FAILED  = -302,
    ROUTINE_MQTT_PUBLISH_FAILED   = -303,
    ROUTINE_MQTT_CONNECT_FAILED   = -304,
    ROUTINE_MQTT_SUBSCRIBE_FAILED = -305
} arrow_routine_error_t;

typedef int (*get_data_cb)(void *);

// There is only one gateway and device for this space
// to get these static objects use the functions:
arrow_device_t *current_device(void);
arrow_gateway_t *current_gateway(void);
arrow_gateway_config_t *current_gateway_config(void);

// Initialize the gateway object and device object as well
// This function implemented the algorithm to get complete information about a gateway and device.
// The WDT function is used.
arrow_routine_error_t arrow_init(void);
arrow_routine_error_t arrow_deinit(void);

arrow_routine_error_t arrow_initialize_routine(void);

arrow_routine_error_t arrow_gateway_initialize_routine(void);

// Routine function for terminating current connections with the cloud
// and terminate all gateway/device information.
void arrow_close(void);

// If there is no the stored gateway information
// form the register gateway request and save a taken information
// In other case just form the gateway checkin request
int arrow_connect_gateway(arrow_gateway_t *gateway);

// If there is no any device information form the device register request
// and save the taken information
int arrow_connect_device(arrow_gateway_t *gateway, arrow_device_t *device);

// Routine function for telemetry sending to the cloud
// there is extremely needed the telemetry_serialize function implementation to serealize 'data' correctly
arrow_routine_error_t arrow_send_telemetry_routine(void *data);

// Funtion set the new state for this device
arrow_routine_error_t arrow_device_states_sync();
arrow_routine_error_t arrow_device_states_update();

// Routine for MQTT connection establishing
// Automatically prepare needed information and send it to the cloud MQTT
arrow_routine_error_t arrow_mqtt_connect_routine(void);
arrow_routine_error_t arrow_mqtt_disconnect_routine(void);
arrow_routine_error_t arrow_mqtt_terminate_routine(void);

// telemetry specific
arrow_routine_error_t arrow_mqtt_connect_telemetry_routine(void);
arrow_routine_error_t arrow_mqtt_disconnect_telemetry_routine(void);
arrow_routine_error_t arrow_mqtt_terminate_telemetry_routine(void);

// command specific
arrow_routine_error_t arrow_mqtt_connect_event_routine(void);
arrow_routine_error_t arrow_mqtt_subscribe_event_routine(void);
arrow_routine_error_t arrow_mqtt_disconnect_event_routine(void);
arrow_routine_error_t arrow_mqtt_terminate_event_routine(void);

// This routine send the telemetry data every TELEMETRY_DELAY msec
// using the data_cb function for forming current telemetry values
arrow_routine_error_t arrow_mqtt_send_telemetry_routine(get_data_cb data_cb, void *data);

arrow_routine_error_t arrow_mqtt_telemetry_routine(get_data_cb data_cb, void *data);
arrow_routine_error_t arrow_mqtt_telemetry_once_routine(get_data_cb data_cb, void *data);
arrow_routine_error_t arrow_mqtt_event_receive_routine();

#if defined(__cplusplus)
}
#endif

#endif /* ARROW_ROUTINE_H_ */
