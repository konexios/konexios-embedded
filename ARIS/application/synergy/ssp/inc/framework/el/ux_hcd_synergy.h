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


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   SYNERGY Controller Driver                                                */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  COMPONENT DEFINITION                                   RELEASE        */ 
/*                                                                        */ 
/*    ux_hcd_synergy.h                                         PORTABLE C      */ 
/*                                                           5.7          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains all the header and extern functions used by the  */
/*    USBX host synergy Controller.                                            */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  10-10-2012     TCRG                     Initial Version 5.6           */ 
/*  06-01-2014     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.7    */ 
/*                                                                        */ 
/**************************************************************************/ 

#ifndef UX_HCD_SYNERGY_H
#define UX_HCD_SYNERGY_H

#include "r_transfer_api.h"

/* Define Synergy generic definitions.  */

#define UX_SYNERGY_CONTROLLER                            0
#define UX_SYNERGY_CONTROLLER_S7G2                       1
#define UX_SYNERGY_CONTROLLER_S3A7                       2
#define UX_SYNERGY_MAX_BULK_PAYLOAD                      512 
#define UX_SYNERGY_MAX_CONTROL_PAYLOAD                   512 
#define UX_SYNERGY_FRAME_DELAY                           4 
#define UX_SYNERGY_PERIODIC_ENTRY_NB                     32    
#define UX_SYNERGY_PERIODIC_ENTRY_MASK                   0x1f
#define UX_SYNERGY_ENABLE                                1
#define UX_SYNERGY_DISABLE                               0

                                                        
/* Protection against no definition of Synergy controller.   */
#ifdef UX_SYNERGY_CONTROLLER_S7G2
#define UX_SYNERGY_HC_SYSCFG                             (0x00U)
#define UX_SYNERGY_HC_BUSWAIT                            (0x02U)
#define UX_SYNERGY_HC_SYSSTS0                            (0x04U)
#define UX_SYNERGY_HC_PLLSTA                             (0x06U)
#define UX_SYNERGY_HC_DVSTCTR0                           (0x08U)
#define UX_SYNERGY_HC_CFIFO                              (0x14U)
#define UX_SYNERGY_HC_CFIFOH                             (0x16U)
#define UX_SYNERGY_HC_CFIFOHH                            (0x17U)
#define UX_SYNERGY_HC_D0FIFO                             (0x18U)
#define UX_SYNERGY_HC_D1FIFO                             (0x1CU)
#define UX_SYNERGY_HC_CFIFOSEL                           (0x20U)
#define UX_SYNERGY_HC_CFIFOCTR                           (0x22U)
#define UX_SYNERGY_HC_D0FIFOSEL                          (0x28U)
#define UX_SYNERGY_HC_D0FIFOCTR                          (0x2AU)
#define UX_SYNERGY_HC_D1FIFOSEL                          (0x2CU)
#define UX_SYNERGY_HC_D1FIFOCTR                          (0x2EU)
#define UX_SYNERGY_HC_INTENB0                            (0x30U)
#define UX_SYNERGY_HC_INTENB1                            (0x32U)
#define UX_SYNERGY_HC_BRDYENB                            (0x36U)
#define UX_SYNERGY_HC_NRDYENB                            (0x38U)
#define UX_SYNERGY_HC_BEMPENB                            (0x3AU)
#define UX_SYNERGY_HC_SOFCFG                             (0x3CU)
#define UX_SYNERGY_HC_PHYSET                             (0x3EU)
#define UX_SYNERGY_HC_INTSTS0                            (0x40U)
#define UX_SYNERGY_HC_INTSTS1                            (0x42U)
#define UX_SYNERGY_HC_BRDYSTS                            (0x46U)
#define UX_SYNERGY_HC_NRDYSTS                            (0x48U)
#define UX_SYNERGY_HC_BEMPSTS                            (0x4AU)
#define UX_SYNERGY_HC_FRMNUM                             (0x4CU)
#define UX_SYNERGY_HC_DVCHGR                             (0x4EU)
#define UX_SYNERGY_HC_USBADDR                            (0x50U)
#define UX_SYNERGY_HC_USBREQ                             (0x54U)
#define UX_SYNERGY_HC_USBVAL                             (0x56U)
#define UX_SYNERGY_HC_USBINDX                            (0x58U)
#define UX_SYNERGY_HC_USBLENG                            (0x5AU)
#define UX_SYNERGY_HC_DCPCFG                             (0x5CU)
#define UX_SYNERGY_HC_DCPMAXP                            (0x5EU)
#define UX_SYNERGY_HC_DCPCTR                             (0x60U)
#define UX_SYNERGY_HC_PIPESEL                            (0x64U)
#define UX_SYNERGY_HC_PIPECFG                            (0x68U)
#define UX_SYNERGY_HC_PIPEBUF                            (0x6AU)
#define UX_SYNERGY_HC_PIPEMAXP                           (0x6CU)
#define UX_SYNERGY_HC_PIPEPERI                           (0x6EU)
#define UX_SYNERGY_HC_PIPE1CTR                           (0x70U)
#define UX_SYNERGY_HC_PIPE2CTR                           (0x72U)
#define UX_SYNERGY_HC_PIPE3CTR                           (0x74U)
#define UX_SYNERGY_HC_PIPE4CTR                           (0x76U)
#define UX_SYNERGY_HC_PIPE5CTR                           (0x78U)
#define UX_SYNERGY_HC_PIPE6CTR                           (0x7AU)
#define UX_SYNERGY_HC_PIPE7CTR                           (0x7CU)
#define UX_SYNERGY_HC_PIPE8CTR                           (0x7EU)
#define UX_SYNERGY_HC_PIPE9CTR                           (0x80U)
#define UX_SYNERGY_HC_PIPE1TRE                           (0x90U)
#define UX_SYNERGY_HC_PIPE1TRN                           (0x92U)
#define UX_SYNERGY_HC_PIPE2TRE                           (0x94U)
#define UX_SYNERGY_HC_PIPE2TRN                           (0x96U)
#define UX_SYNERGY_HC_PIPE3TRE                           (0x98U)
#define UX_SYNERGY_HC_PIPE3TRN                           (0x9AU)
#define UX_SYNERGY_HC_PIPE4TRE                           (0x9CU)
#define UX_SYNERGY_HC_PIPE4TRN                           (0x9EU)
#define UX_SYNERGY_HC_PIPE5TRE                           (0xA0U)
#define UX_SYNERGY_HC_PIPE5TRN                           (0xA2U)
#define UX_SYNERGY_HC_DEVADD0                            (0xD0U)
#define UX_SYNERGY_HC_DEVADD1                            (0xD2U)
#define UX_SYNERGY_HC_DEVADD2                            (0xD4U)
#define UX_SYNERGY_HC_DEVADD3                            (0xD6U)
#define UX_SYNERGY_HC_DEVADD4                            (0xD8U)
#define UX_SYNERGY_HC_DEVADD5                            (0xDAU)
#define UX_SYNERGY_HC_PHYSLEW                            (0xF0U)
#define UX_SYNERGY_HC_LPSTS                              (0x102U)

