/***********************************************************************************************************************
 * Copyright [2016] RELOC s.r.l. - All rights strictly reserved
 * This Software is provided for evaluation purposes; any other use — including reproduction, modification, use for
 * a commercial product, distribution, or republication — without the prior written permission of the Copyright holder
 * is strictly prohibited. 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : bsp_pin_cfg_ref.h
* Description  : This configuration header file has information for setting up pins out of reset.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_CONFIG_ARIS
 * @defgroup BSP_CONFIG_ARIS_PINS Build Time Configurations - Pin Settings
 *
 * This file contains an array of pin configurations. During start-up, and before main() is executed, the BSP will
 * iterate over this array and initialize the MCU's port pins based on the settings in the array.
 *
 * @note It is recommended to use the ISDE Pin Configurator to create this file.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_PIN_CFG_REF_H_
#define BSP_PIN_CFG_REF_H_

/***********************************************************************************************************************
Pin Configuration Table
***********************************************************************************************************************/

/* This conditional makes sure that this table is only allocated once. Without this check the table would be allocated
 * in every module that includes this header file. */
#if defined(BSP_PIN_ALLOCATE)

/* Used for getting IOPORT types. */
#include "r_ioport.h"

/** Array of pin configurations. */
const ioport_pin_cfg_t g_bsp_pin_cfg_data[] =
{
    {
        .pin     = IOPORT_PORT_00_PIN_02,
        .pin_cfg = (IOPORT_CFG_IRQ_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_11,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_12,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_13,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_14,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_01_PIN_15,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC),
    },
    {
        .pin     = IOPORT_PORT_02_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_08,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_09,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_10,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_11,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_03_PIN_12,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_06,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_FS),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_10,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_11,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_12,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_13,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC),
    },
    {
        .pin     = IOPORT_PORT_04_PIN_14,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_14,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_05_PIN_15,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_09,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_10,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_11,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_12,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_13,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_06_PIN_14,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_02,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_ETHER_RMII),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_06,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_07_PIN_07,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_HS),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_01,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDRAM | IOPORT_CFG_DRIVE_HIGH),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_07,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_08,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_09,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_08_PIN_10,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_09_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC),
    },
    {
        .pin     = IOPORT_PORT_11_PIN_00,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_HS),
    },
    {
        .pin     = IOPORT_PORT_11_PIN_02,
        .pin_cfg = (IOPORT_CFG_PORT_DIRECTION_OUTPUT),
    },
    {
        .pin     = IOPORT_PORT_11_PIN_03,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8 | IOPORT_CFG_PULLUP_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_11_PIN_04,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8 | IOPORT_CFG_PULLUP_ENABLE),
    },
    {
        .pin     = IOPORT_PORT_11_PIN_05,
        .pin_cfg = (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8 | IOPORT_CFG_PULLUP_ENABLE),
    },
};

/** Has pointer to pin configuration array and number of pins to be configured. */
const ioport_cfg_t g_bsp_pin_cfg =
{
    .number_of_pins = sizeof(g_bsp_pin_cfg_data)/sizeof(ioport_pin_cfg_t),
    .p_pin_cfg_data = &g_bsp_pin_cfg_data[0],
};

#endif /* BSP_PIN_ALLOCATE */

/** @} (end defgroup BSP_CONFIG_ARIS_PINS) */

#endif /* BSP_PIN_CFG_REF_H_ */
