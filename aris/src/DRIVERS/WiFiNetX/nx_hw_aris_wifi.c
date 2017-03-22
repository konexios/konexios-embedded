/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "nx_hw_aris_wifi.h"
#include "nx_api.h"
#include "wifi_thread.h"

#include "wifi_internal_thread.h"
#include "nx_debug.h"
#include "sf_el_nx_cfg.h"
#include "flasharis.h"

#define NX_ETHERNET_IP      0x0800
#define NX_ETHERNET_ARP     0x0806
#define NX_ETHERNET_RARP    0x8035

void wifiIRQIsr( external_irq_callback_args_t *p_args ) {
    ( void )( p_args );
    nm_bsp_wifi_chip_isr();
}

union ipaddr {
    uint8   au8DHCPServerIP[4];
    ULONG   netx_ip;
};

static NX_WF_REC *_wf;
static NX_PACKET *current;

static VOID do_receive(NX_IP *ip, NX_PACKET *packet_ptr) {
    UCHAR *pvMsg = packet_ptr->nx_packet_data_start + NX_PADDING_SIZE;
    USHORT ethertype = (USHORT) (((char*)(pvMsg))[NX_ETHERNET_SIZE - 2]<<8 | ((char*)(pvMsg))[NX_ETHERNET_SIZE - 1]);

//    DBG("do receive [%02x] %d {%08x}", ethertype,
//            (UINT)packet_ptr->nx_packet_length,
//            (UINT)packet_ptr->nx_packet_prepend_ptr);
    tx_mutex_get(&_wf->mtx, TX_WAIT_FOREVER);
    switch(ethertype) {
        case NX_ETHERNET_ARP:
            _nx_arp_packet_deferred_receive(ip, packet_ptr);
            break;
        case NX_ETHERNET_IP:
            _nx_ip_packet_deferred_receive(ip, packet_ptr);
            break;
        case NX_ETHERNET_RARP:
            _nx_rarp_packet_deferred_receive(ip, packet_ptr);
            break;
        default:
            NX_DBG("unknown pack type %04x", ethertype);
#ifdef DEBUG
            NX_DBGby8(packet_ptr->nx_packet_data_start, 3);
#endif
            nx_packet_release(packet_ptr);
            break;
    }
    tx_mutex_put(&_wf->mtx);
}

static int allocate_new_pack(NX_PACKET **pack, USHORT *len) {
    UINT status = nx_packet_allocate(_wf->ip_ptr->nx_ip_default_packet_pool, pack, NX_RECEIVE_PACKET, TX_WAIT_FOREVER);
    if ( status != NX_SUCCESS ) {
        NX_DBG("allocate fail %d", status);
        return NX_NOT_SUCCESSFUL;
    }
    (*pack)->nx_packet_prepend_ptr = (*pack)->nx_packet_data_start + NX_PADDING_SIZE + NX_ETHERNET_SIZE;
    *len = (USHORT)((USHORT)((*pack)->nx_packet_data_end - (*pack)->nx_packet_data_start) - NX_PADDING_SIZE);
    return NX_SUCCESS;
}

static void _appEthCb(uint8 u8MsgType, void * pvMsg, void * pvCtrlBuf) {
    if ( u8MsgType == M2M_WIFI_RESP_ETHERNET_RX_PACKET ) {
        NX_PACKET *allocated_packet_ptr;
        tstrM2mIpCtrlBuf *ipCtrlBuf = pvCtrlBuf;
        if ( !current ) {
            NX_DBG("receiving process crashed");
            return;
        }

        current->nx_packet_length = (ULONG)(ipCtrlBuf->u16DataSize - NX_ETHERNET_SIZE);
//        current->nx_packet_prepend_ptr = (UCHAR*)pvMsg + NX_ETHERNET_SIZE;
        current->nx_packet_append_ptr = current->nx_packet_prepend_ptr + current->nx_packet_length;

        USHORT allocated_size;
        if ( allocate_new_pack(&allocated_packet_ptr, &allocated_size) != NX_SUCCESS ) {
            return;
        }
        m2m_wifi_set_receive_buffer(allocated_packet_ptr->nx_packet_data_start + NX_PADDING_SIZE, (uint16) (allocated_size));

        NX_DBG("app ETH fill %p [%d] - new buffer %p \\ %d",
                (UINT) pvMsg,
                ipCtrlBuf->u16DataSize,
                (UINT)(allocated_packet_ptr->nx_packet_prepend_ptr),
                allocated_size);

        do_receive(_wf->ip_ptr, current);
        current = allocated_packet_ptr;
    }
}

