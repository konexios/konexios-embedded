/*
 * nx_aris_wifi.c
 *
 *  Created on: 28 нояб. 2016 г.
 *      Author: ddemidov
 */


#include "bsp_api.h"

#include "tx_api.h"
#include "nx_api.h"
#include "sf_el_nx_cfg.h"

#include "nx_aris_wifi.h"
#include "nx_hw_aris_wifi.h"

#include <debug.h>
//#include "TRACE_USE.h" // FIXME delete
#include "reloc_macro.h"
#include "driver/include/m2m_wifi.h"

NX_WF_REC nx_record;
extern NX_PACKET_POOL pool_1;

VOID nx_wifi_driver(NX_IP_DRIVER *driver_req_ptr, NX_WF_REC *nx_req);

VOID nx_wifi_ap_wlan0(NX_IP_DRIVER *driver_req_ptr) {
    nx_record.mode = AP;
    nx_wifi_driver(driver_req_ptr, &nx_record);
}

VOID nx_wifi_sta_wlan0(NX_IP_DRIVER *driver_req_ptr) {
    nx_record.mode = STA;
    nx_wifi_driver(driver_req_ptr, &nx_record);
}

#define WIDTH  (8 * sizeof(uint8_t))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0xD8

static UINT nx_wifi_write(NX_PACKET *packet, NX_WF_REC *nx_rec_ptr) {
    NX_PACKET *current;
    tx_mutex_get(&nx_rec_ptr->mtx, TX_WAIT_FOREVER);
    current = nx_rec_ptr->tx_packs;
    while(current && current->nx_packet_next){
//        DBG(" -> %08x", (UINT)current);
        current = current->nx_packet_next;
    }
    if (current) {
        current->nx_packet_next = packet;
    } else {
        // first element
        nx_rec_ptr->tx_packs = packet;
    }
//    DBG("curr (%08x)->(%08x)", (UINT)current, (UINT)packet);
    tx_mutex_put(&nx_rec_ptr->mtx);

    return NX_SUCCESS;
}

static UINT nx_wifi_send(NX_PACKET *packet, NX_WF_REC *nx_rec_ptr) {
    if (nx_rec_ptr->nx_state & NX_LINK_ENABLED) {
        if ( nx_wifi_write(packet, nx_rec_ptr) != NX_SUCCESS ) {
            NX_BACKOFF_RELEASE(packet);
            return NX_TX_QUEUE_DEPTH;
        }
    } else {
        NX_BACKOFF_RELEASE(packet);
        return NX_NOT_ENABLED;
    }

    return NX_SUCCESS;
}

