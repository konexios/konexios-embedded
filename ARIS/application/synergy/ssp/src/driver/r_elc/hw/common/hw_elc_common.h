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

/**********************************************************************************************************************
 * File Name    : hw_elc_common.h
 * Description  : ELC LLD API
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup ELC
 * @{
 **********************************************************************************************************************/

#ifndef HW_ELC_COMMON_H
#define HW_ELC_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* TODO_ELC : RELEASE Remove mock reference */
#if !BSP_MCU_MOCKED

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if defined(BSP_BOARD_FPGA2R7FS)
#define ELC_BASE_REG  (R_ELC_BASE)
#define MSTP_BASE_REG (R_MSTP_BASE)
#else
#define ELC_BASE_REG  (R_ELC_BASE)
#define MSTP_BASE_REG (R_MSTP_BASE)
#endif
#define ELC_REG_SIZE  (0x100)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#define ELC_ELSEGRN_STEP1 (0)     /* WI = 0, WE = 0, SEG = 0 */
#define ELC_ELSEGRN_STEP2 (0x40)  /* WI = 0, WE = 1, SEG = 0 */
#define ELC_ELSEGRN_STEP3 (0x41)  /* WI = 0, WE = 1, SEG = 1 */

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Pointer to MSTP channels.  */
#define p_mstp ((R_MSTP_Type *) (MSTP_BASE_REG))

/** Pointer to ELC registers.  */
#define p_elc ((R_ELC_Type *) (ELC_BASE_REG))

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Powers on the ELC block.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ELC_PowerOn (void)
{
    p_mstp->MSTPCRC_b.MSTPC14 = 0;
}

/*******************************************************************************************************************//**
 * Powers off the ELC block.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ELC_PowerOff (void)
{
    p_mstp->MSTPCRC_b.MSTPC14 = 1;
}

/*******************************************************************************************************************//**
 * Generate a software event 0
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ELC_SoftwareEvent0Generate (void)
{
    /* Step 1. enable the ELSEGR0 register for writing */
    p_elc->ELSEGRnRC0[0].ELSEGRn = ELC_ELSEGRN_STEP1;

    /* Step 2. Enable the SEG bit for writing */
    p_elc->ELSEGRnRC0[0].ELSEGRn = ELC_ELSEGRN_STEP2;

    /* Step 3. Set the SEG bit which causes the software event generation */
    p_elc->ELSEGRnRC0[0].ELSEGRn = ELC_ELSEGRN_STEP3;
}

/*******************************************************************************************************************//**
 * Generate a software event 1
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ELC_SoftwareEvent1Generate (void)
{
    /* Step 1. enable the ELSEGR0 register for writing */
    p_elc->ELSEGRnRC0[1].ELSEGRn = ELC_ELSEGRN_STEP1;

    /* Step 2. Enable the SEG bit for writing */
    p_elc->ELSEGRnRC0[1].ELSEGRn = ELC_ELSEGRN_STEP2;

    /* Step 3. Set the SEG bit which causes the software event generation */
    p_elc->ELSEGRnRC0[1].ELSEGRn = ELC_ELSEGRN_STEP3;
}

/*******************************************************************************************************************//**
 * Enable the ELC block
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ELC_Enable (void)
{
    p_elc->ELCR_b.ELCON = 1;
}

/*******************************************************************************************************************//**
 * Disable the ELC block
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ELC_Disable (void)
{
    p_elc->ELCR_b.ELCON = 0;
}

/*******************************************************************************************************************//**
 * Make a link between a signal and a peripheral
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ELC_LinkSet (uint32_t peripheral, uint16_t signal)
{
    /* ELSRn registers are 16 bits wide but spaced 32 bits apart */
    *(&p_elc->ELSRnRC0[0].ELSRn + peripheral * 2) = signal;
}

/*******************************************************************************************************************//**
 * Break a link between a signal and a peripheral
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ELC_LinkBreak (uint32_t peripheral)
{
    /* ELSRn registers are 16 bits wide but spaced 32 bits apart */
    *(((uint32_t *) &p_elc->ELSRnRC0[0].ELSRn) + peripheral) = 0;
}
#endif /* BSP_MCU_MOCKED */
#endif /* HW_ELC_COMMON_H */
