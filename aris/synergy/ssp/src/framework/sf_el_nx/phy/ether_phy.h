/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : ether_phy.h
 * Description  : Ethernet PHY device driver header file
 **********************************************************************************************************************/


#ifndef _SC32_PHY_H_
#define _SC32_PHY_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* PHY return definitions */
#define R_PHY_OK    (0)
#define R_PHY_ERROR (-1)

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
#define ETHER_FLAG_OFF         (0)
#define ETHER_FLAG_ON          (1)
#define ETHER_FLAG_ON_LINK_ON  (3)
#define ETHER_FLAG_ON_LINK_OFF (2)
/* Media Independent Interface */

#define LINK_STAT_MASK         (1U << 2)
#define PHY_MII_ST             (1U << 14)
#define PHY_MII_READ           (0x2 << 12)
#define PHY_MII_WRITE          (0x1U << 12)
#define PHY_MII_ADDRESS        (0x00)   /* @note Configures PHY address 3b'000 due to PE-HMI and DKS7G2 boards
                                                  does not configure PHY address */

/* Define the access timing of MII/RMII register */
#define ETHER_CFG_PHY_MII_WAIT (200)             /* Please define the value of 8 or more */

/* Define the waiting time for reset completion of PHY-LSI */
#define ETHER_CFG_PHY_DELAY_RESET (0x00020000L)

/* Define the waiting time for link-up completion of PHY-LSI */
#define ETHER_CFG_PHY_LINKUP_WAIT (700)

/* Standard PHY Registers */
#define PHY_REG_CONTROL          (0x0)
#define PHY_REG_STATUS           (0x1)
#define PHY_REG_IDENTIFIER1      (0x2)
#define PHY_REG_IDENTIFIER2      (0x3)
#define PHY_REG_AN_ADVERTISEMENT (0x4)
#define PHY_REG_AN_LINK_PARTNER  (0x5)
#define PHY_REG_AN_EXPANSION     (0x6)

/* Vendor Specific PHY Registers */
/* KSZ8091RNB */
#define PHY_REG_PHY_CONTROL_1 (0x1E)
#define PHY_REG_PHY_CONTROL_2 (0x1F)

/* Basic Mode Control Register Bit Definitions */
#define PHY_CONTROL_RESET       (1U << 15)
#define PHY_CONTROL_LOOPBACK    (1U << 14)
#define PHY_CONTROL_100_MBPS    (1U << 13)
#define PHY_CONTROL_AN_ENABLE   (1U << 12)
#define PHY_CONTROL_POWER_DOWN  (1U << 11)
#define PHY_CONTROL_ISOLATE     (1U << 10)
#define PHY_CONTROL_AN_RESTART  (1U << 9)
#define PHY_CONTROL_FULL_DUPLEX (1U << 8)
#define PHY_CONTROL_COLLISION   (1U << 7)

/* Basic Mode Status Register Bit Definitions */
#define PHY_STATUS_100_T4        (1U << 15)
#define PHY_STATUS_100F          (1U << 14)
#define PHY_STATUS_100H          (1U << 13)
#define PHY_STATUS_10F           (1U << 12)
#define PHY_STATUS_10H           (1U << 11)
#define PHY_STATUS_AN_COMPLETE   (1U << 5)
#define PHY_STATUS_RM_FAULT      (1U << 4)
#define PHY_STATUS_AN_ABILITY    (1U << 3)
#define PHY_STATUS_LINK_UP       (1U << 2)
#define PHY_STATUS_JABBER        (1U << 1)
#define PHY_STATUS_EX_CAPABILITY (1U << 0)

/* Auto Negotiation Advertisement Bit Definitions */
#define PHY_AN_ADVERTISEMENT_NEXT_PAGE (1U << 15)
#define PHY_AN_ADVERTISEMENT_RM_FAULT  (1U << 13)
#define PHY_AN_ADVERTISEMENT_ASYMP     (1U << 11)
#define PHY_AN_ADVERTISEMENT_PAUSE     (1U << 10)
#define PHY_AN_ADVERTISEMENT_100_T4    (1U << 9)
#define PHY_AN_ADVERTISEMENT_100F      (1U << 8)
#define PHY_AN_ADVERTISEMENT_100H      (1U << 7)
#define PHY_AN_ADVERTISEMENT_10F       (1U << 6)
#define PHY_AN_ADVERTISEMENT_10H       (1U << 5)
#define PHY_AN_ADVERTISEMENT_SELECTOR  (1U << 0)

/* Auto Negotiate Link Partner Ability Bit Definitions */
#define PHY_AN_LINK_PARTNER_NEXT_PAGE (1U << 15)
#define PHY_AN_LINK_PARTNER_ACK       (1U << 14)
#define PHY_AN_LINK_PARTNER_RM_FAULT  (1U << 13)
#define PHY_AN_LINK_PARTNER_ASYMP     (1U << 11)
#define PHY_AN_LINK_PARTNER_PAUSE     (1U << 10)
#define PHY_AN_LINK_PARTNER_100_T4    (1U << 9)
#define PHY_AN_LINK_PARTNER_100F      (1U << 8)
#define PHY_AN_LINK_PARTNER_100H      (1U << 7)
#define PHY_AN_LINK_PARTNER_10F       (1U << 6)
#define PHY_AN_LINK_PARTNER_10H       (1U << 5)
#define PHY_AN_LINK_PARTNER_SELECTOR  (1U << 0)

/* MICREL KSZ8091RNB PHY Control2  Bit Definitions */
#define PHY_REG_PHY_CONTROL2_LED_MODE_MASK                 (3U << 4)
#define PHY_REG_PHY_CONTROL2_LED_MODE_LINK0_ACTIVITY1      (1U << 4)
#define PHY_REG_PHY_CONTROL2_LED_MODE_LINKACTIVITY0_SPEED1 (0 << 4)

#define PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_MASK    (1U << 7)
#define PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_25MHZ   (0U << 7)
#define PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_50MHZ   (1U << 7)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_linkstat
{
    PHY_NO_LINK = 0,
    PHY_LINK_10H,
    PHY_LINK_10F,
    PHY_LINK_100H,
    PHY_LINK_100F
} linkstat_t;

typedef enum e_ether_pause
{
    ETHER_PHY_PAUSE_NONE       = 0,
    ETHER_PHY_PAUSE_SYMMETRIC  = 1,
    ETHER_PHY_PAUSE_ASYMMETRIC = 2,
} ether_phy_pause_t;

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
int16_t Phy_Init (uint32_t ether_channel);

int16_t Phy_Get_Autonegotiate (uint32_t ether_channel,
                               uint16_t * pline_speed_duplex,
                               uint16_t * plocal_pause,
                               uint16_t * ppartner_pause);

void    Phy_Start_Autonegotiate (uint32_t ether_channel, uint8_t pause);

int16_t Phy_GetLinkStatus (uint32_t ether_channel);
#endif // _SC32_PHY_H_
