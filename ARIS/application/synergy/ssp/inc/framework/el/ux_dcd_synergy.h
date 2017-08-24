/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2012 by Express Logic Inc.               */ 
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


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   SYNERGY Controller Driver                                           */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

/**************************************************************************/ 
/*                                                                        */ 
/*  COMPONENT DEFINITION                                   RELEASE        */ 
/*                                                                        */ 
/*    ux_dcd_synergy.h                                    PORTABLE C      */ 
/*                                                           5.6          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file defines the equivalences for the SYNERGY RENESAS          */ 
/*    USB device controller.                                              */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  10-10-2012     TCRG                     Initial Version 5.6           */ 
/*                                                                        */ 
/**************************************************************************/ 

#ifndef UX_DCD_SYNERGY_H
#define UX_DCD_SYNERGY_H

#include "r_transfer_api.h"

/* Define SYNERGY generic equivalences.  */

#define UX_DCD_SYNERGY_SLAVE_CONTROLLER                          0x80
#if defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128)
#define UX_DCD_SYNERGY_MAX_ED                                    5
#else
#define UX_DCD_SYNERGY_MAX_ED                                    10
#endif
#define UX_DCD_SYNERGY_ENABLE                                    1
#define UX_DCD_SYNERGY_DISABLE                                   0
#define UX_DCD_SYNERGY_MAX_BULK_PAYLOAD                          512 
#define UX_DCD_SYNERGY_MAX_CONTROL_PAYLOAD                       512 
#define UX_DCD_SYNERGY_MAX_BUF_SIZE                              64
#define UX_DCD_SYNERGY_MAX_BUF_NUM                               127

#if defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128)
#define UX_DCD_SYNERGY_MIN_PIPE                                  4
#define UX_DCD_SYNERGY_MAX_PIPE                                  7
#else
#define UX_DCD_SYNERGY_MIN_PIPE                                  1
#define UX_DCD_SYNERGY_MAX_PIPE                                  9
#endif


/* Define SYNERGY HCOR register mapping.  */

#define UX_SYNERGY_DCD_SYSCFG                                    (0x00U)
#define UX_SYNERGY_DCD_BUSWAIT                                   (0x02U)
#define UX_SYNERGY_DCD_SYSSTS                                    (0x04U)
#define UX_SYNERGY_DCD_PLLSTA                                    (0x06U)
#define UX_SYNERGY_DCD_DVSTCTR                                   (0x08U)
#define UX_SYNERGY_DCD_CFIFO                                     (0x14U)
#define UX_SYNERGY_DCD_CFIFOH                                    (0x16U)
#define UX_SYNERGY_DCD_CFIFOHH                                   (0x17U)
#define UX_SYNERGY_DCD_D0FIFO                                    (0x18U)
#define UX_SYNERGY_DCD_D0FIFOH                                   (0x1AU)
#define UX_SYNERGY_DCD_D0FIFOHH                                  (0x1BU)
#define UX_SYNERGY_DCD_D1FIFO                                    (0x1CU)
#define UX_SYNERGY_DCD_D1FIFOH                                   (0x1EU)
#define UX_SYNERGY_DCD_D1FIFOHH                                  (0x1FU)
#define UX_SYNERGY_DCD_CFIFOSEL                                  (0x20U)
#define UX_SYNERGY_DCD_CFIFOCTR                                  (0x22U)
#define UX_SYNERGY_DCD_D0FIFOSEL                                 (0x28U)
#define UX_SYNERGY_DCD_D0FIFOCTR                                 (0x2AU)
#define UX_SYNERGY_DCD_D1FIFOSEL                                 (0x2CU)
#define UX_SYNERGY_DCD_D1FIFOCTR                                 (0x2EU)
#define UX_SYNERGY_DCD_INTENB0                                   (0x30U)
#define UX_SYNERGY_DCD_INTENB1                                   (0x32U)
#define UX_SYNERGY_DCD_BRDYENB                                   (0x36U)
#define UX_SYNERGY_DCD_NRDYENB                                   (0x38U)
#define UX_SYNERGY_DCD_BEMPENB                                   (0x3AU)
#define UX_SYNERGY_DCD_SOFCFG                                    (0x3CU)
#define UX_SYNERGY_DCD_PHYSET                                    (0x3EU)
#define UX_SYNERGY_DCD_INTSTS0                                   (0x40U)
#define UX_SYNERGY_DCD_INTSTS1                                   (0x42U)
#define UX_SYNERGY_DCD_BRDYSTS                                   (0x46U)
#define UX_SYNERGY_DCD_NRDYSTS                                   (0x48U)
#define UX_SYNERGY_DCD_BEMPSTS                                   (0x4AU)
#define UX_SYNERGY_DCD_FRMNUM                                    (0x4CU)