#endif

#define UX_SYNERGY_HC_PFKUSB_ENABLED                     (1<<4)
#define UX_SYNERGY_HC_PFKUSB_MODE_HOST                   (1)
#define UX_SYNERGY_HC_BUSWAIT_VAL                        (5)

/* Define Synergy control register values.  */
#define UX_SYNERGY_HC_SYSCFG_SCKE                        (1<<10)
#define UX_SYNERGY_HC_SYSCFG_CNEN                        (1<<8)
#define UX_SYNERGY_HC_SYSCFG_HSE                         (1<<7)
#define UX_SYNERGY_HC_SYSCFG_DCFM                        (1<<6)
#define UX_SYNERGY_HC_SYSCFG_DRPD                        (1<<5)
#define UX_SYNERGY_HC_SYSCFG_DPRPU                       (1<<4)
#define UX_SYNERGY_HC_SYSCFG_USBE                        (1<<0)

#define UX_SYNERGY_HC_SYSSTS0_LNST                       3
#define UX_SYNERGY_HC_SYSSTS0_IDMON                      (1<<2)
#define UX_SYNERGY_HC_SYSSTS0_SOFEA                      (1<<6)
#define UX_SYNERGY_HC_SYSSTS0_HTACT                      (1<<6)
#define UX_SYNERGY_HC_SYSSTS0_OVCMON                     (0xC0000)

#define UX_SYNERGY_HC_PLLSTA_PLLLOCK                     (1<<0)



#define UX_SYNERGY_HC_DVSTCTR0_HNPBTOA                   (1<<11)
#define UX_SYNERGY_HC_DVSTCTR0_EXICEN                    (1<<10)
#define UX_SYNERGY_HC_DVSTCTR0_VBUSEN                    (1<<9)
#define UX_SYNERGY_HC_DVSTCTR0_WKUP                      (1<<8)
#define UX_SYNERGY_HC_DVSTCTR0_RWUPE                     (1<<7)
#define UX_SYNERGY_HC_DVSTCTR0_USBRST                    (1<<6)
#define UX_SYNERGY_HC_DVSTCTR0_RESUME                    (1<<5)
#define UX_SYNERGY_HC_DVSTCTR0_UACT                      (1<<4)
#define UX_SYNERGY_HC_DVSTCTR0_RHST                      (0x7)

#define UX_SYNERGY_HC_DVSTCTR0_SPEED_LOW                 (1)
#define UX_SYNERGY_HC_DVSTCTR0_SPEED_FULL                (2)
#define UX_SYNERGY_HC_DVSTCTR0_SPEED_HIGH                (3)
#define UX_SYNERGY_HC_DVSTCTR0_RESET_IN_PROGRESS         (4)

