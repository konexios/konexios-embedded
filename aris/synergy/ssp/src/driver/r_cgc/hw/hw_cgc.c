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
 * File Name    : hw_cgc.c
 * Description  : Hardware related  LLD functions for the CGC HAL
 **********************************************************************************************************************/

#include "r_cgc_api.h"
#include "hw_cgc_private.h"
#include "bsp_api.h"
#include "bsp_clock_cfg.h"
#include "r_cgc_cfg.h"
#include "hw_cgc.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if defined (BSP_MCU_GROUP_S3A7) || defined (BSP_MCU_GROUP_S124) || defined (BSP_MCU_GROUP_S128)
#define CGC_PLL_DIV_1_SETTING 0
#define CGC_PLL_DIV_2_SETTING 1
#define CGC_PLL_DIV_4_SETTING 2
#endif
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/


/**********************************************************************************************************************
* Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** This section of RAM should not be initialized by the C runtime environment */
#ifdef __IAR_SYSTEMS_ICC__
__no_init static uint32_t g_clock_freq[CGC_CLOCK_PLL + 1];
#elif defined(__GNUC__) /*------------------ GNU Compiler ---------------------*/
static uint32_t           g_clock_freq[CGC_CLOCK_PLL + 1] __attribute__((section(".noinit")));
#endif

#if defined (BSP_MCU_GROUP_S7G2)
/** These are the values to write to the PLLCCR register selected by clock source  (S7G2 only) */
static const uint16_t g_pllccr_src_setting[] =
{
    [CGC_CLOCK_HOCO]     = CGC_PLL_HOCO,        ///< Value to write when the PLL source is the HOCO.
    [CGC_CLOCK_MOCO]     = CGC_PLL_HOCO,        ///< The default value to write is HOCO.
    [CGC_CLOCK_LOCO]     = CGC_PLL_HOCO,        ///< The default value to write is HOCO.
    [CGC_CLOCK_MAIN_OSC] = CGC_PLL_MAIN_OSC     ///< This is the value to write when the PLL source is the Main
                                                // Oscillator.
};

/** These are the divisor values to use when calculating the system clock frequency, using the CGC_PLL_DIV enum type */
static const uint16_t g_pllccr_div_value[] =
{
    [CGC_PLL_DIV_1] = 0x01,
    [CGC_PLL_DIV_2] = 0x02,
    [CGC_PLL_DIV_3] = 0x03
};
/** These are the values to use to set the PLL divider register according to the CGC_PLL_DIV enum type */
static const uint16_t g_pllccr_div_setting[] =
{
    [CGC_PLL_DIV_1] = 0x00,
    [CGC_PLL_DIV_2] = 0x01,
    [CGC_PLL_DIV_3] = 0x02
};
#endif
#if defined (BSP_MCU_GROUP_S3A7)
/** These are the divisor values to use when calculating the system clock frequency, using the CGC_PLL_DIV enum type */
#define CGC_PLL_DIV_1_SETTING 0
#define CGC_PLL_DIV_2_SETTING 1
#define CGC_PLL_DIV_4_SETTING 2

static const uint16_t g_pllccr_div_value[] =
{
    [CGC_PLL_DIV_1_SETTING]         = 0x01,
    [CGC_PLL_DIV_2_SETTING]         = 0x02,
    [CGC_PLL_DIV_4_SETTING]         = 0x04
};

/** These are the values to use to set the PLL divider register according to the CGC_PLL_DIV enum type */
static const uint16_t g_pllccr_div_setting[] =
{
    [CGC_PLL_DIV_1] = 0x00,
    [CGC_PLL_DIV_2] = 0x01,
    [CGC_PLL_DIV_4] = 0x02
};
#endif /* ifdef  BSP_MCU_GROUP_S3A7 */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      This function initializes CGC variables independent of the C runtime environment.
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_Init (void)
{
    /** initialize the clock frequency array */
    g_clock_freq[CGC_CLOCK_HOCO]     = BSP_HOCO_HZ;         // Initialize the HOCO value.
    g_clock_freq[CGC_CLOCK_MOCO]     = CGC_MOCO_FREQ;       // Initialize the MOCO value.
    g_clock_freq[CGC_CLOCK_LOCO]     = CGC_LOCO_FREQ;       // Initialize the LOCO value.
    g_clock_freq[CGC_CLOCK_MAIN_OSC] = CGC_MAIN_OSC_FREQ;   // Initialize the Main oscillator value.
    g_clock_freq[CGC_CLOCK_SUBCLOCK] = CGC_SUBCLOCK_FREQ;   // Initialize the subclock value.
    g_clock_freq[CGC_CLOCK_PLL]      = CGC_PLL_FREQ;        // The PLL value will be calculated at initialization.
}