#define UX_SYNERGY_DCD_DVCHGR                                    (0x4EU)

#define UX_SYNERGY_DCD_USBADDR                                   (0x50U)
#define UX_SYNERGY_DCD_USBREQ                                    (0x54U)
#define UX_SYNERGY_DCD_USBVAL                                    (0x56U)
#define UX_SYNERGY_DCD_USBINDX                                   (0x58U)
#define UX_SYNERGY_DCD_USBLENG                                   (0x5AU)
#define UX_SYNERGY_DCD_DCPCFG                                    (0x5CU)
#define UX_SYNERGY_DCD_DCPMAXP                                   (0x5EU)
#define UX_SYNERGY_DCD_DCPCTR                                    (0x60U)
#define UX_SYNERGY_DCD_PIPESEL                                   (0x64U)
#define UX_SYNERGY_DCD_PIPECFG                                   (0x68U)
#define UX_SYNERGY_DCD_PIPEMAXP                                  (0x6CU)
#define UX_SYNERGY_DCD_PIPEPERI                                  (0x6EU)
#define UX_SYNERGY_DCD_PIPE1CTR                                  (0x70U)
#define UX_SYNERGY_DCD_PIPE2CTR                                  (0x72U)
#define UX_SYNERGY_DCD_PIPE3CTR                                  (0x74U)
#define UX_SYNERGY_DCD_PIPE4CTR                                  (0x76U)
#define UX_SYNERGY_DCD_PIPE5CTR                                  (0x78U)
#define UX_SYNERGY_DCD_PIPE6CTR                                  (0x7AU)
#define UX_SYNERGY_DCD_PIPE7CTR                                  (0x7CU)
#define UX_SYNERGY_DCD_PIPE8CTR                                  (0x7EU)
#define UX_SYNERGY_DCD_PIPE9CTR                                  (0x80U)
#define UX_SYNERGY_DCD_PIPE1TRE                                  (0x90U)
#define UX_SYNERGY_DCD_PIPE1TRN                                  (0x92U)
#define UX_SYNERGY_DCD_PIPE2TRE                                  (0x94U)
#define UX_SYNERGY_DCD_PIPE2TRN                                  (0x96U)
#define UX_SYNERGY_DCD_PIPE3TRE                                  (0x98U)
#define UX_SYNERGY_DCD_PIPE3TRN                                  (0x9AU)
#define UX_SYNERGY_DCD_PIPE4TRE                                  (0x9CU)
#define UX_SYNERGY_DCD_PIPE4TRN                                  (0x9EU)
#define UX_SYNERGY_DCD_PIPE5TRE                                  (0xA0U)
#define UX_SYNERGY_DCD_PIPE5TRN                                  (0xA2U)
#define UX_SYNERGY_DCD_DEVADD0                                   (0xD0U)
#define UX_SYNERGY_DCD_DEVADD1                                   (0xD2U)
#define UX_SYNERGY_DCD_DEVADD2                                   (0xD4U)
#define UX_SYNERGY_DCD_DEVADD3                                   (0xD6U)
#define UX_SYNERGY_DCD_DEVADD4                                   (0xD8U)
#define UX_SYNERGY_DCD_DEVADD5                                   (0xDAU)
#define UX_SYNERGY_DCD_PHYSLEW                                   (0xF0U)
#define UX_SYNERGY_DCD_LPSTS                                     (0x102U)

#if defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128)
#define UX_SYNERGY_DCD_UCKSEL                                    (0xC4U)
#endif

#define UX_SYNERGY_DCD_BUSWAIT_DEFAULT_VAL                       (0xF)
#define UX_SYNERGY_DCD_BUSWAIT_VAL                               (0x5)

/* Define SYNERGY control register values.  */
#define UX_SYNERGY_DCD_SYSCFG_SCKE                               (1U<<10)
#define UX_SYNERGY_DCD_SYSCFG_CNEN                               (1U<<8)
#define UX_SYNERGY_DCD_SYSCFG_HSE                                (1U<<7)
#define UX_SYNERGY_DCD_SYSCFG_DCFM                               (1U<<6)
#define UX_SYNERGY_DCD_SYSCFG_DRPD                               (1U<<5)
#define UX_SYNERGY_DCD_SYSCFG_DPRPU                              (1U<<4)
#define UX_SYNERGY_DCD_SYSCFG_DMRPU                              (1U<<3)
#define UX_SYNERGY_DCD_SYSCFG_USBE                               1