#define UX_SYNERGY_HC_CFIFOSEL_RCNT                      (1<<15)
#define UX_SYNERGY_HC_CFIFOSEL_REW                       (1<<14)
#define UX_SYNERGY_HC_CFIFOSEL_MBW_8                     (0<<10)
#define UX_SYNERGY_HC_CFIFOSEL_MBW_16                    (1<<10)
#define UX_SYNERGY_HC_CFIFOSEL_MBW_32                    (2<<10)
#define UX_SYNERGY_HC_CFIFOSEL_MBW_MASK                  (3U<<10)
#define UX_SYNERGY_HC_CFIFOSEL_BIGEND                    (1<<8)
#define UX_SYNERGY_HC_CFIFOSEL_ISEL                      (1U<<5)
#define UX_SYNERGY_HC_CFIFOSEL_CURPIPE_MASK              (0xF)

#define UX_SYNERGY_HC_DFIFOSEL_RCNT                      (1<<15)
#define UX_SYNERGY_HC_DFIFOSEL_REW                       (1<<14)
#define UX_SYNERGY_HC_DFIFOSEL_DCLRM                     (1<<13)
#define UX_SYNERGY_HC_DFIFOSEL_DREQE                     (1<<12)
#define UX_SYNERGY_HC_DFIFOSEL_MBW_8                     (0<<10)
#define UX_SYNERGY_HC_DFIFOSEL_MBW_16                    (1<<10)
#define UX_SYNERGY_HC_DFIFOSEL_MBW_32                    (2<<10)
#define UX_SYNERGY_HC_DFIFOSEL_MBW_MASK                  (3<<10)
#define UX_SYNERGY_HC_DFIFOSEL_BIGEND                    (1<<8)
#define UX_SYNERGY_HC_DFIFOSEL_CURPIPE_MASK              (0xF)

#define UX_SYNERGY_HC_FIFOCTR_BVAL                       (1<<15)
#define UX_SYNERGY_HC_FIFOCTR_BCLR                       (1<<14)
#define UX_SYNERGY_HC_FIFOCTR_FRDY                       (1<<13)
#define UX_SYNERGY_HC_FIFOCTR_DTLN                       (0xFFF)
                                                    
#define UX_SYNERGY_HC_INTENB0_VBSE                       (1<<15)
#define UX_SYNERGY_HC_INTENB0_RSME                       (1<<14)
#define UX_SYNERGY_HC_INTENB0_SOFE                       (1<<13)
#define UX_SYNERGY_HC_INTENB0_DVSE                       (1<<12)
#define UX_SYNERGY_HC_INTENB0_CTRE                       (1<<11)
#define UX_SYNERGY_HC_INTENB0_BEMPE                      (1<<10)
#define UX_SYNERGY_HC_INTENB0_NRDYE                      (1<<9)
#define UX_SYNERGY_HC_INTENB0_BRDYE                      (1<<8)

#define UX_SYNERGY_HC_INTENB1_OVRCRE                     (1<<15)
#define UX_SYNERGY_HC_INTENB1_BCHGE                      (1<<14)
#define UX_SYNERGY_HC_INTENB1_DTCHE                      (1<<12)
#define UX_SYNERGY_HC_INTENB1_ATTCHE                     (1<<11)
#define UX_SYNERGY_HC_INTENB1_L1RSMENDE                  (1<<9)
#define UX_SYNERGY_HC_INTENB1_LPSMENDE                   (1<<8)
#define UX_SYNERGY_HC_INTENB1_EOFERRE                    (1<<6)
#define UX_SYNERGY_HC_INTENB1_SIGNE                      (1<<5)
#define UX_SYNERGY_HC_INTENB1_SACKE                      (1<<4)
#define UX_SYNERGY_HC_INTENB1_PDDETINTE                  (1<<0)

#define UX_SYNERGY_HC_PIPE0                              (1<<0)
#define UX_SYNERGY_HC_PIPE1                              (1<<1)
#define UX_SYNERGY_HC_PIPE2                              (1<<2)
#define UX_SYNERGY_HC_PIPE3                              (1<<3)
#define UX_SYNERGY_HC_PIPE4                              (1<<4)
#define UX_SYNERGY_HC_PIPE5                              (1<<5)
#define UX_SYNERGY_HC_PIPE6                              (1<<6)
#define UX_SYNERGY_HC_PIPE7                              (1<<7)
#define UX_SYNERGY_HC_PIPE8                              (1<<8)
#define UX_SYNERGY_HC_PIPE9                              (1<<9)
#define UX_SYNERGY_HC_PIPE_ALL                           (0x3FF)

#define UX_SYNERGY_HC_SOFCFG_TRNENSEL                    (1<<8)
#define UX_SYNERGY_HC_SOFCFG_BRDYM                       (1<<6)
#define UX_SYNERGY_HC_SOFCFG_INTL                        (1<<5)
#define UX_SYNERGY_HC_SOFCFG_EDGESTS                     (1<<4)

#define UX_SYNERGY_HC_PHYSET_HSEB                        (1<<15)
#define UX_SYNERGY_HC_PHYSET_REPSTART                    (1<<11)
#define UX_SYNERGY_HC_PHYSET_REPSEL                      (1<<8)
#define UX_SYNERGY_HC_PHYSET_CLKSEL_1                    (1<<5)
#define UX_SYNERGY_HC_PHYSET_CLKSEL_0                    (1<<4)
#define UX_SYNERGY_HC_PHYSET_CDPEN                       (1<<3)
#define UX_SYNERGY_HC_PHYSET_PLLRESET                    (1<<1)
#define UX_SYNERGY_HC_PHYSET_DIRPD                       (1<<0)


