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

/** NOTE: This file exists to maintain backward compatibility with Renesas Synergy SSP projects created with versions
 * prior to SSP version 1.1.0.  DO NOT USE THIS FILE IN NEW PROJECTS.  Use ux_hcd_synergy.h instead. */

#ifndef UX_HCD_RX_H
#define UX_HCD_RX_H

#include "ux_hcd_synergy.h"

/* Define synergy generic definitions.  */

#define UX_RX_CONTROLLER                            (UX_SYNERGY_CONTROLLER)
#define UX_RX_CONTROLLER_RX62N                      (UX_SYNERGY_CONTROLLER_S7G2)
#define UX_RX_MAX_BULK_PAYLOAD                      (UX_SYNERGY_MAX_BULK_PAYLOAD)
#define UX_RX_MAX_CONTROL_PAYLOAD                   (UX_SYNERGY_MAX_CONTROL_PAYLOAD)
#define UX_RX_FRAME_DELAY                           (UX_SYNERGY_FRAME_DELAY)
#define UX_RX_PERIODIC_ENTRY_NB                     (UX_SYNERGY_PERIODIC_ENTRY_NB)
#define UX_RX_PERIODIC_ENTRY_MASK                   (UX_SYNERGY_PERIODIC_ENTRY_MASK)
#define UX_RX_ENABLE                                (UX_SYNERGY_ENABLE)
#define UX_RX_DISABLE                               (UX_SYNERGY_DISABLE)

                                                        
/* Protection against no definition of SYNERGY controller.   */


#define UX_RX_USB_BASE                              (UX_SYNERGY_USB_BASE)
                                                    

#define UX_RX_HC_SYSCFG                             (UX_SYNERGY_HC_SYSCFG)
#define UX_RX_HC_SYSSTS0                            (UX_SYNERGY_HC_SYSSTS0)
#define UX_RX_HC_PLLSTA                             (UX_SYNERGY_HC_PLLSTA)
#define UX_RX_HC_DVSTCTR0                           (UX_SYNERGY_HC_DVSTCTR0)
#define UX_RX_HC_CFIFO                              (UX_SYNERGY_HC_CFIFO)
#define UX_RX_HC_D0FIFO                             (UX_SYNERGY_HC_D0FIFO)
#define UX_RX_HC_D1FIFO                             (UX_SYNERGY_HC_D1FIFO)
#define UX_RX_HC_CFIFOSEL                           (UX_SYNERGY_HC_CFIFOSEL)
#define UX_RX_HC_CFIFOCTR                           (UX_SYNERGY_HC_CFIFOCTR)
#define UX_RX_HC_D0FIFOSEL                          (UX_SYNERGY_HC_D0FIFOSEL)
#define UX_RX_HC_D0FIFOCTR                          (UX_SYNERGY_HC_D0FIFOCTR)
#define UX_RX_HC_D1FIFOSEL                          (UX_SYNERGY_HC_D1FIFOSEL)
#define UX_RX_HC_D1FIFOCTR                          (UX_SYNERGY_HC_D1FIFOCTR)
#define UX_RX_HC_INTENB0                            (UX_SYNERGY_HC_INTENB0)
#define UX_RX_HC_INTENB1                            (UX_SYNERGY_HC_INTENB1)
#define UX_RX_HC_BRDYENB                            (UX_SYNERGY_HC_BRDYENB)
#define UX_RX_HC_NRDYENB                            (UX_SYNERGY_HC_NRDYENB)
#define UX_RX_HC_BEMPENB                            (UX_SYNERGY_HC_BEMPENB)
#define UX_RX_HC_SOFCFG                             (UX_SYNERGY_HC_SOFCFG)
#define UX_RX_HC_PHYSET                             (UX_SYNERGY_HC_PHYSET)
#define UX_RX_HC_INTSTS0                            (UX_SYNERGY_HC_INTSTS0)
#define UX_RX_HC_INTSTS1                            (UX_SYNERGY_HC_INTSTS1)
#define UX_RX_HC_BRDYSTS                            (UX_SYNERGY_HC_BRDYSTS)
#define UX_RX_HC_NRDYSTS                            (UX_SYNERGY_HC_NRDYSTS)
#define UX_RX_HC_BEMPSTS                            (UX_SYNERGY_HC_BEMPSTS)
#define UX_RX_HC_FRMNUM                             (UX_SYNERGY_HC_FRMNUM)
#define UX_RX_HC_DVCHGR                             (UX_SYNERGY_HC_DVCHGR)
#define UX_RX_HC_USBADDR                            (UX_SYNERGY_HC_USBADDR)
#define UX_RX_HC_USBREQ                             (UX_SYNERGY_HC_USBREQ)
#define UX_RX_HC_USBVAL                             (UX_SYNERGY_HC_USBVAL)
#define UX_RX_HC_USBINDX                            (UX_SYNERGY_HC_USBINDX)
#define UX_RX_HC_USBLENG                            (UX_SYNERGY_HC_USBLENG)
#define UX_RX_HC_DCPCFG                             (UX_SYNERGY_HC_DCPCFG)
#define UX_RX_HC_DCPMAXP                            (UX_SYNERGY_HC_DCPMAXP)
#define UX_RX_HC_DCPCTR                             (UX_SYNERGY_HC_DCPCTR)
#define UX_RX_HC_PIPESEL                            (UX_SYNERGY_HC_PIPESEL)
#define UX_RX_HC_PIPECFG                            (UX_SYNERGY_HC_PIPECFG)
#define UX_RX_HC_PIPEMAXP                           (UX_SYNERGY_HC_PIPEMAXP)
#define UX_RX_HC_PIPEPERI                           (UX_SYNERGY_HC_PIPEPERI)
#define UX_RX_HC_PIPE1CTR                           (UX_SYNERGY_HC_PIPE1CTR)
#define UX_RX_HC_PIPE2CTR                           (UX_SYNERGY_HC_PIPE2CTR)
#define UX_RX_HC_PIPE3CTR                           (UX_SYNERGY_HC_PIPE3CTR)
#define UX_RX_HC_PIPE4CTR                           (UX_SYNERGY_HC_PIPE4CTR)
#define UX_RX_HC_PIPE5CTR                           (UX_SYNERGY_HC_PIPE5CTR)
#define UX_RX_HC_PIPE6CTR                           (UX_SYNERGY_HC_PIPE6CTR)
#define UX_RX_HC_PIPE7CTR                           (UX_SYNERGY_HC_PIPE7CTR)
#define UX_RX_HC_PIPE8CTR                           (UX_SYNERGY_HC_PIPE8CTR)
#define UX_RX_HC_PIPE9CTR                           (UX_SYNERGY_HC_PIPE9CTR)
#define UX_RX_HC_PIPE1TRE                           (UX_SYNERGY_HC_PIPE1TRE)
#define UX_RX_HC_PIPE1TRN                           (UX_SYNERGY_HC_PIPE1TRN)
#define UX_RX_HC_PIPE2TRE                           (UX_SYNERGY_HC_PIPE2TRE)
#define UX_RX_HC_PIPE2TRN                           (UX_SYNERGY_HC_PIPE2TRN)
#define UX_RX_HC_PIPE3TRE                           (UX_SYNERGY_HC_PIPE3TRE)
#define UX_RX_HC_PIPE3TRN                           (UX_SYNERGY_HC_PIPE3TRN)
#define UX_RX_HC_PIPE4TRE                           (UX_SYNERGY_HC_PIPE4TRE)
#define UX_RX_HC_PIPE4TRN                           (UX_SYNERGY_HC_PIPE4TRN)
#define UX_RX_HC_PIPE5TRE                           (UX_SYNERGY_HC_PIPE5TRE)
#define UX_RX_HC_PIPE5TRN                           (UX_SYNERGY_HC_PIPE5TRN)
#define UX_RX_HC_DEVADD0                            (UX_SYNERGY_HC_DEVADD0)
#define UX_RX_HC_DEVADD1                            (UX_SYNERGY_HC_DEVADD1)
#define UX_RX_HC_DEVADD2                            (UX_SYNERGY_HC_DEVADD2)
#define UX_RX_HC_DEVADD3                            (UX_SYNERGY_HC_DEVADD3)
#define UX_RX_HC_DEVADD4                            (UX_SYNERGY_HC_DEVADD4)
#define UX_RX_HC_DEVADD5                            (UX_SYNERGY_HC_DEVADD5)

