/*
 * nx_aris_wifi.h
 *
 *  Created on: 29 нояб. 2016 г.
 *      Author: ddemidov
 */

#ifndef NX_ARIS_WIFI_H_
#define NX_ARIS_WIFI_H_

#include "nx_api.h"
#include "sf_el_nx_cfg.h"
#include "common/include/nm_common.h"

VOID nx_wifi_ap_wlan0(NX_IP_DRIVER *driver_req_ptr);
VOID nx_wifi_sta_wlan0(NX_IP_DRIVER *driver_req_ptr);

/* bits in the status word of the tx / rx descriptors */
#ifndef  DESCRIPTOR_FLAG_ACTIVE
#define DESCRIPTOR_FLAG_ACTIVE      0x80000000  /* TACT/RACT - Tx/Rx descriptor active */
#define DESCRIPTOR_FLAG_LAST        0x40000000  /* TDLE/RDLE - Tx/RX descriptor list last */
#define FRAME_POSITION_HEAD_END     0x30000000  /* Define Frame Position bits */
#define FRAME_POSITION_HEAD         0x20000000  /* Define Frame Position -- Head */
#define FRAME_POSITION_END          0x10000000  /* Define Frame Position -- End */
#define FRAME_POSITION_MID          0x00000000  /* Define Frame Position -- Middle */

#define  TX_QUEUE_DEPTH    10  /* Netx packets queued before dropping */
#define  NX_PADDING_SIZE   2U

#define NX_ETHERNET_IP 0x0800
#define NX_ETHERNET_SIZE 14
#define NX_ETHERNET_ARP 0x0806
#define NX_ETHERNET_RARP 0x8035

#ifndef NX_ENET_MAX_MTU
#define NX_ENET_MAX_MTU     1518    /* max Ethernet packet size (14+ 1500 +2) */
#endif

#define NX_NOT_INITIALIZED       0x0000

#define NX_INITIALIZE_DONE       0x0002U    /* device is Initialized */
#define NX_LINK_ENABLED          0x0004U    /* device is enabled*/

#define NX_CANNOT_INITSTORAGE    0x0001U
#define NX_CANNOT_INITHARDWARE   0x0002U

#endif

enum {
    AP=0,
    STA
};

enum {
    NO_ACTIVITY = 0,
    RX_ACTIVITY = (1<<0),
    TX_ACTIVITY = (1<<1),
    RX_DONE     = (1<<2),
    TX_DONE     = (1<<3)
};

/* Driver record structure - keep related info in one place */
typedef struct NX_REC_WF_STRUCT
{
    UINT         nx_state ;   /* state of this driver */
    NX_IP        *ip_ptr;      /* NetX IP structure handling this controller */
    NX_INTERFACE *ether_interface_ptr;

    TX_MUTEX    mtx;
    UINT        mode;
    TX_EVENT_FLAGS_GROUP flags;
    NX_PACKET *tx_packs;

#ifdef NX_DRIVER_ENABLE_DEFERRED
    UINT        nx_driver_deferred_events;
#endif

}   NX_WF_REC;

#define NX_BACKOFF_RELEASE(p) \
{ \
    (p) -> nx_packet_prepend_ptr += NX_ETHERNET_SIZE;  \
    (p) -> nx_packet_length -= NX_ETHERNET_SIZE;    \
    nx_packet_transmit_release(p); \
}

#endif /* NX_ARIS_WIFI_H_ */