UINT nx_arrow_wifi_init(NX_WF_REC *nx_rec_ptr) {

    nm_bsp_init (&g_wifi_irq);
    nm_bus_config (&g_sf_spi_device);
    _wf = nx_rec_ptr;

    USHORT allocated_size;
    if ( allocate_new_pack(&current, &allocated_size) != NX_SUCCESS ) {
        return NX_NOT_SUCCESSFUL;
    }

    tstrWifiInitParam param;
    memset((uint8_t *)&param, 0x00, sizeof(tstrWifiInitParam));
//    param.pfAppWifiCb = wifi_event_cb;
    param.strEthInitParam.au8ethRcvBuf = current->nx_packet_data_start + NX_PADDING_SIZE;
    param.strEthInitParam.u16ethRcvBufSize = allocated_size;
    param.strEthInitParam.u8EthernetEnable = M2M_WIFI_MODE_ETHERNET;
    param.strEthInitParam.pfAppEthCb = _appEthCb;
//    param.strEthInitParam.pfAppWifiCb = _appWifiCb;

    SSP_ASSERT( m2m_wifi_init(&param) == M2M_SUCCESS );

    if ( nx_rec_ptr->mode == AP ) {
        tstrM2MAPConfig strM2MAPConfig;
        memset(&strM2MAPConfig, 0x00, sizeof(tstrM2MAPConfig));
        strcpy((char *)&strM2MAPConfig.au8SSID, "ARIS_WIFI");
        strM2MAPConfig.u8ListenChannel = 6;
        strM2MAPConfig.u8SecType = 1; //open
        union ipaddr ip;
        NX_DBG("name: %s", nx_rec_ptr->ether_interface_ptr->nx_interface_name);
        ip.netx_ip = nx_rec_ptr->ether_interface_ptr->nx_interface_ip_address;
        strM2MAPConfig.au8DHCPServerIP[0] = ip.au8DHCPServerIP[3];
        strM2MAPConfig.au8DHCPServerIP[1] = ip.au8DHCPServerIP[2];
        strM2MAPConfig.au8DHCPServerIP[2] = ip.au8DHCPServerIP[1];
        strM2MAPConfig.au8DHCPServerIP[3] = ip.au8DHCPServerIP[0];
        /* Bring up AP mode with parameters structure. */
        NX_DBG("AP mode init...");
        int ret = m2m_wifi_enable_ap(&strM2MAPConfig);
        if (M2M_SUCCESS != ret) {
            NX_DBG("m2m ap enable not successful");
            return NX_NOT_SUCCESSFUL;
        }
    } else if ( nx_rec_ptr->mode == STA ) {
        NX_DBG("STA mode init...");
        uint8_t tmp[100];
        flash_storage_read(tmp, 100);

        int *client_magic = (int*)tmp;
        char *client_ssid = (char*)(tmp+sizeof(int));
        char *client_pass = client_ssid+strlen(client_ssid)+1;
        int *client_sec = (int*)(client_pass+strlen(client_pass)+1);

        if ( (int)*client_magic != (int)MAGIC_NUMBER_FLASH ) {
            NX_DBG("mg: [%d, %d]", *client_magic, MAGIC_NUMBER_FLASH);
            return NX_NOT_SUCCESSFUL;
        }
//        CHAR *client_ssid = "TP-LINK_POCKET_3020_403A9A";
//        CHAR *client_pass = "53319579";
//        int sk = M2M_WIFI_SEC_WPA_PSK;
//        int *client_sec = &sk;

        NX_DBG("ssid: [%s]", client_ssid);
        NX_DBG("pass: [%s]", client_pass);
        NX_DBG("sec:  [%d]", *client_sec);

        if ( *client_sec == M2M_WIFI_SEC_INVALID || *client_sec > M2M_WIFI_SEC_802_1X ) return NX_NOT_SUCCESSFUL;

        int ret = m2m_wifi_connect( client_ssid,
                                (uint8)strlen(client_ssid),
                                (uint8)*client_sec,
                                client_pass,
                                M2M_WIFI_CH_ALL );

        if ( ret != M2M_SUCCESS ) {
            NX_DBG("m2m wifi connection not successful");
            return NX_NOT_SUCCESSFUL;
        } else {
            NX_DBG("connected");
        }
    }

    SSP_ASSERT( wifi_internal_run(_wf) == TX_SUCCESS );
    tx_thread_sleep(100);

    return NX_SUCCESS;
}
