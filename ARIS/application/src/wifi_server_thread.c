/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "reloc_macro.h"
#include "wifi_thread.h"
#include "wifi_server_thread.h"
#include "TRACE_USE.h"
#include "tx_port.h"

static TX_THREAD net_server_thread;
static uint8_t wifi_server_thread_stack[1024];
static uint8_t tmp_buffer[300];
static proc_client_func cli_proc = NULL;

static VOID net_wifi_server_thread (ULONG initial_input);


void set_client_cb(proc_client_func fnc) {
    cli_proc = fnc;
}

VOID net_wifi_server_thread (ULONG initial_input) {
//    NetworkStack *net = (NetworkStack *)initial_input;
//    wifi_socket_t *ccli = NULL;

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if ( server_sock < 0 ) {
        DBG("server socket fail %d", server_sock);
        return;
    }

/*    while(1) {
        if ( !ccli ) {
            ccli = net->sock;
            uint32_t state = 0;
            if ( tx_event_flags_get( &net->events,
                                     NET_EVENT_DHCP,
                                     TX_AND_CLEAR,
                                     &state,
                                     TX_NO_WAIT ) == TX_SUCCESS ) {
                struct sockaddr_in addr;
                addr.sin_family = AF_INET;
                addr.sin_port = _htons((80));
                addr.sin_addr.s_addr = 0;
                SOCKET server_sock = ccli->sock;
                sint8 err = bind(server_sock, (struct sockaddr*)&addr, sizeof(struct sockaddr));
                DBG("server: bind: %d [%d]", err, server_sock);

                // FIXME spec timeout?
                SSP_ASSERT( tx_event_flags_get( &ccli->flags,
                                                SOCK_BIND,
                                                TX_AND,
                                                &state,
                                                net->timeout_ms ) == TX_SUCCESS );

                int ret = listen(server_sock, 0);
                DBG("[server] listen: %d", ret);
                if ( ret < 0 ) TRACE("listen fail\r\n");
                SSP_ASSERT ( tx_event_flags_get( &ccli->flags,
                                                 SOCK_LISTEN,
                                                 TX_AND,
                                                 &state,
                                                 net->timeout_ms ) == TX_SUCCESS );

                ret = accept(server_sock, NULL, 0);
                if ( ret < 0 ) TRACE("accept fail\r\n");
                DBG("[server] accept: %d", ret);
            } else {
                tx_thread_sleep ( CONV_MS_TO_TICK(100) );
                continue;
            }
        }
        if ( ccli->sock < 0 ) {
            // socket is disable
            tx_thread_sleep ( CONV_MS_TO_TICK(10) );
            ccli = ccli->next;
            continue;
        }
        uint32_t state = 0;
        if ( tx_event_flags_get( &ccli->flags,
                                 SOCK_ACCEPT|SOCK_RECV|SOCK_TIMEOUT,
                                 TX_OR,
                                 &state,
                                 10 ) == TX_SUCCESS ) {
            if ( state & SOCK_ACCEPT ) {
                tx_event_flags_set(&ccli->flags, SOCK_CONNECT, TX_AND);
                tx_event_flags_set(&ccli->flags, SOCK_CONNECT, TX_OR);
                TRACE("new client!\r\n");
            }
            if ( state & SOCK_RECV ) {
                tx_event_flags_set(&ccli->flags, (uint32_t)~SOCK_RECV, TX_AND);
                DBG("recv [%d] %d", ccli->sock, ccli->buf.size);
                // TRACE(ccli->buf.buffer);
                if (cli_proc) {
                    cli_proc(ccli->sock, ccli->buf.size);
                    tx_thread_sleep(CONV_MS_TO_TICK(200));
                }
            } else if ( state & SOCK_TIMEOUT ) {
                DBG("timeout [%d]", ccli->sock);
                tx_event_flags_set(&ccli->flags, (uint32_t)~SOCK_TIMEOUT, TX_AND);
            }
        } else {
            // state not valid
            if ( tx_event_flags_get( &net->events,
                                     NET_EVENT_STATE_CONNECT,
                                     TX_OR,
                                     &state,
                                     TX_NO_WAIT ) == TX_SUCCESS ) {
                if ( ccli->sock > 0 ) {
                    if ( tx_event_flags_get( &ccli->flags,
                                             SOCK_WAIT_RX|SOCK_RESERVE,
                                             TX_OR,
                                             &state,
                                             TX_NO_WAIT ) != TX_SUCCESS ) {
                        tx_event_flags_set(&ccli->flags, SOCK_WAIT_RX, TX_OR);
                        DBG("wait recv from %d {%u, %08x}...", (int)ccli->sock, (unsigned int)state, (int)ccli);
                        recv(ccli->sock, tmp_buffer, 300, 0*net->timeout_ms);
                    }
                }
            }
        }
        ccli = ccli->next;
    }*/
}

UINT wifi_server_run(void *net) {
    return tx_thread_create(&net_server_thread,
                     (CHAR*)"wifi_server_thread",
                     net_wifi_server_thread,
                     (ULONG) net,
                     wifi_server_thread_stack, 1024,
                     3, 3, 1,
                     TX_AUTO_START);
}
