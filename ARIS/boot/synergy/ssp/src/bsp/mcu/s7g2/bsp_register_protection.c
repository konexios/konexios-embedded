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
* File Name    : bsp_register_protection.c
* Description  : This module provides APIs for modifying register write protection.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

#if defined(BSP_MCU_GROUP_S7G2)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Key code for writing PRCR register. */
#define BSP_PRV_PRCR_KEY        (0xA500)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/** Used for holding reference counters for protection bits. */
static volatile uint16_t g_protect_counters[BSP_REG_PROTECT_TOTAL_ITEMS];
/** Masks for setting or clearing the PRCR register. Use -1 for size because PWPR in MPC is used differently. */
static const    uint16_t g_prcr_masks[BSP_REG_PROTECT_TOTAL_ITEMS] =
{
    0x0001,         /* PRC0. */
    0x0002,         /* PRC1. */
    0x0008,         /* PRC3. */
};

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_REG_PROTECT_S7G2
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Enable register protection. Registers that are protected cannot be written to. Register protection is
 *          enabled by using the Protect Register (PRCR) and the MPC's Write-Protect Register (PWPR).
 *
 * @param[in] regs_to_protect Registers which have write protection enabled.
 **********************************************************************************************************************/
void R_BSP_RegisterProtectEnable (bsp_reg_protect_t regs_to_protect)
{
    /* Is it safe to disable write access? */
    if (0 != g_protect_counters[regs_to_protect])
    {
        /* Decrement the protect counter */
        g_protect_counters[regs_to_protect]--;
    }

    /* Is it safe to disable write access? */
    if (0 == g_protect_counters[regs_to_protect])
    {
        /** Enable protection using PRCR register for S7G2 MCUs. */
        /** When writing to the PRCR register the upper 8-bits must be the correct key. Set lower bits to 0 to
           disable writes.
           This is the PRCR register description for S7G2 MCUs:

           <PRE>
           b15:b8 PRKEY - Write 0xA5 to upper byte to enable writing to lower byte
           b7:b4  Reserved (set to 0)
           b3     PRC3  - Enables writing to the registers related to the LVD:
           LVD1CR1, LVD1SR, LVD2CR1, LVD2SR, LVCMPCR, LVDLVLR, LVD1CR0, LVD2CR0.
           b2     Reserved (set to 0)
           b1     PRC1  - Enables writing to the registers related to low power modes:
           SBYCR, SNZCR, SNZEDCR, SNZREQCR, OPCCR, SOPCCR, DPSBYCR, DPSIER0-3, DPSIFR0-3,
           DPSIEGR0-3, SYOCDCR, STCONR.
           Enables writing to registers related to the Battery Backup function:
           VBTBKRn (n = 0 to 511)
           b0     PRC0  - Enables writing to the registers related to the clock generation circuit:
           SCKDIVCR, SCKDIVCR2, SCKSCR, PLLCCR, PLLCR, BCKCR, MOSCCR, HOCOCR, MOCOCR, CKOCR,
           TRCKCR, OSTDCR, OSTDSR, EBCKOCR, SDCKOCR, MOCOUTCR, HOCOUTCR, MOSCWTCR, MOMCR,
           SOSCCR, SOMCR, LOCOCR, LOCOUTCR, HOCOWTCR.
           </PRE>
        */
        R_SYSTEM->PRCR = (uint16_t)((R_SYSTEM->PRCR | BSP_PRV_PRCR_KEY) & (~g_prcr_masks[regs_to_protect]));
    }
}

/*******************************************************************************************************************//**
 * @brief Disable register protection. Registers that are protected cannot be written to. Register protection is
 *          disabled by using the Protect Register (PRCR) and the MPC's Write-Protect Register (PWPR).
 *
 * @param[in] regs_to_unprotect Registers which have write protection disabled.
 **********************************************************************************************************************/
void R_BSP_RegisterProtectDisable (bsp_reg_protect_t regs_to_unprotect)
{
    /* If this is first entry then disable protection. */
    if (0 == g_protect_counters[regs_to_unprotect])
    {
        /** Disable protection using PRCR register for S7G2 MCUs. */
        /** When writing to the PRCR register the upper 8-bits must be the correct key. Set lower bits to 0 to
           disable writes:
           This is the PRCR register description for S7G2 MCUs:

           <PRE>
           b15:b8 PRKEY - Write 0xA5 to upper byte to enable writing to lower byte
           b7:b4  Reserved (set to 0)
           b3     PRC3  - Enables writing to the registers related to the LVD:
           LVD1CR1, LVD1SR, LVD2CR1, LVD2SR, LVCMPCR, LVDLVLR, LVD1CR0, LVD2CR0.
           b2     Reserved (set to 0)
           b1     PRC1  - Enables writing to the registers related to low power modes:
           SBYCR, SNZCR, SNZEDCR, SNZREQCR, OPCCR, SOPCCR, DPSBYCR, DPSIER0-3, DPSIFR0-3,
           DPSIEGR0-3, SYOCDCR, STCONR.
           Enables writing to registers related to the Battery Backup function:
           VBTBKRn (n = 0 to 511)
           b0     PRC0  - Enables writing to the registers related to the clock generation circuit:
           SCKDIVCR, SCKDIVCR2, SCKSCR, PLLCCR, PLLCR, BCKCR, MOSCCR, HOCOCR, MOCOCR, CKOCR,
           TRCKCR, OSTDCR, OSTDSR, EBCKOCR, SDCKOCR, MOCOUTCR, HOCOUTCR, MOSCWTCR, MOMCR,
           SOSCCR, SOMCR, LOCOCR, LOCOUTCR, HOCOWTCR.
           </PRE>
        */
    	R_SYSTEM->PRCR = (uint16_t)((R_SYSTEM->PRCR | BSP_PRV_PRCR_KEY) | g_prcr_masks[regs_to_unprotect]);
    }

    /* Increment the protect counter */
    g_protect_counters[regs_to_unprotect]++;
}

/** @} (end addtogroup BSP_MCU_REG_PROTECT_S7G2) */

/*******************************************************************************************************************//**
 * @brief Initializes variables needed for register protection functionality.
 **********************************************************************************************************************/
void bsp_register_protect_open (void)
{
    uint32_t i;

    /** Initialize reference counters to 0. */
    for (i = (uint32_t)0; i < BSP_REG_PROTECT_TOTAL_ITEMS; i++)
    {
        g_protect_counters[i] = (uint16_t)0;
    }
}

#endif