#define UX_SYNERGY_HC_INTSTS0_VBINT                      (1<<15)
#define UX_SYNERGY_HC_INTSTS0_RESM                       (1<<14)
#define UX_SYNERGY_HC_INTSTS0_SOFR                       (1<<13)
#define UX_SYNERGY_HC_INTSTS0_DVST                       (1<<12)
#define UX_SYNERGY_HC_INTSTS0_CTRT                       (1<<11)
#define UX_SYNERGY_HC_INTSTS0_BEMP                       (1<<10)
#define UX_SYNERGY_HC_INTSTS0_NRDY                       (1<<9)
#define UX_SYNERGY_HC_INTSTS0_BRDY                       (1<<8)
#define UX_SYNERGY_HC_INTSTS0_VBSTS                      (1<<7)
#define UX_SYNERGY_HC_INTSTS0_VALID                      (1<<3)
#define UX_SYNERGY_HC_INTSTS0_CTSQ_MASK                  (7)

#define UX_SYNERGY_HC_INTSTS1_OVRCRE                     (1<<15)
#define UX_SYNERGY_HC_INTSTS1_BCHG                       (1<<14)
#define UX_SYNERGY_HC_INTSTS1_DTCH                       (1<<12)
#define UX_SYNERGY_HC_INTSTS1_ATTCH                      (1<<11)
#define UX_SYNERGY_HC_INTSTS1_EOFERR                     (1<<6)
#define UX_SYNERGY_HC_INTSTS1_SIGN                       (1U<<5)
#define UX_SYNERGY_HC_INTSTS1_SACK                       (1U<<4)

#define UX_SYNERGY_HC_FRMNUM_OVRN                        (1<<15)
#define UX_SYNERGY_HC_FRMNUM_CRCE                        (1<<14)
#define UX_SYNERGY_HC_FRMNUM_FRNM_MASK                   (0x7FF)
 
#define UX_SYNERGY_HC_DCPCFG_DIR                         (1<<4)

#define UX_SYNERGY_HC_DCPMAXP_DEVADDR_SHIFT              (12)
#define UX_SYNERGY_HC_DCPMAXP_DEVADDR_MASK               0xf000

#define UX_SYNERGY_HC_DCPCTR_BSTS                        (1<<15)
#define UX_SYNERGY_HC_DCPCTR_SUREQ                       (1<<14)
#define UX_SYNERGY_HC_DCPCTR_SUREQCLR                    (1<<11)
#define UX_SYNERGY_HC_DCPCTR_SQCLR                       (1<<8)
#define UX_SYNERGY_HC_DCPCTR_SQSET                       (1<<7)
#define UX_SYNERGY_HC_DCPCTR_SQMON                       (1<<6)
#define UX_SYNERGY_HC_DCPCTR_PBUSY                       (1<<5)
#define UX_SYNERGY_HC_DCPCTR_CCPL                        (1<<2)
#define UX_SYNERGY_HC_DCPCTR_PID_MASK                    (3)
#define UX_SYNERGY_HC_DCPCTR_PIDNAK                      (0)
#define UX_SYNERGY_HC_DCPCTR_PIDBUF                      (1)
#define UX_SYNERGY_HC_DCPCTR_PIDSTALL                    (2)
#define UX_SYNERGY_HC_DCPCTR_PIDSTALL2                   (3)


#define UX_SYNERGY_HC_PIPECFG_TYPE_MASK                  (0xC000)
#define UX_SYNERGY_HC_PIPECFG_TYPE_BIT_USED              (0)
#define UX_SYNERGY_HC_PIPECFG_TYPE_BULK                  (1<<14)
#define UX_SYNERGY_HC_PIPECFG_TYPE_INTERRUPT             (2<<14)
#define UX_SYNERGY_HC_PIPECFG_TYPE_ISOCHRONOUS           (3<<14)
#define UX_SYNERGY_HC_PIPECFG_BFRE                       (1<<10)
#define UX_SYNERGY_HC_PIPECFG_DBLB                       (1<<9)
#define UX_SYNERGY_HC_PIPECFG_SHTNAK                     (1<<7)
#define UX_SYNERGY_HC_PIPECFG_DIR                        (1<<4)
#define UX_SYNERGY_HC_PIPECFG_EPNUM_MASK                 (0xF)

#define UX_SYNERGY_HC_PIPEBUF_SIZEMASK                   (0x1F<<10)
#define UX_SYNERGY_HC_PIPEBUF_BUFNMBMASK                 (0xFF<<10)
#define UX_SYNERGY_HC_PIPEBUF_SHIFT                      (10)

#define UX_SYNERGY_HC_PIPEMAXP_DEVSELMASK                (0xF<<12)
#define UX_SYNERGY_HC_PIPEMAXP_DEVSEL_SHIFT              (12)
#define UX_SYNERGY_HC_PIPEMAXP_MXPSMASK                  (0x7FF)