#define UX_RX_HC_LPSTS                              (UX_SYNERGY_HC_LPSTS)


#define UX_RX_HC_PFKUSB_ENABLED                     (UX_SYNERGY_HC_PFKUSB_ENABLED)
#define UX_RX_HC_PFKUSB_MODE_HOST                   (UX_SYNERGY_HC_PFKUSB_MODE_HOST)

/* Define SYNERGY control register values.  */
#define UX_RX_HC_SYSCFG_CNEN                        (UX_SYNERGY_HC_SYSCFG_CNEN)
#define UX_RX_HC_SYSCFG_HSE                         (UX_SYNERGY_HC_SYSCFG_HSE)
#define UX_RX_HC_SYSCFG_DCFM                        (UX_SYNERGY_HC_SYSCFG_DCFM)
#define UX_RX_HC_SYSCFG_DRPD                        (UX_SYNERGY_HC_SYSCFG_DRPD)
#define UX_RX_HC_SYSCFG_DPRPU                       (UX_SYNERGY_HC_SYSCFG_DPRPU)
#define UX_RX_HC_SYSCFG_USBE                        (UX_SYNERGY_HC_SYSCFG_USBE)

#define UX_RX_HC_SYSSTS0_LNST                       (UX_SYNERGY_HC_SYSSTS0_LNST)
#define UX_RX_HC_SYSSTS0_IDMON                      (UX_SYNERGY_HC_SYSSTS0_IDMON)
#define UX_RX_HC_SYSSTS0_SOFEA                      (UX_SYNERGY_HC_SYSSTS0_SOFEA)
#define UX_RX_HC_SYSSTS0_HTACT                      (UX_SYNERGY_HC_SYSSTS0_HTACT)
#define UX_RX_HC_SYSSTS0_OVCMON                     (UX_SYNERGY_HC_SYSSTS0_OVCMON)

#define UX_RX_HC_PLLSTA_PLLLOCK                     (UX_SYNERGY_HC_PLLSTA_PLLLOCK)



#define UX_RX_HC_DVSTCTR0_HNPBTOA                   (UX_SYNERGY_HC_DVSTCTR0_HNPBTOA)
#define UX_RX_HC_DVSTCTR0_EXICEN                    (UX_SYNERGY_HC_DVSTCTR0_EXICEN)
#define UX_RX_HC_DVSTCTR0_VBUSEN                    (UX_SYNERGY_HC_DVSTCTR0_VBUSEN)
#define UX_RX_HC_DVSTCTR0_WKUP                      (UX_SYNERGY_HC_DVSTCTR0_WKUP)
#define UX_RX_HC_DVSTCTR0_RWUPE                     (UX_SYNERGY_HC_DVSTCTR0_RWUPE)
#define UX_RX_HC_DVSTCTR0_USBRST                    (UX_SYNERGY_HC_DVSTCTR0_USBRST)
#define UX_RX_HC_DVSTCTR0_RESUME                    (UX_SYNERGY_HC_DVSTCTR0_RESUME)
#define UX_RX_HC_DVSTCTR0_UACT                      (UX_SYNERGY_HC_DVSTCTR0_UACT)
#define UX_RX_HC_DVSTCTR0_RHST                      (UX_SYNERGY_HC_DVSTCTR0_RHST)

