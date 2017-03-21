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
 * prior to v1.1.0.  DO NOT USE THIS FILE IN NEW PROJECTS.  Use ux_dcd_synergy.h instead. */

#ifndef UX_DCD_RX_H
#define UX_DCD_RX_H

#include "ux_dcd_synergy.h"

/* Define SYNERGY generic equivalences.  */

#define UX_DCD_RX_SLAVE_CONTROLLER                          (UX_DCD_SYNERGY_SLAVE_CONTROLLER)
#define UX_DCD_RX_MAX_ED                                    (UX_DCD_SYNERGY_MAX_ED)
#define UX_DCD_RX_ENABLE                                    (UX_DCD_SYNERGY_ENABLE)
#define UX_DCD_RX_DISABLE                                   (UX_DCD_SYNERGY_DISABLE)
#define UX_DCD_RX_MAX_BULK_PAYLOAD                          (UX_DCD_SYNERGY_MAX_BULK_PAYLOAD)
#define UX_DCD_RX_MAX_CONTROL_PAYLOAD                       (UX_DCD_SYNERGY_MAX_CONTROL_PAYLOAD)
#define UX_DCD_RX_MAX_BUF_SIZE                              (UX_DCD_SYNERGY_MAX_BUF_SIZE)
#define UX_DCD_RX_MAX_BUF_NUM                               (UX_DCD_SYNERGY_MAX_BUF_NUM)


/* Define SYNERGY HCOR register mapping.  */
#define UX_RX_USB_BASE                          (UX_SYNERGY_USB_BASE)

#define UX_RX_DCD_SYSCFG                                    (UX_SYNERGY_DCD_SYSCFG)
#define UX_RX_DCD_SYSSTS                                    (UX_SYNERGY_DCD_SYSSTS)
#define UX_RX_DCD_DVSTCTR                                   (UX_SYNERGY_DCD_DVSTCTR)
#define UX_RX_DCD_CFIFO                                     (UX_SYNERGY_DCD_CFIFO)
#define UX_RX_DCD_D0FIFO                                    (UX_SYNERGY_DCD_D0FIFO)
#define UX_RX_DCD_D1FIFO                                    (UX_SYNERGY_DCD_D1FIFO)
#define UX_RX_DCD_CFIFOSEL                                  (UX_SYNERGY_DCD_CFIFOSEL)
#define UX_RX_DCD_CFIFOCTR                                  (UX_SYNERGY_DCD_CFIFOCTR)
#define UX_RX_DCD_D0FIFOSEL                                 (UX_SYNERGY_DCD_D0FIFOSEL)
#define UX_RX_DCD_D0FIFOCTR                                 (UX_SYNERGY_DCD_D0FIFOCTR)
#define UX_RX_DCD_D1FIFOSEL                                 (UX_SYNERGY_DCD_D1FIFOSEL)
#define UX_RX_DCD_D1FIFOCTR                                 (UX_SYNERGY_DCD_D1FIFOCTR)
#define UX_RX_DCD_INTENB0                                   (UX_SYNERGY_DCD_INTENB0)
#define UX_RX_DCD_INTENB1                                   (UX_SYNERGY_DCD_INTENB1)
#define UX_RX_DCD_BRDYENB                                   (UX_SYNERGY_DCD_BRDYENB)
#define UX_RX_DCD_NRDYENB                                   (UX_SYNERGY_DCD_NRDYENB)
#define UX_RX_DCD_BEMPENB                                   (UX_SYNERGY_DCD_BEMPENB)
#define UX_RX_DCD_SOFCFG                                    (UX_SYNERGY_DCD_SOFCFG)
#define UX_RX_DCD_INTSTS0                                   (UX_SYNERGY_DCD_INTSTS0)
#define UX_RX_DCD_INTSTS1                                   (UX_SYNERGY_DCD_INTSTS1)
#define UX_RX_DCD_BRDYSTS                                   (UX_SYNERGY_DCD_BRDYSTS)
#define UX_RX_DCD_NRDYSTS                                   (UX_SYNERGY_DCD_NRDYSTS)
#define UX_RX_DCD_BEMPSTS                                   (UX_SYNERGY_DCD_BEMPSTS)
#define UX_RX_DCD_FRMNUM                                    (UX_SYNERGY_DCD_FRMNUM)

#define UX_RX_DCD_DVCHGR                                    (UX_SYNERGY_DCD_DVCHGR)

