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
 * File Name    : hw_cgc.h
 * Description  : Header file for hw_cgc.c
 **********************************************************************************************************************/
#ifndef HW_CGC_H
#define HW_CGC_H

#include "bsp_cfg.h"
#include "bsp_clock_cfg.h"
#include "hw_cgc_private.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** RAM and ROM wait register settings */
#define CGC_SRAM_PRCR_MASK        ((uint8_t) 0x01)
/** Key code for writing PRCR register. */
#define CGC_SRAM_PRCR_KEY         ((uint8_t) 0xF0) ///< 1111000m  where m is the enable/disable mask

#define CGC_SRAM_ZERO_WAIT_CYCLES (0)              ///< Specify zero wait states for SRAM
#define CGC_SRAM_ONE_WAIT_CYCLES  (1)              ///< Specify one wait states for SRAM

#ifdef  BSP_MCU_GROUP_S7G2
#define CGC_SYS_CLOCK_FREQ_NO_RAM_WAITS   (120000000)
#define CGC_SYS_CLOCK_FREQ_NO_HSRAM_WAITS (200000000)
#define CGC_SYS_CLOCK_FREQ_ONE_ROM_WAITS  (80000000)
#define CGC_SYS_CLOCK_FREQ_TWO_ROM_WAITS  (160000000)
#endif

#define CGC_HIGH_SPEED_MODE   (0x00)
#define CGC_MIDDLE_SPEED_MODE (0x00)

/** PRC0 mask */
#define CGC_PRC0_MASK             ((uint16_t) 0x0001)
/** PRC1 mask */
#define CGC_PRC1_MASK             ((uint16_t) 0x0002)
/** Key code for writing PRCR register. */
#define CGC_PRCR_KEY              ((uint16_t) 0xA500)
#define CGC_PLL_MAIN_OSC          (0x00)            ///< PLL clock source register value for the main oscillator
#define CGC_PLL_HOCO              (0x01)            ///< PLL clock source register value for the HOCO

#define CGC_LOCO_FREQ             (32768)           ///< LOCO frequency is fixed at 32768 Hz
#define CGC_MOCO_FREQ             (8000000)         ///< MOCO frequency is fixed at 8 MHz
#define CGC_SUBCLOCK_FREQ         (32768)           ///< Subclock frequency is 32768 Hz
#define CGC_MAIN_OSC_FREQ         (BSP_CFG_XTAL_HZ) ///< The main oscillator frequency is specified in the bsp clock
                                                    // configuration file.
#define CGC_PLL_FREQ              (0)               ///< The PLL frequency must be calculated.
#define CGC_IWDT_FREQ             (15000)           ///< The IWDT frequency is fixed at 15 kHz
#define CGC_S124_LOW_V_MODE_FREQ  (4000000)         ///< Max ICLK frequency while in Low Voltage Mode for S124

#define CGC_MAX_ZERO_WAIT_FREQ    (32000000)
#define CGC_MAX_MIDDLE_SPEED_FREQ (12000000)        ///< Maximum frequency for Middle Speed mode
#define CGC_ZERO_WAIT_CYCLES      (0)
#define CGC_TWO_WAIT_CYCLES       (1)
#ifdef  BSP_MCU_GROUP_S7G2
/** HOCO frequency Option Function Select Register 1 (OFS1) value */
#define CGC_HOCO_16_MHZ (0)                     ///< HOCO 16 MHZ value in Option Function Select Register 1
#define CGC_HOCO_18_MHZ (1)                     ///< HOCO 18 MHZ value in Option Function Select Register 1
#define CGC_HOCO_20_MHZ (2)                     ///< HOCO 20 MHZ value in Option Function Select Register 1

