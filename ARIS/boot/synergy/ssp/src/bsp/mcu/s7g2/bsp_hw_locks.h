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
* File Name    : bsp_hw_locks.h
* Description  : This module contains common HW locks that can be used to atomically access MCU resources.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_LOCKING
 *
 * @{
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU_LOCKING) */

#ifndef BSP_HW_LOCKS_H_
#define BSP_HW_LOCKS_H_

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** This enum defines all of the available hardware locks for this MCU. If you delete an entry out of this list then you
   will decrease the size of the locks array but will not be able to use that lock. For example, if your design is not
   using CAN at all then you can safely remove the BSP_HW_LOCK_CAN# entries below. */
typedef enum e_bsp_hw_lock
{
    BSP_HW_LOCK_IRDA,
    BSP_HW_LOCK_QSPI,
    BSP_HW_LOCK_IIC0,
    BSP_HW_LOCK_IIC1,
    BSP_HW_LOCK_IIC2,
    BSP_HW_LOCK_RSPI0,
    BSP_HW_LOCK_RSPI1,
    BSP_HW_LOCK_SCI0,
    BSP_HW_LOCK_SCI1,
    BSP_HW_LOCK_SCI2,
    BSP_HW_LOCK_SCI3,
    BSP_HW_LOCK_SCI4,
    BSP_HW_LOCK_SCI5,
    BSP_HW_LOCK_SCI6,
    BSP_HW_LOCK_SCI7,
    BSP_HW_LOCK_SCI8,
    BSP_HW_LOCK_SCI9,
    BSP_HW_LOCK_CAN0,
    BSP_HW_LOCK_CAN1,
    BSP_HW_LOCK_DMAC0,
    BSP_HW_LOCK_DMAC1,
    BSP_HW_LOCK_DMAC2,
    BSP_HW_LOCK_DMAC3,
    BSP_HW_LOCK_DMAC4,
    BSP_HW_LOCK_DMAC5,
    BSP_HW_LOCK_DMAC6,
    BSP_HW_LOCK_DMAC7,
    BSP_HW_LOCK_DMA,
    BSP_HW_LOCK_DTC,
    BSP_HW_LOCK_ICU,
    BSP_HW_LOCK_ICU_IRQ0,
    BSP_HW_LOCK_ICU_IRQ1,
    BSP_HW_LOCK_ICU_IRQ2,
    BSP_HW_LOCK_ICU_IRQ3,
    BSP_HW_LOCK_ICU_IRQ4,
    BSP_HW_LOCK_ICU_IRQ5,
    BSP_HW_LOCK_ICU_IRQ6,
    BSP_HW_LOCK_ICU_IRQ7,
    BSP_HW_LOCK_ICU_IRQ8,
    BSP_HW_LOCK_ICU_IRQ9,
    BSP_HW_LOCK_ICU_IRQ10,
    BSP_HW_LOCK_ICU_IRQ11,
    BSP_HW_LOCK_ICU_IRQ12,
    BSP_HW_LOCK_ICU_IRQ13,
    BSP_HW_LOCK_ICU_IRQ14,
    BSP_HW_LOCK_ICU_IRQ15,
    BSP_HW_LOCK_MMF,
    BSP_HW_LOCK_SRAM,
    BSP_HW_LOCK_BUS,
    BSP_HW_LOCK_ROMC,
    BSP_HW_LOCK_EEPROM,
    BSP_HW_LOCK_FACI,
    BSP_HW_LOCK_EDMAC0,
    BSP_HW_LOCK_EDMAC1,
    BSP_HW_LOCK_ETHERC0,
    BSP_HW_LOCK_ETHERC1,
    BSP_HW_LOCK_PTPEDMAC,
    BSP_HW_LOCK_EPTPC,
    BSP_HW_LOCK_EPTPC0,
    BSP_HW_LOCK_EPTPC1,
    BSP_HW_LOCK_EPTPC_CFG,
    BSP_HW_LOCK_G2D,
    BSP_HW_LOCK_GLCD,
    BSP_HW_LOCK_JPEG,
    BSP_HW_LOCK_PDC,
    BSP_HW_LOCK_SRCRAM,
    BSP_HW_LOCK_SRC,
    BSP_HW_LOCK_USBHS,
    BSP_HW_LOCK_MSTP,
    BSP_HW_LOCK_GPT_OPS,
    BSP_HW_LOCK_GPT0_A,
    BSP_HW_LOCK_GPT1_A,
    BSP_HW_LOCK_GPT2_A,
    BSP_HW_LOCK_GPT3_A,
    BSP_HW_LOCK_GPT4_A,
    BSP_HW_LOCK_GPT5_A,
    BSP_HW_LOCK_GPT6_A,
    BSP_HW_LOCK_GPT7_A,
    BSP_HW_LOCK_GPT8_B,
    BSP_HW_LOCK_GPT9_B,
    BSP_HW_LOCK_GPT10_B,
    BSP_HW_LOCK_GPT11_B,
    BSP_HW_LOCK_GPT12_B,
    BSP_HW_LOCK_GPT13_B,
    BSP_HW_LOCK_POEG,
    BSP_HW_LOCK_GPT_ODC,
    BSP_HW_LOCK_RTC,
    BSP_HW_LOCK_AGT0,
    BSP_HW_LOCK_AGT1,
    BSP_HW_LOCK_COMP0,
    BSP_HW_LOCK_COMP1,
    BSP_HW_LOCK_COMP2,
    BSP_HW_LOCK_COMP3,
    BSP_HW_LOCK_COMP4,
    BSP_HW_LOCK_COMP5,
    BSP_HW_LOCK_CTSU,
    BSP_HW_LOCK_DAC0,
    BSP_HW_LOCK_DAC1,
    BSP_HW_LOCK_AMI,
    BSP_HW_LOCK_TSN,
    BSP_HW_LOCK_S12ADC0,
    BSP_HW_LOCK_S12ADC1,
    BSP_HW_LOCK_CRC,
    BSP_HW_LOCK_DOC,
    BSP_HW_LOCK_ELC,
    BSP_HW_LOCK_PMISC,
    BSP_HW_LOCK_IWDT,
    BSP_HW_LOCK_KINT,
    BSP_HW_LOCK_SSI0,
    BSP_HW_LOCK_SSI1,
    BSP_HW_LOCK_USBFS,
    BSP_HW_LOCK_WDT,
    BSP_HW_LOCK_CAC,
    BSP_HW_LOCK_SDHI0,
    BSP_HW_LOCK_SDHI1,
    BSP_HW_LOCK_MMPU,
    BSP_HW_LOCK_SMPU,
    BSP_HW_LOCK_SPMON,
    BSP_HW_LOCK_LVD1,
    BSP_HW_LOCK_LVD2,
    BSP_NUM_LOCKS //This entry is not a valid lock. It is used for sizing the HW lock array in the BSP. Do not touch!
} bsp_hw_lock_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* BSP_HW_LOCKS_H_ */