#define UX_RX_DCD_USBADDR                                   (UX_SYNERGY_DCD_USBADDR)
#define UX_RX_DCD_USBREQ                                    (UX_SYNERGY_DCD_USBREQ)
#define UX_RX_DCD_USBVAL                                    (UX_SYNERGY_DCD_USBVAL)
#define UX_RX_DCD_USBINDX                                   (UX_SYNERGY_DCD_USBINDX)
#define UX_RX_DCD_USBLENG                                   (UX_SYNERGY_DCD_USBLENG)
#define UX_RX_DCD_DCPCFG                                    (UX_SYNERGY_DCD_DCPCFG)
#define UX_RX_DCD_DCPMAXP                                   (UX_SYNERGY_DCD_DCPMAXP)
#define UX_RX_DCD_DCPCTR                                    (UX_SYNERGY_DCD_DCPCTR)
#define UX_RX_DCD_PIPESEL                                   (UX_SYNERGY_DCD_PIPESEL)
#define UX_RX_DCD_PIPECFG                                   (UX_SYNERGY_DCD_PIPECFG)
#define UX_RX_DCD_PIPEMAXP                                  (UX_SYNERGY_DCD_PIPEMAXP)
#define UX_RX_DCD_PIPEPERI                                  (UX_SYNERGY_DCD_PIPEPERI)
#define UX_RX_DCD_PIPE1CTR                                  (UX_SYNERGY_DCD_PIPE1CTR)
#define UX_RX_DCD_PIPE2CTR                                  (UX_SYNERGY_DCD_PIPE2CTR)
#define UX_RX_DCD_PIPE3CTR                                  (UX_SYNERGY_DCD_PIPE3CTR)
#define UX_RX_DCD_PIPE4CTR                                  (UX_SYNERGY_DCD_PIPE4CTR)
#define UX_RX_DCD_PIPE5CTR                                  (UX_SYNERGY_DCD_PIPE5CTR)
#define UX_RX_DCD_PIPE6CTR                                  (UX_SYNERGY_DCD_PIPE6CTR)
#define UX_RX_DCD_PIPE7CTR                                  (UX_SYNERGY_DCD_PIPE7CTR)
#define UX_RX_DCD_PIPE8CTR                                  (UX_SYNERGY_DCD_PIPE8CTR)
#define UX_RX_DCD_PIPE9CTR                                  (UX_SYNERGY_DCD_PIPE9CTR)
#define UX_RX_DCD_PIPE1TRE                                  (UX_SYNERGY_DCD_PIPE1TRE)
#define UX_RX_DCD_PIPE1TRN                                  (UX_SYNERGY_DCD_PIPE1TRN)
#define UX_RX_DCD_PIPE2TRE                                  (UX_SYNERGY_DCD_PIPE2TRE)
#define UX_RX_DCD_PIPE2TRN                                  (UX_SYNERGY_DCD_PIPE2TRN)
#define UX_RX_DCD_PIPE3TRE                                  (UX_SYNERGY_DCD_PIPE3TRE)
#define UX_RX_DCD_PIPE3TRN                                  (UX_SYNERGY_DCD_PIPE3TRN)
#define UX_RX_DCD_PIPE4TRE                                  (UX_SYNERGY_DCD_PIPE4TRE)
#define UX_RX_DCD_PIPE4TRN                                  (UX_SYNERGY_DCD_PIPE4TRN)
#define UX_RX_DCD_PIPE5TRE                                  (UX_SYNERGY_DCD_PIPE5TRE)
#define UX_RX_DCD_PIPE5TRN                                  (UX_SYNERGY_DCD_PIPE5TRN)
#define UX_RX_DCD_DEVADD0                                   (UX_SYNERGY_DCD_DEVADD0)
#define UX_RX_DCD_DEVADD1                                   (UX_SYNERGY_DCD_DEVADD1)
#define UX_RX_DCD_DEVADD2                                   (UX_SYNERGY_DCD_DEVADD2)
#define UX_RX_DCD_DEVADD3                                   (UX_SYNERGY_DCD_DEVADD3)
#define UX_RX_DCD_DEVADD4                                   (UX_SYNERGY_DCD_DEVADD4)
#define UX_RX_DCD_DEVADD5                                   (UX_SYNERGY_DCD_DEVADD5)

/* Define SYNERGY control register values.  */
#define UX_RX_DCD_SYSCFG_SCKE                               (UX_SYNERGY_DCD_SYSCFG_SCKE)
#define UX_RX_DCD_SYSCFG_DCFM                               (UX_SYNERGY_DCD_SYSCFG_DCFM)
#define UX_RX_DCD_SYSCFG_DRPD                               (UX_SYNERGY_DCD_SYSCFG_DRPD)
#define UX_RX_DCD_SYSCFG_DPRPU                              (UX_SYNERGY_DCD_SYSCFG_DPRPU)
#define UX_RX_DCD_SYSCFG_USBE                               (UX_SYNERGY_DCD_SYSCFG_USBE)

#define UX_RX_DCD_SYSSTS_LNST                               (UX_SYNERGY_DCD_SYSSTS_LNST)
#define UX_RX_DCD_SYSSTS_SOFEN                              (UX_SYNERGY_DCD_SYSSTS_SOFEN)

#define UX_RX_DCD_DVSTCTR_UACKEY0                           (UX_SYNERGY_DCD_DVSTCTR_UACKEY0)
#define UX_RX_DCD_DVSTCTR_UACKEY1                           (UX_SYNERGY_DCD_DVSTCTR_UACKEY1)
#define UX_RX_DCD_DVSTCTR_WKUP                              (UX_SYNERGY_DCD_DVSTCTR_WKUP)
#define UX_RX_DCD_DVSTCTR_RWUPE                             (UX_SYNERGY_DCD_DVSTCTR_RWUPE)
#define UX_RX_DCD_DVSTCTR_USBRST                            (UX_SYNERGY_DCD_DVSTCTR_USBRST)
#define UX_RX_DCD_DVSTCTR_RESUME                            (UX_SYNERGY_DCD_DVSTCTR_RESUME)
#define UX_RX_DCD_DVSTCTR_UACT                              (UX_SYNERGY_DCD_DVSTCTR_UACT)
#define UX_RX_DCD_DVSTCTR_RHST                              (UX_SYNERGY_DCD_DVSTCTR_RHST)