#define UX_SYNERGY_DCD_SYSSTS_LNST                               3
#define UX_SYNERGY_DCD_SYSSTS_SOFEN                              0x20

#define UX_SYNERGY_DCD_DVSTCTR_UACKEY0                           (1U<<15)
#define UX_SYNERGY_DCD_DVSTCTR_UACKEY1                           (1U<<12)
#define UX_SYNERGY_DCD_DVSTCTR_WKUP                              (1U<<8)
#define UX_SYNERGY_DCD_DVSTCTR_RWUPE                             (1U<<7)
#define UX_SYNERGY_DCD_DVSTCTR_USBRST                            (1U<<6)
#define UX_SYNERGY_DCD_DVSTCTR_RESUME                            (1U<<5)
#define UX_SYNERGY_DCD_DVSTCTR_UACT                              (1U<<4)
#define UX_SYNERGY_DCD_DVSTCTR_RHST                              (0x7)

#define UX_SYNERGY_DCD_DVSTCTR_SPEED_LOW                         (1)
#define UX_SYNERGY_DCD_DVSTCTR_SPEED_FULL                        (2)
#define UX_SYNERGY_DCD_DVSTCTR_SPEED_HIGH                        (3)
#define UX_SYNERGY_DCD_DVSTCTR_RESET_IN_PROGRESS                 (4)

#define UX_SYNERGY_DCD_TESTMODE_HOSTPCC                          (1U<<15)

#define UX_SYNERGY_DCD_DXFBCFG_DFACC                             (0x00<<12)

#define UX_SYNERGY_DCD_CFIFOSEL_RCNT                             (1U<<15)
#define UX_SYNERGY_DCD_CFIFOSEL_REW                              (1U<<14)
#define UX_SYNERGY_DCD_CFIFOSEL_MBW_8                            (0<<10)
#define UX_SYNERGY_DCD_CFIFOSEL_MBW_16                           (1U<<10)
#define UX_SYNERGY_DCD_CFIFOSEL_MBW_32                           (2U<<10)
#define UX_SYNERGY_DCD_CFIFOSEL_MBW_MASK                         (3U<<10)
#define UX_SYNERGY_DCD_CFIFOSEL_BIGEND                           (1U<<8)
#define UX_SYNERGY_DCD_CFIFOSEL_ISEL                             (1U<<5)
#define UX_SYNERGY_DCD_CFIFOSEL_CURPIPE_MASK                     (0xF)

#define UX_SYNERGY_DCD_DFIFOSEL_RCNT                             (1U<<15)
#define UX_SYNERGY_DCD_DFIFOSEL_REW                              (1U<<14)
#define UX_SYNERGY_DCD_DFIFOSEL_DCLRM                            (1U<<13)
#define UX_SYNERGY_DCD_DFIFOSEL_DREQE                            (1U<<12)
#define UX_SYNERGY_DCD_DFIFOSEL_MBW_8                            (0<<10)
#define UX_SYNERGY_DCD_DFIFOSEL_MBW_16                           (1U<<10)
#define UX_SYNERGY_DCD_DFIFOSEL_MBW_32                           (2U<<10)
#define UX_SYNERGY_DCD_DFIFOSEL_MBW_MASK                         (3U<<10)
#define UX_SYNERGY_DCD_DFIFOSEL_BIGEND                           (1U<<8)
#define UX_SYNERGY_DCD_DFIFOSEL_CURPIPE_MASK                     (0xF)

#define UX_SYNERGY_DCD_FIFOCTR_BVAL                              (1U<<15)
#define UX_SYNERGY_DCD_FIFOCTR_BCLR                              (1U<<14)
#define UX_SYNERGY_DCD_FIFOCTR_FRDY                              (1U<<13)
#define UX_SYNERGY_DCD_FIFOCTR_DTLN                              (0xFFF)
                                                        
#define UX_SYNERGY_DCD_INTENB0_VBSE                              (1U<<15)
#define UX_SYNERGY_DCD_INTENB0_RSME                              (1U<<14)
#define UX_SYNERGY_DCD_INTENB0_SOFE                              (1U<<13)
#define UX_SYNERGY_DCD_INTENB0_DVSE                              (1U<<12)
#define UX_SYNERGY_DCD_INTENB0_CTRE                              (1U<<11)
#define UX_SYNERGY_DCD_INTENB0_BEMPE                             (1U<<10)
#define UX_SYNERGY_DCD_INTENB0_NRDYE                             (1U<<9)
#define UX_SYNERGY_DCD_INTENB0_BRDYE                             (1U<<8)