#define UX_SYNERGY_HC_PIPE1TRE_TRCLR                     (1<<8)
#define UX_SYNERGY_HC_PIPE1TRE_TRENB                     (1<<9)

#define UX_SYNERGY_HC_FIFO_D0                            0
#define UX_SYNERGY_HC_FIFO_D1                            1
#define UX_SYNERGY_HC_FIFO_C                             2

#define UX_SYNERGY_HC_DEVADD_SPEED_LOW                  (1<<6)
#define UX_SYNERGY_HC_DEVADD_SPEED_FULL                 (2<<6)
#define UX_SYNERGY_HC_DEVADD_SPEED_HIGH                 (3<<6)

#define UX_SYNERGY_HC_DEVADD_UPPHUB_SHIFT                11
#define UX_SYNERGY_HC_DEVADD_HUBPORT_SHIFT               8

/* Define Synergy HCOR command/status bitmaps.  */

#define UX_SYNERGY_HC_DCP                                0

#define UX_SYNERGY_HC_DCPCTR_DATA1                       (1<<7)
#define UX_SYNERGY_HC_DCPCTR_DATA0                       (1<<8)

/* Define Synergy PIPE selection definitions.  */
#define UX_SYNERGY_HC_PIPESEL_NO_PIPE                    0x000f

/* Define Synergy fifo definition.  */
#define UX_SYNERGY_HC_PIPE0_SIZE                         256
#define UX_SYNERGY_HC_PIPE_NB_BUFFERS                    64


#define UX_SYNERGY_HC_PIPECTR_BSTS                       (1<<15)
#define UX_SYNERGY_HC_PIPECTR_INBUFM                     (1<<14)
#define UX_SYNERGY_HC_PIPECTR_ATREPM                     (1<<10)
#define UX_SYNERGY_HC_PIPECTR_ACLRM                      (1<<9)
#define UX_SYNERGY_HC_PIPECTR_SQCLR                      (1<<8)
#define UX_SYNERGY_HC_PIPECTR_SQSET                      (1<<7)
#define UX_SYNERGY_HC_PIPECTR_SQMON                      (1<<6)
#define UX_SYNERGY_HC_PIPECTR_PBUSY                      (1<<5)
#define UX_SYNERGY_HC_PIPECTR_PID_MASK                   (3)
#define UX_SYNERGY_HC_PIPECTR_PIDNAK                     (0)
#define UX_SYNERGY_HC_PIPECTR_PIDBUF                     (1)
#define UX_SYNERGY_HC_PIPECTR_PIDSTALL                   (2)
#define UX_SYNERGY_HC_PIPECTR_PIDSTALL2                  (3)
#define UX_SYNERGY_HC_PIPECTR_DATA1                      (1<<7)
#define UX_SYNERGY_HC_PIPECTR_DATA0                      (1<<8)

/* Define Synergy static definition.  */

#define UX_SYNERGY_HC_AVAILABLE_BANDWIDTH                920
#define UX_SYNERGY_HC_INIT_DELAY                         1000
#define UX_SYNERGY_HC_RESET_RETRY                        1000
#define UX_SYNERGY_HC_RESET_DELAY                        10
#define UX_SYNERGY_HC_PORT_RESET_RETRY                   50
#define UX_SYNERGY_HC_FORCE_PORT_RESET_RETRY             50
#define UX_SYNERGY_HC_FORCE_PORT_RESET_DELAY             1
#define UX_SYNERGY_HC_CHECK_PORT_RESET_RETRY             500
#define UX_SYNERGY_HC_PORT_RESET_DELAY                   300 
#define UX_SYNERGY_HC_PORT_RESET_RECOVERY_DELAY          100 

/* Define Synergy initialization values.  */

#define UX_SYNERGY_HC_COMMAND_STATUS_RESET               0
#define UX_SYNERGY_HC_INIT_RESET_DELAY                   10
#define UX_SYNERGY_HC_MAX_BUF_SIZE                       64
#define UX_SYNERGY_HC_MAX_BUF_NUM                        127


/* Define Synergy FIFO write completion code.  */
#define UX_SYNERGY_HC_FIFO_WRITING                       2
#define UX_SYNERGY_HC_FIFO_WRITE_END                     3
#define UX_SYNERGY_HC_FIFO_WRITE_SHORT                   4
#define UX_SYNERGY_HC_FIFO_WRITE_DMA                     5
#define UX_SYNERGY_HC_FIFO_WRITE_ERROR                   6

/* Define Synergy FIFO read completion code.  */
#define UX_SYNERGY_HC_FIFO_READING                       2
#define UX_SYNERGY_HC_FIFO_READ_END                      3
#define UX_SYNERGY_HC_FIFO_READ_SHORT                    4
#define UX_SYNERGY_HC_FIFO_READ_DMA                      5
#define UX_SYNERGY_HC_FIFO_READ_ERROR                    6
#define UX_SYNERGY_HC_FIFO_READ_OVER                     7