/*******************************************************************************************************************//**
 * @brief      This function initializes PLL frequency value
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_InitPLLFreq (void)
{
#ifndef  BSP_MCU_SC32_4
#if defined  (BSP_MCU_GROUP_S7G2) || defined (BSP_MCU_GROUP_S3A7)

    uint32_t divider = 0;
    divider = (g_pllccr_div_value[HW_CGC_PLLDividerGet()]);
    if (divider != (uint32_t)0) /* prevent divide by 0 */
    {
#if defined  (BSP_MCU_GROUP_S7G2)
        /* This casts the float result back to an integer.  The multiplier is always a multiple of 0.5, and the clock
         * frequency is always a multiple of 2, so casting to an integer is safe. */
        g_clock_freq[CGC_CLOCK_PLL] = (uint32_t)
            (((float)g_clock_freq[HW_CGC_PLLClockSourceGet()] / (float)divider) * HW_CGC_PLLMultiplierGet());
#endif
#if defined (BSP_MCU_GROUP_S3A7)
        /* This casts the float result back to an integer.  The multiplier is always a multiple of 0.5, and the clock
         * frequency is always a multiple of 2, so casting to an integer is safe. */
        g_clock_freq[CGC_CLOCK_PLL] = (uint32_t)
            (((float)(g_clock_freq[CGC_CLOCK_MAIN_OSC]) / (float)divider) * HW_CGC_PLLMultiplierGet());
#endif
    }
#endif
#endif /* ifndef  BSP_MCU_SC32_4 */
}