#define UX_SYNERGY_DCD_INTENB1_BCHGE                             (1U<<14)
#define UX_SYNERGY_DCD_INTENB1_DTCHE                             (1U<<12)
#define UX_SYNERGY_DCD_INTENB1_ATTCHE                            (1U<<11)
#define UX_SYNERGY_DCD_INTENB1_EOFERRE                           (1U<<6)
#define UX_SYNERGY_DCD_INTENB1_SIGNE                             (1U<<5)
#define UX_SYNERGY_DCD_INTENB1_SACKE                             (1U<<4)

#define UX_SYNERGY_DCD_PIPE0                                     (1U<<0)
#define UX_SYNERGY_DCD_PIPE1                                     (1U<<1)
#define UX_SYNERGY_DCD_PIPE2                                     (1U<<2)
#define UX_SYNERGY_DCD_PIPE3                                     (1U<<3)
#define UX_SYNERGY_DCD_PIPE4                                     (1U<<4)
#define UX_SYNERGY_DCD_PIPE5                                     (1U<<5)
#define UX_SYNERGY_DCD_PIPE6                                     (1U<<6)
#define UX_SYNERGY_DCD_PIPE7                                     (1U<<7)
#define UX_SYNERGY_DCD_PIPE8                                     (1U<<8)
#define UX_SYNERGY_DCD_PIPE9                                     (1U<<9)
#define UX_SYNERGY_DCD_PIPE_ALL                                  (0x3FF)

#define UX_SYNERGY_DCD_SOFCFG_TRNENSEL                           (1U<<8)
#define UX_SYNERGY_DCD_SOFCFG_BRDYM                              (1U<<6)
#define UX_SYNERGY_DCD_SOFCFG_INIT                               (1U<<5)

#define UX_SYNERGY_DCD_PHYSET_HSEB                                (1<<15)
#define UX_SYNERGY_DCD_PHYSET_REPSTART                            (1<<11)
#define UX_SYNERGY_DCD_PHYSET_REPSEL                              (1<<8)
#define UX_SYNERGY_DCD_PHYSET_CLKSEL_1                            (1<<5)
#define UX_SYNERGY_DCD_PHYSET_CLKSEL_0                            (1<<4)
#define UX_SYNERGY_DCD_PHYSET_CDPEN                               (1<<3)
#define UX_SYNERGY_DCD_PHYSET_PLLRESET                            (1<<1)
#define UX_SYNERGY_DCD_PHYSET_DIRPD                               (1<<0)

#define UX_SYNERGY_DCD_INTSTS0_VBINT                             (USHORT)(1U<<15)
#define UX_SYNERGY_DCD_INTSTS0_RESM                              (USHORT)(1U<<14)
#define UX_SYNERGY_DCD_INTSTS0_SOFR                              (1U<<13)
#define UX_SYNERGY_DCD_INTSTS0_DVST                              (USHORT)(1U<<12)
#define UX_SYNERGY_DCD_INTSTS0_CTRT                              (USHORT)(1U<<11)
#define UX_SYNERGY_DCD_INTSTS0_BEMP                              (1U<<10)
#define UX_SYNERGY_DCD_INTSTS0_NRDY                              (1U<<9)
#define UX_SYNERGY_DCD_INTSTS0_BRDY                              (1U<<8)
#define UX_SYNERGY_DCD_INTSTS0_VBSTS                             (1U<<7)
#define UX_SYNERGY_DCD_INTSTS0_DVSQ_MASK                         (7<<4)
#define UX_SYNERGY_DCD_INTSTS0_VALID                             (USHORT)(1U<<3)
#define UX_SYNERGY_DCD_INTSTS0_CTSQ_MASK                         (7)

#define UX_SYNERGY_DCD_INTSTS0_DVSQ_POWERED                      0x0000
#define UX_SYNERGY_DCD_INTSTS0_DVSQ_DEFAULT                      0x0010
#define UX_SYNERGY_DCD_INTSTS0_DVSQ_ADDRESS                      0x0020
#define UX_SYNERGY_DCD_INTSTS0_DVSQ_CONFIGURED                   0x0030
#define UX_SYNERGY_DCD_INTSTS0_DVSQ_SUSPENDED_POWERED            0x0040
#define UX_SYNERGY_DCD_INTSTS0_DVSQ_SUSPENDED_DEFAULT            0x0050
#define UX_SYNERGY_DCD_INTSTS0_DVSQ_SUSPENDED_ADDRESS            0x0060
#define UX_SYNERGY_DCD_INTSTS0_DVSQ_SUSPENDED_CONFIGURED         0x0070