#define UX_RX_HC_DVSTCTR0_SPEED_LOW                 (UX_SYNERGY_HC_DVSTCTR0_SPEED_LOW)
#define UX_RX_HC_DVSTCTR0_SPEED_FULL                (UX_SYNERGY_HC_DVSTCTR0_SPEED_FULL)
#define UX_RX_HC_DVSTCTR0_SPEED_HIGH                (UX_SYNERGY_HC_DVSTCTR0_SPEED_HIGH)
#define UX_RX_HC_DVSTCTR0_RESET_IN_PROGRESS         (UX_SYNERGY_HC_DVSTCTR0_RESET_IN_PROGRESS)

#define UX_RX_HC_CFIFOSEL_RCNT                      (UX_SYNERGY_HC_CFIFOSEL_RCNT)
#define UX_RX_HC_CFIFOSEL_REW                       (UX_SYNERGY_HC_CFIFOSEL_REW)
#define UX_RX_HC_CFIFOSEL_MBW_8                     (UX_SYNERGY_HC_CFIFOSEL_MBW_8)
#define UX_RX_HC_CFIFOSEL_MBW_16                    (UX_SYNERGY_HC_CFIFOSEL_MBW_16)
#define UX_RX_HC_CFIFOSEL_MBW_32                    (UX_SYNERGY_HC_CFIFOSEL_MBW_32)
#define UX_RX_HC_CFIFOSEL_MBW_MASK                  (UX_SYNERGY_HC_CFIFOSEL_MBW_MASK)
#define UX_RX_HC_CFIFOSEL_BIGEND                    (UX_SYNERGY_HC_CFIFOSEL_BIGEND)
#define UX_RX_HC_CFIFOSEL_ISEL                      (UX_SYNERGY_HC_CFIFOSEL_ISEL)
#define UX_RX_HC_CFIFOSEL_CURPIPE_MASK              (UX_SYNERGY_HC_CFIFOSEL_CURPIPE_MASK)

#define UX_RX_HC_DFIFOSEL_RCNT                      (UX_SYNERGY_HC_DFIFOSEL_RCNT)
#define UX_RX_HC_DFIFOSEL_REW                       (UX_SYNERGY_HC_DFIFOSEL_REW)
#define UX_RX_HC_DFIFOSEL_DCLRM                     (UX_SYNERGY_HC_DFIFOSEL_DCLRM)
#define UX_RX_HC_DFIFOSEL_DREQE                     (UX_SYNERGY_HC_DFIFOSEL_DREQE)
#define UX_RX_HC_DFIFOSEL_MBW_8                     (UX_SYNERGY_HC_DFIFOSEL_MBW_8)
#define UX_RX_HC_DFIFOSEL_MBW_16                    (UX_SYNERGY_HC_DFIFOSEL_MBW_16)
#define UX_RX_HC_DFIFOSEL_MBW_32                    (UX_SYNERGY_HC_DFIFOSEL_MBW_32)
#define UX_RX_HC_DFIFOSEL_MBW_MASK                  (UX_SYNERGY_HC_DFIFOSEL_MBW_MASK)
#define UX_RX_HC_DFIFOSEL_BIGEND                    (UX_SYNERGY_HC_DFIFOSEL_BIGEND)
#define UX_RX_HC_DFIFOSEL_CURPIPE_MASK              (UX_SYNERGY_HC_DFIFOSEL_CURPIPE_MASK)

#define UX_RX_HC_FIFOCTR_BVAL                       (UX_SYNERGY_HC_FIFOCTR_BVAL)
#define UX_RX_HC_FIFOCTR_BCLR                       (UX_SYNERGY_HC_FIFOCTR_BCLR)
#define UX_RX_HC_FIFOCTR_FRDY                       (UX_SYNERGY_HC_FIFOCTR_FRDY)
#define UX_RX_HC_FIFOCTR_DTLN                       (UX_SYNERGY_HC_FIFOCTR_DTLN)
                                                    
#define UX_RX_HC_INTENB0_VBSE                       (UX_SYNERGY_HC_INTENB0_VBSE)
#define UX_RX_HC_INTENB0_RSME                       (UX_SYNERGY_HC_INTENB0_RSME)
#define UX_RX_HC_INTENB0_SOFE                       (UX_SYNERGY_HC_INTENB0_SOFE)
#define UX_RX_HC_INTENB0_DVSE                       (UX_SYNERGY_HC_INTENB0_DVSE)
#define UX_RX_HC_INTENB0_CTRE                       (UX_SYNERGY_HC_INTENB0_CTRE)
#define UX_RX_HC_INTENB0_BEMPE                      (UX_SYNERGY_HC_INTENB0_BEMPE)
#define UX_RX_HC_INTENB0_NRDYE                      (UX_SYNERGY_HC_INTENB0_NRDYE)
#define UX_RX_HC_INTENB0_BRDYE                      (UX_SYNERGY_HC_INTENB0_BRDYE)

