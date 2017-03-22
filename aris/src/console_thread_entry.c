/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#define MODULE_NAME "Console"

#include "console_thread.h"
#include "APPLICATION/threads_sync_events.h"
#include "TRACE_OWN.h"
#include "TRACE_USE.h"
#include "reloc_macro.h"
#include "reloc_assert.h"
#include "version.h"
#include "THREADS_SYNC/THREADS_SYNC_USE.h"
#include "threads_sync_events.h"

void console_thread_entry(void);

/* Console Thread entry function */
void console_thread_entry(void) {
    g_sf_trace.p_api->open(g_sf_trace.p_ctrl, g_sf_trace.p_cfg);
    TRACE_build( &g_sf_trace );
    tx_thread_sleep(CONV_MS_TO_TICK(500));

    ASSERT( THREADS_SYNC_setEvent( THREAD_CONSOLE_RDY_EV ) == true );
    ASSERT( THREADS_SYNC_wait( THREADS_ALL_RDY_EV, TX_WAIT_FOREVER ) == true );

    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00 ,IOPORT_LEVEL_HIGH);

    TRACE(BOOT_HEADER_STRING);

    while (1) {
        tx_thread_sleep ( CONV_MS_TO_TICK(1000) );
    }
}