VOID nx_wifi_driver(NX_IP_DRIVER *driver_req_ptr, NX_WF_REC *nx_rec_ptr) {
    UINT status;
    NX_PACKET *packet_ptr;
    ULONG *ethernet_frame_ptr;
    NX_INTERFACE *interface_ptr;
    UINT driver_command = driver_req_ptr->nx_ip_driver_command;

    driver_req_ptr->nx_ip_driver_status = NX_SUCCESS;

    DBG("start wifi driver %d", driver_command);

    switch (driver_command) {
        case NX_LINK_INTERFACE_ATTACH :
            memset(nx_rec_ptr, 0, sizeof(NX_WF_REC));
            nx_rec_ptr->ether_interface_ptr = driver_req_ptr->nx_ip_driver_interface;
            tx_mutex_create(&nx_rec_ptr->mtx, "NETX mutex", TX_NO_INHERIT);
            if ( tx_event_flags_create(&nx_rec_ptr->flags, "txrx_events") != TX_SUCCESS ) {
                DBG("tx_event_flags_create fail");
            }
            if ( tx_event_flags_set(&nx_rec_ptr->flags, NO_ACTIVITY, TX_AND) != TX_SUCCESS ) {
                DBG("tx_event_flags_set fail (init)");
            }
            break;

        case NX_LINK_INITIALIZE:
            if ( nx_rec_ptr->nx_state == NX_NOT_INITIALIZED ) {
                nx_rec_ptr->ip_ptr = driver_req_ptr->nx_ip_driver_ptr;
                interface_ptr = driver_req_ptr->nx_ip_driver_interface;
                interface_ptr->nx_interface_ip_mtu_size = NX_ENET_MAX_MTU - NX_ETHERNET_SIZE - 4;
                status = nx_arrow_wifi_init(nx_rec_ptr);
                if ( status != NX_SUCCESS ) {
                    nx_rec_ptr->nx_state = NX_CANNOT_INITHARDWARE;
                    driver_req_ptr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
                    break;
                }

                UCHAR mac[6];
                m2m_wifi_get_mac_address(mac);

                interface_ptr->nx_interface_physical_address_msw  = (ULONG)mac[0]<<8;
                interface_ptr->nx_interface_physical_address_msw |= (ULONG)mac[1];

                interface_ptr->nx_interface_physical_address_lsw  = (ULONG)mac[2]<<24;
                interface_ptr->nx_interface_physical_address_lsw |= (ULONG)mac[3]<<16;
                interface_ptr->nx_interface_physical_address_lsw |= (ULONG)mac[4]<<8;
                interface_ptr->nx_interface_physical_address_lsw |= (ULONG)mac[5];

                interface_ptr->nx_interface_address_mapping_needed = NX_TRUE;
                interface_ptr->nx_interface_link_up = NX_FALSE;

                nx_rec_ptr->tx_packs = NULL;
                nx_rec_ptr->nx_state |= NX_INITIALIZE_DONE;
            }
            break;

        case NX_LINK_ENABLE:
            interface_ptr = driver_req_ptr->nx_ip_driver_interface;
            if ( !(nx_rec_ptr->nx_state & NX_INITIALIZE_DONE) ) {
                driver_req_ptr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
                break;
            }
            if ( nx_rec_ptr->nx_state & NX_LINK_ENABLED ) {
                driver_req_ptr->nx_ip_driver_status = NX_ALREADY_ENABLED;
            } else {
                nx_rec_ptr->nx_state = nx_record.nx_state | NX_LINK_ENABLED;
                driver_req_ptr->nx_ip_driver_status = NX_SUCCESS;
            }
            interface_ptr->nx_interface_link_up = NX_TRUE;
            break;

        case NX_LINK_PACKET_SEND:
        case NX_LINK_PACKET_BROADCAST:
        case NX_LINK_ARP_SEND:
        case NX_LINK_ARP_RESPONSE_SEND:
        case NX_LINK_RARP_SEND:
            if ( !(nx_rec_ptr->nx_state & NX_LINK_ENABLED) ) {
                driver_req_ptr->nx_ip_driver_status = NX_NOT_ENABLED;
                nx_packet_transmit_release(driver_req_ptr->nx_ip_driver_packet);
                break;
            }
            packet_ptr = driver_req_ptr->nx_ip_driver_packet;
            packet_ptr->nx_packet_prepend_ptr -= NX_ETHERNET_SIZE;
            packet_ptr->nx_packet_length += NX_ETHERNET_SIZE;

            ethernet_frame_ptr = (ULONG*)(packet_ptr->nx_packet_prepend_ptr-NX_PADDING_SIZE);

            ULONG total_packets;
            ULONG free_packets;
            ULONG empty_pool_requests;
            ULONG empty_pool_suspensions;
            ULONG invalid_packet_releases;

            nx_packet_pool_info_get(&pool_1, &total_packets, &free_packets, &empty_pool_requests, &empty_pool_suspensions, &invalid_packet_releases);

            DBG("total: %d, free: %d, {%d, %d, %d}", total_packets, free_packets,
                    empty_pool_requests, empty_pool_suspensions, invalid_packet_releases);

            if ( (driver_command==NX_LINK_PACKET_SEND) || (driver_command==NX_LINK_ARP_RESPONSE_SEND) ) {
                *ethernet_frame_ptr = driver_req_ptr->nx_ip_driver_physical_address_msw;
                *(ethernet_frame_ptr+1) = driver_req_ptr->nx_ip_driver_physical_address_lsw;
            } else {
                *ethernet_frame_ptr = 0x0000ffffUL;
                *(ethernet_frame_ptr+1) = 0xffffffffUL;
            }
            interface_ptr = nx_rec_ptr->ether_interface_ptr;
            *(ethernet_frame_ptr+2) = (interface_ptr->nx_interface_physical_address_msw << 16) |
                    (interface_ptr->nx_interface_physical_address_lsw >> 16);
            *(ethernet_frame_ptr+3) = (interface_ptr->nx_interface_physical_address_lsw << 16);

            if ( (driver_req_ptr->nx_ip_driver_command == NX_LINK_ARP_RESPONSE_SEND)||
                 (driver_req_ptr->nx_ip_driver_command == NX_LINK_ARP_SEND) ) {
                *(ethernet_frame_ptr+3) |= NX_ETHERNET_ARP;
            } else  if (driver_req_ptr->nx_ip_driver_command == NX_LINK_RARP_SEND) {
                *(ethernet_frame_ptr+3) |= NX_ETHERNET_RARP;
            }
            *(ethernet_frame_ptr+3) |= NX_ETHERNET_IP;

            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr));
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+1));
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+2));
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+3));

            driver_req_ptr->nx_ip_driver_status = nx_wifi_send(packet_ptr, nx_rec_ptr);
            driver_req_ptr->nx_ip_driver_packet = NULL;

            break;

        default:
            DBG("NX_UNHANDLED_COMMAND");
            driver_req_ptr->nx_ip_driver_status = NX_UNHANDLED_COMMAND;
            break;
    }
}