#define UX_SYNERGY_DCD_INTSTS0_CTSQ_SETUP                        0x0000
#define UX_SYNERGY_DCD_INTSTS0_CTSQ_CRDS                         0x0001
#define UX_SYNERGY_DCD_INTSTS0_CTSQ_CRSS                         0x0002
#define UX_SYNERGY_DCD_INTSTS0_CTSQ_CWDS                         0x0003
#define UX_SYNERGY_DCD_INTSTS0_CTSQ_CWSS                         0x0004
#define UX_SYNERGY_DCD_INTSTS0_CTSQ_CWNDSS                       0x0005
#define UX_SYNERGY_DCD_INTSTS0_CTSQ_CTSE                         0x0006

#define UX_SYNERGY_DCD_INTSTS1_BCHG                              (1U<<14)
#define UX_SYNERGY_DCD_INTSTS1_DTCH                              (1U<<12)
#define UX_SYNERGY_DCD_INTSTS1_ATTCH                             (1U<<11)
#define UX_SYNERGY_DCD_INTSTS1_EOFERR                            (1U<<6)
#define UX_SYNERGY_DCD_INTSTS1_SIGN                              (1U<<5)
#define UX_SYNERGY_DCD_INTSTS1_SACK                              (1U<<4)

#define UX_SYNERGY_DCD_FRMNUM_OVRN                               (1U<<15)
#define UX_SYNERGY_DCD_FRMNUM_CRCE                               (1U<<14)
#define UX_SYNERGY_DCD_FRMNUM_FRNM_MASK                          (0x7FF)
 
#define UX_SYNERGY_DCD_DCPCFG_DIR                                (1U<<4)
#define UX_SYNERGY_DCD_DCPCFG_SHTNAK                             (1U<<7)
#define UX_SYNERGY_DCD_DCPCFG_CNTMD                              (1U<<8)

#define UX_SYNERGY_DCD_DCPMAXP_DEVADDR_SHIFT                     (12)
#define UX_SYNERGY_DCD_DCPMAXP_DEVADDR_MASK                      0xf000

#define UX_SYNERGY_DCD_DCPCTR_BSTS                               (1U<<15)
#define UX_SYNERGY_DCD_DCPCTR_INBUFM                             (1U<<14)
#define UX_SYNERGY_DCD_DCPCTR_CSCLR                              (1U<<13)
#define UX_SYNERGY_DCD_DCPCTR_CSSTS                              (1U<<12)
#define UX_SYNERGY_DCD_DCPCTR_SUREQCLR                           (1U<<11)
#define UX_SYNERGY_DCD_DCPCTR_SQCLR                              (1U<<8)
#define UX_SYNERGY_DCD_DCPCTR_SQSET                              (1U<<7)
#define UX_SYNERGY_DCD_DCPCTR_SQMON                              (1U<<6)
#define UX_SYNERGY_DCD_DCPCTR_PBUSY                              (1U<<5)
#define UX_SYNERGY_DCD_DCPCTR_PINGE                              (1U<<4)
#define UX_SYNERGY_DCD_DCPCTR_CCPL                               (1U<<2)
#define UX_SYNERGY_DCD_DCPCTR_PID_MASK                           (3)
#define UX_SYNERGY_DCD_DCPCTR_PIDNAK                             (0)
#define UX_SYNERGY_DCD_DCPCTR_PIDBUF                             (1)
#define UX_SYNERGY_DCD_DCPCTR_PIDSTALL                           (2)
#define UX_SYNERGY_DCD_DCPCTR_PIDSTALL2                          (3)


#define UX_SYNERGY_DCD_PIPECFG_TYPE_MASK                         (0xC000)
#define UX_SYNERGY_DCD_PIPECFG_TYPE_BIT_USED                     (0)
#define UX_SYNERGY_DCD_PIPECFG_TYPE_BULK                         (1U<<14)
#define UX_SYNERGY_DCD_PIPECFG_TYPE_INTERRUPT                    (2<<14)
#define UX_SYNERGY_DCD_PIPECFG_TYPE_ISOCHRONOUS                  (3<<14)
#define UX_SYNERGY_DCD_PIPECFG_BFRE                              (1U<<10)
#define UX_SYNERGY_DCD_PIPECFG_DBLB                              (1U<<9)
#define UX_SYNERGY_DCD_PIPECFG_CNTMD                             (1U<<8)
#define UX_SYNERGY_DCD_PIPECFG_SHTNAK                            (1U<<7)
#define UX_SYNERGY_DCD_PIPECFG_DIR                               (1U<<4)
#define UX_SYNERGY_DCD_PIPECFG_EPNUM_MASK                        (0xF)

