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
* File Name    : bsp_clocks.c
* Description  : Calls the CGC module to setup the system clocks. Settings for clocks are based on macros in
*                bsp_clock_cfg.h.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_CLOCKS
 *
 * @{
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU_CLOCKS) */

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

#if defined(BSP_MCU_GROUP_S7G2)

#include "r_cgc_api.h"
#include "r_cgc.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief      Sets up system clocks.
 **********************************************************************************************************************/
void bsp_clock_init (void)
{
	g_cgc_on_cgc.init();

    /** MOCO is default clock out of reset. Enable new clock if chosen. */
    cgc_clock_t clock;

    if (BSP_CFG_CLOCK_SOURCE != CGC_CLOCK_PLL)
    {
        clock = BSP_CFG_CLOCK_SOURCE;
        g_cgc_on_cgc.clockStart(clock, NULL);
    }
    else
    {
        /** Need to start PLL source clock and let it stabilize before starting PLL */
        clock = BSP_CFG_PLL_SOURCE;
        g_cgc_on_cgc.clockStart(clock, NULL);

        cgc_clock_cfg_t pll_cfg;

        /** Set PLL Divider. */
        pll_cfg.divider = BSP_CFG_PLL_DIV;

        /** Set PLL Multiplier. */
        pll_cfg.multiplier = (float)BSP_CFG_PLL_MUL;

        /** Set PLL Source clock. */
        pll_cfg.source_clock = clock;

        while (SSP_ERR_STABILIZED != g_cgc_on_cgc.clockCheck(clock))
        {
            /** Wait for PLL clock source to stabilize */
        }

        g_cgc_on_cgc.clockStart(CGC_CLOCK_PLL, &pll_cfg);

        clock = CGC_CLOCK_PLL;
    }

    R_ROMC->ROMCE_b.ROMCEN = 1;         /** Enable ROM cache */

    /** MOCO, LOCO, and subclock do not have stabilization flags that can be checked. */
    if ((CGC_CLOCK_MOCO != clock) && (CGC_CLOCK_LOCO != clock) && (CGC_CLOCK_SUBCLOCK != clock))
    {
        while (SSP_ERR_STABILIZED != g_cgc_on_cgc.clockCheck(clock))
        {
            /** Wait for clock source to stabilize */
        }
    }

    cgc_system_clock_cfg_t sys_cfg;

    sys_cfg.iclk_div  = BSP_CFG_ICK_DIV;
    sys_cfg.pclka_div = BSP_CFG_PCKA_DIV;
    sys_cfg.pclkb_div = BSP_CFG_PCKB_DIV;
    sys_cfg.pclkc_div = BSP_CFG_PCKC_DIV;
    sys_cfg.pclkd_div = BSP_CFG_PCKD_DIV;
    sys_cfg.fclk_div  = BSP_CFG_FCK_DIV;
    sys_cfg.bclk_div  = BSP_CFG_BCK_DIV;

    /** Set which clock to use for system clock and divisors for all system clocks. */
    g_cgc_on_cgc.systemClockSet(clock, &sys_cfg);


    /** Set USB clock divisor. */
    g_cgc_on_cgc.usbClockCfg(BSP_CFG_UCK_DIV);

    /** Configure BCLK */
#if   BSP_CFG_BCLK_OUTPUT == 1
    g_cgc_on_cgc.busClockOutCfg(CGC_BCLOCKOUT_DIV_1);
    g_cgc_on_cgc.busClockOutEnable();
#elif BSP_CFG_BCLK_OUTPUT == 2
    g_cgc_on_cgc.busClockOutCfg(CGC_BCLOCKOUT_DIV_2);
    g_cgc_on_cgc.busClockOutEnable();
#else
    g_cgc_on_cgc.busClockOutDisable();
#endif

    /** Configure SDRAM Clock */
#if   BSP_CFG_SDCLK_OUTPUT == 0
    g_cgc_on_cgc.sdramClockOutDisable();
#else
    g_cgc_on_cgc.sdramClockOutEnable();
#endif
}

/*******************************************************************************************************************//**
 * @brief      Returns frequency of CPU clock in Hz.
 *
 * @retval     Frequency of the CPU in Hertz
 **********************************************************************************************************************/
uint32_t bsp_cpu_clock_get (void)
{
    uint32_t freq = (uint32_t)0;

    g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_ICLK, &freq);

    return freq;
}

#endif