/*******************************************************************************************************************//**
 * @brief      This function locks CGC registers
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_HardwareLock (void)
{
    R_SYSTEM->PRCR = (uint16_t) ((R_SYSTEM->PRCR | CGC_PRCR_KEY) & (~CGC_PRC0_MASK));
}

/*******************************************************************************************************************//**
 * @brief      This function unlocks CGC registers
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_HardwareUnLock (void)
{
    R_SYSTEM->PRCR = (uint16_t) ((R_SYSTEM->PRCR | CGC_PRCR_KEY) | CGC_PRC0_MASK);
}

/*******************************************************************************************************************//**
 * @brief      This function locks CGC registers
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_LPMHardwareLock (void)
{
    R_SYSTEM->PRCR = (uint16_t) ((R_SYSTEM->PRCR | CGC_PRCR_KEY) & (~CGC_PRC1_MASK));
}

/*******************************************************************************************************************//**
 * @brief      This function unlocks CGC registers
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_LPMHardwareUnLock (void)
{
    R_SYSTEM->PRCR = (uint16_t) ((R_SYSTEM->PRCR | CGC_PRCR_KEY) | CGC_PRC1_MASK);
}


/*******************************************************************************************************************//**
 * @brief      This function sets the main clock drive
 * @param[in]  setting  - clock drive setting
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_MainClockDriveSet (uint8_t setting)
{
    /*  Set the drive capacity for the main clock. */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->MOMCR =
        (uint8_t) ((R_SYSTEM->MOMCR & (~CGC_MODRV_MASK)) | ((setting << CGC_MODRV_SHIFT) & CGC_MODRV_MASK));
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function sets the subclock drive
 * @param[in]  setting  - clock drive setting
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_SubClockDriveSet (uint8_t setting)
{
    /*  Set the drive capacity for the subclock. */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->SOMCR =
        (uint8_t) ((R_SYSTEM->SOMCR & (~CGC_SODRV_MASK)) | ((setting << CGC_SODRV_SHIFT) & CGC_SODRV_MASK));
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function starts the selected clock
 * @param[in]  clock - the clock to start
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_ClockStart (cgc_clock_t clock)
{
    /* Start the selected clock. */
    HW_CGC_HardwareUnLock();

    switch (clock)
    {
        case CGC_CLOCK_HOCO:
            R_SYSTEM->HOCOCR_b.HCSTP = 0; /* Start the HOCO clock. */
            break;

        case CGC_CLOCK_MOCO:
            R_SYSTEM->MOCOCR_b.MCSTP = 0; /* Start the MOCO clock.*/
            break;

        case CGC_CLOCK_LOCO:
            R_SYSTEM->LOCOCR_b.LCSTP = 0; /* Start the LOCO clock.*/
            break;

        case CGC_CLOCK_MAIN_OSC:
            R_SYSTEM->MOSCCR_b.MOSTP = 0; /* Start the Main oscillator.*/
            break;

        case CGC_CLOCK_SUBCLOCK:
            R_SYSTEM->SOSCCR_b.SOSTP = 0; /* Start the subClock.*/
            break;

        case CGC_CLOCK_PLL:
            R_SYSTEM->PLLCR_b.PLLSTP = 0; /* Start the PLL clock.*/
            break;

        default:
            break;
    }

    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function stops the selected clock
 * @param[in]  clock - the clock to stop
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_ClockStop (cgc_clock_t clock)
{
    /*  Stop the selected clock. */
    HW_CGC_HardwareUnLock();
    switch (clock)
    {
        case CGC_CLOCK_HOCO:
            R_SYSTEM->HOCOCR_b.HCSTP = 1; /* Stop the HOCO clock.*/
            break;

        case CGC_CLOCK_MOCO:
            R_SYSTEM->MOCOCR_b.MCSTP = 1; /* Stop the MOCO clock.*/
            break;

        case CGC_CLOCK_LOCO:
            R_SYSTEM->LOCOCR_b.LCSTP = 1; /* Stop the LOCO clock.*/
            break;

        case CGC_CLOCK_MAIN_OSC:
            R_SYSTEM->MOSCCR_b.MOSTP = 1; /* Stop the  main oscillator.*/
            break;

        case CGC_CLOCK_SUBCLOCK:
            R_SYSTEM->SOSCCR_b.SOSTP = 1; /* Stop the subClock.*/
            break;

        case CGC_CLOCK_PLL:
            R_SYSTEM->PLLCR_b.PLLSTP = 1; /* Stop PLL clock.*/
            break;

        default:
            break;
    }

    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function checks the selected clock for stability
 * @param[in]  clock - the clock to check
 * @retval     true if stable, false if not stable or stopped
 **********************************************************************************************************************/

bool HW_CGC_ClockCheck (cgc_clock_t clock)
{
    /*  Check for stability of selected clock. */
    switch (clock)
    {
        case CGC_CLOCK_HOCO:
            if (R_SYSTEM->OSCSF_b.HOCOSF)
            {
                return true; /* HOCO is stable */
            }

            break;

        case CGC_CLOCK_MAIN_OSC:
            if (R_SYSTEM->OSCSF_b.MOSCSF)
            {
                return true; /* Main Osc is stable */
            }

            break;

        case CGC_CLOCK_PLL:
            if (R_SYSTEM->OSCSF_b.PLLSF)
            {
                return true; /* PLL is stable */
            }

            break;

        default:

            break;
    }

    return false;
}

/*******************************************************************************************************************//**
 * @brief      This function returns the run state of the selected clock
 * @param[in]  clock - the clock to check
 * @retval     true if running, false if stopped
 **********************************************************************************************************************/

bool HW_CGC_ClockRunStateGet (cgc_clock_t clock)
{
    /* Get clock run state. */

    switch (clock)
    {
        case CGC_CLOCK_HOCO:
            if (!(R_SYSTEM->HOCOCR_b.HCSTP))
            {
                return true; /* HOCO clock is running */
            }

            break;

        case CGC_CLOCK_MOCO:
            if (!(R_SYSTEM->MOCOCR_b.MCSTP))
            {
                return true; /* MOCO clock is running */
            }

            break;

        case CGC_CLOCK_LOCO:
            if (!(R_SYSTEM->LOCOCR_b.LCSTP))
            {
                return true; /* LOCO clock is running */
            }

            break;

        case CGC_CLOCK_MAIN_OSC:
            if (!(R_SYSTEM->MOSCCR_b.MOSTP))
            {
                return true; /* main osc clock is running */
            }

            break;

        case CGC_CLOCK_SUBCLOCK:
            if (!(R_SYSTEM->SOSCCR_b.SOSTP))
            {
                return true; /* Subclock is running */
            }

            break;

        case CGC_CLOCK_PLL:
            if (!(R_SYSTEM->PLLCR_b.PLLSTP))
            {
                return true; /* PLL clock is running */
            }

            break;

        default:
            return false;
            break;
    }

    return false;
}

/*******************************************************************************************************************//**
 * @brief      This function delays for a specified number of clock cycles, of the selected clock
 * @param[in]  clock - the clock to time
 * @param[in]  cycles - the number of cycles to delay
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_DelayCycles (cgc_clock_t clock, uint16_t cycles)
{
    /* delay for number of clock cycles specified */

    uint32_t               delay_count;
    uint32_t               clock_freq_in;
    uint32_t               system_clock_freq;

    system_clock_freq = HW_CGC_ClockHzGet(CGC_SYSTEM_CLOCKS_ICLK);
    clock_freq_in     = g_clock_freq[clock];
    if (clock_freq_in != (uint32_t)0)             // ensure divide by zero doesn't happen
    {
        delay_count = ((system_clock_freq / clock_freq_in) * cycles);

        while (delay_count > (uint32_t)0)
        {
            delay_count--;
        }
    }
}

/*******************************************************************************************************************//**
 * @brief      This function sets the source of the main oscillator
 * @param[in]  osc - the source of the main clock oscillator
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_MainOscSourceSet (cgc_osc_source_t osc)
{
    /* Set the source to resonator or external osc. */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->MOMCR_b.MOSEL = osc;
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function sets the clock wait time
 * @param[in]  clock is the clock to set the wait time for
 * @param[in]  setting is wait time
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_ClockWaitSet (cgc_clock_t clock, uint8_t setting)
{
    SSP_PARAMETER_NOT_USED(clock);
    /* Set the clock wait time */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->MOSCWTCR_b.MSTS = (uint8_t)(setting & 0x0F);
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function checks that the clock is available in the hardware
 * @param[in]  clock is the clock to check to see if it is valid
 * @retval     true if clock available, false if not
 **********************************************************************************************************************/

bool HW_CGC_ClockSourceValidCheck (cgc_clock_t clock)
{
    /* Check for valid clock source */
    if (CGC_CLOCK_MAX < clock)
    {
        return false;
    }

    return true;
}

/*******************************************************************************************************************//**
 * @brief      This function checks for a valid system clock
 * @param[in]  clock is the clock to check
 * @retval     true if clock available, false if not
 **********************************************************************************************************************/

bool HW_CGC_SystemClockValidCheck (cgc_system_clocks_t clock)
{
    /* Check for valid system clock */
    if (CGC_SYSTEMS_CLOCKS_MAX < clock)
    {
        return false;               // clock not valid
    }

    return true;
}

/*******************************************************************************************************************//**
 * @brief      This function returns the system clock source
 * @retval     the enum of the current system clock
 **********************************************************************************************************************/

cgc_clock_t HW_CGC_ClockSourceGet (void)
{
    /* Get the system clock source */
    return (cgc_clock_t) R_SYSTEM->SCKSCR_b.CKSEL;
}

/*******************************************************************************************************************//**
 * @brief      This function sets the system clock source
 * @param[in]  clock is the clock to use for the system clock
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_ClockSourceSet (cgc_clock_t clock)
{
    /* Set the system clock source */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->SCKSCR_b.CKSEL = clock;    //set the system clock source
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function returns the PLL clock source
 * @retval     the enum of the PLL clock source
 **********************************************************************************************************************/

cgc_clock_t HW_CGC_PLLClockSourceGet (void)
{
    /*  PLL source selection only available on S7G2 */
 #ifdef  BSP_MCU_GROUP_S7G2
     /* Get the PLL clock source */

     if (R_SYSTEM->PLLCCR_b.PLLSRCSEL == (uint16_t)1)
     {
         return CGC_CLOCK_HOCO;
     }

 #endif /* ifdef  BSP_MCU_GROUP_S7G2 */
    return CGC_CLOCK_MAIN_OSC;
}

/*******************************************************************************************************************//**
 * @brief      This function sets the PLL clock source
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_PLLClockSourceSet (cgc_clock_t clock)
{    
    /*  PLL source selection only available on S7G2 */
#ifdef  BSP_MCU_GROUP_S7G2
    /* Set the PLL clock source */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->PLLCCR_b.PLLSRCSEL = (uint16_t)(g_pllccr_src_setting[clock] & 0x01);
    HW_CGC_HardwareLock();
#else
    SSP_PARAMETER_NOT_USED(clock);
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function sets the PLL multiplier
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_PLLMultiplierSet (float multiplier)
{
    /* Set the PLL multiplier */
    SSP_PARAMETER_NOT_USED(multiplier);          /// Prevent compiler 'unused' warning

#ifdef  BSP_MCU_GROUP_S7G2
    uint32_t write_val                  = (uint32_t) (multiplier * 2) - 1;
    HW_CGC_HardwareUnLock();
    R_SYSTEM->PLLCCR_b.PLLMUL  = (uint16_t)(write_val & 0x3F);
    HW_CGC_HardwareLock();
#elif   BSP_MCU_GROUP_S3A7
    uint32_t write_val                  = (uint32_t) multiplier - 1;
    HW_CGC_HardwareUnLock();
    R_SYSTEM->PLLCCR2_b.PLLMUL = (uint8_t)(write_val & 0x1F);
    HW_CGC_HardwareLock();
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function gets the PLL multiplier
 * @retval     float multiplier value
 **********************************************************************************************************************/

float HW_CGC_PLLMultiplierGet (void)
{
    /* Get the PLL multiplier */

#ifdef  BSP_MCU_GROUP_S7G2
    return (((float)(R_SYSTEM->PLLCCR_b.PLLMUL) + (float)1.0) / (float)2.0);
#elif   BSP_MCU_GROUP_S3A7
    /* This cast is used for compatibility with the S7G2 implementation. */
    return (float) R_SYSTEM->PLLCCR2_b.PLLMUL  + 1;
#elif   (BSP_MCU_GROUP_S124 || (BSP_MCU_GROUP_S128))
    return (float)0;            ///< S124 does not have a PLL
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function sets the PLL divider
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_PLLDividerSet (cgc_pll_div_t divider)
{
    SSP_PARAMETER_NOT_USED(divider);          /// Prevent compiler 'unused' warning

    /* Set the PLL divider */
#ifdef  BSP_MCU_GROUP_S7G2
    uint16_t register_value = g_pllccr_div_setting[divider];
    HW_CGC_HardwareUnLock();
    R_SYSTEM->PLLCCR_b.PLIDIV  = (uint16_t)(register_value & 0x3);
    HW_CGC_HardwareLock();
#elif   BSP_MCU_GROUP_S3A7
    uint16_t register_value = g_pllccr_div_setting[divider];
    HW_CGC_HardwareUnLock();
    R_SYSTEM->PLLCCR2_b.PLODIV = (uint8_t)(register_value & 0x3);
    HW_CGC_HardwareLock();
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function gets the PLL divider
 * @retval     divider
 **********************************************************************************************************************/

cgc_pll_div_t HW_CGC_PLLDividerGet (void)
{
    /* Get the PLL divider */
#ifdef  BSP_MCU_GROUP_S7G2
    /* This cast maps the register value to an enumerated list. */
    return (cgc_pll_div_t) R_SYSTEM->PLLCCR_b.PLIDIV;
#elif  BSP_MCU_GROUP_S3A7
    /* This cast maps the register value to an enumerated list. */
    return (cgc_pll_div_t) R_SYSTEM->PLLCCR2_b.PLODIV;
#else
    return (cgc_pll_div_t)1;
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function sets the system dividers
 * @param[in]  cfg is a pointer to a cgc_system_clock_cfg_t struct
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_SystemDividersSet (cgc_system_clock_cfg_t * cfg)
{
    sckdivcr_clone_t sckdivcr;
    /* Set the system dividers */
    HW_CGC_HardwareUnLock();
    /* The cgc_sys_clock_div_t fits in the 3 bits, and each of the elements of sckdivcr_b are 3 bits of a 32-bit value,
     * so these casts are safe. */
    sckdivcr.sckdivcr_w = (uint32_t) 0x00;
#ifndef  BSP_MCU_GROUP_S124
    sckdivcr.sckdivcr_b.pcka = (uint32_t)(cfg->pclka_div & 0x7);
    sckdivcr.sckdivcr_b.pckc = (uint32_t)(cfg->pclkc_div & 0x7);
    sckdivcr.sckdivcr_b.bck  = (uint32_t)(cfg->bclk_div  & 0x7);
    sckdivcr.sckdivcr_b.fck  = (uint32_t)(cfg->fclk_div  & 0x7);
#endif
#ifndef  BSP_MCU_GROUP_S128
    sckdivcr.sckdivcr_b.pcka = (uint32_t)(cfg->pclka_div & 0x7);
    sckdivcr.sckdivcr_b.pckc = (uint32_t)(cfg->pclkc_div & 0x7);
    sckdivcr.sckdivcr_b.bck  = (uint32_t)(cfg->bclk_div  & 0x7);
    sckdivcr.sckdivcr_b.fck  = (uint32_t)(cfg->fclk_div  & 0x7);
#endif

    sckdivcr.sckdivcr_b.pckb = (uint32_t)(cfg->pclkb_div & 0x7);
    sckdivcr.sckdivcr_b.pckd = (uint32_t)(cfg->pclkd_div & 0x7);
    sckdivcr.sckdivcr_b.ick  = (uint32_t)(cfg->iclk_div  & 0x7);
    R_SYSTEM->SCKDIVCR       = sckdivcr.sckdivcr_w;
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function gets the system dividers
 * @param[in]  cfg is a pointer to a cgc_system_clock_cfg_t struct
 * @param[out]  cfg is a pointer to a cgc_system_clock_cfg_t struct
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_SystemDividersGet (cgc_system_clock_cfg_t * cfg)
{
    /* Get the system dividers */
    /* The cgc_sys_clock_div_t defines all valid values (3 bits each) for these registers as per the hardware manual,
     * and each of the elements of SCKDIVCR_b are 3 bits of a 32-bit value, so these casts are safe. */
    cfg->pclka_div = (cgc_sys_clock_div_t) R_SYSTEM->SCKDIVCR_b.PCKA;
    cfg->pclkb_div = (cgc_sys_clock_div_t) R_SYSTEM->SCKDIVCR_b.PCKB;
    cfg->pclkc_div = (cgc_sys_clock_div_t) R_SYSTEM->SCKDIVCR_b.PCKC;
    cfg->pclkd_div = (cgc_sys_clock_div_t) R_SYSTEM->SCKDIVCR_b.PCKD;
    cfg->bclk_div  = (cgc_sys_clock_div_t) R_SYSTEM->SCKDIVCR_b.BCK;
    cfg->fclk_div  = (cgc_sys_clock_div_t) R_SYSTEM->SCKDIVCR_b.FCK;
    cfg->iclk_div  = (cgc_sys_clock_div_t) R_SYSTEM->SCKDIVCR_b.ICK;
}

/*******************************************************************************************************************//**
 * @brief      This function tests the clock configuration for validity
 * @param[in]  cfg is a pointer to a cgc_clock_cfg_t struct
 * @retval     true if configuration is valid
 **********************************************************************************************************************/

bool HW_CGC_ClockCfgValidCheck (cgc_clock_cfg_t * cfg)
{
    /* check for valid configuration */

    /* Check maximum and minimum divider values */
    if (CGC_PLL_DIV_MAX < cfg->divider)
    {
        return false;    // Value is out of range.
    }

    /* Check maximum and minimum multiplier values */
    if ((CGC_PLL_MUL_MAX < cfg->multiplier) || (CGC_PLL_MUL_MIN > cfg->multiplier))
    {
        return false;   // Value is out of range.
    }

    if ((CGC_CLOCK_MAIN_OSC != cfg->source_clock) && (CGC_CLOCK_HOCO != cfg->source_clock))
    {
        return false;   // Value is out of range.
    }

    return true;
}

/*******************************************************************************************************************//**
 * @brief      This function returns the divider of the selected clock
 * @param[in]  clock is the system clock to get the vivider for
 * @retval     divider
 **********************************************************************************************************************/

uint32_t HW_CGC_ClockDividerGet (cgc_system_clocks_t clock)
{
    /*  get divider of selected clock */
    uint32_t divider;
    divider = (uint32_t)0;

    switch (clock)
    {
        case CGC_SYSTEM_CLOCKS_PCLKA:
            divider = R_SYSTEM->SCKDIVCR_b.PCKA;
            break;

        case CGC_SYSTEM_CLOCKS_PCLKB:
            divider = R_SYSTEM->SCKDIVCR_b.PCKB;
            break;

        case CGC_SYSTEM_CLOCKS_PCLKC:
            divider = R_SYSTEM->SCKDIVCR_b.PCKC;
            break;

        case CGC_SYSTEM_CLOCKS_PCLKD:
            divider = R_SYSTEM->SCKDIVCR_b.PCKD;
            break;

        case CGC_SYSTEM_CLOCKS_BCLK:
            divider = R_SYSTEM->SCKDIVCR_b.BCK;
            break;

        case CGC_SYSTEM_CLOCKS_FCLK:
            divider = R_SYSTEM->SCKDIVCR_b.FCK;
            break;

        case CGC_SYSTEM_CLOCKS_ICLK:
            divider = R_SYSTEM->SCKDIVCR_b.ICK;
            break;
        default:
            break;
    }

    return (divider);
}

/*******************************************************************************************************************//**
 * @brief      This function returns the frequency of the selected clock
 * @param[in]  clock is the system clock to get the freq for
 * @retval     frequency
 **********************************************************************************************************************/

uint32_t HW_CGC_ClockHzGet (cgc_system_clocks_t clock)
{
    /*  get frequency of selected clock */
    uint32_t divider;
    divider =  HW_CGC_ClockDividerGet(clock);
    return (uint32_t) ((g_clock_freq[HW_CGC_ClockSourceGet()]) >> divider);
}

/*******************************************************************************************************************//**
 * @brief      This function returns the frequency of the selected clock
 * @param[in]  clock is the system clock, such as iclk or fclk, to get the freq for
 * @param[in]  source_clock is the source clock, such as the main osc or PLL
 * @retval     frequency
 **********************************************************************************************************************/

uint32_t HW_CGC_ClockHzCalculate (cgc_clock_t source_clock,  cgc_sys_clock_div_t divider)
{
    /*  calculate frequency of selected system clock, given the source clock and divider */
    return (uint32_t) ((g_clock_freq[source_clock]) >> (uint32_t)divider);
}

/*******************************************************************************************************************//**
 * @brief      This function sets the value of the MEMWAIT register
 * @param[in]  none
 * @retval     none
 **********************************************************************************************************************/

#ifdef BSP_MCU_GROUP_S3A7
void HW_CGC_MemWaitSet (uint32_t setting)
{
    HW_CGC_HardwareUnLock();
    R_SYSTEM->MEMWAITCR_b.MEMWAIT = (uint8_t)(setting & 0x01);
    HW_CGC_HardwareLock();
}
#endif

/*******************************************************************************************************************//**
 * @brief      This function gets the value of the MEMWAIT register
 * @param[in]  none
 * @retval     MEMWAIT setting
 **********************************************************************************************************************/

#ifdef BSP_MCU_GROUP_S3A7
uint32_t HW_CGC_MemWaitGet (void)
{
    return (R_SYSTEM->MEMWAITCR_b.MEMWAIT);
}
#endif


/*******************************************************************************************************************//**
 * @brief      This function places the S3A7/S124/S128 MCU in High Speed Mode
 * @param[in]  none
 * @retval     None
 **********************************************************************************************************************/

#if defined(BSP_MCU_GROUP_S3A7) || defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128)
void HW_CGC_SetHighSpeedMode (void)
{
    bsp_cache_state_t cache_state;

    /** Enable writing to OPCCR register. */
    HW_CGC_LPMHardwareUnLock();

    cache_state = BSP_CACHE_STATE_OFF;
    R_BSP_CacheOff(&cache_state);                           // Turn the cache off.

    /** Set to High-Speed Mode. */
    R_SYSTEM->OPCCR = CGC_HIGH_SPEED_MODE;

    while (0 != R_SYSTEM->OPCCR_b.OPCMTSF)
    {
        /** Wait for transition to complete. */
    }

    R_BSP_CacheSet(cache_state);                            // Restore cache to previous state.

    /** Disable writing to OPCCR register. */
    HW_CGC_LPMHardwareLock();
}
#endif

/*******************************************************************************************************************//**
 * @brief      This function places the S3A7 MCU in Middle Speed Mode
 * @param[in]  none
 * @retval     None
 **********************************************************************************************************************/

#ifdef BSP_MCU_GROUP_S3A7
void HW_CGC_SetMiddleSpeedMode (void)
{
    bsp_cache_state_t cache_state;

    /** Enable writing to OPCCR register. */
    HW_CGC_LPMHardwareUnLock();

    cache_state = BSP_CACHE_STATE_OFF;
    R_BSP_CacheOff(&cache_state);                           // Turn the cache off.

    /** Set to Middle-Speed Mode. */
    R_SYSTEM->OPCCR = CGC_MIDDLE_SPEED_MODE;

    while (0 != R_SYSTEM->OPCCR_b.OPCMTSF)
    {
        /** Wait for transition to complete. */
    }

    R_BSP_CacheSet(cache_state);                            // Restore cache to previous state.

    /** Disable writing to OPCCR register. */
    HW_CGC_LPMHardwareLock();
}
#endif


/*******************************************************************************************************************//**
 * @brief      This function checks the MCU for High Speed Mode
 * @param[in]  none
 * @retval     true if MCU is in High Speed Mode
 **********************************************************************************************************************/

bool HW_CGC_HighSpeedMode (void)
{
    return (0 == R_SYSTEM->OPCCR_b.OPCM);
}

/*******************************************************************************************************************//**
 * @brief      This function enables the osc stop detection function and interrupt
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_OscStopDetectEnable (void)
{
    /* enable hardware oscillator stop detect function */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->OSTDCR_b.OSTDE  = 1;        // enable osc stop detection
    R_SYSTEM->OSTDCR_b.OSTDIE = 1;        // enable osc stop detection interrupt
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function disables the osc stop detection function and interrupt
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_OscStopDetectDisable (void)
{
    /* disable hardware oscillator stop detect function */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->OSTDCR_b.OSTDIE = 0;        // disable osc stop detection interrupt
    R_SYSTEM->OSTDCR_b.OSTDE  = 0;        // disable osc stop detection
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function returns the status of the stop detection function
 * @retval     true if detected, false if not detected
 **********************************************************************************************************************/
bool HW_CGC_OscStopDetectEnabledGet (void)
{
    if (R_SYSTEM->OSTDCR_b.OSTDE == 1)          // is stop detection enabled?
    {
        return true;                       // Function enabled.
    }
    return false;                          // Function not enabled.

}
/*******************************************************************************************************************//**
 * @brief      This function returns the status of the stop detection function
 * @retval     true if detected, false if not detected
 **********************************************************************************************************************/

bool HW_CGC_OscStopDetectGet (void)
{
    /* oscillator stop detected */
    if (R_SYSTEM->OSTDSR_b.OSTDF == 1)
    {
        return true;            // stop detected
    }

    return false;               // no stop detected
}

/*******************************************************************************************************************//**
 * @brief      This function clear the status of the stop detection function
 * @retval     none
 **********************************************************************************************************************/

bool HW_CGC_OscStopStatusClear (void)
{
    /* clear hardware oscillator stop detect status */
    if (R_SYSTEM->OSTDSR_b.OSTDF == 1)
    {
        HW_CGC_HardwareUnLock();
        R_SYSTEM->OSTDSR_b.OSTDF = 0;
        HW_CGC_HardwareLock();
        return true;            // stop detection cleared
    }

    return false;               // can't clear flag because no stop detected
}

/*******************************************************************************************************************//**
 * @brief      This function configures the BusClock Out divider
 * @param[in]  divider is the bus clock out divider
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_BusClockOutCfg (cgc_bclockout_dividers_t divider)
{
    /*  */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->BCKCR_b.BCLKDIV = divider;      // set external bus clock output divider
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function enables BusClockOut
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_BusClockOutEnable (void)
{
    /*  */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->EBCKOCR_b.EBCKOEN = 1;           // enable bus clock output
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function disables BusClockOut
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_BusClockOutDisable (void)
{
    /*  */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->EBCKOCR_b.EBCKOEN = 0;           // disable bus clock output (fixed high level)
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function configures the ClockOut divider and clock source
 * @param[in]  clock is the clock out source
 * @param[in]  divider is the clock out divider
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_ClockOutCfg (cgc_clock_t clock, cgc_clockout_dividers_t divider)
{
    /*  */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->CKOCR_b.CKOEN  = 0;             // disable CLKOUT to change configuration
    R_SYSTEM->CKOCR_b.CKODIV = divider;       // set CLKOUT dividers
    R_SYSTEM->CKOCR_b.CKOSEL = clock;         // set CLKOUT clock source
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function enables ClockOut
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_ClockOutEnable (void)
{
    /* Enable CLKOUT output  */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->CKOCR_b.CKOEN = 1;
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function disables ClockOut
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_ClockOutDisable (void)
{
    /* Disable CLKOUT output */
    HW_CGC_HardwareUnLock();
    R_SYSTEM->CKOCR_b.CKOEN = 0;
    HW_CGC_HardwareLock();
}

/*******************************************************************************************************************//**
 * @brief      This function enables SDRAM ClockOut
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_SDRAMClockOutEnable (void)
{
    /* SD Ram clock only for S7G2 */
#ifdef  BSP_MCU_GROUP_S7G2
    HW_CGC_HardwareUnLock();
    R_SYSTEM->SDCKOCR_b.SDCKOEN = 1;          // enable SDRAM output
    HW_CGC_HardwareLock();
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function disables SDRAM ClockOut
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_SDRAMClockOutDisable (void)
{
    /* SD Ram clock only for S7G2 */
#ifdef  BSP_MCU_GROUP_S7G2
    HW_CGC_HardwareUnLock();
    R_SYSTEM->SDCKOCR_b.SDCKOEN = 0;          // disable SDRAM output (fixed high level)
    HW_CGC_HardwareLock();
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function configures the USB clock divider
 * @param[in]  divider is the USB clock divider
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_USBClockCfg (cgc_usb_clock_div_t divider)
{    
    /*  USB configuration only for S7G2 */
#ifdef  BSP_MCU_GROUP_S7G2
    HW_CGC_HardwareUnLock();
    R_SYSTEM->SCKDIVCR2_b.UCK = divider;      // set USB divider
    HW_CGC_HardwareLock();
#else
    SSP_PARAMETER_NOT_USED(divider);
#endif
}

/*******************************************************************************************************************//**
 * @brief      This updates the Systick timer period
 * @param[in]  uint32_t reload_value - Reload value, calculated by caller
 * @retval     none
 **********************************************************************************************************************/
bool HW_CGC_SystickUpdate(uint32_t reload_value)
{
    bool result = false;
    uint32_t systick_ctrl = SysTick->CTRL;

    /** If there is an RTOS in place AND the Systick interrupt is not yet enabled, just return and do nothing */
#if (1 == BSP_CFG_RTOS)
    if ((SysTick->CTRL & SysTick_CTRL_TICKINT_Msk) == 0)
    {
        return(true);           ///< Not really an error.
    }
#endif
    SysTick->CTRL = 0;          ///< Disable systick while we reset the counter

    // Number of ticks between two interrupts.
    if ((uint32_t)0 == SysTick_Config(reload_value))
    {
        result = true;
    }
    else
    {
        SysTick->CTRL = systick_ctrl;          ///< If we were provided an invalid (ie too large) reload value,
                                               ///< keep using prior value.
    }
    return(result);
}


/*******************************************************************************************************************//**
 * @brief      This function configures the Segment LCD clock
 * @param[in]  clock is the LCD clock source
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_LCDClockCfg (cgc_clock_t clock)
{    
    /*  LCD clock not available for S7G2 */
#ifndef  BSP_MCU_GROUP_S7G2
    R_SYSTEM->SLCDSCKCR_b.LCDSCKSEL = clock;
#else
    SSP_PARAMETER_NOT_USED(clock);
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function enables LCD Clock Out
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_LCDClockEnable (void)
{
    /*  LCD clock not available for S7G2 */
#ifndef  BSP_MCU_GROUP_S7G2
    R_SYSTEM->SLCDSCKCR_b.LCDSCKEN = 1;
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function disables LCD Clock Out
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_LCDClockDisable (void)
{
    /*  LCD clock not available for S7G2 */
#ifndef  BSP_MCU_GROUP_S7G2
    R_SYSTEM->SLCDSCKCR_b.LCDSCKEN = 0;
#endif
}

/*******************************************************************************************************************//**
 * @brief      This function locks the SRAM Protection Register
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_SRAM_ProtectLock (void)
{
    R_SRAM->RAMPRCR = (uint8_t) ((R_SRAM->RAMPRCR | CGC_SRAM_PRCR_KEY) & (~CGC_SRAM_PRCR_MASK));
}

/*******************************************************************************************************************//**
 * @brief      This function unlocks the SRAM Protection Register
 * @retval     none
 **********************************************************************************************************************/

void HW_CGC_SRAM_ProtectUnLock (void)
{
    R_SRAM->RAMPRCR = (uint16_t) ((R_SRAM->RAMPRCR | CGC_SRAM_PRCR_KEY) | CGC_SRAM_PRCR_MASK);
}


/*******************************************************************************************************************//**
 * @brief      This function sets the RAM wait state settings for both the SRAM0 and SRAM1 RAM memory
 * @param[in]  The number of wait states to be used.
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_SRAM_RAMWaitSet (uint32_t setting)
{
    HW_CGC_SRAM_ProtectLock();
    R_SRAM->RAMWTSC_b.SRAM0WTEN = (uint8_t)(setting & 0x01);
    R_SRAM->RAMWTSC_b.SRAM1WTEN = (uint8_t)(setting & 0x01);
    HW_CGC_SRAM_ProtectUnLock();
}

#ifdef  BSP_MCU_GROUP_S7G2
/*******************************************************************************************************************//**
 * @brief      This function sets the RAM wait state settings for High Speed RAM memory
 * @param[in]  The number of wait states to be used.
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_SRAM_HSRAMWaitSet (uint32_t setting)
{
    HW_CGC_SRAM_ProtectUnLock();
    R_SRAM->RAMWTSC_b.SRAMHSWTEN = (uint8_t)(setting & 0x01);
    HW_CGC_SRAM_ProtectLock();
}
#endif


/*******************************************************************************************************************//**
 * @brief      This function sets the value of the ROMWT register which is used to specify wait states required
 *             when accessing Flash ROM.
 * @param[in]  The number of wait states to be used.
 * @retval     none
 **********************************************************************************************************************/
void HW_CGC_ROMWaitSet (uint32_t setting)
{
    R_ROMC->ROMWT_b.ROMWT = (uint8_t)(setting & 0x07);
}


/*******************************************************************************************************************//**
 * @brief      This function gets the value of the ROMWT register
 * @param[in]  none
 * @retval     MEMWAIT setting
 **********************************************************************************************************************/
uint32_t HW_CGC_ROMWaitGet (void)
{
    return (R_ROMC->ROMWT_b.ROMWT);
}