#define UX_RX_HC_INTENB1_OVRCRE                     (UX_SYNERGY_HC_INTENB1_OVRCRE)
#define UX_RX_HC_INTENB1_BCHGE                      (UX_SYNERGY_HC_INTENB1_BCHGE)
#define UX_RX_HC_INTENB1_DTCHE                      (UX_SYNERGY_HC_INTENB1_DTCHE)
#define UX_RX_HC_INTENB1_ATTCHE                     (UX_SYNERGY_HC_INTENB1_ATTCHE)
#define UX_RX_HC_INTENB1_L1RSMENDE                  (UX_SYNERGY_HC_INTENB1_L1RSMENDE)
#define UX_RX_HC_INTENB1_LPSMENDE                   (UX_SYNERGY_HC_INTENB1_LPSMENDE)
#define UX_RX_HC_INTENB1_EOFERRE                    (UX_SYNERGY_HC_INTENB1_EOFERRE)
#define UX_RX_HC_INTENB1_SIGNE                      (UX_SYNERGY_HC_INTENB1_SIGNE)
#define UX_RX_HC_INTENB1_SACKE                      (UX_SYNERGY_HC_INTENB1_SACKE)
#define UX_RX_HC_INTENB1_PDDETINTE                  (UX_SYNERGY_HC_INTENB1_PDDETINTE)

#define UX_RX_HC_PIPE0                              (UX_SYNERGY_HC_PIPE0)
#define UX_RX_HC_PIPE1                              (UX_SYNERGY_HC_PIPE1)
#define UX_RX_HC_PIPE2                              (UX_SYNERGY_HC_PIPE2)
#define UX_RX_HC_PIPE3                              (UX_SYNERGY_HC_PIPE3)
#define UX_RX_HC_PIPE4                              (UX_SYNERGY_HC_PIPE4)
#define UX_RX_HC_PIPE5                              (UX_SYNERGY_HC_PIPE5)
#define UX_RX_HC_PIPE6                              (UX_SYNERGY_HC_PIPE6)
#define UX_RX_HC_PIPE7                              (UX_SYNERGY_HC_PIPE7)
#define UX_RX_HC_PIPE8                              (UX_SYNERGY_HC_PIPE8)
#define UX_RX_HC_PIPE9                              (UX_SYNERGY_HC_PIPE9)
#define UX_RX_HC_PIPE_ALL                           (UX_SYNERGY_HC_PIPE_ALL)

#define UX_RX_HC_SOFCFG_TRNENSEL                    (UX_SYNERGY_HC_SOFCFG_TRNENSEL)
#define UX_RX_HC_SOFCFG_BRDYM                       (UX_SYNERGY_HC_SOFCFG_BRDYM)
#define UX_RX_HC_SOFCFG_INTL                        (UX_SYNERGY_HC_SOFCFG_INTL)
#define UX_RX_HC_SOFCFG_EDGESTS                     (UX_SYNERGY_HC_SOFCFG_EDGESTS)

#define UX_RX_HC_PHYSET_HSEB                        (UX_SYNERGY_HC_PHYSET_HSEB)
#define UX_RX_HC_PHYSET_REPSTART                    (UX_SYNERGY_HC_PHYSET_REPSTART)
#define UX_RX_HC_PHYSET_REPSEL                      (UX_SYNERGY_HC_PHYSET_REPSEL)
#define UX_RX_HC_PHYSET_CLKSEL_1                    (UX_SYNERGY_HC_PHYSET_CLKSEL_1)
#define UX_RX_HC_PHYSET_CLKSEL_0                    (UX_SYNERGY_HC_PHYSET_CLKSEL_0)
#define UX_RX_HC_PHYSET_CDPEN                       (UX_SYNERGY_HC_PHYSET_CDPEN)
#define UX_RX_HC_PHYSET_PLLRESET                    (UX_SYNERGY_HC_PHYSET_PLLRESET)
#define UX_RX_HC_PHYSET_DIRPD                       (UX_SYNERGY_HC_PHYSET_DIRPD)


#define UX_RX_HC_INTSTS0_VBINT                      (UX_SYNERGY_HC_INTSTS0_VBINT)
#define UX_RX_HC_INTSTS0_RESM                       (UX_SYNERGY_HC_INTSTS0_RESM)
#define UX_RX_HC_INTSTS0_SOFR                       (UX_SYNERGY_HC_INTSTS0_SOFR)
#define UX_RX_HC_INTSTS0_DVST                       (UX_SYNERGY_HC_INTSTS0_DVST)
#define UX_RX_HC_INTSTS0_CTRT                       (UX_SYNERGY_HC_INTSTS0_CTRT)
#define UX_RX_HC_INTSTS0_BEMP                       (UX_SYNERGY_HC_INTSTS0_BEMP)
#define UX_RX_HC_INTSTS0_NRDY                       (UX_SYNERGY_HC_INTSTS0_NRDY)
#define UX_RX_HC_INTSTS0_BRDY                       (UX_SYNERGY_HC_INTSTS0_BRDY)
#define UX_RX_HC_INTSTS0_VBSTS                      (UX_SYNERGY_HC_INTSTS0_VBSTS)
#define UX_RX_HC_INTSTS0_VALID                      (UX_SYNERGY_HC_INTSTS0_VALID)
#define UX_RX_HC_INTSTS0_CTSQ_MASK                  (UX_SYNERGY_HC_INTSTS0_CTSQ_MASK)

#define UX_RX_HC_INTSTS1_OVRCRE                     (UX_SYNERGY_HC_INTSTS1_OVRCRE)
#define UX_RX_HC_INTSTS1_BCHG                       (UX_SYNERGY_HC_INTSTS1_BCHG)
#define UX_RX_HC_INTSTS1_DTCH                       (UX_SYNERGY_HC_INTSTS1_DTCH)
#define UX_RX_HC_INTSTS1_ATTCH                      (UX_SYNERGY_HC_INTSTS1_ATTCH)
#define UX_RX_HC_INTSTS1_EOFERR                     (UX_SYNERGY_HC_INTSTS1_EOFERR)
#define UX_RX_HC_INTSTS1_SIGN                       (UX_SYNERGY_HC_INTSTS1_SIGN)
#define UX_RX_HC_INTSTS1_SACK                       (UX_SYNERGY_HC_INTSTS1_SACK)