/* Define Synergy physical endpoint definitions.  */
#define UX_SYNERGY_HC_ED_BRDY                            0x00000001U
#define UX_SYNERGY_HC_ED_NRDY                            0x00000002U
#define UX_SYNERGY_HC_ED_BEMP                            0x00000004U
#define UX_SYNERGY_HC_ED_EOFERR                          0x00000010U
#define UX_SYNERGY_HC_ED_SIGN                            0x00000020U
#define UX_SYNERGY_HC_ED_SACK                            0x00000040U

/* PHY Cross Point Adjustment, note that Hardware Manual to be updated(0xE->0x5) */
#define UX_SYNERGY_HC_PHYSLEW_SLEW_SLEWR00               (1U<<0)
#define UX_SYNERGY_HC_PHYSLEW_SLEW_SLEWR01               (1U<<1)
#define UX_SYNERGY_HC_PHYSLEW_SLEW_SLEWF00               (1U<<2)
#define UX_SYNERGY_HC_PHYSLEW_SLEW_SLEWF01               (1U<<3)

#define UX_SYNERGY_HC_LPSTS_SUSPENDM                     (1<<14)

/* Define Synergy Root hub states.  */
#define UX_SYNERGY_HC_PORT_ENABLED                       1
#define UX_SYNERGY_HC_PORT_DISABLED                      0


/* Define synergy transfer structure*/
typedef struct UX_HCD_SYNERGY_TRANSFER_STRUCT
{
    transfer_instance_t *ux_synergy_transfer_tx;
    transfer_instance_t *ux_synergy_transfer_rx;

} UX_HCD_SYNERGY_TRANSFER;

/* Define Synergy structure.  */

typedef struct UX_HCD_SYNERGY_STRUCT
{

    struct UX_HCD_STRUCT
                    *ux_hcd_synergy_hcd_owner;
    ULONG           ux_hcd_synergy_base;
    ULONG           ux_hcd_synergy_hcor;
    UINT            ux_hcd_synergy_nb_root_hubs;
    ULONG           ux_hcd_synergy_port_status;
    ULONG           ux_hcd_synergy_frame_number;
    ULONG           ux_hcd_synergy_root_hub_device_address;
    struct UX_SYNERGY_ED_STRUCT         
                    *ux_hcd_synergy_ed_list;
    struct UX_SYNERGY_TD_STRUCT         
                    *ux_hcd_synergy_td_list;
    struct UX_SYNERGY_ISO_TD_STRUCT     
                    *ux_hcd_synergy_iso_td_list;
    struct UX_SYNERGY_ED_STRUCT         
                    *ux_hcd_synergy_asynch_head_ed;   
    struct UX_SYNERGY_ED_STRUCT         
                    *ux_hcd_synergy_asynch_current_ed;              
    struct UX_SYNERGY_ED_STRUCT         
                    *ux_hcd_synergy_iso_head_ed;
    struct UX_SYNERGY_ED_STRUCT         
                    *ux_hcd_synergy_interrupt_ed_list[32];
    UINT            ux_hcd_synergy_queue_empty;
    UINT            ux_hcd_synergy_periodic_scheduler_active;
    ULONG           ux_hcd_synergy_number_tds;
    UINT            ux_hcd_synergy_interruptible;
    ULONG           ux_hcd_synergy_interrupt_count;
    ULONG           ux_hcd_synergy_intsts0;
    ULONG           ux_hcd_synergy_intsts1;
    ULONG           ux_synergy_device_attached;
    ULONG           ux_synergy_next_available_bufnum;    /* will need to implement some type of dynamic buffer management, for now just carve off */
    ULONG           ux_hcd_synergy_ed_irq[10];
    struct UX_SYNERGY_ED_STRUCT  
                    *ux_hcd_synergy_ed_ptr[10];
    UX_HCD_SYNERGY_TRANSFER  ux_hcd_synergy_transfer;
    transfer_cfg_t  ux_hcd_synergy_transfer_cfg_instance_tx;
    transfer_cfg_t  ux_hcd_synergy_transfer_cfg_instance_rx;
    TX_SEMAPHORE    ux_hcd_synergy_semaphore_tx;
    TX_SEMAPHORE    ux_hcd_synergy_semaphore_rx;

} UX_HCD_SYNERGY;


/* Define Synergy  ED structure.  */

typedef struct UX_SYNERGY_ED_STRUCT
{

    struct UX_SYNERGY_TD_STRUCT        
                    *ux_synergy_ed_tail_td;
    struct UX_SYNERGY_TD_STRUCT        
                    *ux_synergy_ed_head_td;
    struct UX_SYNERGY_ED_STRUCT        
                    *ux_synergy_ed_next_ed;
    struct UX_SYNERGY_ED_STRUCT        
                    *ux_synergy_ed_previous_ed;

    ULONG           ux_synergy_ed_status;
    struct UX_ENDPOINT_STRUCT          
                    *ux_synergy_ed_endpoint;
    ULONG           ux_synergy_ed_toggle;   
    ULONG           ux_synergy_ed_frame;   

    ULONG           ux_synergy_endpoint_index; 
    ULONG           ux_synergy_pipe_index; 
    ULONG           ux_synergy_fifo_index; 

    ULONG           ux_synergy_buffer_size;
    ULONG           ux_synergy_buffer_number;

} UX_SYNERGY_ED;