#define UX_RX_DCD_DVSTCTR_SPEED_LOW                         (UX_SYNERGY_DCD_DVSTCTR_SPEED_LOW)
#define UX_RX_DCD_DVSTCTR_SPEED_FULL                        (UX_SYNERGY_DCD_DVSTCTR_SPEED_FULL)
#define UX_RX_DCD_DVSTCTR_SPEED_HIGH                        (UX_SYNERGY_DCD_DVSTCTR_SPEED_HIGH)
#define UX_RX_DCD_DVSTCTR_RESET_IN_PROGRESS                 (UX_SYNERGY_DCD_DVSTCTR_RESET_IN_PROGRESS)

#define UX_RX_DCD_TESTMODE_HOSTPCC                          (UX_SYNERGY_DCD_TESTMODE_HOSTPCC)

#define UX_RX_DCD_DXFBCFG_DFACC                             (UX_SYNERGY_DCD_DXFBCFG_DFACC)

#define UX_RX_DCD_CFIFOSEL_RCNT                             (UX_SYNERGY_DCD_CFIFOSEL_RCNT)
#define UX_RX_DCD_CFIFOSEL_REW                              (UX_SYNERGY_DCD_CFIFOSEL_REW)
#define UX_RX_DCD_CFIFOSEL_MBW_8                            (UX_SYNERGY_DCD_CFIFOSEL_MBW_8)
#define UX_RX_DCD_CFIFOSEL_MBW_MASK                         (UX_SYNERGY_DCD_CFIFOSEL_MBW_MASK)
#define UX_RX_DCD_CFIFOSEL_BIGEND                           (UX_SYNERGY_DCD_CFIFOSEL_BIGEND)
#define UX_RX_DCD_CFIFOSEL_ISEL                             (UX_SYNERGY_DCD_CFIFOSEL_ISEL)
#define UX_RX_DCD_CFIFOSEL_CURPIPE_MASK                     (UX_SYNERGY_DCD_CFIFOSEL_CURPIPE_MASK)

#define UX_RX_DCD_DFIFOSEL_RCNT                             (UX_SYNERGY_DCD_DFIFOSEL_RCNT)
#define UX_RX_DCD_DFIFOSEL_REW                              (UX_SYNERGY_DCD_DFIFOSEL_REW)
#define UX_RX_DCD_DFIFOSEL_DCLRM                            (UX_SYNERGY_DCD_DFIFOSEL_DCLRM)
#define UX_RX_DCD_DFIFOSEL_DREQE                            (UX_SYNERGY_DCD_DFIFOSEL_DREQE)
#define UX_RX_DCD_DFIFOSEL_MBW_8                            (UX_SYNERGY_DCD_DFIFOSEL_MBW_8)
#define UX_RX_DCD_DFIFOSEL_BIGEND                           (UX_SYNERGY_DCD_DFIFOSEL_BIGEND)

#define UX_RX_DCD_FIFOCTR_BVAL                              (UX_SYNERGY_DCD_FIFOCTR_BVAL)
#define UX_RX_DCD_FIFOCTR_BCLR                              (UX_SYNERGY_DCD_FIFOCTR_BCLR)
#define UX_RX_DCD_FIFOCTR_FRDY                              (UX_SYNERGY_DCD_FIFOCTR_FRDY)
#define UX_RX_DCD_FIFOCTR_DTLN                              (UX_SYNERGY_DCD_FIFOCTR_DTLN)
                                                        
#define UX_RX_DCD_INTENB0_VBSE                              (UX_SYNERGY_DCD_INTENB0_VBSE)
#define UX_RX_DCD_INTENB0_RSME                              (UX_SYNERGY_DCD_INTENB0_RSME)
#define UX_RX_DCD_INTENB0_SOFE                              (UX_SYNERGY_DCD_INTENB0_SOFE)
#define UX_RX_DCD_INTENB0_DVSE                              (UX_SYNERGY_DCD_INTENB0_DVSE)
#define UX_RX_DCD_INTENB0_CTRE                              (UX_SYNERGY_DCD_INTENB0_CTRE)
#define UX_RX_DCD_INTENB0_BEMPE                             (UX_SYNERGY_DCD_INTENB0_BEMPE)
#define UX_RX_DCD_INTENB0_NRDYE                             (UX_SYNERGY_DCD_INTENB0_NRDYE)
#define UX_RX_DCD_INTENB0_BRDYE                             (UX_SYNERGY_DCD_INTENB0_BRDYE)