#define UX_RX_HC_FRMNUM_OVRN                        (UX_SYNERGY_HC_FRMNUM_OVRN)
#define UX_RX_HC_FRMNUM_CRCE                        (UX_SYNERGY_HC_FRMNUM_CRCE)
#define UX_RX_HC_FRMNUM_FRNM_MASK                   (UX_SYNERGY_HC_FRMNUM_FRNM_MASK)
 
#define UX_RX_HC_DCPCFG_DIR                         (UX_SYNERGY_HC_DCPCFG_DIR)

#define UX_RX_HC_DCPMAXP_DEVADDR_SHIFT              (UX_SYNERGY_HC_DCPMAXP_DEVADDR_SHIFT)
#define UX_RX_HC_DCPMAXP_DEVADDR_MASK               (UX_SYNERGY_HC_DCPMAXP_DEVADDR_MASK)

#define UX_RX_HC_DCPCTR_BSTS                        (UX_SYNERGY_HC_DCPCTR_BSTS)
#define UX_RX_HC_DCPCTR_SUREQ                       (UX_SYNERGY_HC_DCPCTR_SUREQ)
#define UX_RX_HC_DCPCTR_SUREQCLR                    (UX_SYNERGY_HC_DCPCTR_SUREQCLR)
#define UX_RX_HC_DCPCTR_SQCLR                       (UX_SYNERGY_HC_DCPCTR_SQCLR)
#define UX_RX_HC_DCPCTR_SQSET                       (UX_SYNERGY_HC_DCPCTR_SQSET)
#define UX_RX_HC_DCPCTR_SQMON                       (UX_SYNERGY_HC_DCPCTR_SQMON)
#define UX_RX_HC_DCPCTR_PBUSY                       (UX_SYNERGY_HC_DCPCTR_PBUSY)
#define UX_RX_HC_DCPCTR_CCPL                        (UX_SYNERGY_HC_DCPCTR_CCPL)
#define UX_RX_HC_DCPCTR_PID_MASK                    (UX_SYNERGY_HC_DCPCTR_PID_MASK)
#define UX_RX_HC_DCPCTR_PIDNAK                      (UX_SYNERGY_HC_DCPCTR_PIDNAK)
#define UX_RX_HC_DCPCTR_PIDBUF                      (UX_SYNERGY_HC_DCPCTR_PIDBUF)
#define UX_RX_HC_DCPCTR_PIDSTALL                    (UX_SYNERGY_HC_DCPCTR_PIDSTALL)
#define UX_RX_HC_DCPCTR_PIDSTALL2                   (UX_SYNERGY_HC_DCPCTR_PIDSTALL2)


#define UX_RX_HC_PIPECFG_TYPE_MASK                  (UX_SYNERGY_HC_PIPECFG_TYPE_MASK)
#define UX_RX_HC_PIPECFG_TYPE_BIT_USED              (UX_SYNERGY_HC_PIPECFG_TYPE_BIT_USED)
#define UX_RX_HC_PIPECFG_TYPE_BULK                  (UX_SYNERGY_HC_PIPECFG_TYPE_BULK)
#define UX_RX_HC_PIPECFG_TYPE_INTERRUPT             (UX_SYNERGY_HC_PIPECFG_TYPE_INTERRUPT)
#define UX_RX_HC_PIPECFG_TYPE_ISOCHRONOUS           (UX_SYNERGY_HC_PIPECFG_TYPE_ISOCHRONOUS)
#define UX_RX_HC_PIPECFG_BFRE                       (UX_SYNERGY_HC_PIPECFG_BFRE)
#define UX_RX_HC_PIPECFG_DBLB                       (UX_SYNERGY_HC_PIPECFG_DBLB)
#define UX_RX_HC_PIPECFG_SHTNAK                     (UX_SYNERGY_HC_PIPECFG_SHTNAK)
#define UX_RX_HC_PIPECFG_DIR                        (UX_SYNERGY_HC_PIPECFG_DIR)
#define UX_RX_HC_PIPECFG_EPNUM_MASK                 (UX_SYNERGY_HC_PIPECFG_EPNUM_MASK)

#define UX_RX_HC_PIPEBUF_SIZEMASK                   (UX_SYNERGY_HC_PIPEBUF_SIZEMASK)
#define UX_RX_HC_PIPEBUF_BUFNMBMASK                 (UX_SYNERGY_HC_PIPEBUF_BUFNMBMASK)
#define UX_RX_HC_PIPEBUF_SHIFT                      (UX_SYNERGY_HC_PIPEBUF_SHIFT)

#define UX_RX_HC_PIPEMAXP_DEVSELMASK                (UX_SYNERGY_HC_PIPEMAXP_DEVSELMASK)
#define UX_RX_HC_PIPEMAXP_DEVSEL_SHIFT              (UX_SYNERGY_HC_PIPEMAXP_DEVSEL_SHIFT)
#define UX_RX_HC_PIPEMAXP_MXPSMASK                  (UX_SYNERGY_HC_PIPEMAXP_MXPSMASK)

#define UX_RX_HC_PIPE1TRE_TRCLR                     (UX_SYNERGY_HC_PIPE1TRE_TRCLR)
#define UX_RX_HC_PIPE1TRE_TRENB                     (UX_SYNERGY_HC_PIPE1TRE_TRENB)

#define UX_RX_HC_FIFO_D0                            (UX_SYNERGY_HC_FIFO_D0)
#define UX_RX_HC_FIFO_D1                            (UX_SYNERGY_HC_FIFO_D1)
#define UX_RX_HC_FIFO_C                             (UX_SYNERGY_HC_FIFO_C)

