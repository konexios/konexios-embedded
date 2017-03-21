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
 * File Name    : hw_icu_private.h
 * Description  : Lower level driver interface to ICU.  Register setting and reading is done here.
 **********************************************************************************************************************/

#ifndef HW_ICU_PRIVATE_H
#define HW_ICU_PRIVATE_H

#if defined(__GNUC__) && defined(__ARM_EABI__)
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdbool.h>
#include "r_icu.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if BSP_MCU_MOCKED
#undef __STATIC_INLINE
#define __STATIC_INLINE
#ifndef BSP_MAX_NUM_IRQn
#define BSP_MAX_NUM_IRQn (-1)
#endif
#endif

#define ICU_MAX_CH                 (16)
#define INTERRUPT_PRIORITY_DEFAULT (3)

/* Bit vector of available channels */
#define ICU_BUTTON_AVAILABLE_CHANNELS (0xFFFF)
#define ICU_BUTTON_MAX_NUM_CHANNELS   (16)

/* Check to make sure selected channels are valid */
#define ICU_BUTTON_CH_BITMASK(ch) (1 << (ch))
#define ICU_BUTTON_CHECK_CHANNEL(ch)                                   \
                                  (ICU_BUTTON_CFG_CH ## ch ## _USED && \
                                      !(ICU_BUTTON_CH_BITMASK(ch) & ICU_BUTTON_AVAILABLE_CHANNELS))
#if ICU_BUTTON_CHECK_CHANNEL(0)
#error "ICU_BUTTON Channel 0 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(1)
#error "ICU_BUTTON Channel 1 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(2)
#error "ICU_BUTTON Channel 2 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(3)
#error "ICU_BUTTON Channel 3 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(4)
#error "ICU_BUTTON Channel 4 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(5)
#error "ICU_BUTTON Channel 5 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(6)
#error "ICU_BUTTON Channel 6 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(7)
#error "ICU_BUTTON Channel 7 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(8)
#error "ICU_BUTTON Channel 8 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(9)
#error "ICU_BUTTON Channel 9 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(10)
#error "ICU_BUTTON Channel 10 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(11)
#error "ICU_BUTTON Channel 11 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(12)
#error "ICU_BUTTON Channel 12 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(13)
#error "ICU_BUTTON Channel 13 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(14)
#error "ICU_BUTTON Channel 14 not available on this part"
#endif
#if ICU_BUTTON_CHECK_CHANNEL(15)
#error "ICU_BUTTON Channel 15 not available on this part"
#endif

/* Total number of channels used */
#define ICU_BUTTON_NUM_USED                                                           \
    (ICU_BUTTON_CFG_CH0_USED + ICU_BUTTON_CFG_CH1_USED + ICU_BUTTON_CFG_CH2_USED      \
     + ICU_BUTTON_CFG_CH3_USED + ICU_BUTTON_CFG_CH4_USED + ICU_BUTTON_CFG_CH5_USED    \
     + ICU_BUTTON_CFG_CH6_USED + ICU_BUTTON_CFG_CH7_USED + ICU_BUTTON_CFG_CH8_USED    \
     + ICU_BUTTON_CFG_CH9_USED + ICU_BUTTON_CFG_CH10_USED + ICU_BUTTON_CFG_CH11_USED  \
     + ICU_BUTTON_CFG_CH12_USED + ICU_BUTTON_CFG_CH13_USED + ICU_BUTTON_CFG_CH14_USED \
     + ICU_BUTTON_CFG_CH15_USED)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
__STATIC_INLINE void      HW_ICU_DivisorSet (uint8_t ch, external_irq_pclk_div_t pclk_divisor);

__STATIC_INLINE void      HW_ICU_FilterEnable (uint8_t ch);

__STATIC_INLINE void      HW_ICU_FilterDisable (uint8_t ch);

__STATIC_INLINE IRQn_Type HW_ICU_IRQGet (uint8_t ch);

__STATIC_INLINE void      HW_ICU_TriggerSet (uint8_t ch, external_irq_trigger_t trigger);

void icu_irq0_isr (void);
void icu_irq1_isr (void);
void icu_irq2_isr (void);
void icu_irq3_isr (void);
void icu_irq4_isr (void);
void icu_irq5_isr (void);
void icu_irq6_isr (void);
void icu_irq7_isr (void);
void icu_irq8_isr (void);
void icu_irq9_isr (void);
void icu_irq10_isr (void);
void icu_irq11_isr (void);
void icu_irq12_isr (void);
void icu_irq13_isr (void);
void icu_irq14_isr (void);
void icu_irq15_isr (void);




/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Select implementation based on BSP here */
#include "common/hw_icu_common.h"
#endif /* HW_ICU_PRIVATE_H */
