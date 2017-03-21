/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "reloc_macro.h"
#include "wifi_thread.h"
#include "wifi_internal_thread.h"
#include "driver/include/m2m_wifi.h"

#define INT_STACK_LEN 0x1000

static TX_THREAD net_wifi_thread;
static int thread_start = 0;
static VOID net_wifi_internal_thread (ULONG initial_input);
static uint8_t wifi_internal_thread_stack[INT_STACK_LEN];

VOID net_wifi_internal_thread (ULONG initial_input) {
    SSP_PARAMETER_NOT_USED( initial_input );
    int i = 0;
    ioport_level_t lvl;
    while(1) {
        tx_thread_sleep ( CONV_MS_TO_TICK(10) );
        while (m2m_wifi_handle_events(NULL) != M2M_SUCCESS) { }
        if ( ( i++ % 100 ) == 0 ) {
            g_ioport.p_api->pinRead(IOPORT_PORT_06_PIN_00, &lvl);
            if (lvl==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00 ,IOPORT_LEVEL_HIGH);
            else g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00 ,IOPORT_LEVEL_LOW);
        }
    }
}

UINT wifi_internal_run() {
    if ( ! thread_start ) {
        thread_start = 1;
        return tx_thread_create(&net_wifi_thread,
                     (CHAR*)"wifi_internal_thread",
                     net_wifi_internal_thread,
                     (ULONG) NULL,
                     wifi_internal_thread_stack, INT_STACK_LEN,
                     3, 3, 1,
                     TX_AUTO_START);
    }
    return TX_SUCCESS;
}

void wifi_internal_stop() {
    tx_thread_terminate(&net_wifi_thread);
}