#define UX_RX_HC_DEVADD_SPEED_LOW                  (UX_SYNERGY_HC_DEVADD_SPEED_LOW)
#define UX_RX_HC_DEVADD_SPEED_FULL                 (UX_SYNERGY_HC_DEVADD_SPEED_FULL)
#define UX_RX_HC_DEVADD_SPEED_HIGH                 (UX_SYNERGY_HC_DEVADD_SPEED_HIGH)

#define UX_RX_HC_DEVADD_UPPHUB_SHIFT                (UX_SYNERGY_HC_DEVADD_UPPHUB_SHIFT)
#define UX_RX_HC_DEVADD_HUBPORT_SHIFT               (UX_SYNERGY_HC_DEVADD_HUBPORT_SHIFT)

/* Define synergy HCOR command/status bitmaps.  */

#define UX_RX_HC_DCP                                (UX_SYNERGY_HC_DCP)

#define UX_RX_HC_DCPCTR_DATA1                       (UX_SYNERGY_HC_DCPCTR_DATA1)
#define UX_RX_HC_DCPCTR_DATA0                       (UX_SYNERGY_HC_DCPCTR_DATA0)

/* Define synergy PIPE selection definitions.  */
#define UX_RX_HC_PIPESEL_NO_PIPE                    (UX_SYNERGY_HC_PIPESEL_NO_PIPE)

/* Define synergy fifo definition.  */
#define UX_RX_HC_PIPE0_SIZE                         (UX_SYNERGY_HC_PIPE0_SIZE)
#define UX_RX_HC_PIPE_NB_BUFFERS                    (UX_SYNERGY_HC_PIPE_NB_BUFFERS)


#define UX_RX_HC_PIPECTR_BSTS                       (UX_SYNERGY_HC_PIPECTR_BSTS)
#define UX_RX_HC_PIPECTR_INBUFM                     (UX_SYNERGY_HC_PIPECTR_INBUFM)
#define UX_RX_HC_PIPECTR_ATREPM                     (UX_SYNERGY_HC_PIPECTR_ATREPM)
#define UX_RX_HC_PIPECTR_ACLRM                      (UX_SYNERGY_HC_PIPECTR_ACLRM)
#define UX_RX_HC_PIPECTR_SQCLR                      (UX_SYNERGY_HC_PIPECTR_SQCLR)
#define UX_RX_HC_PIPECTR_SQSET                      (UX_SYNERGY_HC_PIPECTR_SQSET)
#define UX_RX_HC_PIPECTR_SQMON                      (UX_SYNERGY_HC_PIPECTR_SQMON)
#define UX_RX_HC_PIPECTR_PBUSY                      (UX_SYNERGY_HC_PIPECTR_PBUSY)
#define UX_RX_HC_PIPECTR_PID_MASK                   (UX_SYNERGY_HC_PIPECTR_PID_MASK)
#define UX_RX_HC_PIPECTR_PIDNAK                     (UX_SYNERGY_HC_PIPECTR_PIDNAK)
#define UX_RX_HC_PIPECTR_PIDBUF                     (UX_SYNERGY_HC_PIPECTR_PIDBUF)
#define UX_RX_HC_PIPECTR_PIDSTALL                   (UX_SYNERGY_HC_PIPECTR_PIDSTALL)
#define UX_RX_HC_PIPECTR_PIDSTALL2                  (UX_SYNERGY_HC_PIPECTR_PIDSTALL2)
#define UX_RX_HC_PIPECTR_DATA1                      (UX_SYNERGY_HC_PIPECTR_DATA1)
#define UX_RX_HC_PIPECTR_DATA0                      (UX_SYNERGY_HC_PIPECTR_DATA0)

/* Define synergy static definition.  */

#define UX_RX_HC_AVAILABLE_BANDWIDTH                (UX_SYNERGY_HC_AVAILABLE_BANDWIDTH)
#define UX_RX_HC_INIT_DELAY                         (UX_SYNERGY_HC_INIT_DELAY)
#define UX_RX_HC_RESET_RETRY                        (UX_SYNERGY_HC_RESET_RETRY)
#define UX_RX_HC_RESET_DELAY                        (UX_SYNERGY_HC_RESET_DELAY)
#define UX_RX_HC_PORT_RESET_RETRY                   (UX_SYNERGY_HC_PORT_RESET_RETRY)
#define UX_RX_HC_FORCE_PORT_RESET_RETRY             (UX_SYNERGY_HC_FORCE_PORT_RESET_RETRY)
#define UX_RX_HC_FORCE_PORT_RESET_DELAY             (UX_SYNERGY_HC_FORCE_PORT_RESET_DELAY)
#define UX_RX_HC_CHECK_PORT_RESET_RETRY             (UX_SYNERGY_HC_CHECK_PORT_RESET_RETRY)
#define UX_RX_HC_PORT_RESET_DELAY                   (UX_SYNERGY_HC_PORT_RESET_DELAY)
#define UX_RX_HC_PORT_RESET_RECOVERY_DELAY          (UX_SYNERGY_HC_PORT_RESET_RECOVERY_DELAY)

/* Define synergy initialization values.  */

#define UX_RX_HC_COMMAND_STATUS_RESET               (UX_SYNERGY_HC_COMMAND_STATUS_RESET)
#define UX_RX_HC_INIT_RESET_DELAY                   (UX_SYNERGY_HC_INIT_RESET_DELAY)
#define UX_RX_HC_MAX_BUF_SIZE                       (UX_SYNERGY_HC_MAX_BUF_SIZE)
#define UX_RX_HC_MAX_BUF_NUM                        (UX_SYNERGY_HC_MAX_BUF_NUM)


