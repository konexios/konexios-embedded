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
#include "nx_debug.h"

#define INT_STACK_LEN 1512

static TX_THREAD net_wifi_thread;
static int thread_start = 0;
static VOID net_wifi_internal_thread (ULONG initial_input);
static uint8_t wifi_internal_thread_stack[INT_STACK_LEN];

static void wifi_handle_tx_packets(NX_WF_REC *wf) {
    NX_PACKET *current;

    // pop one pack
    tx_mutex_get(&wf->mtx, TX_WAIT_FOREVER);
    current = wf->tx_packs;
    wf->tx_packs = current ? current->nx_packet_next : NULL;
    tx_mutex_put(&wf->mtx);

//    while(current)
    if ( current )
    {
        NX_DBG("m2m_wifi_send_ethernet_pkt |%08x|:: (%d) {%08x}",
                (UINT)current, (UINT)current->nx_packet_length, (UINT)current->nx_packet_prepend_ptr);
        sint8 status = m2m_wifi_send_ethernet_pkt((uint8*)current->nx_packet_prepend_ptr, (uint16)current->nx_packet_length);
        current->nx_packet_next = 0;
        NX_BACKOFF_RELEASE(current);
        // tx done
        if ( status != NX_SUCCESS ) {
            NX_DBG("m2m_wifi_send_ethernet_pkt fail: %d", status);
            return; // NX_NOT_SUCCESSFUL;
        }
    }
}


VOID net_wifi_internal_thread (ULONG initial_input) {
    NX_WF_REC *wf_cfg = (NX_WF_REC*)initial_input;
    int i = 0;
    ioport_level_t lvl;
    while(1) {
        tx_thread_sleep(CONV_MS_TO_TICK(1));

        wifi_handle_tx_packets(wf_cfg);

        m2m_wifi_handle_events(NULL);
//        while (m2m_wifi_handle_events(NULL) != M2M_SUCCESS) { }

        if ( ( i++ % 250 ) == 0 ) {
            g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_14, &lvl);
            if (lvl==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(IOPORT_PORT_00_PIN_14 ,IOPORT_LEVEL_HIGH);
            else g_ioport.p_api->pinWrite(IOPORT_PORT_00_PIN_14 ,IOPORT_LEVEL_LOW);
        }
    }
}

UINT wifi_internal_run(NX_WF_REC *wf) {
    if ( ! thread_start ) {
        thread_start = 1;
        return tx_thread_create(&net_wifi_thread,
                     (CHAR*)"wifi_internal_thread",
                     net_wifi_internal_thread,
                     (ULONG) wf,
                     wifi_internal_thread_stack, INT_STACK_LEN,
                     3, 3, 1,
                     TX_AUTO_START);
    }
    return TX_SUCCESS;
}

void wifi_internal_stop() {
    tx_thread_terminate(&net_wifi_thread);
}
