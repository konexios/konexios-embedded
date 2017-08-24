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
 * File Name    : hw_icu_common.h
 * Description  : ICU register access functions.
 **********************************************************************************************************************/

#ifndef HW_ICU_COMMON_H
#define HW_ICU_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Register definitions */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro Definitions
 **********************************************************************************************************************/
static IRQn_Type irq_lookup[ICU_BUTTON_MAX_NUM_CHANNELS] =
{
#if defined(BSP_IRQ_CFG_ICU_IRQ0) && (BSP_IRQ_CFG_ICU_IRQ0 != BSP_IRQ_DISABLED)
    ICU_IRQ0_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ1) && (BSP_IRQ_CFG_ICU_IRQ1 != BSP_IRQ_DISABLED)
    ICU_IRQ1_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ2) && (BSP_IRQ_CFG_ICU_IRQ2 != BSP_IRQ_DISABLED)
    ICU_IRQ2_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ3) && (BSP_IRQ_CFG_ICU_IRQ3 != BSP_IRQ_DISABLED)
    ICU_IRQ3_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ4) && (BSP_IRQ_CFG_ICU_IRQ4 != BSP_IRQ_DISABLED)
    ICU_IRQ4_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ5) && (BSP_IRQ_CFG_ICU_IRQ5 != BSP_IRQ_DISABLED)
    ICU_IRQ5_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ6) && (BSP_IRQ_CFG_ICU_IRQ6 != BSP_IRQ_DISABLED)
    ICU_IRQ6_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ7) && (BSP_IRQ_CFG_ICU_IRQ7 != BSP_IRQ_DISABLED)
    ICU_IRQ7_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ8) && (BSP_IRQ_CFG_ICU_IRQ8 != BSP_IRQ_DISABLED)
    ICU_IRQ8_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ9) && (BSP_IRQ_CFG_ICU_IRQ9 != BSP_IRQ_DISABLED)
    ICU_IRQ9_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ10) && (BSP_IRQ_CFG_ICU_IRQ10 != BSP_IRQ_DISABLED)
    ICU_IRQ10_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ11) && (BSP_IRQ_CFG_ICU_IRQ11 != BSP_IRQ_DISABLED)
    ICU_IRQ11_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ12) && (BSP_IRQ_CFG_ICU_IRQ12 != BSP_IRQ_DISABLED)
    ICU_IRQ12_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ13) && (BSP_IRQ_CFG_ICU_IRQ13 != BSP_IRQ_DISABLED)
    ICU_IRQ13_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ14) && (BSP_IRQ_CFG_ICU_IRQ14 != BSP_IRQ_DISABLED)
    ICU_IRQ14_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if defined(BSP_IRQ_CFG_ICU_IRQ15) && (BSP_IRQ_CFG_ICU_IRQ15 != BSP_IRQ_DISABLED)
    ICU_IRQ15_IRQn
#else
    BSP_MAX_NUM_IRQn,
#endif
};

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
static inline void HW_ICU_DivisorSet (uint8_t ch, external_irq_pclk_div_t pclk_divisor)
{
    R_ICU->IRQCRn_b[ch].FCLKSEL = pclk_divisor;
}

static inline uint8_t HW_ICU_DivisorGet (uint8_t ch)
{
    return R_ICU->IRQCRn_b[ch].FCLKSEL;
}

static inline void HW_ICU_FilterDisable (uint8_t ch)
{
    R_ICU->IRQCRn_b[ch].FLTEN = 0;
}

static inline void HW_ICU_FilterEnable (uint8_t ch)
{
    R_ICU->IRQCRn_b[ch].FLTEN = 1;
}

static inline uint8_t HW_ICU_FilterGet (uint8_t ch)
{
    return R_ICU->IRQCRn_b[ch].FLTEN;
}

static inline IRQn_Type HW_ICU_IRQGet (uint8_t ch)
{
    return irq_lookup[ch];
}

static inline uint8_t HW_ICU_TriggerGet (uint8_t ch)
{
    return R_ICU->IRQCRn_b[ch].IRQMD;
}

static inline void HW_ICU_TriggerSet (uint8_t ch, external_irq_trigger_t trigger)
{
    R_ICU->IRQCRn_b[ch].IRQMD = trigger;
}
#endif /* HW_GPT_COMMON_H */