/* Define Synergy ED bitmap.  */

#define UX_SYNERGY_ED_STATIC                                0x80000000
#define UX_SYNERGY_ED_SKIP                                  0x40000000UL

/* Define Synergy TD structure.  */

typedef struct UX_SYNERGY_TD_STRUCT
{                                                

    UCHAR *         ux_synergy_td_buffer;
    ULONG           ux_synergy_td_length;
    struct UX_SYNERGY_TD_STRUCT        
                    *ux_synergy_td_next_td;
    struct UX_TRANSFER_STRUCT          
                    *ux_synergy_td_transfer_request;
    struct UX_SYNERGY_TD_STRUCT        
                    *ux_synergy_td_next_td_transfer_request;
    struct UX_SYNERGY_ED_STRUCT        
                    *ux_synergy_td_ed;
    ULONG           ux_synergy_td_actual_length;
    ULONG           ux_synergy_td_status;
    ULONG           ux_synergy_td_direction;
    ULONG           ux_synergy_td_toggle;
    ULONG           ux_synergy_td_transfer_type; 
    ULONG           ux_synergy_td_error_count;

} UX_SYNERGY_TD;


/* Define Synergy TD bitmap.  */

#define UX_SYNERGY_TD_SETUP_PHASE                           0x00010000   
#define UX_SYNERGY_TD_DATA_PHASE                            0x00020000   
#define UX_SYNERGY_TD_STATUS_PHASE                          0x00040000   
#define UX_SYNERGY_TD_OUT                                   0x00000800
#define UX_SYNERGY_TD_IN                                    0x00001000   
#define UX_SYNERGY_TD_TOGGLE_FROM_ED                        0x80000000
#define UX_SYNERGY_TD_ACK_PENDING                           0x00080000


#define UX_SYNERGY_TD_SETUP_TYPE                            1
#define UX_SYNERGY_TD_DATA_TYPE                             2
#define UX_SYNERGY_TD_STATUS_TYPE                           3
#define UX_SYNERGY_TD_MAX_ERROR                             3

/* Define Synergy ISOCHRONOUS TD structure.  */

typedef struct UX_SYNERGY_ISO_TD_STRUCT
{

    UCHAR *         ux_synergy_iso_td_buffer;
    ULONG           ux_synergy_iso_td_length;
    struct UX_SYNERGY_ISO_TD_STRUCT    
                    *ux_synergy_iso_td_next_td;
    struct UX_TRANSFER_STRUCT          
                    *ux_synergy_iso_td_transfer_request;
    struct UX_SYNERGY_ISO_TD_STRUCT    
                    *ux_synergy_iso_td_next_td_transfer_request;
    struct UX_SYNERGY_ED_STRUCT        
                    *ux_synergy_iso_td_ed;
    ULONG           ux_synergy_iso_td_actual_length;
    ULONG           ux_synergy_iso_td_status;
    ULONG           ux_synergy_iso_td_direction;
    
} UX_SYNERGY_ISO_TD;


/* Define Synergy function prototypes.  */

