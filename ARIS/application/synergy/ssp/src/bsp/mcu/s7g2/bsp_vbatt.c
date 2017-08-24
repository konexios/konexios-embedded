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
* File Name    : bsp_vbatt.c
* Description  : This module provides APIs for initializing the VBATT Input Control Register.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

#if defined(BSP_MCU_GROUP_S7G2)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Mask to get PSEL bitfield from PFS register. */
#define BSP_PRV_PFS_PSEL_MASK   (0x1F000000uL)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static const ioport_port_pin_t g_vbatt_pins_input[] =
{
    IOPORT_PORT_04_PIN_02,  ///< Associated with VBTICTLR->VCH0INEN
    IOPORT_PORT_04_PIN_03,  ///< Associated with VBTICTLR->VCH1INEN
    IOPORT_PORT_04_PIN_04   ///< Associated with VBTICTLR->VCH2INEN
};

/*******************************************************************************************************************//**
 * @brief Initializes VBTICTLR register based on pin configuration.
 * 
 * The VBTICTLR register may need to be modified based on the project's pin configuration. There is a set of pins that
 * needs to be checked. If one of these pins is found in the pin configuration table then it will be tested to see if
 * the appropriate VBTICTLR bit needs to be set or cleared. If one of the pins that is being searched for is not found
 * then the accompanying VBTICTLR bit is left as-is.
 **********************************************************************************************************************/
void bsp_vbatt_init (ioport_cfg_t const * const p_pin_cfg)
{
    uint32_t pin_index;
    uint32_t vbatt_index;
    uint8_t  local_vbtictlr_set;    ///< Will hold bits to set in VBTICTLR
    uint8_t  local_vbtictlr_clear;  ///< Will hold bits to set in VBTICTLR

    /** Make no changes unless required. */    
    local_vbtictlr_set = 0;
    local_vbtictlr_clear = 0;

    /** Must loop over all pins as pin configuration table is unordered. */
    for (pin_index = 0; pin_index < p_pin_cfg->number_of_pins; pin_index++)
    {
        /** Loop over VBATT input pins. */
        for (vbatt_index = 0; vbatt_index < (sizeof(g_vbatt_pins_input)/sizeof(g_vbatt_pins_input[0])); vbatt_index++)
        {
            if (p_pin_cfg->p_pin_cfg_data[pin_index].pin == g_vbatt_pins_input[vbatt_index])
            {
                /** Get PSEL value for pin. */
                uint32_t pfs_psel_value = p_pin_cfg->p_pin_cfg_data[pin_index].pin_cfg & BSP_PRV_PFS_PSEL_MASK;

                /** Check if pin is being used for RTC or AGT use. */
                if ((IOPORT_PERIPHERAL_AGT == pfs_psel_value) || (IOPORT_PERIPHERAL_CLKOUT_COMP_RTC == pfs_psel_value))
                {
                    /** Bit should be set to 1. */
                    local_vbtictlr_set |= (uint8_t)(1 << vbatt_index);
                }
                else
                {
                    /** Bit should be cleared to 0. */
                    local_vbtictlr_clear |= (uint8_t)(1 << vbatt_index);
                }
            }
        }
    }

    /** Disable write protection on VBTICTLR. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);

    /** Read value, set and clear bits as needed and write back. */
    uint8_t local_vbtictlr = R_SYSTEM->VBTICTLR;
    local_vbtictlr |= local_vbtictlr_set;               ///< Set appropriate bits
    local_vbtictlr &= (uint8_t)~local_vbtictlr_clear;   ///< Clear appropriate bits

    R_SYSTEM->VBTICTLR = local_vbtictlr;

    /** Enable write protection on VBTICTLR. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
}

#endif