/* Define synergy FIFO write completion code.  */
#define UX_RX_HC_FIFO_WRITING                       (UX_SYNERGY_HC_FIFO_WRITING)
#define UX_RX_HC_FIFO_WRITE_END                     (UX_SYNERGY_HC_FIFO_WRITE_END)
#define UX_RX_HC_FIFO_WRITE_SHORT                   (UX_SYNERGY_HC_FIFO_WRITE_SHORT)
#define UX_RX_HC_FIFO_WRITE_DMA                     (UX_SYNERGY_HC_FIFO_WRITE_DMA)
#define UX_RX_HC_FIFO_WRITE_ERROR                   (UX_SYNERGY_HC_FIFO_WRITE_ERROR)

/* Define synergy FIFO read completion code.  */
#define UX_RX_HC_FIFO_READING                       (UX_SYNERGY_HC_FIFO_READING)
#define UX_RX_HC_FIFO_READ_END                      (UX_SYNERGY_HC_FIFO_READ_END)
#define UX_RX_HC_FIFO_READ_SHORT                    (UX_SYNERGY_HC_FIFO_READ_SHORT)
#define UX_RX_HC_FIFO_READ_DMA                      (UX_SYNERGY_HC_FIFO_READ_DMA)
#define UX_RX_HC_FIFO_READ_ERROR                    (UX_SYNERGY_HC_FIFO_READ_ERROR)
#define UX_RX_HC_FIFO_READ_OVER                     (UX_SYNERGY_HC_FIFO_READ_OVER)

/* Define SYNERGY62N physical endpoint definitions.  */
#define UX_RX_HC_ED_BRDY                            (UX_SYNERGY_HC_ED_BRDY)
#define UX_RX_HC_ED_NRDY                            (UX_SYNERGY_HC_ED_NRDY)
#define UX_RX_HC_ED_BEMP                            (UX_SYNERGY_HC_ED_BEMP)
#define UX_RX_HC_ED_EOFERR                          (UX_SYNERGY_HC_ED_EOFERR)
#define UX_RX_HC_ED_SIGN                            (UX_SYNERGY_HC_ED_SIGN)
#define UX_RX_HC_ED_SACK                            (UX_SYNERGY_HC_ED_SACK)

#define UX_RX_HC_LPSTS_SUSPENDM                     (UX_SYNERGY_HC_LPSTS_SUSPENDM)

/* Define SYNERGY Root hub states.  */
#define UX_RX_HC_PORT_ENABLED                       (UX_SYNERGY_HC_PORT_ENABLED)
#define UX_RX_HC_PORT_DISABLED                      (UX_SYNERGY_HC_PORT_DISABLED)



/* Define synergy structure.  */

typedef struct UX_HCD_SYNERGY UX_HCD_RX;


/* Define synergy ED structure.  */

typedef struct UX_SYNERGY_ED UX_RX_ED;


/* Define synergy ED bitmap.  */

#define UX_RX_ED_STATIC                                (UX_SYNERGY_ED_STATIC)
#define UX_RX_ED_SKIP                                  (UX_SYNERGY_ED_SKIP)

/* Define synergy TD structure.  */

typedef struct UX_SYNERGY_TD UX_RX_TD;


/* Define synergy TD bitmap.  */

#define UX_RX_TD_SETUP_PHASE                           (UX_SYNERGY_TD_SETUP_PHASE)
#define UX_RX_TD_DATA_PHASE                            (UX_SYNERGY_TD_DATA_PHASE)
#define UX_RX_TD_STATUS_PHASE                          (UX_SYNERGY_TD_STATUS_PHASE)
#define UX_RX_TD_OUT                                   (UX_SYNERGY_TD_OUT)
#define UX_RX_TD_IN                                    (UX_SYNERGY_TD_IN)
#define UX_RX_TD_TOGGLE_FROM_ED                        (UX_SYNERGY_TD_TOGGLE_FROM_ED)
#define UX_RX_TD_ACK_PENDING                           (UX_SYNERGY_TD_ACK_PENDING)


#define UX_RX_TD_SETUP_TYPE                            (UX_SYNERGY_TD_SETUP_TYPE)
#define UX_RX_TD_DATA_TYPE                             (UX_SYNERGY_TD_DATA_TYPE)
#define UX_RX_TD_STATUS_TYPE                           (UX_SYNERGY_TD_STATUS_TYPE)
#define UX_RX_TD_MAX_ERROR                             (UX_SYNERGY_TD_MAX_ERROR)

/* Define synergy ISOCHRONOUS TD structure.  */

typedef struct UX_SYNERGY_ISO_TD UX_RX_ISO_TD;


/* Define synergy function prototypes.  */