#define UX_RX_DCD_INTENB1_BCHGE                             (UX_SYNERGY_DCD_INTENB1_BCHGE)
#define UX_RX_DCD_INTENB1_DTCHE                             (UX_SYNERGY_DCD_INTENB1_DTCHE)
#define UX_RX_DCD_INTENB1_ATTCHE                            (UX_SYNERGY_DCD_INTENB1_ATTCHE)
#define UX_RX_DCD_INTENB1_EOFERRE                           (UX_SYNERGY_DCD_INTENB1_EOFERRE)
#define UX_RX_DCD_INTENB1_SIGNE                             (UX_SYNERGY_DCD_INTENB1_SIGNE)
#define UX_RX_DCD_INTENB1_SACKE                             (UX_SYNERGY_DCD_INTENB1_SACKE)

#define UX_RX_DCD_PIPE0                                     (UX_SYNERGY_DCD_PIPE0)
#define UX_RX_DCD_PIPE1                                     (UX_SYNERGY_DCD_PIPE1)
#define UX_RX_DCD_PIPE2                                     (UX_SYNERGY_DCD_PIPE2)
#define UX_RX_DCD_PIPE3                                     (UX_SYNERGY_DCD_PIPE3)
#define UX_RX_DCD_PIPE4                                     (UX_SYNERGY_DCD_PIPE4)
#define UX_RX_DCD_PIPE5                                     (UX_SYNERGY_DCD_PIPE5)
#define UX_RX_DCD_PIPE6                                     (UX_SYNERGY_DCD_PIPE6)
#define UX_RX_DCD_PIPE7                                     (UX_SYNERGY_DCD_PIPE7)
#define UX_RX_DCD_PIPE8                                     (UX_SYNERGY_DCD_PIPE8)
#define UX_RX_DCD_PIPE9                                     (UX_SYNERGY_DCD_PIPE9)
#define UX_RX_DCD_PIPE_ALL                                  (UX_SYNERGY_DCD_PIPE_ALL)

#define UX_RX_DCD_SOFCFG_TRNENSEL                           (UX_SYNERGY_DCD_SOFCFG_TRNENSEL)
#define UX_RX_DCD_SOFCFG_BRDYM                              (UX_SYNERGY_DCD_SOFCFG_BRDYM)
#define UX_RX_DCD_SOFCFG_INIT                               (UX_SYNERGY_DCD_SOFCFG_INIT)

#define UX_RX_DCD_INTSTS0_VBINT                             (UX_SYNERGY_DCD_INTSTS0_VBINT)
#define UX_RX_DCD_INTSTS0_RESM                              (UX_SYNERGY_DCD_INTSTS0_RESM)
#define UX_RX_DCD_INTSTS0_SOFR                              (UX_SYNERGY_DCD_INTSTS0_SOFR)
#define UX_RX_DCD_INTSTS0_DVST                              (UX_SYNERGY_DCD_INTSTS0_DVST)
#define UX_RX_DCD_INTSTS0_CTRT                              (UX_SYNERGY_DCD_INTSTS0_CTRT)
#define UX_RX_DCD_INTSTS0_BEMP                              (UX_SYNERGY_DCD_INTSTS0_BEMP)
#define UX_RX_DCD_INTSTS0_NRDY                              (UX_SYNERGY_DCD_INTSTS0_NRDY)
#define UX_RX_DCD_INTSTS0_BRDY                              (UX_SYNERGY_DCD_INTSTS0_BRDY)
#define UX_RX_DCD_INTSTS0_VBSTS                             (UX_SYNERGY_DCD_INTSTS0_VBSTS)
#define UX_RX_DCD_INTSTS0_DVSQ_MASK                         (UX_SYNERGY_DCD_INTSTS0_DVSQ_MASK)
#define UX_RX_DCD_INTSTS0_VALID                             (UX_SYNERGY_DCD_INTSTS0_VALID)
#define UX_RX_DCD_INTSTS0_CTSQ_MASK                         (UX_SYNERGY_DCD_INTSTS0_CTSQ_MASK)

#define UX_RX_DCD_INTSTS0_DVSQ_POWERED                      (UX_SYNERGY_DCD_INTSTS0_DVSQ_POWERED)
#define UX_RX_DCD_INTSTS0_DVSQ_DEFAULT                      (UX_SYNERGY_DCD_INTSTS0_DVSQ_DEFAULT)
#define UX_RX_DCD_INTSTS0_DVSQ_ADDRESS                      (UX_SYNERGY_DCD_INTSTS0_DVSQ_ADDRESS)
#define UX_RX_DCD_INTSTS0_DVSQ_CONFIGURED                   (UX_SYNERGY_DCD_INTSTS0_DVSQ_CONFIGURED)
#define UX_RX_DCD_INTSTS0_DVSQ_SUSPENDED_POWERED            (UX_SYNERGY_DCD_INTSTS0_DVSQ_SUSPENDED_POWERED)
#define UX_RX_DCD_INTSTS0_DVSQ_SUSPENDED_DEFAULT            (UX_SYNERGY_DCD_INTSTS0_DVSQ_SUSPENDED_DEFAULT)
#define UX_RX_DCD_INTSTS0_DVSQ_SUSPENDED_ADDRESS            (UX_SYNERGY_DCD_INTSTS0_DVSQ_SUSPENDED_ADDRESS)
#define UX_RX_DCD_INTSTS0_DVSQ_SUSPENDED_CONFIGURED         (UX_SYNERGY_DCD_INTSTS0_DVSQ_SUSPENDED_CONFIGURED)