VOID    _ux_hcd_synergy_asynch_queue_process(UX_HCD_SYNERGY *hcd_synergy);
VOID    _ux_hcd_synergy_asynch_schedule(UX_HCD_SYNERGY *hcd_synergy);
UINT    _ux_hcd_synergy_asynchronous_endpoint_create(UX_HCD_SYNERGY *hcd_synergy, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_synergy_asynchronous_endpoint_destroy(UX_HCD_SYNERGY *hcd_synergy, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_synergy_control_endpoint_create(UX_HCD_SYNERGY *hcd_synergy, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_synergy_bulk_endpoint_create(UX_HCD_SYNERGY *hcd_synergy, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_synergy_control_td_add(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
UINT    _ux_hcd_synergy_bulk_int_td_add(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
UINT    _ux_hcd_synergy_controller_disable(UX_HCD_SYNERGY *hcd_synergy);
UX_SYNERGY_ED       *_ux_hcd_synergy_ed_obtain(UX_HCD_SYNERGY *hcd_synergy);
VOID    _ux_hcd_synergy_ed_td_clean(UX_SYNERGY_ED *ed);
UINT    _ux_hcd_synergy_endpoint_reset(UX_HCD_SYNERGY *hcd_synergy, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_synergy_entry(UX_HCD *hcd, UINT function, VOID *parameter);
UINT    _ux_hcd_synergy_frame_number_get(UX_HCD_SYNERGY *hcd_synergy, ULONG *frame_number);
VOID    _ux_hcd_synergy_frame_number_set(UX_HCD_SYNERGY *hcd_synergy, ULONG frame_number);
ULONG   _ux_hcd_synergy_register_read(UX_HCD_SYNERGY *hcd_synergy, ULONG synergy_register);
VOID    _ux_hcd_synergy_register_write(UX_HCD_SYNERGY *hcd_synergy, ULONG synergy_register, USHORT register_value);
VOID    _ux_hcd_synergy_register_clear(UX_HCD_SYNERGY *hcd_synergy, ULONG synergy_register, USHORT register_value);
VOID    _ux_hcd_synergy_register_set(UX_HCD_SYNERGY *hcd_synergy, ULONG synergy_register, USHORT register_value);
UINT    _ux_hcd_synergy_initialize(UX_HCD *hcd);
UINT    _ux_hcd_synergy_interrupt_endpoint_create(UX_HCD_SYNERGY *hcd_synergy, UX_ENDPOINT *endpoint);
VOID    _ux_hcd_synergy_interrupt_handler(UINT);
VOID    _ux_hcd_synergy_iso_queue_process(UX_HCD_SYNERGY *hcd_synergy);
VOID    _ux_hcd_synergy_iso_schedule(UX_HCD_SYNERGY *hcd_synergy);
UINT    _ux_hcd_synergy_isochronous_endpoint_create(UX_HCD_SYNERGY *hcd_synergy, UX_ENDPOINT *endpoint);
UX_SYNERGY_ISO_TD   *_ux_hcd_synergy_isochronous_td_obtain(UX_HCD_SYNERGY *hcd_synergy);
UX_SYNERGY_ED       *_ux_hcd_synergy_least_traffic_list_get(UX_HCD_SYNERGY *hcd_synergy);
UINT    _ux_hcd_synergy_periodic_endpoint_destroy(UX_HCD_SYNERGY *hcd_synergy, UX_ENDPOINT *endpoint);
VOID    _ux_hcd_synergy_periodic_schedule(UX_HCD_SYNERGY *hcd_synergy);
UINT    _ux_hcd_synergy_periodic_tree_create(UX_HCD_SYNERGY *hcd_synergy);
UINT    _ux_hcd_synergy_port_disable(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index);
UINT    _ux_hcd_synergy_port_enable(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index);
UINT    _ux_hcd_synergy_port_reset(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index);
UINT    _ux_hcd_synergy_port_resume(UX_HCD_SYNERGY *hcd_synergy, UINT port_index);
ULONG   _ux_hcd_synergy_port_status_get(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index);
UINT    _ux_hcd_synergy_port_suspend(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index);
UINT    _ux_hcd_synergy_power_down_port(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index);
UINT    _ux_hcd_synergy_power_on_port(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index);
VOID    _ux_hcd_synergy_power_root_hubs(UX_HCD_SYNERGY *hcd_synergy);
UINT    _ux_hcd_synergy_td_add(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
UX_SYNERGY_TD       *_ux_hcd_synergy_regular_td_obtain(UX_HCD_SYNERGY *hcd_synergy);
UINT    _ux_hcd_synergy_request_bulk_transfer(UX_HCD_SYNERGY *hcd_synergy, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_synergy_request_control_transfer(UX_HCD_SYNERGY *hcd_synergy, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_synergy_request_interrupt_transfer(UX_HCD_SYNERGY *hcd_synergy, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_synergy_request_isochronous_transfer(UX_HCD_SYNERGY *hcd_synergy, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_synergy_request_transfer(UX_HCD_SYNERGY *hcd_synergy, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_synergy_transfer_abort(UX_HCD_SYNERGY *hcd_synergy, UX_TRANSFER *transfer_request);
ULONG   _ux_hcd_synergy_fifo_port_change(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed, ULONG direction);
VOID    _ux_hcd_synergy_current_endpoint_change(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed, ULONG direction);
ULONG   _ux_hcd_synergy_data_buffer_size(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
UINT    _ux_hcd_synergy_buffer_write(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
UINT    _ux_hcd_synergy_fifod_write(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
UINT    _ux_hcd_synergy_fifoc_write(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
UINT    _ux_hcd_synergy_fifo_read(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
UINT    _ux_hcd_synergy_buffer_read(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
VOID    _ux_hcd_synergy_buffer_ready_interrupt(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed, ULONG flag);
VOID    _ux_hcd_synergy_buffer_empty_interrupt(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed, ULONG flag);
VOID    _ux_hcd_synergy_endpoint_nak_set(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
VOID    _ux_hcd_synergy_asynch_queue_process_bemp(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
VOID    _ux_hcd_synergy_asynch_queue_process_nrdy(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
VOID    _ux_hcd_synergy_asynch_queue_process_brdy(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
VOID    _ux_hcd_synergy_asynch_queue_process_sign(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed);
VOID    _ux_hcd_synergy_buffer_notready_interrupt(UX_HCD_SYNERGY *hcd_synergy, UX_SYNERGY_ED *ed, ULONG flag);
UINT    _ux_hcd_synergy_initialize_transfer_support(UX_HCD *hcd, const UX_HCD_SYNERGY_TRANSFER *p_transfer_instance);

#define ux_hcd_synergy_initialize                      _ux_hcd_synergy_initialize
#define ux_hcd_synergy_initialize_transfer_support     _ux_hcd_synergy_initialize_transfer_support
#define ux_hcd_synergy_interrupt_handler               _ux_hcd_synergy_interrupt_handler

#endif