#define UX_SYNERGY_DCD_PIPEBUF_SIZEMASK                          (0x1F<<10)
#define UX_SYNERGY_DCD_PIPEBUF_BUFNMBMASK                        (0xFF<<10)
#define UX_SYNERGY_DCD_PIPEBUF_SHIFT                             (10)

#define UX_SYNERGY_DCD_PIPEMAXP_DEVSELMASK                       (0xF<<12)
#define UX_SYNERGY_DCD_PIPEMAXP_DEVSEL_SHIFT                     (12)
#define UX_SYNERGY_DCD_PIPEMAXP_MXPSMASK                         (0x7FF)

#define UX_SYNERGY_DCD_PIPE1TRE_TRCLR                            (1U<<8)
#define UX_SYNERGY_DCD_PIPE1TRE_TRENB                            (1U<<9)

#define UX_SYNERGY_DCD_FIFO_D0                                   0
#define UX_SYNERGY_DCD_FIFO_D1                                   1
#define UX_SYNERGY_DCD_FIFO_C                                    2

#define UX_SYNERGY_DCD_DEVADDX_SPEED_LOW                         (1U<<6)
#define UX_SYNERGY_DCD_DEVADDX_SPEED_FULL                        (2<<6)
#define UX_SYNERGY_DCD_DEVADDX_SPEED_HIGH                        (3<<6)
#define UX_SYNERGY_DCD_DEVADDX_UPPHUB_SHIFT                      11
#define UX_SYNERGY_DCD_DEVADDX_HUBPORT_SHIFT                     8

/* Define synergy command/status bitmaps.  */

#define UX_SYNERGY_DCD_DCP                                       0

#define UX_SYNERGY_DCD_DCPCTR_DATA1                              (1U<<7)
#define UX_SYNERGY_DCD_DCPCTR_DATA0                              (1U<<8)

/* Define synergy PIPE selection definitions.  */
#define UX_SYNERGY_DCD_PIPESEL_NO_PIPE                           0x000f

#define UX_SYNERGY_DCD_UCKSEL_UCKSELC                           (1)

/* Define synergy fifo definition.  */
#define UX_SYNERGY_DCD_PIPE0_SIZE                                256
#define UX_SYNERGY_DCD_PIPE_NB_BUFFERS                           64

#define UX_SYNERGY_DCD_PIPECTR_INBUFM                            (1U<<14)
#define UX_SYNERGY_DCD_PIPECTR_BSTS                              (1U<<15)
#define UX_SYNERGY_DCD_PIPECTR_CSCLR                             (1U<<13)
#define UX_SYNERGY_DCD_PIPECTR_CSSTS                             (1U<<12)
#define UX_SYNERGY_DCD_PIPECTR_ATREPM                            (1U<<11)
#define UX_SYNERGY_DCD_PIPECTR_SQCLR                             (1U<<8)
#define UX_SYNERGY_DCD_PIPECTR_SQSET                             (1U<<7)
#define UX_SYNERGY_DCD_PIPECTR_SQMON                             (1U<<6)
#define UX_SYNERGY_DCD_PIPECTR_PBUSY                             (1U<<5)
#define UX_SYNERGY_DCD_PIPECTR_PID_MASK                          (3)
#define UX_SYNERGY_DCD_PIPECTR_PIDNAK                            (0)
#define UX_SYNERGY_DCD_PIPECTR_PIDBUF                            (1)
#define UX_SYNERGY_DCD_PIPECTR_PIDSTALL                          (2)
#define UX_SYNERGY_DCD_PIPECTR_PIDSTALL2                         (3)
#define UX_SYNERGY_DCD_PIPECTR_DATA1                             ( 1U<< 7)
#define UX_SYNERGY_DCD_PIPECTR_DATA0                             ( 1U<< 8)

/* Define synergy initialization values.  */

#define UX_SYNERGY_DCD_COMMAND_STATUS_RESET                      0
#define UX_SYNERGY_DCD_INIT_RESET_DELAY                          10
#define UX_SYNERGY_DCD_MAX_BUF_SIZE                              64
#define UX_SYNERGY_DCD_MAX_BUF_NUM                               127