#define UX_RX_DCD_INTSTS0_CTSQ_SETUP                        (UX_SYNERGY_DCD_INTSTS0_CTSQ_SETUP)
#define UX_RX_DCD_INTSTS0_CTSQ_CRDS                         (UX_SYNERGY_DCD_INTSTS0_CTSQ_CRDS)
#define UX_RX_DCD_INTSTS0_CTSQ_CRSS                         (UX_SYNERGY_DCD_INTSTS0_CTSQ_CRSS)
#define UX_RX_DCD_INTSTS0_CTSQ_CWDS                         (UX_SYNERGY_DCD_INTSTS0_CTSQ_CWDS)
#define UX_RX_DCD_INTSTS0_CTSQ_CWSS                         (UX_SYNERGY_DCD_INTSTS0_CTSQ_CWSS)
#define UX_RX_DCD_INTSTS0_CTSQ_CWNDSS                       (UX_SYNERGY_DCD_INTSTS0_CTSQ_CWNDSS)
#define UX_RX_DCD_INTSTS0_CTSQ_CTSE                         (UX_SYNERGY_DCD_INTSTS0_CTSQ_CTSE)

#define UX_RX_DCD_INTSTS1_BCHG                              (UX_SYNERGY_DCD_INTSTS1_BCHG)
#define UX_RX_DCD_INTSTS1_DTCH                              (UX_SYNERGY_DCD_INTSTS1_DTCH)
#define UX_RX_DCD_INTSTS1_ATTCH                             (UX_SYNERGY_DCD_INTSTS1_ATTCH)
#define UX_RX_DCD_INTSTS1_EOFERR                            (UX_SYNERGY_DCD_INTSTS1_EOFERR)
#define UX_RX_DCD_INTSTS1_SIGN                              (UX_SYNERGY_DCD_INTSTS1_SIGN)
#define UX_RX_DCD_INTSTS1_SACK                              (UX_SYNERGY_DCD_INTSTS1_SACK)

#define UX_RX_DCD_FRMNUM_OVRN                               (UX_SYNERGY_DCD_FRMNUM_OVRN)
#define UX_RX_DCD_FRMNUM_CRCE                               (UX_SYNERGY_DCD_FRMNUM_CRCE)
#define UX_RX_DCD_FRMNUM_FRNM_MASK                          (UX_SYNERGY_DCD_FRMNUM_FRNM_MASK)
 
#define UX_RX_DCD_DCPCFG_DIR                                (UX_SYNERGY_DCD_DCPCFG_DIR)
#define UX_RX_DCD_DCPCFG_SHTNAK                             (UX_SYNERGY_DCD_DCPCFG_SHTNAK)
#define UX_RX_DCD_DCPCFG_CNTMD                              (UX_SYNERGY_DCD_DCPCFG_CNTMD)

#define UX_RX_DCD_DCPMAXP_DEVADDR_SHIFT                     (UX_SYNERGY_DCD_DCPMAXP_DEVADDR_SHIFT)
#define UX_RX_DCD_DCPMAXP_DEVADDR_MASK                      (UX_SYNERGY_DCD_DCPMAXP_DEVADDR_MASK)

#define UX_RX_DCD_DCPCTR_BSTS                               (UX_SYNERGY_DCD_DCPCTR_BSTS)
#define UX_RX_DCD_DCPCTR_INBUFM                             (UX_SYNERGY_DCD_DCPCTR_INBUFM)
#define UX_RX_DCD_DCPCTR_CSCLR                              (UX_SYNERGY_DCD_DCPCTR_CSCLR)
#define UX_RX_DCD_DCPCTR_CSSTS                              (UX_SYNERGY_DCD_DCPCTR_CSSTS)
#define UX_RX_DCD_DCPCTR_SUREQCLR                           (UX_SYNERGY_DCD_DCPCTR_SUREQCLR)
#define UX_RX_DCD_DCPCTR_SQCLR                              (UX_SYNERGY_DCD_DCPCTR_SQCLR)
#define UX_RX_DCD_DCPCTR_SQSET                              (UX_SYNERGY_DCD_DCPCTR_SQSET)
#define UX_RX_DCD_DCPCTR_SQMON                              (UX_SYNERGY_DCD_DCPCTR_SQMON)
#define UX_RX_DCD_DCPCTR_PBUSY                              (UX_SYNERGY_DCD_DCPCTR_PBUSY)
#define UX_RX_DCD_DCPCTR_PINGE                              (UX_SYNERGY_DCD_DCPCTR_PINGE)
#define UX_RX_DCD_DCPCTR_CCPL                               (UX_SYNERGY_DCD_DCPCTR_CCPL)
#define UX_RX_DCD_DCPCTR_PID_MASK                           (UX_SYNERGY_DCD_DCPCTR_PID_MASK)
#define UX_RX_DCD_DCPCTR_PIDNAK                             (UX_SYNERGY_DCD_DCPCTR_PIDNAK)
#define UX_RX_DCD_DCPCTR_PIDBUF                             (UX_SYNERGY_DCD_DCPCTR_PIDBUF)
#define UX_RX_DCD_DCPCTR_PIDSTALL                           (UX_SYNERGY_DCD_DCPCTR_PIDSTALL)
#define UX_RX_DCD_DCPCTR_PIDSTALL2                          (UX_SYNERGY_DCD_DCPCTR_PIDSTALL2)


