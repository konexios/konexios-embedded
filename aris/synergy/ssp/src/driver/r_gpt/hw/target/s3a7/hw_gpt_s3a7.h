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
 * File Name    : hw_gpt_s3a7.h
 * Description  : GPT S3A7 specific macros.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup GPT
 * @{
 **********************************************************************************************************************/

#ifndef HW_GPT_S3A7_H_
#define HW_GPT_S3A7_H_

#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define GPT_REG_SIZE (0x100UL)

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Pointer to GPT channels */
#ifdef R_GPTA0_BASE
#define GPT_CH(channel) (*(R_GPTA0_Type *) (R_GPTA0_BASE + (GPT_REG_SIZE * (channel))))
#elif defined(R_GPTB0_BASE)
#define GPT_CH(channel) (*(R_GPTB0_Type *) (R_GPTB0_BASE + (GPT_REG_SIZE * (channel))))
#endif

static const IRQn_Type irq_lookup[GPT_MAX_NUM_CHANNELS] =
{
#if BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT0_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT1_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT2_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT3_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT4_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT5_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT6_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT7_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT8_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
    GPT9_COUNTER_OVERFLOW_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
};

static const IRQn_Type irq_captureA_lookup[GPT_MAX_NUM_CHANNELS] =
{
#if BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT0_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT1_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT2_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT3_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT4_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT5_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT6_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT7_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT8_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
#if BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED
        GPT9_CAPTURE_COMPARE_A_IRQn,
#else
        BSP_MAX_NUM_IRQn,
#endif
};

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/


#endif /* HW_GPT_S3A7_H_ */

/*******************************************************************************************************************//**
 * @} (end addtogroup GPT)
 **********************************************************************************************************************/