/* Define synergy FIFO write completion code.  */
#define UX_SYNERGY_DCD_FIFO_WRITING                              2
#define UX_SYNERGY_DCD_FIFO_WRITE_END                            3
#define UX_SYNERGY_DCD_FIFO_WRITE_SHORT                          4
#define UX_SYNERGY_DCD_FIFO_WRITE_DMA                            5
#define UX_SYNERGY_DCD_FIFO_WRITE_ERROR                          6

/* Define synergy FIFO read completion code.  */
#define UX_SYNERGY_DCD_FIFO_READING                              2
#define UX_SYNERGY_DCD_FIFO_READ_END                             3
#define UX_SYNERGY_DCD_FIFO_READ_SHORT                           4
#define UX_SYNERGY_DCD_FIFO_READ_DMA                             5
#define UX_SYNERGY_DCD_FIFO_READ_ERROR                           6
#define UX_SYNERGY_DCD_FIFO_READ_OVER                            7

/* Define synergy physical endpoint definitions.  */
#define UX_SYNERGY_DCD_ED_BRDY                                   0x00000001
#define UX_SYNERGY_DCD_ED_NRDY                                   0x00000002
#define UX_SYNERGY_DCD_ED_BEMP                                   0x00000004
#define UX_SYNERGY_DCD_ED_EOFERR                                 0x00000010
#define UX_SYNERGY_DCD_ED_SIGN                                   0x00000020
#define UX_SYNERGY_DCD_ED_SACK                                   0x00000040

/* PHY Cross Point Adjustment, note that Hardware Manual to be updated(0xE->0x5) */
#define UX_SYNERGY_DCD_PHYSLEW_SLEW_SLEWR00                     (1U<<0)
#define UX_SYNERGY_DCD_PHYSLEW_SLEW_SLEWR01                     (1U<<1)
#define UX_SYNERGY_DCD_PHYSLEW_SLEW_SLEWF00                     (1U<<2)
#define UX_SYNERGY_DCD_PHYSLEW_SLEW_SLEWF01                     (1U<<3)

#define UX_SYNERGY_DCD_LPSTS_SUSPENDM                           (1<<14)
/* Define USB SYNERGY physical endpoint status definition.  */

#define UX_DCD_SYNERGY_ED_STATUS_UNUSED                          0U
#define UX_DCD_SYNERGY_ED_STATUS_USED                            1U
#define UX_DCD_SYNERGY_ED_STATUS_TRANSFER                        2U
#define UX_DCD_SYNERGY_ED_STATUS_STALLED                         4U

/* Define USB SYNERGY physical endpoint state machine definition.  */

#define UX_DCD_SYNERGY_ED_STATE_IDLE                             0U
#define UX_DCD_SYNERGY_ED_STATE_DATA_TX                          1U
#define UX_DCD_SYNERGY_ED_STATE_DATA_RX                          2U
#define UX_DCD_SYNERGY_ED_STATE_STATUS_TX                        3U
#define UX_DCD_SYNERGY_ED_STATE_STATUS_RX                        4U

/* Define USB SYNERGY physical endpoint structure.  */

typedef struct UX_DCD_SYNERGY_ED_STRUCT 
{

    ULONG           ux_dcd_synergy_ed_status;
    ULONG           ux_dcd_synergy_ed_state;
    ULONG           ux_dcd_synergy_ed_index;
    ULONG           ux_dcd_synergy_ed_fifo_index;
    ULONG           ux_dcd_synergy_ed_payload_length;
    ULONG           ux_dcd_synergy_ed_packet_length;
    ULONG           ux_dcd_synergy_ed_actual_length;
    UCHAR           *ux_dcd_synergy_ed_payload_address;
    ULONG           ux_dcd_synergy_ed_buffer_number;
    ULONG           ux_dcd_synergy_ed_buffer_size;
    struct UX_SLAVE_ENDPOINT_STRUCT             
                    *ux_dcd_synergy_ed_endpoint;
} UX_DCD_SYNERGY_ED;

/* Define SYNERGY Transfer structure */
typedef struct UX_DCD_SYNERGY_TRANSFER_STRUCT
{
    transfer_instance_t *ux_synergy_transfer_tx;
    transfer_instance_t *ux_synergy_transfer_rx;

} UX_DCD_SYNERGY_TRANSFER;

/* Define USB SYNERGY DCD structure definition.  */