#define UX_RX_DCD_PIPECFG_TYPE_MASK                         (UX_SYNERGY_DCD_PIPECFG_TYPE_MASK)
#define UX_RX_DCD_PIPECFG_TYPE_BIT_USED                     (UX_SYNERGY_DCD_PIPECFG_TYPE_BIT_USED)
#define UX_RX_DCD_PIPECFG_TYPE_BULK                         (UX_SYNERGY_DCD_PIPECFG_TYPE_BULK)
#define UX_RX_DCD_PIPECFG_TYPE_INTERRUPT                    (UX_SYNERGY_DCD_PIPECFG_TYPE_INTERRUPT)
#define UX_RX_DCD_PIPECFG_TYPE_ISOCHRONOUS                  (UX_SYNERGY_DCD_PIPECFG_TYPE_ISOCHRONOUS)
#define UX_RX_DCD_PIPECFG_BFRE                              (UX_SYNERGY_DCD_PIPECFG_BFRE)
#define UX_RX_DCD_PIPECFG_DBLB                              (UX_SYNERGY_DCD_PIPECFG_DBLB)
#define UX_RX_DCD_PIPECFG_CNTMD                             (UX_SYNERGY_DCD_PIPECFG_CNTMD)
#define UX_RX_DCD_PIPECFG_SHTNAK                            (UX_SYNERGY_DCD_PIPECFG_SHTNAK)
#define UX_RX_DCD_PIPECFG_DIR                               (UX_SYNERGY_DCD_PIPECFG_DIR)
#define UX_RX_DCD_PIPECFG_EPNUM_MASK                        (UX_SYNERGY_DCD_PIPECFG_EPNUM_MASK)

#define UX_RX_DCD_PIPEBUF_SIZEMASK                          (UX_SYNERGY_DCD_PIPEBUF_SIZEMASK)
#define UX_RX_DCD_PIPEBUF_BUFNMBMASK                        (UX_SYNERGY_DCD_PIPEBUF_BUFNMBMASK)
#define UX_RX_DCD_PIPEBUF_SHIFT                             (UX_SYNERGY_DCD_PIPEBUF_SHIFT)

#define UX_RX_DCD_PIPEMAXP_DEVSELMASK                       (UX_SYNERGY_DCD_PIPEMAXP_DEVSELMASK)
#define UX_RX_DCD_PIPEMAXP_DEVSEL_SHIFT                     (UX_SYNERGY_DCD_PIPEMAXP_DEVSEL_SHIFT)
#define UX_RX_DCD_PIPEMAXP_MXPSMASK                         (UX_SYNERGY_DCD_PIPEMAXP_MXPSMASK)

#define UX_RX_DCD_PIPE1TRE_TRCLR                            (UX_SYNERGY_DCD_PIPE1TRE_TRCLR)
#define UX_RX_DCD_PIPE1TRE_TRENB                            (UX_SYNERGY_DCD_PIPE1TRE_TRENB)

#define UX_RX_DCD_FIFO_D0                                   (UX_SYNERGY_DCD_FIFO_D0)
#define UX_RX_DCD_FIFO_D1                                   (UX_SYNERGY_DCD_FIFO_D1)
#define UX_RX_DCD_FIFO_C                                    (UX_SYNERGY_DCD_FIFO_C)

#define UX_RX_DCD_DEVADDX_SPEED_LOW                         (UX_SYNERGY_DCD_DEVADDX_SPEED_LOW)
#define UX_RX_DCD_DEVADDX_SPEED_FULL                        (UX_SYNERGY_DCD_DEVADDX_SPEED_FULL)
#define UX_RX_DCD_DEVADDX_SPEED_HIGH                        (UX_SYNERGY_DCD_DEVADDX_SPEED_HIGH)
#define UX_RX_DCD_DEVADDX_UPPHUB_SHIFT                      (UX_SYNERGY_DCD_DEVADDX_UPPHUB_SHIFT)
#define UX_RX_DCD_DEVADDX_HUBPORT_SHIFT                     (UX_SYNERGY_DCD_DEVADDX_HUBPORT_SHIFT)

/* Define synergy command/status bitmaps.  */

#define UX_RX_DCD_DCP                                       (UX_SYNERGY_DCD_DCP)

#define UX_RX_DCD_DCPCTR_DATA1                              (UX_SYNERGY_DCD_DCPCTR_DATA1)
#define UX_RX_DCD_DCPCTR_DATA0                              (UX_SYNERGY_DCD_DCPCTR_DATA0)

/* Define synergy PIPE selection definitions.  */
#define UX_RX_DCD_PIPESEL_NO_PIPE                           (UX_SYNERGY_DCD_PIPESEL_NO_PIPE)