#define CGC_MODRV_MASK  ((uint8_t) 0x30)        ///< Main oscillator drive mask
#define CGC_MODRV_SHIFT (4)                     ///< Main oscillator drive register shift value
#define CGC_SODRV_MASK  ((uint8_t) 0x02)        ///< subclock oscillator drive mask
#define CGC_SODRV_SHIFT (1)                     ///< subclock oscillator drive register shift value
#define CGC_PLL_DIV_MAX (CGC_PLL_DIV_3)         ///< Maximum PLL divider value
#define CGC_PLL_MUL_MIN (10.0f)                 ///< Minimum PLL multiplier value
#define CGC_PLL_MUL_MAX (30.0f)                 ///< Maximum PLL multiplier value
#define CGC_PLL_MUL_INC (.5f)                   ///< PLL multiplier increments

#else /* other than BSP_MCU_GROUP_S7G2 */
/** HOCO frequency Option Function Select Register 1 (OFS1) value */
#define CGC_HOCO_24_MHZ (0)                  ///< HOCO 24 MHZ value in Option Function Select Register 1
#define CGC_HOCO_32_MHZ (2)                  ///< HOCO 32 MHZ value in Option Function Select Register 1
#define CGC_HOCO_48_MHZ (3)                  ///< HOCO 48 MHZ value in Option Function Select Register 1
#define CGC_HOCO_64_MHZ (4)                  ///< HOCO 64 MHZ value in Option Function Select Register 1

#define CGC_MODRV_MASK  ((uint8_t) 0x08)     ///< Main oscillator drive mask
#define CGC_MODRV_SHIFT (3)                  ///< Main oscillator drive register shift value
#define CGC_SODRV_MASK  ((uint8_t) 0x03)     ///< subclock oscillator drive mask
#define CGC_SODRV_SHIFT (0)                  ///< subclock oscillator drive register shift value
#define CGC_PLL_DIV_MAX (CGC_PLL_DIV_4)      ///< Maximum PLL divider value
#define CGC_PLL_MUL_MIN (8.0f)               ///< Minimum PLL multiplier value
#define CGC_PLL_MUL_MAX (31.0f)              ///< Maximum PLL multiplier value
#define CGC_PLL_MUL_INC (1.0f)               ///< PLL multiplier increments
#endif /* ifdef  BSP_MCU_GROUP_S7G2 */

#ifndef BSP_MCU_SC32_4
#define    CGC_CLOCK_MAX (CGC_CLOCK_PLL)        ///< Highest enum value for CGC clock source
#else
#define    CGC_CLOCK_MAX (CGC_CLOCK_SUBCLOCK)   ///< Highest enum value for CGC clock source
#endif

#define    CGC_CLOCKOUT_MAX       (CGC_CLOCK_SUBCLOCK)     ///< Highest enum value for CLKOUT clock source

#define    CGC_SYSTEMS_CLOCKS_MAX (CGC_SYSTEM_CLOCKS_ICLK) ///< Highest enum value for system clock

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef union
{
    __IO uint32_t  sckdivcr_w; /*!< (@ 0x4001E020) System clock Division control register
                                *                */

    struct
    {
        __IO uint32_t  pckd : 3; /*!< [0..2] Peripheral Module Clock D (PCLKD) Select
                                  *                      */
        uint32_t            : 1;
        __IO uint32_t  pckc : 3; /*!< [4..6] Peripheral Module Clock C (PCLKC) Select
                                  *                      */
        uint32_t            : 1;
        __IO uint32_t  pckb : 3; /*!< [8..10] Peripheral Module Clock B (PCLKB) Select
                                  *                     */
        uint32_t            : 1;
        __IO uint32_t  pcka : 3; /*!< [12..14] Peripheral Module Clock A (PCLKA) Select
                                  *                    */
        uint32_t            : 1;
        __IO uint32_t  bck  : 3; /*!< [16..18] External Bus Clock (BCLK) Select
                                  *                            */
        uint32_t            : 5;
        __IO uint32_t  ick  : 3; /*!< [24..26] System Clock (ICLK) Select
                                  *                                  */
        uint32_t            : 1;
        __IO uint32_t  fck  : 3; /*!< [28..30] Flash IF Clock (FCLK) Select
                                  *                                */
    }  sckdivcr_b;               /*!< [31] BitSize
                                  *                                                         */
} sckdivcr_clone_t;
#endif // ifndef HW_CGC_H
