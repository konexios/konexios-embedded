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
 * File Name    : hw_cgc_private.h
 * Description  : hw_cgc Private header file.
 **********************************************************************************************************************/

#ifndef HW_CGC_PRIVATE_H
#define HW_CGC_PRIVATE_H
#include "r_cgc_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Main oscillator source identifier. */
typedef enum e_cgc_osc_source
{
    CGC_OSC_SOURCE_RESONATOR,       ///< A resonator is used as the source of the main oscillator.
    CGC_OSC_SOURCE_EXTERNAL_OSC     ///< An external oscillator is used as the source of the main oscillator.
} cgc_osc_source_t;
/**********************************************************************************************************************
* Functions
 **********************************************************************************************************************/
void          HW_CGC_Init (void);

void          HW_CGC_InitPLLFreq (void);

void          HW_CGC_LPMHardwareLock (void);

void          HW_CGC_LPMHardwareUnLock (void);

void          HW_CGC_HardwareLock (void);

void          HW_CGC_HardwareUnLock (void);

void          HW_CGC_MainClockDriveSet (uint8_t val);                   // Set the drive capacity for the main clock.

void          HW_CGC_SubClockDriveSet (uint8_t val);                    // Set the drive capacity for the subclock.

void          HW_CGC_ClockStart (cgc_clock_t clock);                    // Start selected clock.

void          HW_CGC_ClockStop (cgc_clock_t clock);                     //  Stop selected clock.

bool          HW_CGC_ClockCheck (cgc_clock_t clock);                    //  Check for stability of selected clock.

bool          HW_CGC_ClockRunStateGet (cgc_clock_t clock);              // Get clock run state.

void          HW_CGC_DelayCycles (cgc_clock_t clock, uint16_t cycles);  // delay for number of clock cycles specified

void          HW_CGC_MainOscSourceSet (cgc_osc_source_t osc);           // Set the source to resonator or external osc.

void          HW_CGC_ClockWaitSet (cgc_clock_t clock, uint8_t time);    // Set the clock wait time

bool          HW_CGC_ClockSourceValidCheck (cgc_clock_t clock);         // Check for valid clock source

bool          HW_CGC_SystemClockValidCheck (cgc_system_clocks_t clock); // Check for valid system clock

cgc_clock_t   HW_CGC_ClockSourceGet (void);                             // Get the system clock source

void          HW_CGC_ClockSourceSet (cgc_clock_t clock);                // Set the system clock source

cgc_clock_t   HW_CGC_PLLClockSourceGet (void);                          // Get the PLL clock source

void          HW_CGC_PLLClockSourceSet (cgc_clock_t clock);             // Set the PLL clock source

void          HW_CGC_PLLMultiplierSet (float multiplier);

float         HW_CGC_PLLMultiplierGet (void);

void          HW_CGC_PLLDividerSet (cgc_pll_div_t divider);

cgc_pll_div_t HW_CGC_PLLDividerGet (void);

void          HW_CGC_SystemDividersSet (cgc_system_clock_cfg_t * cfg);  // Set the system dividers

void          HW_CGC_SystemDividersGet (cgc_system_clock_cfg_t * cfg);  // Get the system dividers

bool          HW_CGC_ClockCfgValidCheck (cgc_clock_cfg_t * cfg);        // check for valid configuration

uint32_t      HW_CGC_ClockDividerGet (cgc_system_clocks_t clock);

uint32_t      HW_CGC_ClockHzGet (cgc_system_clocks_t clock);            // get frequency of selected clock

uint32_t      HW_CGC_ClockHzCalculate (cgc_clock_t source_clock,  cgc_sys_clock_div_t divider);

void          HW_CGC_MemWaitSet (uint32_t setting);

uint32_t      HW_CGC_MemWaitGet (void);

bool          HW_CGC_HighSpeedMode (void);

void          HW_CGC_SetHighSpeedMode (void);

void          HW_CGC_SetMiddleSpeedMode (void);

void          HW_CGC_OscStopDetectEnable (void);            // enable hardware oscillator stop detect function

void          HW_CGC_OscStopDetectDisable (void);           // disable hardware oscillator stop detect function

bool          HW_CGC_OscStopDetectEnabledGet (void);        // status of hardware oscillator stop detect enabled

bool          HW_CGC_OscStopDetectGet (void);               // oscillator stop detected

bool          HW_CGC_OscStopStatusClear (void);             // clear hardware oscillator stop detect status

void          HW_CGC_BusClockOutCfg (cgc_bclockout_dividers_t divider);

void          HW_CGC_BusClockOutEnable (void);

void          HW_CGC_BusClockOutDisable (void);

void          HW_CGC_ClockOutCfg (cgc_clock_t clock, cgc_clockout_dividers_t divider);

void          HW_CGC_ClockOutEnable (void);

void          HW_CGC_ClockOutDisable (void);

void          HW_CGC_LCDClockCfg (cgc_clock_t clock);

void          HW_CGC_LCDClockEnable (void);

void          HW_CGC_LCDClockDisable (void);

void          HW_CGC_SDRAMClockOutEnable (void);

void          HW_CGC_SDRAMClockOutDisable (void);

void          HW_CGC_USBClockCfg (cgc_usb_clock_div_t divider);

void          HW_CGC_SRAM_ProtectLock (void);

void          HW_CGC_SRAM_ProtectUnLock (void);

void          HW_CGC_SRAM_RAMWaitSet (uint32_t setting);

void          HW_CGC_SRAM_HSRAMWaitSet (uint32_t setting);

void          HW_CGC_ROMWaitSet (uint32_t setting);

uint32_t      HW_CGC_ROMWaitGet (void);

bool          HW_CGC_SystickUpdate(uint32_t ticks_per_second);


#endif /* HW_CGC_PRIVATE_H */