/* Define synergy fifo definition.  */
#define UX_RX_DCD_PIPE0_SIZE                                (UX_SYNERGY_DCD_PIPE0_SIZE)
#define UX_RX_DCD_PIPE_NB_BUFFERS                           (UX_SYNERGY_DCD_PIPE_NB_BUFFERS)

#define UX_RX_DCD_PIPECTR_INBUFM                            (UX_SYNERGY_DCD_PIPECTR_INBUFM)
#define UX_RX_DCD_PIPECTR_BSTS                              (UX_SYNERGY_DCD_PIPECTR_BSTS)
#define UX_RX_DCD_PIPECTR_CSCLR                             (UX_SYNERGY_DCD_PIPECTR_CSCLR)
#define UX_RX_DCD_PIPECTR_CSSTS                             (UX_SYNERGY_DCD_PIPECTR_CSSTS)
#define UX_RX_DCD_PIPECTR_ATREPM                            (UX_SYNERGY_DCD_PIPECTR_ATREPM)
#define UX_RX_DCD_PIPECTR_SQCLR                             (UX_SYNERGY_DCD_PIPECTR_SQCLR)
#define UX_RX_DCD_PIPECTR_SQSET                             (UX_SYNERGY_DCD_PIPECTR_SQSET)
#define UX_RX_DCD_PIPECTR_SQMON                             (UX_SYNERGY_DCD_PIPECTR_SQMON)
#define UX_RX_DCD_PIPECTR_PBUSY                             (UX_SYNERGY_DCD_PIPECTR_PBUSY)
#define UX_RX_DCD_PIPECTR_PID_MASK                          (UX_SYNERGY_DCD_PIPECTR_PID_MASK)
#define UX_RX_DCD_PIPECTR_PIDNAK                            (UX_SYNERGY_DCD_PIPECTR_PIDNAK)
#define UX_RX_DCD_PIPECTR_PIDBUF                            (UX_SYNERGY_DCD_PIPECTR_PIDBUF)
#define UX_RX_DCD_PIPECTR_PIDSTALL                          (UX_SYNERGY_DCD_PIPECTR_PIDSTALL)
#define UX_RX_DCD_PIPECTR_PIDSTALL2                         (UX_SYNERGY_DCD_PIPECTR_PIDSTALL2)
#define UX_RX_DCD_PIPECTR_DATA1                             (UX_SYNERGY_DCD_PIPECTR_DATA1)
#define UX_RX_DCD_PIPECTR_DATA0                             (UX_SYNERGY_DCD_PIPECTR_DATA0)

/* Define synergy initialization values.  */

#define UX_RX_DCD_COMMAND_STATUS_RESET                      (UX_SYNERGY_DCD_COMMAND_STATUS_RESET)
#define UX_RX_DCD_INIT_RESET_DELAY                          (UX_SYNERGY_DCD_INIT_RESET_DELAY)
#define UX_RX_DCD_MAX_BUF_SIZE                              (UX_SYNERGY_DCD_MAX_BUF_SIZE)
#define UX_RX_DCD_MAX_BUF_NUM                               (UX_SYNERGY_DCD_MAX_BUF_NUM)

/* Define synergy FIFO write completion code.  */
#define UX_RX_DCD_FIFO_WRITING                              (UX_SYNERGY_DCD_FIFO_WRITING)
#define UX_RX_DCD_FIFO_WRITE_END                            (UX_SYNERGY_DCD_FIFO_WRITE_END)
#define UX_RX_DCD_FIFO_WRITE_SHORT                          (UX_SYNERGY_DCD_FIFO_WRITE_SHORT)
#define UX_RX_DCD_FIFO_WRITE_DMA                            (UX_SYNERGY_DCD_FIFO_WRITE_DMA)
#define UX_RX_DCD_FIFO_WRITE_ERROR                          (UX_SYNERGY_DCD_FIFO_WRITE_ERROR)

/* Define synergy FIFO read completion code.  */
#define UX_RX_DCD_FIFO_READING                              (UX_SYNERGY_DCD_FIFO_READING)
#define UX_RX_DCD_FIFO_READ_END                             (UX_SYNERGY_DCD_FIFO_READ_END)
#define UX_RX_DCD_FIFO_READ_SHORT                           (UX_SYNERGY_DCD_FIFO_READ_SHORT)
#define UX_RX_DCD_FIFO_READ_DMA                             (UX_SYNERGY_DCD_FIFO_READ_DMA)
#define UX_RX_DCD_FIFO_READ_ERROR                           (UX_SYNERGY_DCD_FIFO_READ_ERROR)
#define UX_RX_DCD_FIFO_READ_OVER                            (UX_SYNERGY_DCD_FIFO_READ_OVER)