#define _ux_hcd_rx_asynch_queue_process           _ux_hcd_synergy_asynch_queue_process
#define _ux_hcd_rx_asynch_schedule                _ux_hcd_synergy_asynch_schedule
#define _ux_hcd_rx_asynchronous_endpoint_create   _ux_hcd_synergy_asynchronous_endpoint_create
#define _ux_hcd_rx_asynchronous_endpoint_destroy  _ux_hcd_synergy_asynchronous_endpoint_destroy
#define _ux_hcd_rx_control_endpoint_create        _ux_hcd_synergy_control_endpoint_create
#define _ux_hcd_rx_bulk_endpoint_create           _ux_hcd_synergy_bulk_endpoint_create
#define _ux_hcd_rx_control_td_add                 _ux_hcd_synergy_control_td_add
#define _ux_hcd_rx_bulk_int_td_add                _ux_hcd_synergy_bulk_int_td_add
#define _ux_hcd_rx_controller_disable             _ux_hcd_synergy_controller_disable
#define _ux_hcd_rx_ed_obtain                      _ux_hcd_synergy_ed_obtain
#define _ux_hcd_rx_ed_td_clean                    _ux_hcd_synergy_ed_td_clean
#define _ux_hcd_rx_endpoint_reset                 _ux_hcd_synergy_endpoint_reset
#define _ux_hcd_rx_entry                          _ux_hcd_synergy_entry
#define _ux_hcd_rx_frame_number_get               _ux_hcd_synergy_frame_number_get
#define _ux_hcd_rx_frame_number_set               _ux_hcd_synergy_frame_number_set
#define _ux_hcd_rx_register_read                  _ux_hcd_synergy_register_read
#define _ux_hcd_rx_register_write                 _ux_hcd_synergy_register_write
#define _ux_hcd_rx_register_clear                 _ux_hcd_synergy_register_clear
#define _ux_hcd_rx_register_set                   _ux_hcd_synergy_register_set
#define _ux_hcd_rx_initialize                     _ux_hcd_synergy_initialize
#define _ux_hcd_rx_interrupt_endpoint_create      _ux_hcd_synergy_interrupt_endpoint_create
#define _ux_hcd_rx_interrupt_handler              _ux_hcd_synergy_interrupt_handler
#define _ux_hcd_rx_iso_queue_process              _ux_hcd_synergy_iso_queue_process
#define _ux_hcd_rx_iso_schedule                   _ux_hcd_synergy_iso_schedule
#define _ux_hcd_rx_isochronous_endpoint_create    _ux_hcd_synergy_isochronous_endpoint_create
#define _ux_hcd_rx_isochronous_td_obtain          _ux_hcd_synergy_isochronous_td_obtain
#define _ux_hcd_rx_least_traffic_list_get         _ux_hcd_synergy_least_traffic_list_get
#define _ux_hcd_rx_periodic_endpoint_destroy      _ux_hcd_synergy_periodic_endpoint_destroy
#define _ux_hcd_rx_periodic_schedule              _ux_hcd_synergy_periodic_schedule
#define _ux_hcd_rx_periodic_tree_create           _ux_hcd_synergy_periodic_tree_create
#define _ux_hcd_rx_port_disable                   _ux_hcd_synergy_port_disable
#define _ux_hcd_rx_port_enable                    _ux_hcd_synergy_port_enable
#define _ux_hcd_rx_port_reset                     _ux_hcd_synergy_port_reset
#define _ux_hcd_rx_port_resume                    _ux_hcd_synergy_port_resume
#define _ux_hcd_rx_port_status_get                _ux_hcd_synergy_port_status_get
#define _ux_hcd_rx_port_suspend                   _ux_hcd_synergy_port_suspend
#define _ux_hcd_rx_power_down_port                _ux_hcd_synergy_power_down_port
#define _ux_hcd_rx_power_on_port                  _ux_hcd_synergy_power_on_port
#define _ux_hcd_rx_power_root_hubs                _ux_hcd_synergy_power_root_hubs
#define _ux_hcd_rx_td_add                         _ux_hcd_synergy_td_add
#define _ux_hcd_rx_regular_td_obtain              _ux_hcd_synergy_regular_td_obtain
#define _ux_hcd_rx_request_bulk_transfer          _ux_hcd_synergy_request_bulk_transfer
#define _ux_hcd_rx_request_control_transfer       _ux_hcd_synergy_request_control_transfer
#define _ux_hcd_rx_request_interrupt_transfer     _ux_hcd_synergy_request_interrupt_transfer
#define _ux_hcd_rx_request_isochronous_transfer   _ux_hcd_synergy_request_isochronous_transfer
#define _ux_hcd_rx_request_transfer               _ux_hcd_synergy_request_transfer
#define _ux_hcd_rx_transfer_abort                 _ux_hcd_synergy_transfer_abort
#define _ux_hcd_rx_fifo_port_change               _ux_hcd_synergy_fifo_port_change
#define _ux_hcd_rx_current_endpoint_change        _ux_hcd_synergy_current_endpoint_change
#define _ux_hcd_rx_data_buffer_size               _ux_hcd_synergy_data_buffer_size
#define _ux_hcd_rx_buffer_write                   _ux_hcd_synergy_buffer_write
#define _ux_hcd_rx_fifod_write                    _ux_hcd_synergy_fifod_write
#define _ux_hcd_rx_fifoc_write                    _ux_hcd_synergy_fifoc_write
#define _ux_hcd_rx_fifo_read                      _ux_hcd_synergy_fifo_read
#define _ux_hcd_rx_buffer_read                    _ux_hcd_synergy_buffer_read
#define _ux_hcd_rx_buffer_ready_interrupt         _ux_hcd_synergy_buffer_ready_interrupt
#define _ux_hcd_rx_buffer_empty_interrupt         _ux_hcd_synergy_buffer_empty_interrupt
#define _ux_hcd_rx_endpoint_nak_set               _ux_hcd_synergy_endpoint_nak_set
#define _ux_hcd_rx_asynch_queue_process_bemp      _ux_hcd_synergy_asynch_queue_process_bemp
#define _ux_hcd_rx_asynch_queue_process_nrdy      _ux_hcd_synergy_asynch_queue_process_nrdy
#define _ux_hcd_rx_asynch_queue_process_brdy      _ux_hcd_synergy_asynch_queue_process_brdy
#define _ux_hcd_rx_asynch_queue_process_sign      _ux_hcd_synergy_asynch_queue_process_sign
#define _ux_hcd_rx_buffer_notready_interrupt      _ux_hcd_synergy_buffer_notready_interrupt

#define ux_hcd_rx_initialize(a,b,c,d)             _ux_hcd_synergy_initialize(a,b,(ULONG) R_USBHS,d)
#define ux_hcd_rx_interrupt_handler               _ux_hcd_synergy_interrupt_handler

#endif