typedef struct UX_DCD_SYNERGY_STRUCT
{                                            
    
    struct UX_SLAVE_DCD_STRUCT
                    *ux_dcd_synergy_dcd_owner;
    struct UX_DCD_SYNERGY_ED_STRUCT                    
                    ux_dcd_synergy_ed[UX_DCD_SYNERGY_MAX_ED];
    ULONG           ux_dcd_synergy_pipe[UX_DCD_SYNERGY_MAX_PIPE];
    ULONG           ux_dcd_synergy_base;
    ULONG           ux_dcd_synergy_next_available_bufnum;
    ULONG           ux_dcd_synergy_debug;
    UX_DCD_SYNERGY_TRANSFER  ux_dcd_synergy_transfer;
    transfer_cfg_t  ux_dcd_synergy_transfer_cfg_tx;
    transfer_cfg_t  ux_dcd_synergy_transfer_cfg_rx;
    volatile bool   ux_dcd_synergy_dma_done_tx;
    volatile bool   ux_dcd_synergy_dma_done_rx;
} UX_DCD_SYNERGY;

/* Define USB SYNERGY DCD prototypes.  */

UINT    _ux_dcd_synergy_address_set(UX_DCD_SYNERGY *dcd_synergy,ULONG address);
UINT    _ux_dcd_synergy_endpoint_create(UX_DCD_SYNERGY *dcd_synergy, UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_dcd_synergy_endpoint_destroy(UX_DCD_SYNERGY *dcd_synergy, UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_dcd_synergy_endpoint_reset(UX_DCD_SYNERGY *dcd_synergy, UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_dcd_synergy_endpoint_stall(UX_DCD_SYNERGY *dcd_synergy, UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_dcd_synergy_endpoint_status(UX_DCD_SYNERGY *dcd_synergy, ULONG endpoint_index);
UINT    _ux_dcd_synergy_frame_number_get(UX_DCD_SYNERGY *dcd_synergy, ULONG *frame_number);
UINT    _ux_dcd_synergy_function(UX_SLAVE_DCD *dcd, UINT function, VOID *parameter);
UINT    _ux_dcd_synergy_initialize(ULONG dcd_io);
UINT    _ux_dcd_synergy_initialize_transfer_support(ULONG dcd_io, UX_DCD_SYNERGY_TRANSFER * p_transfer_instance);
UINT    _ux_dcd_synergy_initialize_complete(VOID);
VOID    _ux_dcd_synergy_interrupt_handler(VOID);
VOID    _ux_dcd_synergy_register_clear(UX_DCD_SYNERGY *dcd_synergy, ULONG synergy_register, USHORT value);
ULONG   _ux_dcd_synergy_register_read(UX_DCD_SYNERGY *dcd_synergy, ULONG synergy_register);
VOID    _ux_dcd_synergy_register_set(UX_DCD_SYNERGY *dcd_synergy, ULONG synergy_register, USHORT value);
VOID    _ux_dcd_synergy_register_write(UX_DCD_SYNERGY *dcd_synergy, ULONG synergy_register, USHORT value);
UINT    _ux_dcd_synergy_state_change(UX_DCD_SYNERGY *dcd_synergy, ULONG state);
UINT    _ux_dcd_synergy_transfer_callback(UX_DCD_SYNERGY *dcd_synergy, UX_SLAVE_TRANSFER *transfer_request, 
                                        ULONG interrupt_status, ULONG ctsq_mask);
UINT    _ux_dcd_synergy_transfer_request(UX_DCD_SYNERGY *dcd_synergy, UX_SLAVE_TRANSFER *transfer_request);
UINT    _ux_dcd_synergy_buffer_read(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed);
UINT    _ux_dcd_synergy_fifo_read(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed);
VOID    _ux_dcd_synergy_buffer_notready_interrupt(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed, ULONG flag);
VOID    _ux_dcd_synergy_buffer_empty_interrupt(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed, ULONG flag);
VOID    _ux_dcd_synergy_buffer_ready_interrupt(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed, ULONG flag);
ULONG   _ux_dcd_synergy_fifo_port_change(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed, ULONG direction);
UINT    _ux_dcd_synergy_fifod_write(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed);
UINT    _ux_dcd_synergy_fifoc_write(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed);
UINT    _ux_dcd_synergy_buffer_write(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed);
ULONG   _ux_dcd_synergy_data_buffer_size(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed);
VOID    _ux_dcd_synergy_endpoint_nak_set(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed);
VOID    _ux_dcd_synergy_current_endpoint_change(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed, ULONG direction);

#endif

