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
 * File Name    : r_cgc.c
 * Description  : HAL driver for the Clock Generation circuit.
 **********************************************************************************************************************/


/***********************************************************************************************************************
* Includes
 **********************************************************************************************************************/
#include "r_cgc.h"
#include "r_cgc_private.h"
#include "r_cgc_private_api.h"
#include "./hw/hw_cgc_private.h"
/* Configuration for this package. */
#include "r_cgc_cfg.h"
#include "./hw/hw_cgc.h"

#if (1 == BSP_CFG_RTOS)
#include "tx_api.h"
#endif
/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Macro for error logger. */
#ifndef CGC_ERROR_RETURN
#define CGC_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_cgc_version)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t g_cgc_version =
{
    .api_version_minor  = CGC_API_VERSION_MINOR,
    .api_version_major  = CGC_API_VERSION_MAJOR,
    .code_version_major = CGC_CODE_VERSION_MAJOR,
    .code_version_minor = CGC_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char g_module_name[] = "cgc";
#endif

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
const cgc_api_t g_cgc_on_cgc =
{
    .init                 = R_CGC_Init,
    .clockStart           = R_CGC_ClockStart,
    .clockStop            = R_CGC_ClockStop,
    .systemClockSet       = R_CGC_SystemClockSet,
    .systemClockGet       = R_CGC_SystemClockGet,
    .systemClockFreqGet   = R_CGC_SystemClockFreqGet,
    .clockCheck           = R_CGC_ClockCheck,
    .oscStopDetect        = R_CGC_OscStopDetect,
    .oscStopStatusClear   = R_CGC_OscStopStatusClear,
    .busClockOutCfg       = R_CGC_BusClockOutCfg,
    .busClockOutEnable    = R_CGC_BusClockOutEnable,
    .busClockOutDisable   = R_CGC_BusClockOutDisable,
    .clockOutCfg          = R_CGC_ClockOutCfg,
    .clockOutEnable       = R_CGC_ClockOutEnable,
    .clockOutDisable      = R_CGC_ClockOutDisable,
    .lcdClockCfg          = R_CGC_LCDClockCfg,
    .lcdClockEnable       = R_CGC_LCDClockEnable,
    .lcdClockDisable      = R_CGC_LCDClockDisable,
    .sdramClockOutEnable  = R_CGC_SDRAMClockOutEnable,
    .sdramClockOutDisable = R_CGC_SDRAMClockOutDisable,
    .usbClockCfg          = R_CGC_USBClockCfg,
    .systickUpdate        = R_CGC_SystickUpdate,
    .versionGet           = R_CGC_VersionGet
};

/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @addtogroup CGC
 * @brief Clock Generation Circuit API
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Initialize the CGC API.
 *
 *                Configures the following for the clock generator module
 *                - SubClock drive capacity (Compile time configurable: CGC_CFG_SUBCLOCK_DRIVE)
 *                - Initial setting for the SubClock
 *
 *                THIS FUNCTION MUST BE EXECUTED ONCE AT STARTUP BEFORE ANY OF THE OTHER CGC FUNCTIONS
 *                CAN BE USED OR THE CLOCK SOURCE IS CHANGED FROM THE MOCO.
 * @retval SSP_SUCCESS                  Clock initialized successfully.
 * @retval SSP_ERR_HARDWARE_TIMEOUT     Hardware timed out.
 **********************************************************************************************************************/
ssp_err_t R_CGC_Init (void)
{
    volatile uint32_t timeout;
    timeout = MAX_REGISTER_WAIT_COUNT;
    HW_CGC_Init();                              // initialize hardware functions
    HW_CGC_ClockStop(CGC_CLOCK_SUBCLOCK);       // stop SubClock
    while ((HW_CGC_ClockRunStateGet(CGC_CLOCK_SUBCLOCK)) && (0 != timeout))
    {
        /* wait until the clock state is stopped */
        timeout--;                                          // but timeout, if it never happens
    }

    CGC_ERROR_RETURN(timeout, SSP_ERR_HARDWARE_TIMEOUT);    // return error if timed out

    HW_CGC_DelayCycles(CGC_CLOCK_SUBCLOCK, SUBCLOCK_DELAY); // Delay for 5 SubClock cycles.
    HW_CGC_SubClockDriveSet(CGC_CFG_SUBCLOCK_DRIVE);        // set the SubClock drive according to the configuration
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Start the specified clock if it is not currently active.
 *
 *                Configures the following when starting the Main Clock Oscillator:
 *                - MainClock drive capacity (Configured based on external clock frequency)
 *                - MainClock stabilization wait time (Compile time configurable: CGC_CFG_MAIN_OSC_WAIT)
 *
 * @retval SSP_SUCCESS                  Clock initialized successfully.
 * @retval SSP_ERR_ILL_PARAM            Invalid argument used.
 * @retval SSP_ERR_MAIN_OCO_INACTIVE    PLL Initialization attempted with Main OCO turned off/unstable.
 * @retval SSP_ERR_CLOCK_ACTIVE         Active clock source specified for modification. This applies specifically to the
 *                                      PLL dividers/multipliers which cannot be modified if the PLL is active. It has
 *                                      to be stopped first before modification.
 * @retval SSP_ERR_NOT_STABILIZED       The Clock source is not stabilized after being turned off.
 * @retval SSP_ERR_CLKOUT_EXCEEDED      The main oscillator can be only 8 or 16 MHz.
 * @retval SSP_ERR_NULL_PTR             A NULL is passed for configuration data when PLL is the clock_source.
 * argument
 *
 **********************************************************************************************************************/

ssp_err_t R_CGC_ClockStart (cgc_clock_t clock_source, cgc_clock_cfg_t * p_clock_cfg)
{
#if (defined(BSP_MCU_GROUP_S3A7) || defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128))
    uint32_t requested_frequency;
#endif

    /* return error if invalid clock source or not supported by hardware */
    CGC_ERROR_RETURN((HW_CGC_ClockSourceValidCheck(clock_source)), SSP_ERR_INVALID_ARGUMENT);
    if (CGC_CLOCK_PLL == clock_source)                          /* It could be that PLL is already running but speed
                                                                 * info was overwritten */
    {
        if (HW_CGC_ClockRunStateGet(clock_source))
        {
            HW_CGC_InitPLLFreq();                                /* calculate  PLL clock frequency and save it */
            CGC_ERROR_RETURN(0, SSP_ERR_CLOCK_ACTIVE);
        }
    }
    /* return error if clock is already active */
    else
    {
        CGC_ERROR_RETURN(!(HW_CGC_ClockRunStateGet(clock_source)), SSP_ERR_CLOCK_ACTIVE);
    }


#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
    if (clock_source == CGC_CLOCK_PLL)            // is clock source PLL?
    {
        SSP_ASSERT(NULL != p_clock_cfg);          // return error if NULL pointer to configuration

        /* if current clock is PLL, divider/multiplier it cannot be modified */
        CGC_ERROR_RETURN((CGC_CLOCK_PLL != HW_CGC_ClockSourceGet()), SSP_ERR_CLOCK_ACTIVE);

        /* return error if configuration contains illegal parameters */
        CGC_ERROR_RETURN((HW_CGC_ClockCfgValidCheck(p_clock_cfg)), SSP_ERR_INVALID_ARGUMENT);
    }
#endif /* if (CGC_CFG_PARAM_CHECKING_ENABLE == 1) */

    /*  if clock source is LOCO, MOCO or SUBCLOCK */
    if (((clock_source == CGC_CLOCK_LOCO) || (clock_source == CGC_CLOCK_MOCO)) || (clock_source == CGC_CLOCK_SUBCLOCK))
    {
        HW_CGC_ClockStart(clock_source);       // start the clock
        /* if clock source is subclock */
        if (clock_source == CGC_CLOCK_SUBCLOCK)
        {
            volatile uint32_t timeout;
            timeout = MAX_REGISTER_WAIT_COUNT;
            while ((!(HW_CGC_ClockRunStateGet(CGC_CLOCK_SUBCLOCK))) && (0 != timeout))
            {
                /* wait until the clock state is started */
                timeout--;                          // but timeout if it never happens
            }

            CGC_ERROR_RETURN(timeout, SSP_ERR_HARDWARE_TIMEOUT);    // return error if timed out
        }
    }
    else if ((clock_source == CGC_CLOCK_HOCO) || (clock_source == CGC_CLOCK_MAIN_OSC))
    {
        /* make sure the oscillator has stopped before starting again */
        CGC_ERROR_RETURN(!(HW_CGC_ClockCheck(clock_source)), SSP_ERR_NOT_STABILIZED);

        if (CGC_CLOCK_MAIN_OSC == clock_source )
        {
            HW_CGC_MainClockDriveSet(CGC_MAINCLOCK_DRIVE);          /* set the Main Clock drive according to
                                                                     * the configuration */
            HW_CGC_MainOscSourceSet((cgc_osc_source_t) CGC_CFG_MAIN_OSC_CLOCK_SOURCE); /* set the main osc source
                                                                                        * to resonator or
                                                                                        * external osc. */
            HW_CGC_ClockWaitSet(CGC_CLOCK_MAIN_OSC, CGC_CFG_MAIN_OSC_WAIT);            /* set the main osc wait time */
        }

#if (defined(BSP_MCU_GROUP_S3A7) || defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128))
        /** Get the requested Iclk Frequency. */
        requested_frequency = HW_CGC_ClockHzCalculate(clock_source, BSP_CFG_ICK_DIV);
#endif


#if defined(BSP_MCU_GROUP_S3A7)
        /** See if we need to switch to High Speed mode before starting the HOCO. */
        if (requested_frequency > CGC_MAX_ZERO_WAIT_FREQ)
        {
            HW_CGC_SetHighSpeedMode();
        }
#endif
#if  (defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128))
        /** See if we need to switch to High Speed mode before starting the HOCO. */
        if (requested_frequency > CGC_S124_LOW_V_MODE_FREQ)
        {
            HW_CGC_SetHighSpeedMode();
        }
#endif

        HW_CGC_ClockStart(clock_source);       // start the clock
    }

    /*  if clock source is PLL */
    else if (clock_source == CGC_CLOCK_PLL)
    {
        /* if the PLL source clock isn't running, PLL cannot be turned on, return error */
        CGC_ERROR_RETURN(HW_CGC_ClockRunStateGet(p_clock_cfg->source_clock), SSP_ERR_MAIN_OSC_INACTIVE);
        /*  make sure the PLL has stopped before starting again */
        CGC_ERROR_RETURN(!(HW_CGC_ClockCheck(clock_source)), SSP_ERR_NOT_STABILIZED)

#ifdef  BSP_MCU_GROUP_S7G2
        HW_CGC_PLLClockSourceSet(p_clock_cfg->source_clock); // configure PLL source clock for S7G2
#endif
        HW_CGC_PLLDividerSet(p_clock_cfg->divider);          // configure PLL divider
        HW_CGC_PLLMultiplierSet(p_clock_cfg->multiplier);    // configure PLL multiplier
        HW_CGC_InitPLLFreq();                                // calculate  PLL clock frequency

#ifdef BSP_MCU_GROUP_S3A7
        /** See if we need to switch to High Speed mode before starting the PLL. */
        requested_frequency = HW_CGC_ClockHzCalculate(clock_source, BSP_CFG_ICK_DIV);
        if (requested_frequency > CGC_MAX_MIDDLE_SPEED_FREQ)
        {
            HW_CGC_SetHighSpeedMode();
        }
        else
        {
            HW_CGC_SetMiddleSpeedMode();    // PLL will only run in High or Middle Speed modes.
        }
#endif
        HW_CGC_ClockStart(clock_source);                     // start the clock
    }

    else
    {
        /* statement here to follow coding standard */
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Stop the specified clock if it is active and not configured as the system clock.
 * @retval SSP_SUCCESS              Clock stopped successfully.
 * @retval SSP_ERR_CLOCK_ACTIVE     Current System clock source specified for stopping. This is not allowed.
 * @retval SSP_ERR_OSC_DET_ENABLED  MOCO cannot be stopped if Oscillation stop detection is enabled.
 * @retval SSP_ERR_NOT_STABILIZED   Clock not stabilized after starting. A finite stabilization time after starting the
 *                                  clock has to elapse before it can be stopped.
 * @retval SSP_ERR_INVALID_ARGUMENT Invalid argument used.
 **********************************************************************************************************************/

ssp_err_t R_CGC_ClockStop (cgc_clock_t clock_source)
{
    cgc_clock_t current_clock;

    /*  return error if invalid clock source or not supported by hardware */
    CGC_ERROR_RETURN(HW_CGC_ClockSourceValidCheck(clock_source), SSP_ERR_INVALID_ARGUMENT)

    current_clock = HW_CGC_ClockSourceGet();     // The currently active system clock source cannot be stopped

    /* if clock source is the current system clock, return error */
    CGC_ERROR_RETURN((clock_source != current_clock), SSP_ERR_CLOCK_ACTIVE)

    /* if PLL is the current clock and its source is the same as clock_source to stop, return an error */
#ifdef BSP_MCU_GROUP_S7G2
    if ((CGC_CLOCK_PLL == current_clock) && (HW_CGC_PLLClockSourceGet() == clock_source))
    {
        CGC_ERROR_RETURN(0, SSP_ERR_CLOCK_ACTIVE)
    }

#else
#ifdef BSP_MCU_GROUP_S3A7
    if ((CGC_CLOCK_PLL == current_clock) && (CGC_CLOCK_MAIN_OSC == clock_source))
    {
        CGC_ERROR_RETURN(0, SSP_ERR_CLOCK_ACTIVE)
    }
#endif
#endif /* ifdef BSP_MCU_GROUP_S7G2 */

    /* MOCO cannot be stopped if OSC Stop Detect is enabled */
    CGC_ERROR_RETURN(!((clock_source == CGC_CLOCK_MOCO) && ((HW_CGC_OscStopDetectEnabledGet()))), SSP_ERR_OSC_STOP_DET_ENABLED)

    if (!HW_CGC_ClockRunStateGet(clock_source))
    {
        return SSP_SUCCESS;    // if clock is already inactive, return success
    }

    /*  if clock source is LOCO, MOCO or SUBCLOCK */
    if (((clock_source == CGC_CLOCK_LOCO) || (clock_source == CGC_CLOCK_MOCO)) || (clock_source == CGC_CLOCK_SUBCLOCK))
    {
        HW_CGC_ClockStop(clock_source);       // stop the clock

        if (clock_source == CGC_CLOCK_SUBCLOCK)
        {
            volatile uint32_t timeout;
            timeout = MAX_REGISTER_WAIT_COUNT;
            while ((HW_CGC_ClockRunStateGet(CGC_CLOCK_SUBCLOCK)) && (0 != timeout))
            {
                /* wait until the clock state is stopped */
                timeout--;                          // but timeout if it never happens
            }

            CGC_ERROR_RETURN(timeout, SSP_ERR_HARDWARE_TIMEOUT);    // return error if timed out
        }
    }
    else if (((clock_source == CGC_CLOCK_HOCO) || (clock_source == CGC_CLOCK_MAIN_OSC))
             || (clock_source == CGC_CLOCK_PLL))
    {
        /*  make sure the oscillator is stable before stopping */
        CGC_ERROR_RETURN(HW_CGC_ClockCheck(clock_source), SSP_ERR_NOT_STABILIZED);

        HW_CGC_ClockStop(clock_source);         // stop the clock
    }
    else
    {
        /* statement here to follow coding standard */
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set the specified clock as the system clock and configure the internal dividers for
 *              ICLK, PCLKA, PCLKB, PCLKC, PCLKD and FCLK.
 *
 *              THIS FUNCTION DOES NOT CHECK TO SEE IF THE OPERATING MODE SUPPORTS THE SPECIFIED CLOCK SOURCE
 *              AND DIVIDER VALUES. SETTING A CLOCK SOURCE AND DVIDER OUTSIDE THE RANGE SUPPORTED BY THE
 *              CURRENT OPERATING MODE WILL RESULT IN UNDEFINED OPERATION.
 *
 *              IF THE LOCO MOCO OR SUBCLOCK ARE CHOSEN AS THE SYSTEM CLOCK, THIS FUNCTION WILL SET THOSE AS THE
 *              SYSTEM CLOCK WITHOUT CHECKING FOR STABILIZATION. IT IS UP TO THE USER TO ENSURE THAT LOCO, MOCO
 *              OR SUBCLOCK ARE STABLE BEFORE USING THEM AS THE SYSTEM CLOCK.
 *
 *              Additionally this function sets the RAM and ROM wait states for the MCU.
 *              For the S7 MCU the ROMWT register controls ROM wait states.
 *              For the S3 MCU the MEMWAIT register controls ROM wait states.
 *
 * @retval SSP_SUCCESS                  Operation performed successfully.
 * @retval SSP_ERR_CLOCK_INACTIVE       The specified clock source is inactive.
 * @retval SSP_ERR_NULL_PTR             The p_clock_cfg parameter is NULL.
 * @retval SSP_ERR_STABILIZED           The clock source has not stabilized
 * @retval SSP_ERR_INVALID_ARGUMENT     Invalid argument used. ICLK is not set as the fastest clock.
 **********************************************************************************************************************/

ssp_err_t R_CGC_SystemClockSet (cgc_clock_t clock_source, cgc_system_clock_cfg_t * p_clock_cfg)
{
    cgc_clock_t current_clock;
    uint32_t requested_frequency = 0;
    cgc_system_clock_cfg_t  current_clock_cfg;

    SSP_PARAMETER_NOT_USED(requested_frequency);          /// Prevent compiler 'unused' warning for S124

    /* return error if invalid clock source or not supported by hardware */
    CGC_ERROR_RETURN((HW_CGC_ClockSourceValidCheck(clock_source)), SSP_ERR_INVALID_ARGUMENT);

#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
    SSP_ASSERT(NULL != p_clock_cfg);

#if defined (BSP_MCU_GROUP_S124) || defined (BSP_MCU_GROUP_S128)

    if (p_clock_cfg->pclkb_div < p_clock_cfg->iclk_div)
    {
        CGC_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);      // error if ratios are not correct
    }
#else
    if (((p_clock_cfg->fclk_div < p_clock_cfg->iclk_div) || (p_clock_cfg->pclkb_div < p_clock_cfg->iclk_div))
        || ((p_clock_cfg->pclka_div < p_clock_cfg->iclk_div) || (p_clock_cfg->bclk_div < p_clock_cfg->iclk_div)))
    {
        CGC_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);      // error if ratios are not correct
    }
#endif

#endif /* CGC_CFG_PARAM_CHECKING_ENABLE */

    /** In order to correctly set the ROM and RAM wait state registers we need to know the current (S3A7 only) and
     * requested iclk frequencies.
     */
#if defined (BSP_MCU_GROUP_S3A7) || defined (BSP_MCU_GROUP_S7G2)
    uint32_t current_frequency;
    current_frequency   =
        HW_CGC_ClockHzGet(CGC_SYSTEM_CLOCKS_ICLK);
#endif
    requested_frequency = HW_CGC_ClockHzCalculate(clock_source, p_clock_cfg->iclk_div);



#ifdef BSP_MCU_GROUP_S3A7
    /* If the requested frequency is greater than 32 MHz, the current frequency must be less than 32 MHz and the
     * mcu must be in high speed mode, before changing wait cycles to 2.
     * In addition the RAM wait state register needs to reflect the required number of wait states based on the iclk
     * for low speed RAM (SRAM0 and SRAM1)
     */
    if (requested_frequency > CGC_MAX_ZERO_WAIT_FREQ)
    {
        HW_CGC_SRAM_RAMWaitSet(CGC_SRAM_ONE_WAIT_CYCLES);       ///< One RAM wait cycle
        /* Must currently be running at 32 MHz, or less, and in High Speed Mode to switch to two wait cycles. */
        if (((current_frequency <= CGC_MAX_ZERO_WAIT_FREQ) &&
             HW_CGC_HighSpeedMode()))
        {
             HW_CGC_MemWaitSet(CGC_TWO_WAIT_CYCLES);             // change wait cycles if frequency not above 32 MHz and in
                                                                 // high speed mode
        }
        else if (HW_CGC_MemWaitGet() != CGC_TWO_WAIT_CYCLES)
        {
            CGC_ERROR_RETURN(0, SSP_ERR_INVALID_MODE);           // else, error if not in 2 wait cycles already
        }
        else
        {
            /* following coding rules */
        }
    }
    current_clock = HW_CGC_ClockSourceGet();
    if (clock_source != current_clock)    // if clock_source is not the current system clock, check stabilization
    {
        if (((clock_source == CGC_CLOCK_HOCO) || (clock_source == CGC_CLOCK_MAIN_OSC))
            || (clock_source == CGC_CLOCK_PLL))
        {
            /* make sure the oscillator is stable before setting as system clock */
            CGC_ERROR_RETURN(HW_CGC_ClockCheck(clock_source), SSP_ERR_STABILIZED);
        }
    }
    /* In order to avoid a system clock (momentarily) higher than expected, the order of switching the clock and
     * dividers must be so that the frequency of the clock goes lower, instead of higher, before being correct.
     */

    HW_CGC_SystemDividersGet(&current_clock_cfg);
    /* If the current ICLK divider is less (higher frequency) than the requested ICLK divider,
     *  set the divider first.
     */
    if (current_clock_cfg.iclk_div < p_clock_cfg->iclk_div )
    {
        HW_CGC_SystemDividersSet(p_clock_cfg);
        HW_CGC_ClockSourceSet(clock_source);
    }
    /* The current ICLK divider is greater (lower frequency) than the requested ICLK divider, so
     * set the clock source first.
     */
    else
    {
        HW_CGC_ClockSourceSet(clock_source);
        HW_CGC_SystemDividersSet(p_clock_cfg);
    }

        /* Clock is now at requested frequency. */



    /* Update the CMSIS core clock variable so that it reflects the new Iclk freq */
    SystemCoreClock = bsp_cpu_clock_get();
    /** The current frequency must be less than 32 MHz before changing wait cycles to 0. */
    current_frequency = HW_CGC_ClockHzGet(CGC_SYSTEM_CLOCKS_ICLK);

    if (current_frequency <= CGC_MAX_ZERO_WAIT_FREQ)        ///< If frequency <= 32 MHz
    {
        HW_CGC_MemWaitSet(CGC_ZERO_WAIT_CYCLES);            ///< No MEMWAIT cycles.
        HW_CGC_SRAM_RAMWaitSet(CGC_SRAM_ZERO_WAIT_CYCLES);  ///< No RAM wait cycles.
    }


#else
#ifdef    BSP_MCU_GROUP_S7G2

    uint32_t  new_rom_wait_state = 0;

    /** Wait states for low speed RAM (SRAM0 and SRAM1) */
    /** No wait: ICLK <= 120 MHz */
    /** 1 wait:  ICLK > 120 MHz */
    if (requested_frequency > CGC_SYS_CLOCK_FREQ_NO_RAM_WAITS)
    {
          HW_CGC_SRAM_RAMWaitSet((uint32_t)CGC_SRAM_ONE_WAIT_CYCLES);
    }
    /** Wait states for High speed RAM (SRAMHS) */
    /** No wait: ICLK <= 200 MHz */
    /** 1 wait:  ICLK > 200 MHz */
    if (requested_frequency > CGC_SYS_CLOCK_FREQ_NO_HSRAM_WAITS)
    {
      HW_CGC_SRAM_HSRAMWaitSet((uint32_t)CGC_SRAM_ONE_WAIT_CYCLES);
    }

    /** Calculate the Wait states for ROM */
    if (requested_frequency < CGC_SYS_CLOCK_FREQ_ONE_ROM_WAITS)
    {
       new_rom_wait_state = (uint32_t)0;
    }
    else
    if (requested_frequency < CGC_SYS_CLOCK_FREQ_TWO_ROM_WAITS)
    {
      new_rom_wait_state = (uint32_t)1;
    }
    else
    {
       new_rom_wait_state = (uint32_t)2;
    }

    if (new_rom_wait_state > HW_CGC_ROMWaitGet())
    {
       /** Set the wait state BEFORE we change iclk, if switching to higher frequency. */
       HW_CGC_ROMWaitSet(new_rom_wait_state);
    }

    current_clock = HW_CGC_ClockSourceGet();
    if (clock_source != current_clock)    // if clock_source is not the current system clock, check stabilization
    {
       if (((clock_source == CGC_CLOCK_HOCO) || (clock_source == CGC_CLOCK_MAIN_OSC))
           || (clock_source == CGC_CLOCK_PLL))
       {
           /* make sure the oscillator is stable before setting as system clock */
           CGC_ERROR_RETURN(HW_CGC_ClockCheck(clock_source), SSP_ERR_STABILIZED);
       }
    }
   /* In order to avoid a system clock (momentarily) higher than expected, the order of switching the clock and
    * dividers must be so that the frequency of the clock goes lower, instead of higher, before being correct.
    */

   HW_CGC_SystemDividersGet(&current_clock_cfg);
   /* If the current ICLK divider is less (higher frequency) than the requested ICLK divider,
    *  set the divider first.
    */
   if (current_clock_cfg.iclk_div < p_clock_cfg->iclk_div )
   {
       HW_CGC_SystemDividersSet(p_clock_cfg);
       HW_CGC_ClockSourceSet(clock_source);
   }
   /* The current ICLK divider is greater (lower frequency) than the requested ICLK divider, so
    * set the clock source first.
    */
   else
   {
       HW_CGC_ClockSourceSet(clock_source);
       HW_CGC_SystemDividersSet(p_clock_cfg);
   }

       /* Clock is now at requested frequency. */

    /* Update the CMSIS core clock variable so that it reflects the new Iclk freq */
    SystemCoreClock = bsp_cpu_clock_get();

    current_frequency = HW_CGC_ClockHzGet(CGC_SYSTEM_CLOCKS_ICLK);
    /** Wait states for low speed RAM (SRAM0 and SRAM1) */
    /** No wait: ICLK <= 120 MHz */
    /** 1 wait:  ICLK > 120 MHz */
    if (current_frequency <= CGC_SYS_CLOCK_FREQ_NO_RAM_WAITS)
    {
       HW_CGC_SRAM_RAMWaitSet((uint32_t)CGC_SRAM_ZERO_WAIT_CYCLES);
    }
    /** Wait states for High speed RAM (SRAMHS) */
    /** No wait: ICLK <= 200 MHz */
    /** 1 wait:  ICLK > 200 MHz */
    if (current_frequency <= CGC_SYS_CLOCK_FREQ_NO_HSRAM_WAITS)
    {
       HW_CGC_SRAM_HSRAMWaitSet((uint32_t)CGC_SRAM_ZERO_WAIT_CYCLES);
    }


    if (new_rom_wait_state != HW_CGC_ROMWaitGet())
    {
       /** In this case, switching to lower frequency, we need to set the wait state AFTER we change iclk. */
       HW_CGC_ROMWaitSet(new_rom_wait_state);
    }
#else   /* S124 or S128 */
    current_clock = HW_CGC_ClockSourceGet();
    if (clock_source != current_clock)    // if clock_source is not the current system clock, check stabilization
    {
        if (((clock_source == CGC_CLOCK_HOCO) || (clock_source == CGC_CLOCK_MAIN_OSC))
            || (clock_source == CGC_CLOCK_PLL))
        {
            /* make sure the oscillator is stable before setting as system clock */
            CGC_ERROR_RETURN(HW_CGC_ClockCheck(clock_source), SSP_ERR_STABILIZED);
        }
        /* In order to avoid a system clock (momentarily) higher than expected, the order of switching the clock and
         * dividers must be so that the frequency of the clock goes lower, instead of higher, before being correct.
         */
    }
    HW_CGC_SystemDividersGet(&current_clock_cfg);
    /* If the current ICLK divider is less (higher frequency) than the requested ICLK divider,
     *  set the divider first.
     */
    if (current_clock_cfg.iclk_div < p_clock_cfg->iclk_div )
    {
        HW_CGC_SystemDividersSet(p_clock_cfg);
        HW_CGC_ClockSourceSet(clock_source);
    }
    /* The current ICLK divider is greater (lower frequency) than the requested ICLK divider, so
     * set the clock source first.
     */
    else
    {
        HW_CGC_ClockSourceSet(clock_source);
        HW_CGC_SystemDividersSet(p_clock_cfg);
    }

        /* Clock is now at requested frequency. */

    /* Update the CMSIS core clock variable so that it reflects the new Iclk freq */
    SystemCoreClock = bsp_cpu_clock_get();
#endif
#endif

#if (1 == BSP_CFG_RTOS)
    /* If an RTOS is in use, Update the Systick period based on the new frequency, using the ThreadX systick period in Microsecs */
    R_CGC_SystickUpdate((1000000 / TX_TIMER_TICKS_PER_SECOND), CGC_SYSTICK_PERIOD_UNITS_MICROSECONDS);
#endif

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Return the current system clock source and configuration.
 * @retval SSP_SUCCESS   Parameters returned successfully.
 *
 **********************************************************************************************************************/

ssp_err_t R_CGC_SystemClockGet (cgc_clock_t * clock_source, cgc_system_clock_cfg_t * p_set_clock_cfg)
{
    *clock_source = HW_CGC_ClockSourceGet();
    HW_CGC_SystemDividersGet(p_set_clock_cfg);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Return the requested internal clock frequency in Hz.
 * @retval SSP_SUCCESS                  Operation performed successfully.
 * @retval SSP_ERR_INVALID_ARGUMENT     Invalid clock specified.
 **********************************************************************************************************************/
ssp_err_t R_CGC_SystemClockFreqGet (cgc_system_clocks_t clock, uint32_t * p_freq_hz)
{
    /* return error if invalid system clock or not supported by hardware */
    *p_freq_hz = (uint32_t)0x00;
    CGC_ERROR_RETURN((HW_CGC_SystemClockValidCheck(clock)), SSP_ERR_INVALID_ARGUMENT);

    *p_freq_hz = HW_CGC_ClockHzGet(clock);
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Check the specified clock for stability.
 * @retval SSP_ERR_STABILIZED           Clock stabilized.
 * @retval SSP_ERR_NOT_STABILIZED       Clock not stabilized.
 * @retval SSP_ERR_CLOCK_ACTIVE         Clock active but not able to check for stability.
 * @retval SSP_ERR_CLOCK_INACTIVE       Clock not turned on.
 * @retval SSP_ERR_INVALID_ARGUMENT     Illegal parameter passed.
 **********************************************************************************************************************/
ssp_err_t R_CGC_ClockCheck (cgc_clock_t clock_source)
{
    /* return error if invalid clock source or not supported by hardware */
    CGC_ERROR_RETURN((HW_CGC_ClockSourceValidCheck(clock_source)), SSP_ERR_INVALID_ARGUMENT);

    /*  There is no function to check for LOCO, MOCO or SUBCLOCK stability */
    if (((clock_source == CGC_CLOCK_LOCO) || (clock_source == CGC_CLOCK_MOCO)) || (clock_source == CGC_CLOCK_SUBCLOCK))
    {
        if (true == HW_CGC_ClockRunStateGet(clock_source))
        {
            return SSP_ERR_CLOCK_ACTIVE;      // There is no hardware to check for stability so just check for state.
        }
        else
        {
            return SSP_ERR_CLOCK_INACTIVE;    // There is no hardware to check for stability so just check for state.
        }
    }

    /*  There is a function to check for HOCO, MAIN_OSC and PLL stability */
    else if (((clock_source == CGC_CLOCK_HOCO) || (clock_source == CGC_CLOCK_MAIN_OSC))
             || (clock_source == CGC_CLOCK_PLL))
    {
        /* if clock is not active, can't check for stability, return error */
        CGC_ERROR_RETURN(HW_CGC_ClockRunStateGet(clock_source), SSP_ERR_CLOCK_INACTIVE);

        /*  check oscillator for stability, return error if not stable */
        CGC_ERROR_RETURN(HW_CGC_ClockCheck(clock_source), SSP_ERR_NOT_STABILIZED);
        return SSP_ERR_STABILIZED;              // otherwise, return affirmative, not really an error
    }
    else
    {
        /* statement here to follow coding standard */
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable or disable the oscillation stop detection for the main clock.
 *  The MCU will automatically switch the system clock to MOCO when a stop is detected if Main Clock is
 *  the system clock. If the system clock is the PLL, then the clock source will not be changed and the
 *  PLL free running frequency will be the system clock frequency.
 * @retval SSP_SUCCESS                  Operation performed successfully.
 * @retval SSP_ERR_OSC_STOP_DETECTED    The Oscillation stop detect status flag is set. Under this condition it is not
 *                                      possible to disable the Oscillation stop detection function.
 * @retval SSP_ERR_NULL_PTR             Null pointer passed for callback function when the second argument is "true".
 **********************************************************************************************************************/
ssp_err_t R_CGC_OscStopDetect (void (* p_callback) (cgc_callback_args_t * p_args), bool enable)
{
    if (true == enable)
    {
#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
        SSP_ASSERT(NULL != p_callback);
#endif
        /* - add callback function to BSP */
#ifndef BSP_MCU_MOCKED
        R_BSP_GroupIrqWrite(BSP_GRP_IRQ_OSC_STOP_DETECT, (bsp_grp_irq_cb_t) p_callback);
#endif
        HW_CGC_OscStopDetectEnable();          // enable hardware oscillator stop detect
    }
    else
    {
        /* if oscillator stop detected, return error */
        CGC_ERROR_RETURN(!(HW_CGC_OscStopDetectGet()), SSP_ERR_OSC_STOP_DETECTED);
        HW_CGC_OscStopDetectDisable();          // disable hardware oscillator stop detect
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Clear the Oscillation Stop Detection Status register.
 *
 *                This register is not cleared automatically if the stopped clock is restarted.
 *                This function blocks for about 3 ICLK cycles until the status register is cleared.
 * @retval SSP_SUCCESS 						Operation performed successfully.
 * @retval SSP_ERR_OSC_STOP_CLOCK_ACTIVE    The Oscillation Detect Status flag cannot be cleared if the
 *                                          Main Osc or PLL is set as the system clock. Change the
 *                                          system clock before attempting to clear this bit.
 **********************************************************************************************************************/

ssp_err_t R_CGC_OscStopStatusClear (void)
{
    cgc_clock_t current_clock;

    if (HW_CGC_OscStopDetectGet())               // if oscillator stop detected
    {
        current_clock = HW_CGC_ClockSourceGet(); // The currently active system clock source
#ifdef BSP_MCU_GROUP_S7G2
        cgc_clock_t alt_clock;
        alt_clock = HW_CGC_PLLClockSourceGet();
        /* cannot clear oscillator stop status if Main Osc is source of PLL */
        if ((CGC_CLOCK_PLL == current_clock) && (CGC_CLOCK_MAIN_OSC == alt_clock))
        {
            CGC_ERROR_RETURN(0, SSP_ERR_OSC_STOP_CLOCK_ACTIVE);    // return error
        }

#else
#ifdef BSP_MCU_GROUP_S3A7
        /* cannot clear oscillator stop status if PLL is current clock */
        CGC_ERROR_RETURN(!(CGC_CLOCK_PLL == current_clock), SSP_ERR_OSC_STOP_CLOCK_ACTIVE);
#endif
#endif /* ifdef BSP_MCU_GROUP_S7G2 */
        /* cannot clear oscillator stop status if Main Osc is current clock */
        CGC_ERROR_RETURN(!(CGC_CLOCK_MAIN_OSC == current_clock), SSP_ERR_OSC_STOP_CLOCK_ACTIVE);
    }

    HW_CGC_OscStopStatusClear();          // clear hardware oscillator stop detect status

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure the secondary dividers for BCLKOUT. The primary divider is set using the
 *           bsp clock configuration and the R_CGC_SystemClockSet function.
 * @retval SSP_SUCCESS                  Operation performed successfully.
  **********************************************************************************************************************/

ssp_err_t R_CGC_BusClockOutCfg (cgc_bclockout_dividers_t divider)
{
    /* The application must set up the PFS register so the BCLK pin is an output */

    HW_CGC_BusClockOutCfg(divider);
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable the BCLKOUT output.
 * @retval SSP_SUCCESS  Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_BusClockOutEnable (void)
{
    HW_CGC_BusClockOutEnable();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Disable the BCLKOUT output.
 * @retval SSP_SUCCESS  Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_BusClockOutDisable (void)
{
    HW_CGC_BusClockOutDisable();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure the dividers for CLKOUT.
 * @retval SSP_SUCCESS                  Operation performed successfully.
 **********************************************************************************************************************/
ssp_err_t R_CGC_ClockOutCfg (cgc_clock_t clock, cgc_clockout_dividers_t divider)
{
    /* The application must set up the PFS register so the CLKOUT pin is an output */

    HW_CGC_ClockOutCfg(clock, divider);
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable the CLKOUT output.
 * @retval SSP_SUCCESS  Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_ClockOutEnable (void)
{
    HW_CGC_ClockOutEnable();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Disable the CLKOUT output.
 * @retval SSP_SUCCESS  Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_ClockOutDisable (void)
{
    HW_CGC_ClockOutDisable();
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure the source for the segment LCDCLK.
 * @retval SSP_SUCCESS                  Operation performed successfully.
  **********************************************************************************************************************/

ssp_err_t R_CGC_LCDClockCfg (cgc_clock_t clock)
{
    /* The application must set up the PFS register so the LCDCLKOUT pin is an output */
#ifndef  BSP_MCU_GROUP_S7G2
    HW_CGC_LCDClockCfg(clock);
#else
    SSP_PARAMETER_NOT_USED(clock);
#endif
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable the segment LCDCLK output.
 * @retval SSP_SUCCESS  Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_LCDClockEnable (void)
{
#ifndef  BSP_MCU_GROUP_S7G2
    HW_CGC_LCDClockEnable();
#endif
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Disable the segment LCDCLK output.
 * @retval SSP_SUCCESS      Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_LCDClockDisable (void)
{
#ifndef  BSP_MCU_GROUP_S7G2
    HW_CGC_LCDClockDisable();
#endif
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable the SDCLK output.
 * @retval SSP_SUCCESS  Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_SDRAMClockOutEnable (void)
{
#ifdef  BSP_MCU_GROUP_S7G2
    HW_CGC_SDRAMClockOutEnable();
#endif
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Disable the SDCLK output.
 * @retval SSP_SUCCESS      Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_SDRAMClockOutDisable (void)
{
#ifdef  BSP_MCU_GROUP_S7G2
    HW_CGC_SDRAMClockOutDisable();
#endif
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure the dividers for UCLK.
 * @retval SSP_SUCCESS                  Operation performed successfully.
 * @retval SSP_ERR_INVALID_ARGUMENT     Invalid divider specified.
 **********************************************************************************************************************/

ssp_err_t R_CGC_USBClockCfg (cgc_usb_clock_div_t divider)
{
    /* The application must set up the PFS register so the USBCLKOUT pin is an output */

#ifdef  BSP_MCU_GROUP_S7G2
    HW_CGC_USBClockCfg(divider);
#else
    SSP_PARAMETER_NOT_USED(divider);
#endif
    return SSP_SUCCESS;
}


/*******************************************************************************************************************//**
 * @brief  Re-Configure the systick based on the provided period and current system clock frequency.
 * @param[in]   period_count       The duration for the systick period.
 * @param[in]   units              The units for the provided period.
 * @retval SSP_SUCCESS                  Operation performed successfully.
 * @retval SSP_ERR_INVALID_ARGUMENT     Invalid period specified.
 * @retval SSP_ERR_ABORTED              Attempt to update systick timer failed.
 **********************************************************************************************************************/
ssp_err_t R_CGC_SystickUpdate(uint32_t period_count, cgc_systick_period_units_t units)
{
    uint32_t requested_period_count = period_count;
    uint32_t reload_value;
    uint32_t freq;
    cgc_systick_period_units_t period_units = units;
    float period = 0.0;

#if (CGC_CFG_PARAM_CHECKING_ENABLE)
    if (0 == period_count)
    {
        CGC_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);     // Invalid period provided
    }
#endif

    freq = bsp_cpu_clock_get();		                // Get the current ICLK frequency

    /* If an RTOS is in use then we want to set the Systick period to that defined by the RTOS. So we'll convert the macro
     * settings use the existing code and calculate the reload value
     */
#if (1 == BSP_CFG_RTOS)
    period_units = CGC_SYSTICK_PERIOD_UNITS_MICROSECONDS;
    requested_period_count = (RELOAD_COUNT_FOR_1US) / TX_TIMER_TICKS_PER_SECOND;        // Convert ticks per sec to ticks per us
#endif


    period = (((float)1.0)/(float)freq) * (float)period_units;           // This is the period in the provided units
    reload_value = (uint32_t)((float)requested_period_count/period);

    if (HW_CGC_SystickUpdate(reload_value) == false)	// Configure the systick period as requested
    {
        return SSP_ERR_ABORTED;
    }
    return SSP_SUCCESS;
}


/*******************************************************************************************************************//**
 * @brief  Return the driver version.
 * @retval SSP_SUCCESS      Operation performed successfully.
 **********************************************************************************************************************/

ssp_err_t R_CGC_VersionGet (ssp_version_t * const p_version)
{
    p_version->version_id = g_cgc_version.version_id;
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup CGC)
 **********************************************************************************************************************/