/* Define synergy physical endpoint definitions.  */
#define UX_RX_DCD_ED_BRDY                                   (UX_SYNERGY_DCD_ED_BRDY)
#define UX_RX_DCD_ED_NRDY                                   (UX_SYNERGY_DCD_ED_NRDY)
#define UX_RX_DCD_ED_BEMP                                   (UX_SYNERGY_DCD_ED_BEMP)
#define UX_RX_DCD_ED_EOFERR                                 (UX_SYNERGY_DCD_ED_EOFERR)
#define UX_RX_DCD_ED_SIGN                                   (UX_SYNERGY_DCD_ED_SIGN)
#define UX_RX_DCD_ED_SACK                                   (UX_SYNERGY_DCD_ED_SACK)

/* Define USB SYNERGY physical endpoint status definition.  */

#define UX_DCD_RX_ED_STATUS_UNUSED                          (UX_DCD_SYNERGY_ED_STATUS_UNUSED)
#define UX_DCD_RX_ED_STATUS_USED                            (UX_DCD_SYNERGY_ED_STATUS_USED)
#define UX_DCD_RX_ED_STATUS_TRANSFER                        (UX_DCD_SYNERGY_ED_STATUS_TRANSFER)
#define UX_DCD_RX_ED_STATUS_STALLED                         (UX_DCD_SYNERGY_ED_STATUS_STALLED)

/* Define USB SYNERGY physical endpoint state machine definition.  */

#define UX_DCD_RX_ED_STATE_IDLE                             (UX_DCD_SYNERGY_ED_STATE_IDLE)
#define UX_DCD_RX_ED_STATE_DATA_TX                          (UX_DCD_SYNERGY_ED_STATE_DATA_TX)
#define UX_DCD_RX_ED_STATE_DATA_RX                          (UX_DCD_SYNERGY_ED_STATE_DATA_RX)
#define UX_DCD_RX_ED_STATE_STATUS_TX                        (UX_DCD_SYNERGY_ED_STATE_STATUS_TX)
#define UX_DCD_RX_ED_STATE_STATUS_RX                        (UX_DCD_SYNERGY_ED_STATE_STATUS_RX)

/* Define USB SYNERGY physical endpoint structure.  */

typedef struct UX_DCD_SYNERGY_ED UX_DCD_RX_ED;

/* Define USB SYNERGY DCD structure definition.  */

typedef struct UX_DCD_SYNERGY UX_DCD_RX;

/* Define USB SYNERGY DCD prototypes.  */

#define _ux_dcd_rx_address_set                _ux_dcd_synergy_address_set
#define _ux_dcd_rx_endpoint_create            _ux_dcd_synergy_endpoint_create
#define _ux_dcd_rx_endpoint_destroy           _ux_dcd_synergy_endpoint_destroy
#define _ux_dcd_rx_endpoint_reset             _ux_dcd_synergy_endpoint_reset
#define _ux_dcd_rx_endpoint_stall             _ux_dcd_synergy_endpoint_stall
#define _ux_dcd_rx_endpoint_status            _ux_dcd_synergy_endpoint_status
#define _ux_dcd_rx_frame_number_get           _ux_dcd_synergy_frame_number_get
#define _ux_dcd_rx_function                   _ux_dcd_synergy_function
#define _ux_dcd_rx_initialize(x)              _ux_dcd_synergy_initialize((ULONG) R_USBFS)
#define _ux_dcd_rx_initialize_complete        _ux_dcd_synergy_initialize_complete
#define _ux_dcd_rx_interrupt_handler          _ux_dcd_synergy_interrupt_handler
#define _ux_dcd_rx_register_clear             _ux_dcd_synergy_register_clear
#define _ux_dcd_rx_register_read              _ux_dcd_synergy_register_read
#define _ux_dcd_rx_register_set               _ux_dcd_synergy_register_set
#define _ux_dcd_rx_register_write             _ux_dcd_synergy_register_write
#define _ux_dcd_rx_state_change               _ux_dcd_synergy_state_change
#define _ux_dcd_rx_transfer_callback          _ux_dcd_synergy_transfer_callback
#define _ux_dcd_rx_transfer_request           _ux_dcd_synergy_transfer_request
#define _ux_dcd_rx_buffer_read                _ux_dcd_synergy_buffer_read
#define _ux_dcd_rx_fifo_read                  _ux_dcd_synergy_fifo_read
#define _ux_dcd_rx_buffer_notready_interrupt  _ux_dcd_synergy_buffer_notready_interrupt
#define _ux_dcd_rx_buffer_empty_interrupt     _ux_dcd_synergy_buffer_empty_interrupt
#define _ux_dcd_rx_buffer_ready_interrupt     _ux_dcd_synergy_buffer_ready_interrupt
#define _ux_dcd_rx_fifo_port_change           _ux_dcd_synergy_fifo_port_change
#define _ux_dcd_rx_fifod_write                _ux_dcd_synergy_fifod_write
#define _ux_dcd_rx_fifoc_write                _ux_dcd_synergy_fifoc_write
#define _ux_dcd_rx_buffer_write               _ux_dcd_synergy_buffer_write
#define _ux_dcd_rx_data_buffer_size           _ux_dcd_synergy_data_buffer_size
#define _ux_dcd_rx_endpoint_nak_set           _ux_dcd_synergy_endpoint_nak_set
#define _ux_dcd_rx_current_endpoint_change    _ux_dcd_synergy_current_endpoint_change

#endif

