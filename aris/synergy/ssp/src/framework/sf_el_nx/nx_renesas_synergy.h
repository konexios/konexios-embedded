/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2016 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/
/**************************************************************************
 * File: nx_renesas.h
 *
 *  Includes all the ethernet device driver related header files in one 
 *  place. This file will be only one that needs to be included into any
 *  ethernet device driver code file.
 *
 *  It may need to be changed to add or delete features.
 **************************************************************************/
                                                        
#ifndef     NX_RENESAS_H
#define     NX_RENESAS_H


#ifdef   __cplusplus
/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {
#endif

#include "sf_el_nx_cfg.h"

/* Determine if the driver uses IP deferred processing or direct ISR processing.  */
/* #define NX_DRIVER_ENABLE_DEFERRED */

#define  MAX_ENET_INSTANCES  2

#define  TX_QUEUE_DEPTH    10  /* Netx packets queued before dropping */
#define  NX_PADDING_SIZE   2U

#ifndef NX_ENET_MAX_MTU
#define NX_ENET_MAX_MTU     1518    /* max Ethernet packet size (14+ 1500 +2) */
#endif

#define NX_NOT_INITIALIZED       0x0000

#define NX_INITIALIZE_DONE       0x0002U    /* device is Initialized */
#define NX_LINK_ENABLED          0x0004U    /* device is enabled*/

#define NX_CANNOT_INITSTORAGE    0x0001U
#define NX_CANNOT_INITHARDWARE   0x0002U

#define NX_DRIVER_EVENT_POLLING_INTERVAL_SHORT   500
#define NX_DRIVER_EVENT_POLLING_INTERVAL_LONG    1000

#ifdef NX_DRIVER_ENABLE_DEFERRED
#define NX_DRIVER_DEFERRED_PACKET_RECEIVED      1
#define NX_DRIVER_DEFERRED_PACKET_TRANSMITTED   4
#endif

/*** macros ***/
#define NX_BACKOFF_RELEASE(p)   \
{   \
    p -> nx_packet_prepend_ptr =  p -> nx_packet_prepend_ptr + NX_ETHERNET_SIZE;  \
    p -> nx_packet_length =  p -> nx_packet_length - NX_ETHERNET_SIZE;    \
    nx_packet_transmit_release(p); \
}
    
/* Descriptor structure */
typedef struct EMAC_BD_STRUCT
{
    unsigned long       bd_status;         /* Status - 32 bits */
    unsigned short      bd_rxdatalength;
    unsigned short      bd_bufsize;
    unsigned char      *bd_buffer_ptr;     /* Buffer pointer */
    NX_PACKET          *bd_nx_packet;      /* Padding used to associate descriptor with nx_packet */
                                           /* Allows for backoff release in TX interrupt  */
} EMAC_BD;


/* bits in the status word of the tx / rx descriptors */
#define DESCRIPTOR_FLAG_ACTIVE      0x80000000  /* TACT/RACT - Tx/Rx descriptor active */
#define DESCRIPTOR_FLAG_LAST        0x40000000  /* TDLE/RDLE - Tx/RX descriptor list last */
#define FRAME_POSITION_HEAD_END     0x30000000  /* Define Frame Position bits */
#define FRAME_POSITION_HEAD         0x20000000  /* Define Frame Position -- Head */
#define FRAME_POSITION_END          0x10000000  /* Define Frame Position -- End */
#define FRAME_POSITION_MID          0x00000000  /* Define Frame Position -- Middle */


#define FE                          0x08000000  /* TFE - Tx Frame Error */
                                    /* or RFE - Rx Frame Error */


/* RFS - bit 26-0, bit 9, 7, 4-0 are used for error status */
#define RMAF                        0x00800000

/* defines for the EESR bit values, same bit values can be used for the EESIPR */

#define EESR_TWB    0x40000000  /* Write back complete */
#define EESR_TABT   0x04000000  /* Transmit Abort */
#define EESR_RABT   0x02000000  /* Receive Abort */
#define EESR_RFCOF  0x01000000  /* receive frame counter overflow */
#define EESR_ADE    0x00800000  /* Adress error */
#define EESR_ECI    0x00400000  /* EtherC States Register interrupt */
#define EESR_TC     0x00200000  /* Tx complete */
#define EESR_TDE    0x00100000  /* Tx Descriptor Exhausted */
#define EESR_TFUF   0x00080000  /* Tx FIFO Underflow */
#define EESR_FR     0x00040000  /* Frame Received */
#define EESR_RDE    0x00020000  /* Rx Descriptor Exhausted */
#define EESR_RFOF   0x00010000  /* Rx FIFO overflow */
#define EESR_CND    0x00000800  /* Carrier not detected */
#define EESR_DLC    0x00000400  /* Detect Loss of Carrier */
#define EESR_CD     0x00000200  /* Collision Detect */
#define EESR_TRO    0x00000100  /* Tx retry over */
#define EESR_RMAF   0x00000080  /* Receive Multicast Addresss Frame */
#define EESR_RRF    0x00000010  /* Receive Residual bit frame */
#define EESR_RTLF   0x00000008  /* Receive Too-Long Frame */
#define EESR_RTSF   0x00000004  /* Receive too-short Frame */
#define EESR_PRE    0x00000002  /* PHY-LSI Receive Error */
#define EESR_CERF   0x00000001  /* CRC Error on Receive Frame */

#define RFS_BITS    0x000003FF

/* EESIPR error in packets */
#define EESIPR_PKTERRORS ( EESR_RRF | EESR_RTLF | EESR_RTSF | EESR_CERF)
#define EESIPR_BUFERRORS (EESR_RFCOF | EESR_TDE | EESR_TFUF | EESR_RDE | EESR_RFOF)   
#define EESIPR_COMERRORS (EESR_CND |EESR_DLC | EESR_CD | EESR_TRO | EESR_PRE)

#define EESIPR_ALLERRORS (EESIPR_PKTERRORS | EESIPR_BUFERRORS | EESIPR_COMERRORS)

/* current value of EESIPR */
#define EESIPR_CURSETTING (EESR_TC|EESR_FR)

/* Driver record structure - keep related info in one place */
typedef struct NX_REC_STRUCT
{
    UINT         nx_state ;   /* state of this driver */
    NX_IP        *ip_ptr;      /* NetX IP structure handling this controller */
    NX_INTERFACE *ether_interface_ptr;

    /* Allocate space for the BDs.  Build one extra BD there because
       the BDs must be 16 byte aligned. */
    UCHAR        rx_bd_space[sizeof(EMAC_BD) * (NUM_RX_DESC + 1) ];
    UCHAR        tx_bd_space[sizeof(EMAC_BD) * (NUM_TX_DESC + 1) ];

    EMAC_BD     *driver_rx_bd;
    EMAC_BD     *driver_tx_bd;
    
    ULONG       driver_tx_bd_index;
    ULONG       driver_tx_bd_in_use;
    ULONG       driver_tx_release_index;
    ULONG       driver_rx_bd_index;

    
    UINT         driver_packets_queued;
    NX_PACKET   *driver_tx_packet_queue;
    NX_PACKET   *driver_tx_packet_queue_end;
    
    R_ETHERC0_Type *etherc_ptr;
    R_EDMAC0_Type  *edmac_ptr;
    
#ifdef NX_DRIVER_INTERNAL_PACKET_POOL
    NX_PACKET_POOL * receive_pool;
#endif    

    TX_TIMER    driver_event_polling_timer;

    UINT        link_established;

#ifdef NX_DRIVER_ENABLE_DEFERRED
    UINT        nx_driver_deferred_events;
#endif

}   NX_REC;

/*************** External function prototypes ************************/
extern void _nx_ether_interrupt(NX_REC *nx_rec_ptr);
VOID  nx_ether_driver (NX_IP_DRIVER *driver_req_ptr, NX_REC *nx_rec_ptr);


/* Determine if a C++ compiler is being used.  If so, complete the
 * standard C conditional started above.  */
#ifdef   __cplusplus
        }
#endif

#endif      
