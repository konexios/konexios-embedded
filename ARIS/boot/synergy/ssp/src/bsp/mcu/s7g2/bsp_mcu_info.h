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
* File Name    : bsp_mcu_info.h
* Description  : Information about the MCU on this board
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCUs
 * @defgroup BSP_MCU_S7G2 S7G2
 * @brief Code that is common to S7G2 MCUs.
 *
 * Implements functions that are common to S7G2 MCUs.
 *
 * @{
***********************************************************************************************************************/

/** @} (end defgroup BSP_MCU_S7G2) */

#ifndef BSP_MCU_INFO_H_
#define BSP_MCU_INFO_H_

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Gets MCU configuration information. */
#include "bsp_cfg.h"
#include "bsp_clock_cfg.h"
#include "bsp_irq_cfg.h"

/* BSP Common Includes. */
#include "../../src/bsp/mcu/all/bsp_common.h"
#include "../../inc/ssp_common_api.h"

#if defined(__GNUC__)
/* CMSIS-CORE currently generates 2 warnings when compiling with GCC. One in core_cmInstr.h and one in core_cm4_simd.h.
 * We are not modifying these files so we will ignore these warnings temporarily. */
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif

/* CMSIS-CORE Renesas Device Files. */
#include "../../src/bsp/cmsis/Device/RENESAS/S7G2/Include/R7FS7G2x.h"
#include "../../src/bsp/cmsis/Device/RENESAS/S7G2/Include/system_S7G2.h"

/* CMSIS-CORE Standard Files. */
#include "core_cm4.h"
#include "core_cmInstr.h"
#include "core_cmFunc.h"
#include "core_cm4_simd.h"

#if defined(__GNUC__)
/* Restore warning settings for 'conversion' and 'sign-conversion' to as specified on command line. */
#pragma GCC diagnostic pop
#endif

/* BSP MCU Specific Includes. */
#include "../../src/bsp/mcu/s7g2/bsp_mcu_info.h"
#include "../../src/bsp/mcu/s7g2/bsp_register_protection.h"
#include "../../src/bsp/mcu/s7g2/bsp_locking.h"
#include "../../src/bsp/mcu/s7g2/bsp_hw_locks.h"
#include "../../src/bsp/mcu/s7g2/bsp_irq.h"
#include "../../src/bsp/mcu/s7g2/bsp_group_irq.h"
#include "../../src/bsp/mcu/s7g2/bsp_clocks.h"
#include "../../src/bsp/mcu/s7g2/bsp_elc.h"
#include "../../src/bsp/mcu/s7g2/bsp_ioport.h"

/* BSP MCU Specific Includes that depend on pin configuration. */
#include "../../src/bsp/mcu/s7g2/bsp_vbatt.h"

/* BSP Common Includes (Other than bsp_common.h) */
#include "../../src/bsp/mcu/all/bsp_common_leds.h"
#include "../../src/bsp/mcu/all/bsp_delay.h"

#include "../../src/bsp/mcu/s7g2/bsp_mcu_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* MCU Group name. */
#if   BSP_CFG_MCU_PART_CORE == 0x10
    #define BSP_MCU_GROUP_NPP2              (1)     ///< Temporary internal name. Will be removed.
    #define BSP_MCU_GROUP_S7G2              (1)     ///< CM4, FPU, 240MHz
#else
    #error "ERROR - BSP_CFG_MCU_PART_CORE - Unknown MCU Group chosen in r_bsp_config.h"
#endif

/* Memory size of your MCU. */
#if   BSP_CFG_MCU_PART_MEMORY_SIZE == 0x06
    #define BSP_ROM_SIZE_BYTES              (65536)     ///< 64KB of ROM
    #define BSP_RAM_SIZE_BYTES              (16384)     ///< 16KB of RAM
    #define BSP_DATA_FLASH_SIZE_BYTES       (4096)      ///< 4KB of Data Flash
#elif BSP_CFG_MCU_PART_MEMORY_SIZE == 0x07
    #define BSP_ROM_SIZE_BYTES              (131072)    ///< 128KB of ROM
    #define BSP_RAM_SIZE_BYTES              (24576)     ///< 24KB of RAM
    #define BSP_DATA_FLASH_SIZE_BYTES       (8192)      ///< 8KB of Data Flash
#elif BSP_CFG_MCU_PART_MEMORY_SIZE == 0x08
    #define BSP_ROM_SIZE_BYTES              (262144)    ///< 256KB of ROM
    #define BSP_RAM_SIZE_BYTES              (32768)     ///< 32KB of RAM
    #define BSP_DATA_FLASH_SIZE_BYTES       (8192)      ///< 8KB of Data Flash
#elif BSP_CFG_MCU_PART_MEMORY_SIZE == 0x0A
    #define BSP_ROM_SIZE_BYTES              (524288)    ///< 512KB of ROM
    #define BSP_RAM_SIZE_BYTES              (131072)    ///< 128KB of RAM
    #define BSP_DATA_FLASH_SIZE_BYTES       (16384)     ///< 16KB of Data Flash
#elif BSP_CFG_MCU_PART_MEMORY_SIZE == 0x0C
    #define BSP_ROM_SIZE_BYTES              (1048576)   ///< 1MB of ROM
    #define BSP_RAM_SIZE_BYTES              (196608)    ///< 192KB of RAM
    #define BSP_DATA_FLASH_SIZE_BYTES       (16384)     ///< 16KB of Data Flash
#elif BSP_CFG_MCU_PART_MEMORY_SIZE == 0x0E
    #define BSP_ROM_SIZE_BYTES              (2097152)   ///< 2MB of ROM
    #define BSP_RAM_SIZE_BYTES              (524288)    ///< 512KB of RAM
    #define BSP_DATA_FLASH_SIZE_BYTES       (65536)     ///< 64KB of Data Flash
#elif BSP_CFG_MCU_PART_MEMORY_SIZE == 0x10
    #define BSP_ROM_SIZE_BYTES              (3145728)   ///< 3MB of ROM
    #define BSP_RAM_SIZE_BYTES              (655360)    ///< 640KB of RAM
    #define BSP_DATA_FLASH_SIZE_BYTES       (65536)     ///< 64KB of Data Flash
#elif BSP_CFG_MCU_PART_MEMORY_SIZE == 0x11
    #define BSP_ROM_SIZE_BYTES              (4194304)   ///< 4MB of ROM
    #define BSP_RAM_SIZE_BYTES              (655360)    ///< 640KB of RAM
    #define BSP_DATA_FLASH_SIZE_BYTES       (65536)     ///< 64KB of Data Flash
#endif

/* Package and pin count. */
#if   BSP_CFG_MCU_PART_PACKAGE == 0x0
    #define BSP_PACKAGE_BGA
    #define BSP_PACKAGE_PINS        (121)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x1
    #define BSP_PACKAGE_BGA
    #define BSP_PACKAGE_PINS        (176)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x2
    #define BSP_PACKAGE_BGA
    #define BSP_PACKAGE_PINS        (224)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x3
    #define BSP_PACKAGE_LQFP
    #define BSP_PACKAGE_PINS        (48)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x4
    #define BSP_PACKAGE_LQFP
    #define BSP_PACKAGE_PINS        (64)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x5
    #define BSP_PACKAGE_LQFP
    #define BSP_PACKAGE_PINS        (100)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x6
    #define BSP_PACKAGE_LQFP
    #define BSP_PACKAGE_PINS        (144)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x7
    #define BSP_PACKAGE_LQFP
    #define BSP_PACKAGE_PINS        (176)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x8
    #define BSP_PACKAGE_LGA
    #define BSP_PACKAGE_PINS        (36)
#elif BSP_CFG_MCU_PART_PACKAGE == 0x9
    #define BSP_PACKAGE_LGA
    #define BSP_PACKAGE_PINS        (100)
#elif BSP_CFG_MCU_PART_PACKAGE == 0xA
    #define BSP_PACKAGE_LGA
    #define BSP_PACKAGE_PINS        (100)
#elif BSP_CFG_MCU_PART_PACKAGE == 0xB
    #define BSP_PACKAGE_LGA
    #define BSP_PACKAGE_PINS        (145)
#elif BSP_CFG_MCU_PART_PACKAGE == 0xC
    #define BSP_PACKAGE_WQFN
    #define BSP_PACKAGE_PINS        (40)
#elif BSP_CFG_MCU_PART_PACKAGE == 0xD
    #define BSP_PACKAGE_WQFN
    #define BSP_PACKAGE_PINS        (48)
#elif BSP_CFG_MCU_PART_PACKAGE == 0xE
    #define BSP_PACKAGE_WQFN
    #define BSP_PACKAGE_PINS        (64)
#endif

/* These macros define clock speeds for fixed speed clocks. */
#define BSP_LOCO_HZ                 (32768)         ///< Low-speed On-Chip Oscillator
#define BSP_MOCO_HZ                 (8000000)       ///< Middle-speed On-Chip Oscillator
#define BSP_SUB_CLOCK_HZ            (32768)         ///< Sub-Clock Oscillator

#if   BSP_CFG_HOCO_FREQUENCY == 0
    #define BSP_HOCO_HZ                 (16000000)      ///< High-speed On-Chip Oscillator
#elif BSP_CFG_HOCO_FREQUENCY == 1
    #define BSP_HOCO_HZ                 (18000000)      ///< High-speed On-Chip Oscillator
#elif BSP_CFG_HOCO_FREQUENCY == 2
    #define BSP_HOCO_HZ                 (20000000)      ///< High-speed On-Chip Oscillator
#else
    #error "Invalid HOCO frequency chosen (BSP_CFG_HOCO_FREQUENCY) in bsp_clock_cfg.h"
#endif

/* Number of NVIC slots. Includes 16 fixed exception vectors at the beginning. */
#define BSP_VECTOR_TABLE_ENTRIES    (112)

/* This MCU supports VBATT. */
#define BSP_MCU_VBATT_SUPPORT       (1)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* BSP_MCU_INFO_H_ */


