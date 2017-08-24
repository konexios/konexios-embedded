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
* File Name    : bsp_irq.c
* Description  : This module configures certain ELC events so that they can trigger NVIC interrupts.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

#if defined(BSP_MCU_GROUP_S7G2)

/** ELC event definitions. */
#include "r_elc_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Used for accessing bitfields in IELSR registers. */
typedef struct
{
    __IO uint32_t  IELS       :  9;               /* [0..8] Event selection to NVIC */
         uint32_t  res0       :  7;
    __IO uint32_t  IR         :  1;               /* [16..16] Interrupt Status Flag */
         uint32_t  res1       :  7;
    __IO uint32_t  DTCE       :  1;               /* [24..24] DTC Activation Enable */
         uint32_t  res2       :  7;
} bsp_prv_ielsr_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_IRQ
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Clear the interrupt status flag (IR) for a given interrupt. When an interrupt is triggered the IR bit
 *        is set. If it is not cleared in the ISR then the interrupt will trigger again immediately.
 *
 * @param[in] irq Interrupt for which to clear the IR bit.
 *
 * @note This does not work for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
void R_BSP_IrqStatusClear (IRQn_Type irq)
{
    /* This does not work for system exceptions where the IRQn_Type value is < 0 */
    if (((int32_t)irq) >= 0)
    {
        /* Clear the IR bit in the selected IELSR register. */
        ((bsp_prv_ielsr_t *)&R_ICU->IELSRn)[(uint32_t)irq].IR = 0UL;
    }
}

/** @} (end addtogroup BSP_MCU_IRQ) */

/*******************************************************************************************************************//**
 * @brief This function will initialize the ICU so that certain ELC events will trigger interrupts in the NVIC.
 *        It will also set the NVIC priority levels for all enabled interrupts. Which ELC events are enabled and
 *        what priority levels are used depends on the macro settings in bsp_irq_cfg.h.
 *
 * @note This does not work for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
void bsp_irq_cfg (void)
{
    uint32_t * base_addr;

    base_addr = (uint32_t *)&R_ICU->IELSRn;

    /* This next line does not emit any code. It is used to suppress compiler warnings about base_addr not being
     * used when no interrupts are enabled. */
    ((void)(base_addr));

#if (BSP_IRQ_CFG_ICU_IRQ0 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ0_IRQn) = ELC_EVENT_ICU_IRQ0;
    NVIC_SetPriority(ICU_IRQ0_IRQn, BSP_IRQ_CFG_ICU_IRQ0);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ1_IRQn) = ELC_EVENT_ICU_IRQ1;
    NVIC_SetPriority(ICU_IRQ1_IRQn, BSP_IRQ_CFG_ICU_IRQ1);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ2 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ2_IRQn) = ELC_EVENT_ICU_IRQ2;
    NVIC_SetPriority(ICU_IRQ2_IRQn, BSP_IRQ_CFG_ICU_IRQ2);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ3 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ3_IRQn) = ELC_EVENT_ICU_IRQ3;
    NVIC_SetPriority(ICU_IRQ3_IRQn, BSP_IRQ_CFG_ICU_IRQ3);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ4 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ4_IRQn) = ELC_EVENT_ICU_IRQ4;
    NVIC_SetPriority(ICU_IRQ4_IRQn, BSP_IRQ_CFG_ICU_IRQ4);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ5 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ5_IRQn) = ELC_EVENT_ICU_IRQ5;
    NVIC_SetPriority(ICU_IRQ5_IRQn, BSP_IRQ_CFG_ICU_IRQ5);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ6 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ6_IRQn) = ELC_EVENT_ICU_IRQ6;
    NVIC_SetPriority(ICU_IRQ6_IRQn, BSP_IRQ_CFG_ICU_IRQ6);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ7 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ7_IRQn) = ELC_EVENT_ICU_IRQ7;
    NVIC_SetPriority(ICU_IRQ7_IRQn, BSP_IRQ_CFG_ICU_IRQ7);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ8 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ8_IRQn) = ELC_EVENT_ICU_IRQ8;
    NVIC_SetPriority(ICU_IRQ8_IRQn, BSP_IRQ_CFG_ICU_IRQ8);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ9 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ9_IRQn) = ELC_EVENT_ICU_IRQ9;
    NVIC_SetPriority(ICU_IRQ9_IRQn, BSP_IRQ_CFG_ICU_IRQ9);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ10 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ10_IRQn) = ELC_EVENT_ICU_IRQ10;
    NVIC_SetPriority(ICU_IRQ10_IRQn, BSP_IRQ_CFG_ICU_IRQ10);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ11 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ11_IRQn) = ELC_EVENT_ICU_IRQ11;
    NVIC_SetPriority(ICU_IRQ11_IRQn, BSP_IRQ_CFG_ICU_IRQ11);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ12 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ12_IRQn) = ELC_EVENT_ICU_IRQ12;
    NVIC_SetPriority(ICU_IRQ12_IRQn, BSP_IRQ_CFG_ICU_IRQ12);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ13 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ13_IRQn) = ELC_EVENT_ICU_IRQ13;
    NVIC_SetPriority(ICU_IRQ13_IRQn, BSP_IRQ_CFG_ICU_IRQ13);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ14 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ14_IRQn) = ELC_EVENT_ICU_IRQ14;
    NVIC_SetPriority(ICU_IRQ14_IRQn, BSP_IRQ_CFG_ICU_IRQ14);
#endif
#if (BSP_IRQ_CFG_ICU_IRQ15 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_IRQ15_IRQn) = ELC_EVENT_ICU_IRQ15;
    NVIC_SetPriority(ICU_IRQ15_IRQn, BSP_IRQ_CFG_ICU_IRQ15);
#endif
#if (BSP_IRQ_CFG_DMAC0_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DMAC0_INT_IRQn) = ELC_EVENT_DMAC0_INT;
    NVIC_SetPriority(DMAC0_INT_IRQn, BSP_IRQ_CFG_DMAC0_INT);
#endif
#if (BSP_IRQ_CFG_DMAC1_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DMAC1_INT_IRQn) = ELC_EVENT_DMAC1_INT;
    NVIC_SetPriority(DMAC1_INT_IRQn, BSP_IRQ_CFG_DMAC1_INT);
#endif
#if (BSP_IRQ_CFG_DMAC2_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DMAC2_INT_IRQn) = ELC_EVENT_DMAC2_INT;
    NVIC_SetPriority(DMAC2_INT_IRQn, BSP_IRQ_CFG_DMAC2_INT);
#endif
#if (BSP_IRQ_CFG_DMAC3_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DMAC3_INT_IRQn) = ELC_EVENT_DMAC3_INT;
    NVIC_SetPriority(DMAC3_INT_IRQn, BSP_IRQ_CFG_DMAC3_INT);
#endif
#if (BSP_IRQ_CFG_DMAC4_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DMAC4_INT_IRQn) = ELC_EVENT_DMAC4_INT;
    NVIC_SetPriority(DMAC4_INT_IRQn, BSP_IRQ_CFG_DMAC4_INT);
#endif
#if (BSP_IRQ_CFG_DMAC5_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DMAC5_INT_IRQn) = ELC_EVENT_DMAC5_INT;
    NVIC_SetPriority(DMAC5_INT_IRQn, BSP_IRQ_CFG_DMAC5_INT);
#endif
#if (BSP_IRQ_CFG_DMAC6_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DMAC6_INT_IRQn) = ELC_EVENT_DMAC6_INT;
    NVIC_SetPriority(DMAC6_INT_IRQn, BSP_IRQ_CFG_DMAC6_INT);
#endif
#if (BSP_IRQ_CFG_DMAC7_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DMAC7_INT_IRQn) = ELC_EVENT_DMAC7_INT;
    NVIC_SetPriority(DMAC7_INT_IRQn, BSP_IRQ_CFG_DMAC7_INT);
#endif
#if (BSP_IRQ_CFG_DTC_COMPLETE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DTC_COMPLETE_IRQn) = ELC_EVENT_DTC_COMPLETE;
    NVIC_SetPriority(DTC_COMPLETE_IRQn, BSP_IRQ_CFG_DTC_COMPLETE);
#endif
#if (BSP_IRQ_CFG_DTC_END != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DTC_END_IRQn) = ELC_EVENT_DTC_END;
    NVIC_SetPriority(DTC_END_IRQn, BSP_IRQ_CFG_DTC_END);
#endif
#if (BSP_IRQ_CFG_ICU_SNOOZE_CANCEL != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ICU_SNOOZE_CANCEL_IRQn) = ELC_EVENT_ICU_SNOOZE_CANCEL;
    NVIC_SetPriority(ICU_SNOOZE_CANCEL_IRQn, BSP_IRQ_CFG_ICU_SNOOZE_CANCEL);
#endif
#if (BSP_IRQ_CFG_FCU_FIFERR != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)FCU_FIFERR_IRQn) = ELC_EVENT_FCU_FIFERR;
    NVIC_SetPriority(FCU_FIFERR_IRQn, BSP_IRQ_CFG_FCU_FIFERR);
#endif
#if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)FCU_FRDYI_IRQn) = ELC_EVENT_FCU_FRDYI;
    NVIC_SetPriority(FCU_FRDYI_IRQn, BSP_IRQ_CFG_FCU_FRDYI);
#endif
#if (BSP_IRQ_CFG_LVD_LVD1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)LVD_LVD1_IRQn) = ELC_EVENT_LVD_LVD1;
    NVIC_SetPriority(LVD_LVD1_IRQn, BSP_IRQ_CFG_LVD_LVD1);
#endif
#if (BSP_IRQ_CFG_LVD_LVD2 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)LVD_LVD2_IRQn) = ELC_EVENT_LVD_LVD2;
    NVIC_SetPriority(LVD_LVD2_IRQn, BSP_IRQ_CFG_LVD_LVD2);
#endif
#if (BSP_IRQ_CFG_CGC_MOSC_STOP != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CGC_MOSC_STOP_IRQn) = ELC_EVENT_CGC_MOSC_STOP;
    NVIC_SetPriority(CGC_MOSC_STOP_IRQn, BSP_IRQ_CFG_CGC_MOSC_STOP);
#endif
#if (BSP_IRQ_CFG_LPM_SNOOZE_REQUEST != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)LPM_SNOOZE_REQUEST_IRQn) = ELC_EVENT_LPM_SNOOZE_REQUEST;
    NVIC_SetPriority(LPM_SNOOZE_REQUEST_IRQn, BSP_IRQ_CFG_LPM_SNOOZE_REQUEST);
#endif
#if (BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)AGT0_INT_IRQn) = ELC_EVENT_AGT0_INT;
    NVIC_SetPriority(AGT0_INT_IRQn, BSP_IRQ_CFG_AGT0_INT);
#endif
#if (BSP_IRQ_CFG_AGT0_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)AGT0_COMPARE_A_IRQn) = ELC_EVENT_AGT0_COMPARE_A;
    NVIC_SetPriority(AGT0_COMPARE_A_IRQn, BSP_IRQ_CFG_AGT0_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_AGT0_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)AGT0_COMPARE_B_IRQn) = ELC_EVENT_AGT0_COMPARE_B;
    NVIC_SetPriority(AGT0_COMPARE_B_IRQn, BSP_IRQ_CFG_AGT0_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)AGT1_INT_IRQn) = ELC_EVENT_AGT1_INT;
    NVIC_SetPriority(AGT1_INT_IRQn, BSP_IRQ_CFG_AGT1_INT);
#endif
#if (BSP_IRQ_CFG_AGT1_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)AGT1_COMPARE_A_IRQn) = ELC_EVENT_AGT1_COMPARE_A;
    NVIC_SetPriority(AGT1_COMPARE_A_IRQn, BSP_IRQ_CFG_AGT1_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_AGT1_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)AGT1_COMPARE_B_IRQn) = ELC_EVENT_AGT1_COMPARE_B;
    NVIC_SetPriority(AGT1_COMPARE_B_IRQn, BSP_IRQ_CFG_AGT1_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_IWDT_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IWDT_UNDERFLOW_IRQn) = ELC_EVENT_IWDT_UNDERFLOW;
    NVIC_SetPriority(IWDT_UNDERFLOW_IRQn, BSP_IRQ_CFG_IWDT_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_WDT_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)WDT_UNDERFLOW_IRQn) = ELC_EVENT_WDT_UNDERFLOW;
    NVIC_SetPriority(WDT_UNDERFLOW_IRQn, BSP_IRQ_CFG_WDT_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)RTC_ALARM_IRQn) = ELC_EVENT_RTC_ALARM;
    NVIC_SetPriority(RTC_ALARM_IRQn, BSP_IRQ_CFG_RTC_ALARM);
#endif
#if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)RTC_PERIOD_IRQn) = ELC_EVENT_RTC_PERIOD;
    NVIC_SetPriority(RTC_PERIOD_IRQn, BSP_IRQ_CFG_RTC_PERIOD);
#endif
#if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)RTC_CARRY_IRQn) = ELC_EVENT_RTC_CARRY;
    NVIC_SetPriority(RTC_CARRY_IRQn, BSP_IRQ_CFG_RTC_CARRY);
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC0_SCAN_END_IRQn) = ELC_EVENT_ADC0_SCAN_END;
    NVIC_SetPriority(ADC0_SCAN_END_IRQn, BSP_IRQ_CFG_ADC0_SCAN_END);
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC0_SCAN_END_B_IRQn) = ELC_EVENT_ADC0_SCAN_END_B;
    NVIC_SetPriority(ADC0_SCAN_END_B_IRQn, BSP_IRQ_CFG_ADC0_SCAN_END_B);
#endif
#if (BSP_IRQ_CFG_ADC0_WINDOW_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC0_WINDOW_A_IRQn) = ELC_EVENT_ADC0_WINDOW_A;
    NVIC_SetPriority(ADC0_WINDOW_A_IRQn, BSP_IRQ_CFG_ADC0_WINDOW_A);
#endif
#if (BSP_IRQ_CFG_ADC0_WINDOW_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC0_WINDOW_B_IRQn) = ELC_EVENT_ADC0_WINDOW_B;
    NVIC_SetPriority(ADC0_WINDOW_B_IRQn, BSP_IRQ_CFG_ADC0_WINDOW_B);
#endif
#if (BSP_IRQ_CFG_ADC0_COMPARE_MATCH != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC0_COMPARE_MATCH_IRQn) = ELC_EVENT_ADC0_COMPARE_MATCH;
    NVIC_SetPriority(ADC0_COMPARE_MATCH_IRQn, BSP_IRQ_CFG_ADC0_COMPARE_MATCH);
#endif
#if (BSP_IRQ_CFG_ADC0_COMPARE_MISMATCH != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC0_COMPARE_MISMATCH_IRQn) = ELC_EVENT_ADC0_COMPARE_MISMATCH;
    NVIC_SetPriority(ADC0_COMPARE_MISMATCH_IRQn, BSP_IRQ_CFG_ADC0_COMPARE_MISMATCH);
#endif
#if (BSP_IRQ_CFG_ADC1_SCAN_END != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC1_SCAN_END_IRQn) = ELC_EVENT_ADC1_SCAN_END;
    NVIC_SetPriority(ADC1_SCAN_END_IRQn, BSP_IRQ_CFG_ADC1_SCAN_END);
#endif
#if (BSP_IRQ_CFG_ADC1_SCAN_END_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC1_SCAN_END_B_IRQn) = ELC_EVENT_ADC1_SCAN_END_B;
    NVIC_SetPriority(ADC1_SCAN_END_B_IRQn, BSP_IRQ_CFG_ADC1_SCAN_END_B);
#endif
#if (BSP_IRQ_CFG_ADC1_WINDOW_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC1_WINDOW_A_IRQn) = ELC_EVENT_ADC1_WINDOW_A;
    NVIC_SetPriority(ADC1_WINDOW_A_IRQn, BSP_IRQ_CFG_ADC1_WINDOW_A);
#endif
#if (BSP_IRQ_CFG_ADC1_WINDOW_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC1_WINDOW_B_IRQn) = ELC_EVENT_ADC1_WINDOW_B;
    NVIC_SetPriority(ADC1_WINDOW_B_IRQn, BSP_IRQ_CFG_ADC1_WINDOW_B);
#endif
#if (BSP_IRQ_CFG_ADC1_COMPARE_MATCH != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC1_COMPARE_MATCH_IRQn) = ELC_EVENT_ADC1_COMPARE_MATCH;
    NVIC_SetPriority(ADC1_COMPARE_MATCH_IRQn, BSP_IRQ_CFG_ADC1_COMPARE_MATCH);
#endif
#if (BSP_IRQ_CFG_ADC1_COMPARE_MISMATCH != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ADC1_COMPARE_MISMATCH_IRQn) = ELC_EVENT_ADC1_COMPARE_MISMATCH;
    NVIC_SetPriority(ADC1_COMPARE_MISMATCH_IRQn, BSP_IRQ_CFG_ADC1_COMPARE_MISMATCH);
#endif
#if (BSP_IRQ_CFG_COMP_HS_0 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)COMP_HS_0_IRQn) = ELC_EVENT_COMP_HS_0;
    NVIC_SetPriority(COMP_HS_0_IRQn, BSP_IRQ_CFG_COMP_HS_0);
#endif
#if (BSP_IRQ_CFG_COMP_HS_1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)COMP_HS_1_IRQn) = ELC_EVENT_COMP_HS_1;
    NVIC_SetPriority(COMP_HS_1_IRQn, BSP_IRQ_CFG_COMP_HS_1);
#endif
#if (BSP_IRQ_CFG_COMP_HS_2 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)COMP_HS_2_IRQn) = ELC_EVENT_COMP_HS_2;
    NVIC_SetPriority(COMP_HS_2_IRQn, BSP_IRQ_CFG_COMP_HS_2);
#endif
#if (BSP_IRQ_CFG_COMP_HS_3 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)COMP_HS_3_IRQn) = ELC_EVENT_COMP_HS_3;
    NVIC_SetPriority(COMP_HS_3_IRQn, BSP_IRQ_CFG_COMP_HS_3);
#endif
#if (BSP_IRQ_CFG_COMP_HS_4 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)COMP_HS_4_IRQn) = ELC_EVENT_COMP_HS_4;
    NVIC_SetPriority(COMP_HS_4_IRQn, BSP_IRQ_CFG_COMP_HS_4);
#endif
#if (BSP_IRQ_CFG_COMP_HS_5 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)COMP_HS_5_IRQn) = ELC_EVENT_COMP_HS_5;
    NVIC_SetPriority(COMP_HS_5_IRQn, BSP_IRQ_CFG_COMP_HS_5);
#endif
#if (BSP_IRQ_CFG_USBFS_FIFO_0 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)USBFS_FIFO_0_IRQn) = ELC_EVENT_USBFS_FIFO_0;
    NVIC_SetPriority(USBFS_FIFO_0_IRQn, BSP_IRQ_CFG_USBFS_FIFO_0);
#endif
#if (BSP_IRQ_CFG_USBFS_FIFO_1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)USBFS_FIFO_1_IRQn) = ELC_EVENT_USBFS_FIFO_1;
    NVIC_SetPriority(USBFS_FIFO_1_IRQn, BSP_IRQ_CFG_USBFS_FIFO_1);
#endif
#if (BSP_IRQ_CFG_USBFS_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)USBFS_INT_IRQn) = ELC_EVENT_USBFS_INT;
    NVIC_SetPriority(USBFS_INT_IRQn, BSP_IRQ_CFG_USBFS_INT);
#endif
#if (BSP_IRQ_CFG_USBFS_RESUME != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)USBFS_RESUME_IRQn) = ELC_EVENT_USBFS_RESUME;
    NVIC_SetPriority(USBFS_RESUME_IRQn, BSP_IRQ_CFG_USBFS_RESUME);
#endif
#if (BSP_IRQ_CFG_IIC0_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC0_RXI_IRQn) = ELC_EVENT_IIC0_RXI;
    NVIC_SetPriority(IIC0_RXI_IRQn, BSP_IRQ_CFG_IIC0_RXI);
#endif
#if (BSP_IRQ_CFG_IIC0_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC0_TXI_IRQn) = ELC_EVENT_IIC0_TXI;
    NVIC_SetPriority(IIC0_TXI_IRQn, BSP_IRQ_CFG_IIC0_TXI);
#endif
#if (BSP_IRQ_CFG_IIC0_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC0_TEI_IRQn) = ELC_EVENT_IIC0_TEI;
    NVIC_SetPriority(IIC0_TEI_IRQn, BSP_IRQ_CFG_IIC0_TEI);
#endif
#if (BSP_IRQ_CFG_IIC0_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC0_ERI_IRQn) = ELC_EVENT_IIC0_ERI;
    NVIC_SetPriority(IIC0_ERI_IRQn, BSP_IRQ_CFG_IIC0_ERI);
#endif
#if (BSP_IRQ_CFG_IIC0_WUI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC0_WUI_IRQn) = ELC_EVENT_IIC0_WUI;
    NVIC_SetPriority(IIC0_WUI_IRQn, BSP_IRQ_CFG_IIC0_WUI);
#endif
#if (BSP_IRQ_CFG_IIC1_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC1_RXI_IRQn) = ELC_EVENT_IIC1_RXI;
    NVIC_SetPriority(IIC1_RXI_IRQn, BSP_IRQ_CFG_IIC1_RXI);
#endif
#if (BSP_IRQ_CFG_IIC1_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC1_TXI_IRQn) = ELC_EVENT_IIC1_TXI;
    NVIC_SetPriority(IIC1_TXI_IRQn, BSP_IRQ_CFG_IIC1_TXI);
#endif
#if (BSP_IRQ_CFG_IIC1_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC1_TEI_IRQn) = ELC_EVENT_IIC1_TEI;
    NVIC_SetPriority(IIC1_TEI_IRQn, BSP_IRQ_CFG_IIC1_TEI);
#endif
#if (BSP_IRQ_CFG_IIC1_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC1_ERI_IRQn) = ELC_EVENT_IIC1_ERI;
    NVIC_SetPriority(IIC1_ERI_IRQn, BSP_IRQ_CFG_IIC1_ERI);
#endif
#if (BSP_IRQ_CFG_IIC2_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC2_RXI_IRQn) = ELC_EVENT_IIC2_RXI;
    NVIC_SetPriority(IIC2_RXI_IRQn, BSP_IRQ_CFG_IIC2_RXI);
#endif
#if (BSP_IRQ_CFG_IIC2_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC2_TXI_IRQn) = ELC_EVENT_IIC2_TXI;
    NVIC_SetPriority(IIC2_TXI_IRQn, BSP_IRQ_CFG_IIC2_TXI);
#endif
#if (BSP_IRQ_CFG_IIC2_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC2_TEI_IRQn) = ELC_EVENT_IIC2_TEI;
    NVIC_SetPriority(IIC2_TEI_IRQn, BSP_IRQ_CFG_IIC2_TEI);
#endif
#if (BSP_IRQ_CFG_IIC2_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IIC2_ERI_IRQn) = ELC_EVENT_IIC2_ERI;
    NVIC_SetPriority(IIC2_ERI_IRQn, BSP_IRQ_CFG_IIC2_ERI);
#endif
#if (BSP_IRQ_CFG_SSI0_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SSI0_TXI_IRQn) = ELC_EVENT_SSI0_TXI;
    NVIC_SetPriority(SSI0_TXI_IRQn, BSP_IRQ_CFG_SSI0_TXI);
#endif
#if (BSP_IRQ_CFG_SSI0_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SSI0_RXI_IRQn) = ELC_EVENT_SSI0_RXI;
    NVIC_SetPriority(SSI0_RXI_IRQn, BSP_IRQ_CFG_SSI0_RXI);
#endif
#if (BSP_IRQ_CFG_SSI0_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SSI0_INT_IRQn) = ELC_EVENT_SSI0_INT;
    NVIC_SetPriority(SSI0_INT_IRQn, BSP_IRQ_CFG_SSI0_INT);
#endif
#if (BSP_IRQ_CFG_SSI1_TXI_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SSI1_TXI_RXI_IRQn) = ELC_EVENT_SSI1_TXI_RXI;
    NVIC_SetPriority(SSI1_TXI_RXI_IRQn, BSP_IRQ_CFG_SSI1_TXI_RXI);
#endif
#if (BSP_IRQ_CFG_SSI1_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SSI1_INT_IRQn) = ELC_EVENT_SSI1_INT;
    NVIC_SetPriority(SSI1_INT_IRQn, BSP_IRQ_CFG_SSI1_INT);
#endif
#if (BSP_IRQ_CFG_SRC_INPUT_FIFO_EMPTY != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SRC_INPUT_FIFO_EMPTY_IRQn) = ELC_EVENT_SRC_INPUT_FIFO_EMPTY;
    NVIC_SetPriority(SRC_INPUT_FIFO_EMPTY_IRQn, BSP_IRQ_CFG_SRC_INPUT_FIFO_EMPTY);
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_FULL != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SRC_OUTPUT_FIFO_FULL_IRQn) = ELC_EVENT_SRC_OUTPUT_FIFO_FULL;
    NVIC_SetPriority(SRC_OUTPUT_FIFO_FULL_IRQn, BSP_IRQ_CFG_SRC_OUTPUT_FIFO_FULL);
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SRC_OUTPUT_FIFO_OVERFLOW_IRQn) = ELC_EVENT_SRC_OUTPUT_FIFO_OVERFLOW;
    NVIC_SetPriority(SRC_OUTPUT_FIFO_OVERFLOW_IRQn, BSP_IRQ_CFG_SRC_OUTPUT_FIFO_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SRC_OUTPUT_FIFO_UNDERFLOW_IRQn) = ELC_EVENT_SRC_OUTPUT_FIFO_UNDERFLOW;
    NVIC_SetPriority(SRC_OUTPUT_FIFO_UNDERFLOW_IRQn, BSP_IRQ_CFG_SRC_OUTPUT_FIFO_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_SRC_CONVERSION_END != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SRC_CONVERSION_END_IRQn) = ELC_EVENT_SRC_CONVERSION_END;
    NVIC_SetPriority(SRC_CONVERSION_END_IRQn, BSP_IRQ_CFG_SRC_CONVERSION_END);
#endif
#if (BSP_IRQ_CFG_PDC_RECEIVE_DATA_READY != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)PDC_RECEIVE_DATA_READY_IRQn) = ELC_EVENT_PDC_RECEIVE_DATA_READY;
    NVIC_SetPriority(PDC_RECEIVE_DATA_READY_IRQn, BSP_IRQ_CFG_PDC_RECEIVE_DATA_READY);
#endif
#if (BSP_IRQ_CFG_PDC_FRAME_END != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)PDC_FRAME_END_IRQn) = ELC_EVENT_PDC_FRAME_END;
    NVIC_SetPriority(PDC_FRAME_END_IRQn, BSP_IRQ_CFG_PDC_FRAME_END);
#endif
#if (BSP_IRQ_CFG_PDC_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)PDC_INT_IRQn) = ELC_EVENT_PDC_INT;
    NVIC_SetPriority(PDC_INT_IRQn, BSP_IRQ_CFG_PDC_INT);
#endif
#if (BSP_IRQ_CFG_CTSU_WRITE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CTSU_WRITE_IRQn) = ELC_EVENT_CTSU_WRITE;
    NVIC_SetPriority(CTSU_WRITE_IRQn, BSP_IRQ_CFG_CTSU_WRITE);
#endif
#if (BSP_IRQ_CFG_CTSU_READ != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CTSU_READ_IRQn) = ELC_EVENT_CTSU_READ;
    NVIC_SetPriority(CTSU_READ_IRQn, BSP_IRQ_CFG_CTSU_READ);
#endif
#if (BSP_IRQ_CFG_CTSU_END != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CTSU_END_IRQn) = ELC_EVENT_CTSU_END;
    NVIC_SetPriority(CTSU_END_IRQn, BSP_IRQ_CFG_CTSU_END);
#endif
#if (BSP_IRQ_CFG_KEY_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)KEY_INT_IRQn) = ELC_EVENT_KEY_INT;
    NVIC_SetPriority(KEY_INT_IRQn, BSP_IRQ_CFG_KEY_INT);
#endif
#if (BSP_IRQ_CFG_DOC_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DOC_INT_IRQn) = ELC_EVENT_DOC_INT;
    NVIC_SetPriority(DOC_INT_IRQn, BSP_IRQ_CFG_DOC_INT);
#endif
#if (BSP_IRQ_CFG_CAC_FREQUENCY_ERROR != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAC_FREQUENCY_ERROR_IRQn) = ELC_EVENT_CAC_FREQUENCY_ERROR;
    NVIC_SetPriority(CAC_FREQUENCY_ERROR_IRQn, BSP_IRQ_CFG_CAC_FREQUENCY_ERROR);
#endif
#if (BSP_IRQ_CFG_CAC_MEASUREMENT_END != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAC_MEASUREMENT_END_IRQn) = ELC_EVENT_CAC_MEASUREMENT_END;
    NVIC_SetPriority(CAC_MEASUREMENT_END_IRQn, BSP_IRQ_CFG_CAC_MEASUREMENT_END);
#endif
#if (BSP_IRQ_CFG_CAC_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAC_OVERFLOW_IRQn) = ELC_EVENT_CAC_OVERFLOW;
    NVIC_SetPriority(CAC_OVERFLOW_IRQn, BSP_IRQ_CFG_CAC_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_CAN0_ERROR != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN0_ERROR_IRQn) = ELC_EVENT_CAN0_ERROR;
    NVIC_SetPriority(CAN0_ERROR_IRQn, BSP_IRQ_CFG_CAN0_ERROR);
#endif
#if (BSP_IRQ_CFG_CAN0_FIFO_RX != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN0_FIFO_RX_IRQn) = ELC_EVENT_CAN0_FIFO_RX;
    NVIC_SetPriority(CAN0_FIFO_RX_IRQn, BSP_IRQ_CFG_CAN0_FIFO_RX);
#endif
#if (BSP_IRQ_CFG_CAN0_FIFO_TX != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN0_FIFO_TX_IRQn) = ELC_EVENT_CAN0_FIFO_TX;
    NVIC_SetPriority(CAN0_FIFO_TX_IRQn, BSP_IRQ_CFG_CAN0_FIFO_TX);
#endif
#if (BSP_IRQ_CFG_CAN0_MAILBOX_RX != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN0_MAILBOX_RX_IRQn) = ELC_EVENT_CAN0_MAILBOX_RX;
    NVIC_SetPriority(CAN0_MAILBOX_RX_IRQn, BSP_IRQ_CFG_CAN0_MAILBOX_RX);
#endif
#if (BSP_IRQ_CFG_CAN0_MAILBOX_TX != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN0_MAILBOX_TX_IRQn) = ELC_EVENT_CAN0_MAILBOX_TX;
    NVIC_SetPriority(CAN0_MAILBOX_TX_IRQn, BSP_IRQ_CFG_CAN0_MAILBOX_TX);
#endif
#if (BSP_IRQ_CFG_CAN1_ERROR != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN1_ERROR_IRQn) = ELC_EVENT_CAN1_ERROR;
    NVIC_SetPriority(CAN1_ERROR_IRQn, BSP_IRQ_CFG_CAN1_ERROR);
#endif
#if (BSP_IRQ_CFG_CAN1_FIFO_RX != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN1_FIFO_RX_IRQn) = ELC_EVENT_CAN1_FIFO_RX;
    NVIC_SetPriority(CAN1_FIFO_RX_IRQn, BSP_IRQ_CFG_CAN1_FIFO_RX);
#endif
#if (BSP_IRQ_CFG_CAN1_FIFO_TX != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN1_FIFO_TX_IRQn) = ELC_EVENT_CAN1_FIFO_TX;
    NVIC_SetPriority(CAN1_FIFO_TX_IRQn, BSP_IRQ_CFG_CAN1_FIFO_TX);
#endif
#if (BSP_IRQ_CFG_CAN1_MAILBOX_RX != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN1_MAILBOX_RX_IRQn) = ELC_EVENT_CAN1_MAILBOX_RX;
    NVIC_SetPriority(CAN1_MAILBOX_RX_IRQn, BSP_IRQ_CFG_CAN1_MAILBOX_RX);
#endif
#if (BSP_IRQ_CFG_CAN1_MAILBOX_TX != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)CAN1_MAILBOX_TX_IRQn) = ELC_EVENT_CAN1_MAILBOX_TX;
    NVIC_SetPriority(CAN1_MAILBOX_TX_IRQn, BSP_IRQ_CFG_CAN1_MAILBOX_TX);
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IOPORT_EVENT_1_IRQn) = ELC_EVENT_IOPORT_EVENT_1;
    NVIC_SetPriority(IOPORT_EVENT_1_IRQn, BSP_IRQ_CFG_IOPORT_EVENT_1);
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_2 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IOPORT_EVENT_2_IRQn) = ELC_EVENT_IOPORT_EVENT_2;
    NVIC_SetPriority(IOPORT_EVENT_2_IRQn, BSP_IRQ_CFG_IOPORT_EVENT_2);
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_3 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IOPORT_EVENT_3_IRQn) = ELC_EVENT_IOPORT_EVENT_3;
    NVIC_SetPriority(IOPORT_EVENT_3_IRQn, BSP_IRQ_CFG_IOPORT_EVENT_3);
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_4 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)IOPORT_EVENT_4_IRQn) = ELC_EVENT_IOPORT_EVENT_4;
    NVIC_SetPriority(IOPORT_EVENT_4_IRQn, BSP_IRQ_CFG_IOPORT_EVENT_4);
#endif
#if (BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ELC_SOFTWARE_EVENT_0_IRQn) = ELC_EVENT_ELC_SOFTWARE_EVENT_0;
    NVIC_SetPriority(ELC_SOFTWARE_EVENT_0_IRQn, BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0);
#endif
#if (BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)ELC_SOFTWARE_EVENT_1_IRQn) = ELC_EVENT_ELC_SOFTWARE_EVENT_1;
    NVIC_SetPriority(ELC_SOFTWARE_EVENT_1_IRQn, BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_1);
#endif
#if (BSP_IRQ_CFG_POEG0_EVENT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)POEG0_EVENT_IRQn) = ELC_EVENT_POEG0_EVENT;
    NVIC_SetPriority(POEG0_EVENT_IRQn, BSP_IRQ_CFG_POEG0_EVENT);
#endif
#if (BSP_IRQ_CFG_POEG1_EVENT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)POEG1_EVENT_IRQn) = ELC_EVENT_POEG1_EVENT;
    NVIC_SetPriority(POEG1_EVENT_IRQn, BSP_IRQ_CFG_POEG1_EVENT);
#endif
#if (BSP_IRQ_CFG_POEG2_EVENT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)POEG2_EVENT_IRQn) = ELC_EVENT_POEG2_EVENT;
    NVIC_SetPriority(POEG2_EVENT_IRQn, BSP_IRQ_CFG_POEG2_EVENT);
#endif
#if (BSP_IRQ_CFG_POEG3_EVENT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)POEG3_EVENT_IRQn) = ELC_EVENT_POEG3_EVENT;
    NVIC_SetPriority(POEG3_EVENT_IRQn, BSP_IRQ_CFG_POEG3_EVENT);
#endif
#if (BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT0_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT0_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT0_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT0_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_COMPARE_C_IRQn) = ELC_EVENT_GPT0_COMPARE_C;
    NVIC_SetPriority(GPT0_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT0_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_COMPARE_D_IRQn) = ELC_EVENT_GPT0_COMPARE_D;
    NVIC_SetPriority(GPT0_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT0_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_COMPARE_E_IRQn) = ELC_EVENT_GPT0_COMPARE_E;
    NVIC_SetPriority(GPT0_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT0_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_COMPARE_F_IRQn) = ELC_EVENT_GPT0_COMPARE_F;
    NVIC_SetPriority(GPT0_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT0_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT0_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT0_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT0_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT0_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT0_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT0_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT0_AD_TRIG_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_AD_TRIG_A_IRQn) = ELC_EVENT_GPT0_AD_TRIG_A;
    NVIC_SetPriority(GPT0_AD_TRIG_A_IRQn, BSP_IRQ_CFG_GPT0_AD_TRIG_A);
#endif
#if (BSP_IRQ_CFG_GPT0_AD_TRIG_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT0_AD_TRIG_B_IRQn) = ELC_EVENT_GPT0_AD_TRIG_B;
    NVIC_SetPriority(GPT0_AD_TRIG_B_IRQn, BSP_IRQ_CFG_GPT0_AD_TRIG_B);
#endif
#if (BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT1_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT1_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT1_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT1_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_COMPARE_C_IRQn) = ELC_EVENT_GPT1_COMPARE_C;
    NVIC_SetPriority(GPT1_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT1_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_COMPARE_D_IRQn) = ELC_EVENT_GPT1_COMPARE_D;
    NVIC_SetPriority(GPT1_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT1_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_COMPARE_E_IRQn) = ELC_EVENT_GPT1_COMPARE_E;
    NVIC_SetPriority(GPT1_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT1_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_COMPARE_F_IRQn) = ELC_EVENT_GPT1_COMPARE_F;
    NVIC_SetPriority(GPT1_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT1_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT1_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT1_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT1_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT1_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT1_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT1_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT1_AD_TRIG_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_AD_TRIG_A_IRQn) = ELC_EVENT_GPT1_AD_TRIG_A;
    NVIC_SetPriority(GPT1_AD_TRIG_A_IRQn, BSP_IRQ_CFG_GPT1_AD_TRIG_A);
#endif
#if (BSP_IRQ_CFG_GPT1_AD_TRIG_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT1_AD_TRIG_B_IRQn) = ELC_EVENT_GPT1_AD_TRIG_B;
    NVIC_SetPriority(GPT1_AD_TRIG_B_IRQn, BSP_IRQ_CFG_GPT1_AD_TRIG_B);
#endif
#if (BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT2_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT2_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT2_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT2_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_COMPARE_C_IRQn) = ELC_EVENT_GPT2_COMPARE_C;
    NVIC_SetPriority(GPT2_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT2_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_COMPARE_D_IRQn) = ELC_EVENT_GPT2_COMPARE_D;
    NVIC_SetPriority(GPT2_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT2_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_COMPARE_E_IRQn) = ELC_EVENT_GPT2_COMPARE_E;
    NVIC_SetPriority(GPT2_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT2_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_COMPARE_F_IRQn) = ELC_EVENT_GPT2_COMPARE_F;
    NVIC_SetPriority(GPT2_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT2_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT2_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT2_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT2_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT2_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT2_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT2_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT2_AD_TRIG_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_AD_TRIG_A_IRQn) = ELC_EVENT_GPT2_AD_TRIG_A;
    NVIC_SetPriority(GPT2_AD_TRIG_A_IRQn, BSP_IRQ_CFG_GPT2_AD_TRIG_A);
#endif
#if (BSP_IRQ_CFG_GPT2_AD_TRIG_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT2_AD_TRIG_B_IRQn) = ELC_EVENT_GPT2_AD_TRIG_B;
    NVIC_SetPriority(GPT2_AD_TRIG_B_IRQn, BSP_IRQ_CFG_GPT2_AD_TRIG_B);
#endif
#if (BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT3_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT3_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT3_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT3_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_COMPARE_C_IRQn) = ELC_EVENT_GPT3_COMPARE_C;
    NVIC_SetPriority(GPT3_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT3_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_COMPARE_D_IRQn) = ELC_EVENT_GPT3_COMPARE_D;
    NVIC_SetPriority(GPT3_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT3_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_COMPARE_E_IRQn) = ELC_EVENT_GPT3_COMPARE_E;
    NVIC_SetPriority(GPT3_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT3_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_COMPARE_F_IRQn) = ELC_EVENT_GPT3_COMPARE_F;
    NVIC_SetPriority(GPT3_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT3_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT3_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT3_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT3_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT3_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT3_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT3_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT3_AD_TRIG_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_AD_TRIG_A_IRQn) = ELC_EVENT_GPT3_AD_TRIG_A;
    NVIC_SetPriority(GPT3_AD_TRIG_A_IRQn, BSP_IRQ_CFG_GPT3_AD_TRIG_A);
#endif
#if (BSP_IRQ_CFG_GPT3_AD_TRIG_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT3_AD_TRIG_B_IRQn) = ELC_EVENT_GPT3_AD_TRIG_B;
    NVIC_SetPriority(GPT3_AD_TRIG_B_IRQn, BSP_IRQ_CFG_GPT3_AD_TRIG_B);
#endif
#if (BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT4_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT4_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT4_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT4_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_COMPARE_C_IRQn) = ELC_EVENT_GPT4_COMPARE_C;
    NVIC_SetPriority(GPT4_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT4_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_COMPARE_D_IRQn) = ELC_EVENT_GPT4_COMPARE_D;
    NVIC_SetPriority(GPT4_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT4_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_COMPARE_E_IRQn) = ELC_EVENT_GPT4_COMPARE_E;
    NVIC_SetPriority(GPT4_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT4_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_COMPARE_F_IRQn) = ELC_EVENT_GPT4_COMPARE_F;
    NVIC_SetPriority(GPT4_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT4_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT4_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT4_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT4_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT4_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT4_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT4_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT4_AD_TRIG_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_AD_TRIG_A_IRQn) = ELC_EVENT_GPT4_AD_TRIG_A;
    NVIC_SetPriority(GPT4_AD_TRIG_A_IRQn, BSP_IRQ_CFG_GPT4_AD_TRIG_A);
#endif
#if (BSP_IRQ_CFG_GPT4_AD_TRIG_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT4_AD_TRIG_B_IRQn) = ELC_EVENT_GPT4_AD_TRIG_B;
    NVIC_SetPriority(GPT4_AD_TRIG_B_IRQn, BSP_IRQ_CFG_GPT4_AD_TRIG_B);
#endif
#if (BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT5_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT5_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT5_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT5_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_COMPARE_C_IRQn) = ELC_EVENT_GPT5_COMPARE_C;
    NVIC_SetPriority(GPT5_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT5_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_COMPARE_D_IRQn) = ELC_EVENT_GPT5_COMPARE_D;
    NVIC_SetPriority(GPT5_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT5_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_COMPARE_E_IRQn) = ELC_EVENT_GPT5_COMPARE_E;
    NVIC_SetPriority(GPT5_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT5_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_COMPARE_F_IRQn) = ELC_EVENT_GPT5_COMPARE_F;
    NVIC_SetPriority(GPT5_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT5_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT5_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT5_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT5_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT5_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT5_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT5_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT5_AD_TRIG_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_AD_TRIG_A_IRQn) = ELC_EVENT_GPT5_AD_TRIG_A;
    NVIC_SetPriority(GPT5_AD_TRIG_A_IRQn, BSP_IRQ_CFG_GPT5_AD_TRIG_A);
#endif
#if (BSP_IRQ_CFG_GPT5_AD_TRIG_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT5_AD_TRIG_B_IRQn) = ELC_EVENT_GPT5_AD_TRIG_B;
    NVIC_SetPriority(GPT5_AD_TRIG_B_IRQn, BSP_IRQ_CFG_GPT5_AD_TRIG_B);
#endif
#if (BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT6_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT6_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT6_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT6_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_COMPARE_C_IRQn) = ELC_EVENT_GPT6_COMPARE_C;
    NVIC_SetPriority(GPT6_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT6_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_COMPARE_D_IRQn) = ELC_EVENT_GPT6_COMPARE_D;
    NVIC_SetPriority(GPT6_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT6_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_COMPARE_E_IRQn) = ELC_EVENT_GPT6_COMPARE_E;
    NVIC_SetPriority(GPT6_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT6_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_COMPARE_F_IRQn) = ELC_EVENT_GPT6_COMPARE_F;
    NVIC_SetPriority(GPT6_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT6_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT6_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT6_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT6_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT6_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT6_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT6_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT6_AD_TRIG_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_AD_TRIG_A_IRQn) = ELC_EVENT_GPT6_AD_TRIG_A;
    NVIC_SetPriority(GPT6_AD_TRIG_A_IRQn, BSP_IRQ_CFG_GPT6_AD_TRIG_A);
#endif
#if (BSP_IRQ_CFG_GPT6_AD_TRIG_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT6_AD_TRIG_B_IRQn) = ELC_EVENT_GPT6_AD_TRIG_B;
    NVIC_SetPriority(GPT6_AD_TRIG_B_IRQn, BSP_IRQ_CFG_GPT6_AD_TRIG_B);
#endif
#if (BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT7_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT7_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT7_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT7_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_COMPARE_C_IRQn) = ELC_EVENT_GPT7_COMPARE_C;
    NVIC_SetPriority(GPT7_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT7_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_COMPARE_D_IRQn) = ELC_EVENT_GPT7_COMPARE_D;
    NVIC_SetPriority(GPT7_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT7_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_COMPARE_E_IRQn) = ELC_EVENT_GPT7_COMPARE_E;
    NVIC_SetPriority(GPT7_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT7_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_COMPARE_F_IRQn) = ELC_EVENT_GPT7_COMPARE_F;
    NVIC_SetPriority(GPT7_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT7_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT7_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT7_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT7_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT7_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT7_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT7_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT7_AD_TRIG_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_AD_TRIG_A_IRQn) = ELC_EVENT_GPT7_AD_TRIG_A;
    NVIC_SetPriority(GPT7_AD_TRIG_A_IRQn, BSP_IRQ_CFG_GPT7_AD_TRIG_A);
#endif
#if (BSP_IRQ_CFG_GPT7_AD_TRIG_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT7_AD_TRIG_B_IRQn) = ELC_EVENT_GPT7_AD_TRIG_B;
    NVIC_SetPriority(GPT7_AD_TRIG_B_IRQn, BSP_IRQ_CFG_GPT7_AD_TRIG_B);
#endif
#if (BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT8_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT8_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT8_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT8_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT8_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT8_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT8_COMPARE_C_IRQn) = ELC_EVENT_GPT8_COMPARE_C;
    NVIC_SetPriority(GPT8_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT8_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT8_COMPARE_D_IRQn) = ELC_EVENT_GPT8_COMPARE_D;
    NVIC_SetPriority(GPT8_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT8_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT8_COMPARE_E_IRQn) = ELC_EVENT_GPT8_COMPARE_E;
    NVIC_SetPriority(GPT8_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT8_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT8_COMPARE_F_IRQn) = ELC_EVENT_GPT8_COMPARE_F;
    NVIC_SetPriority(GPT8_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT8_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT8_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT8_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT8_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT8_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT8_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT8_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT8_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT8_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT9_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT9_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT9_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT9_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT9_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT9_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT9_COMPARE_C_IRQn) = ELC_EVENT_GPT9_COMPARE_C;
    NVIC_SetPriority(GPT9_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT9_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT9_COMPARE_D_IRQn) = ELC_EVENT_GPT9_COMPARE_D;
    NVIC_SetPriority(GPT9_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT9_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT9_COMPARE_E_IRQn) = ELC_EVENT_GPT9_COMPARE_E;
    NVIC_SetPriority(GPT9_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT9_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT9_COMPARE_F_IRQn) = ELC_EVENT_GPT9_COMPARE_F;
    NVIC_SetPriority(GPT9_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT9_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT9_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT9_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT9_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT9_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT9_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT9_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT9_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT9_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT10_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT10_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT10_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT10_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT10_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT10_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT10_COMPARE_C_IRQn) = ELC_EVENT_GPT10_COMPARE_C;
    NVIC_SetPriority(GPT10_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT10_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT10_COMPARE_D_IRQn) = ELC_EVENT_GPT10_COMPARE_D;
    NVIC_SetPriority(GPT10_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT10_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT10_COMPARE_E_IRQn) = ELC_EVENT_GPT10_COMPARE_E;
    NVIC_SetPriority(GPT10_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT10_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT10_COMPARE_F_IRQn) = ELC_EVENT_GPT10_COMPARE_F;
    NVIC_SetPriority(GPT10_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT10_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT10_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT10_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT10_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT10_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT10_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT10_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT10_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT10_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT11_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT11_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT11_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT11_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT11_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT11_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT11_COMPARE_C_IRQn) = ELC_EVENT_GPT11_COMPARE_C;
    NVIC_SetPriority(GPT11_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT11_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT11_COMPARE_D_IRQn) = ELC_EVENT_GPT11_COMPARE_D;
    NVIC_SetPriority(GPT11_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT11_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT11_COMPARE_E_IRQn) = ELC_EVENT_GPT11_COMPARE_E;
    NVIC_SetPriority(GPT11_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT11_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT11_COMPARE_F_IRQn) = ELC_EVENT_GPT11_COMPARE_F;
    NVIC_SetPriority(GPT11_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT11_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT11_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT11_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT11_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT11_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT11_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT11_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT11_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT11_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT12_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT12_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT12_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT12_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT12_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT12_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT12_COMPARE_C_IRQn) = ELC_EVENT_GPT12_COMPARE_C;
    NVIC_SetPriority(GPT12_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT12_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT12_COMPARE_D_IRQn) = ELC_EVENT_GPT12_COMPARE_D;
    NVIC_SetPriority(GPT12_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT12_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT12_COMPARE_E_IRQn) = ELC_EVENT_GPT12_COMPARE_E;
    NVIC_SetPriority(GPT12_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT12_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT12_COMPARE_F_IRQn) = ELC_EVENT_GPT12_COMPARE_F;
    NVIC_SetPriority(GPT12_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT12_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT12_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT12_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT12_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT12_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT12_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT12_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT12_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT12_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT13_CAPTURE_COMPARE_A_IRQn) = ELC_EVENT_GPT13_CAPTURE_COMPARE_A;
    NVIC_SetPriority(GPT13_CAPTURE_COMPARE_A_IRQn, BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_A);
#endif
#if (BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT13_CAPTURE_COMPARE_B_IRQn) = ELC_EVENT_GPT13_CAPTURE_COMPARE_B;
    NVIC_SetPriority(GPT13_CAPTURE_COMPARE_B_IRQn, BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_B);
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_C != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT13_COMPARE_C_IRQn) = ELC_EVENT_GPT13_COMPARE_C;
    NVIC_SetPriority(GPT13_COMPARE_C_IRQn, BSP_IRQ_CFG_GPT13_COMPARE_C);
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_D != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT13_COMPARE_D_IRQn) = ELC_EVENT_GPT13_COMPARE_D;
    NVIC_SetPriority(GPT13_COMPARE_D_IRQn, BSP_IRQ_CFG_GPT13_COMPARE_D);
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_E != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT13_COMPARE_E_IRQn) = ELC_EVENT_GPT13_COMPARE_E;
    NVIC_SetPriority(GPT13_COMPARE_E_IRQn, BSP_IRQ_CFG_GPT13_COMPARE_E);
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_F != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT13_COMPARE_F_IRQn) = ELC_EVENT_GPT13_COMPARE_F;
    NVIC_SetPriority(GPT13_COMPARE_F_IRQn, BSP_IRQ_CFG_GPT13_COMPARE_F);
#endif
#if (BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT13_COUNTER_OVERFLOW_IRQn) = ELC_EVENT_GPT13_COUNTER_OVERFLOW;
    NVIC_SetPriority(GPT13_COUNTER_OVERFLOW_IRQn, BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW);
#endif
#if (BSP_IRQ_CFG_GPT13_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GPT13_COUNTER_UNDERFLOW_IRQn) = ELC_EVENT_GPT13_COUNTER_UNDERFLOW;
    NVIC_SetPriority(GPT13_COUNTER_UNDERFLOW_IRQn, BSP_IRQ_CFG_GPT13_COUNTER_UNDERFLOW);
#endif
#if (BSP_IRQ_CFG_OPS_UVW_EDGE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)OPS_UVW_EDGE_IRQn) = ELC_EVENT_OPS_UVW_EDGE;
    NVIC_SetPriority(OPS_UVW_EDGE_IRQn, BSP_IRQ_CFG_OPS_UVW_EDGE);
#endif
#if (BSP_IRQ_CFG_EPTPC_IPLS != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_IPLS_IRQn) = ELC_EVENT_EPTPC_IPLS;
    NVIC_SetPriority(EPTPC_IPLS_IRQn, BSP_IRQ_CFG_EPTPC_IPLS);
#endif
#if (BSP_IRQ_CFG_EPTPC_MINT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_MINT_IRQn) = ELC_EVENT_EPTPC_MINT;
    NVIC_SetPriority(EPTPC_MINT_IRQn, BSP_IRQ_CFG_EPTPC_MINT);
#endif
#if (BSP_IRQ_CFG_EPTPC_PINT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_PINT_IRQn) = ELC_EVENT_EPTPC_PINT;
    NVIC_SetPriority(EPTPC_PINT_IRQn, BSP_IRQ_CFG_EPTPC_PINT);
#endif
#if (BSP_IRQ_CFG_EDMAC0_EINT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EDMAC0_EINT_IRQn) = ELC_EVENT_EDMAC0_EINT;
    NVIC_SetPriority(EDMAC0_EINT_IRQn, BSP_IRQ_CFG_EDMAC0_EINT);
#endif
#if (BSP_IRQ_CFG_EDMAC1_EINT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EDMAC1_EINT_IRQn) = ELC_EVENT_EDMAC1_EINT;
    NVIC_SetPriority(EDMAC1_EINT_IRQn, BSP_IRQ_CFG_EDMAC1_EINT);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER0_RISE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER0_RISE_IRQn) = ELC_EVENT_EPTPC_TIMER0_RISE;
    NVIC_SetPriority(EPTPC_TIMER0_RISE_IRQn, BSP_IRQ_CFG_EPTPC_TIMER0_RISE);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER1_RISE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER1_RISE_IRQn) = ELC_EVENT_EPTPC_TIMER1_RISE;
    NVIC_SetPriority(EPTPC_TIMER1_RISE_IRQn, BSP_IRQ_CFG_EPTPC_TIMER1_RISE);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER2_RISE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER2_RISE_IRQn) = ELC_EVENT_EPTPC_TIMER2_RISE;
    NVIC_SetPriority(EPTPC_TIMER2_RISE_IRQn, BSP_IRQ_CFG_EPTPC_TIMER2_RISE);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER3_RISE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER3_RISE_IRQn) = ELC_EVENT_EPTPC_TIMER3_RISE;
    NVIC_SetPriority(EPTPC_TIMER3_RISE_IRQn, BSP_IRQ_CFG_EPTPC_TIMER3_RISE);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER4_RISE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER4_RISE_IRQn) = ELC_EVENT_EPTPC_TIMER4_RISE;
    NVIC_SetPriority(EPTPC_TIMER4_RISE_IRQn, BSP_IRQ_CFG_EPTPC_TIMER4_RISE);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER5_RISE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER5_RISE_IRQn) = ELC_EVENT_EPTPC_TIMER5_RISE;
    NVIC_SetPriority(EPTPC_TIMER5_RISE_IRQn, BSP_IRQ_CFG_EPTPC_TIMER5_RISE);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER0_FALL != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER0_FALL_IRQn) = ELC_EVENT_EPTPC_TIMER0_FALL;
    NVIC_SetPriority(EPTPC_TIMER0_FALL_IRQn, BSP_IRQ_CFG_EPTPC_TIMER0_FALL);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER1_FALL != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER1_FALL_IRQn) = ELC_EVENT_EPTPC_TIMER1_FALL;
    NVIC_SetPriority(EPTPC_TIMER1_FALL_IRQn, BSP_IRQ_CFG_EPTPC_TIMER1_FALL);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER2_FALL != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER2_FALL_IRQn) = ELC_EVENT_EPTPC_TIMER2_FALL;
    NVIC_SetPriority(EPTPC_TIMER2_FALL_IRQn, BSP_IRQ_CFG_EPTPC_TIMER2_FALL);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER3_FALL != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER3_FALL_IRQn) = ELC_EVENT_EPTPC_TIMER3_FALL;
    NVIC_SetPriority(EPTPC_TIMER3_FALL_IRQn, BSP_IRQ_CFG_EPTPC_TIMER3_FALL);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER4_FALL != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER4_FALL_IRQn) = ELC_EVENT_EPTPC_TIMER4_FALL;
    NVIC_SetPriority(EPTPC_TIMER4_FALL_IRQn, BSP_IRQ_CFG_EPTPC_TIMER4_FALL);
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER5_FALL != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)EPTPC_TIMER5_FALL_IRQn) = ELC_EVENT_EPTPC_TIMER5_FALL;
    NVIC_SetPriority(EPTPC_TIMER5_FALL_IRQn, BSP_IRQ_CFG_EPTPC_TIMER5_FALL);
#endif
#if (BSP_IRQ_CFG_USBHS_FIFO_0 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)USBHS_FIFO_0_IRQn) = ELC_EVENT_USBHS_FIFO_0;
    NVIC_SetPriority(USBHS_FIFO_0_IRQn, BSP_IRQ_CFG_USBHS_FIFO_0);
#endif
#if (BSP_IRQ_CFG_USBHS_FIFO_1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)USBHS_FIFO_1_IRQn) = ELC_EVENT_USBHS_FIFO_1;
    NVIC_SetPriority(USBHS_FIFO_1_IRQn, BSP_IRQ_CFG_USBHS_FIFO_1);
#endif
#if (BSP_IRQ_CFG_USBHS_USB_INT_RESUME != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)USBHS_USB_INT_RESUME_IRQn) = ELC_EVENT_USBHS_USB_INT_RESUME;
    NVIC_SetPriority(USBHS_USB_INT_RESUME_IRQn, BSP_IRQ_CFG_USBHS_USB_INT_RESUME);
#endif
#if (BSP_IRQ_CFG_SCI0_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI0_RXI_IRQn) = ELC_EVENT_SCI0_RXI;
    NVIC_SetPriority(SCI0_RXI_IRQn, BSP_IRQ_CFG_SCI0_RXI);
#endif
#if (BSP_IRQ_CFG_SCI0_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI0_TXI_IRQn) = ELC_EVENT_SCI0_TXI;
    NVIC_SetPriority(SCI0_TXI_IRQn, BSP_IRQ_CFG_SCI0_TXI);
#endif
#if (BSP_IRQ_CFG_SCI0_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI0_TEI_IRQn) = ELC_EVENT_SCI0_TEI;
    NVIC_SetPriority(SCI0_TEI_IRQn, BSP_IRQ_CFG_SCI0_TEI);
#endif
#if (BSP_IRQ_CFG_SCI0_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI0_ERI_IRQn) = ELC_EVENT_SCI0_ERI;
    NVIC_SetPriority(SCI0_ERI_IRQn, BSP_IRQ_CFG_SCI0_ERI);
#endif
#if (BSP_IRQ_CFG_SCI0_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI0_AM_IRQn) = ELC_EVENT_SCI0_AM;
    NVIC_SetPriority(SCI0_AM_IRQn, BSP_IRQ_CFG_SCI0_AM);
#endif
#if (BSP_IRQ_CFG_SCI0_RXI_OR_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI0_RXI_OR_ERI_IRQn) = ELC_EVENT_SCI0_RXI_OR_ERI;
    NVIC_SetPriority(SCI0_RXI_OR_ERI_IRQn, BSP_IRQ_CFG_SCI0_RXI_OR_ERI);
#endif
#if (BSP_IRQ_CFG_SCI1_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI1_RXI_IRQn) = ELC_EVENT_SCI1_RXI;
    NVIC_SetPriority(SCI1_RXI_IRQn, BSP_IRQ_CFG_SCI1_RXI);
#endif
#if (BSP_IRQ_CFG_SCI1_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI1_TXI_IRQn) = ELC_EVENT_SCI1_TXI;
    NVIC_SetPriority(SCI1_TXI_IRQn, BSP_IRQ_CFG_SCI1_TXI);
#endif
#if (BSP_IRQ_CFG_SCI1_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI1_TEI_IRQn) = ELC_EVENT_SCI1_TEI;
    NVIC_SetPriority(SCI1_TEI_IRQn, BSP_IRQ_CFG_SCI1_TEI);
#endif
#if (BSP_IRQ_CFG_SCI1_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI1_ERI_IRQn) = ELC_EVENT_SCI1_ERI;
    NVIC_SetPriority(SCI1_ERI_IRQn, BSP_IRQ_CFG_SCI1_ERI);
#endif
#if (BSP_IRQ_CFG_SCI1_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI1_AM_IRQn) = ELC_EVENT_SCI1_AM;
    NVIC_SetPriority(SCI1_AM_IRQn, BSP_IRQ_CFG_SCI1_AM);
#endif
#if (BSP_IRQ_CFG_SCI2_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI2_RXI_IRQn) = ELC_EVENT_SCI2_RXI;
    NVIC_SetPriority(SCI2_RXI_IRQn, BSP_IRQ_CFG_SCI2_RXI);
#endif
#if (BSP_IRQ_CFG_SCI2_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI2_TXI_IRQn) = ELC_EVENT_SCI2_TXI;
    NVIC_SetPriority(SCI2_TXI_IRQn, BSP_IRQ_CFG_SCI2_TXI);
#endif
#if (BSP_IRQ_CFG_SCI2_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI2_TEI_IRQn) = ELC_EVENT_SCI2_TEI;
    NVIC_SetPriority(SCI2_TEI_IRQn, BSP_IRQ_CFG_SCI2_TEI);
#endif
#if (BSP_IRQ_CFG_SCI2_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI2_ERI_IRQn) = ELC_EVENT_SCI2_ERI;
    NVIC_SetPriority(SCI2_ERI_IRQn, BSP_IRQ_CFG_SCI2_ERI);
#endif
#if (BSP_IRQ_CFG_SCI2_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI2_AM_IRQn) = ELC_EVENT_SCI2_AM;
    NVIC_SetPriority(SCI2_AM_IRQn, BSP_IRQ_CFG_SCI2_AM);
#endif
#if (BSP_IRQ_CFG_SCI3_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI3_RXI_IRQn) = ELC_EVENT_SCI3_RXI;
    NVIC_SetPriority(SCI3_RXI_IRQn, BSP_IRQ_CFG_SCI3_RXI);
#endif
#if (BSP_IRQ_CFG_SCI3_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI3_TXI_IRQn) = ELC_EVENT_SCI3_TXI;
    NVIC_SetPriority(SCI3_TXI_IRQn, BSP_IRQ_CFG_SCI3_TXI);
#endif
#if (BSP_IRQ_CFG_SCI3_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI3_TEI_IRQn) = ELC_EVENT_SCI3_TEI;
    NVIC_SetPriority(SCI3_TEI_IRQn, BSP_IRQ_CFG_SCI3_TEI);
#endif
#if (BSP_IRQ_CFG_SCI3_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI3_ERI_IRQn) = ELC_EVENT_SCI3_ERI;
    NVIC_SetPriority(SCI3_ERI_IRQn, BSP_IRQ_CFG_SCI3_ERI);
#endif
#if (BSP_IRQ_CFG_SCI3_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI3_AM_IRQn) = ELC_EVENT_SCI3_AM;
    NVIC_SetPriority(SCI3_AM_IRQn, BSP_IRQ_CFG_SCI3_AM);
#endif
#if (BSP_IRQ_CFG_SCI4_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI4_RXI_IRQn) = ELC_EVENT_SCI4_RXI;
    NVIC_SetPriority(SCI4_RXI_IRQn, BSP_IRQ_CFG_SCI4_RXI);
#endif
#if (BSP_IRQ_CFG_SCI4_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI4_TXI_IRQn) = ELC_EVENT_SCI4_TXI;
    NVIC_SetPriority(SCI4_TXI_IRQn, BSP_IRQ_CFG_SCI4_TXI);
#endif
#if (BSP_IRQ_CFG_SCI4_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI4_TEI_IRQn) = ELC_EVENT_SCI4_TEI;
    NVIC_SetPriority(SCI4_TEI_IRQn, BSP_IRQ_CFG_SCI4_TEI);
#endif
#if (BSP_IRQ_CFG_SCI4_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI4_ERI_IRQn) = ELC_EVENT_SCI4_ERI;
    NVIC_SetPriority(SCI4_ERI_IRQn, BSP_IRQ_CFG_SCI4_ERI);
#endif
#if (BSP_IRQ_CFG_SCI4_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI4_AM_IRQn) = ELC_EVENT_SCI4_AM;
    NVIC_SetPriority(SCI4_AM_IRQn, BSP_IRQ_CFG_SCI4_AM);
#endif
#if (BSP_IRQ_CFG_SCI5_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI5_RXI_IRQn) = ELC_EVENT_SCI5_RXI;
    NVIC_SetPriority(SCI5_RXI_IRQn, BSP_IRQ_CFG_SCI5_RXI);
#endif
#if (BSP_IRQ_CFG_SCI5_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI5_TXI_IRQn) = ELC_EVENT_SCI5_TXI;
    NVIC_SetPriority(SCI5_TXI_IRQn, BSP_IRQ_CFG_SCI5_TXI);
#endif
#if (BSP_IRQ_CFG_SCI5_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI5_TEI_IRQn) = ELC_EVENT_SCI5_TEI;
    NVIC_SetPriority(SCI5_TEI_IRQn, BSP_IRQ_CFG_SCI5_TEI);
#endif
#if (BSP_IRQ_CFG_SCI5_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI5_ERI_IRQn) = ELC_EVENT_SCI5_ERI;
    NVIC_SetPriority(SCI5_ERI_IRQn, BSP_IRQ_CFG_SCI5_ERI);
#endif
#if (BSP_IRQ_CFG_SCI5_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI5_AM_IRQn) = ELC_EVENT_SCI5_AM;
    NVIC_SetPriority(SCI5_AM_IRQn, BSP_IRQ_CFG_SCI5_AM);
#endif
#if (BSP_IRQ_CFG_SCI6_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI6_RXI_IRQn) = ELC_EVENT_SCI6_RXI;
    NVIC_SetPriority(SCI6_RXI_IRQn, BSP_IRQ_CFG_SCI6_RXI);
#endif
#if (BSP_IRQ_CFG_SCI6_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI6_TXI_IRQn) = ELC_EVENT_SCI6_TXI;
    NVIC_SetPriority(SCI6_TXI_IRQn, BSP_IRQ_CFG_SCI6_TXI);
#endif
#if (BSP_IRQ_CFG_SCI6_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI6_TEI_IRQn) = ELC_EVENT_SCI6_TEI;
    NVIC_SetPriority(SCI6_TEI_IRQn, BSP_IRQ_CFG_SCI6_TEI);
#endif
#if (BSP_IRQ_CFG_SCI6_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI6_ERI_IRQn) = ELC_EVENT_SCI6_ERI;
    NVIC_SetPriority(SCI6_ERI_IRQn, BSP_IRQ_CFG_SCI6_ERI);
#endif
#if (BSP_IRQ_CFG_SCI6_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI6_AM_IRQn) = ELC_EVENT_SCI6_AM;
    NVIC_SetPriority(SCI6_AM_IRQn, BSP_IRQ_CFG_SCI6_AM);
#endif
#if (BSP_IRQ_CFG_SCI7_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI7_RXI_IRQn) = ELC_EVENT_SCI7_RXI;
    NVIC_SetPriority(SCI7_RXI_IRQn, BSP_IRQ_CFG_SCI7_RXI);
#endif
#if (BSP_IRQ_CFG_SCI7_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI7_TXI_IRQn) = ELC_EVENT_SCI7_TXI;
    NVIC_SetPriority(SCI7_TXI_IRQn, BSP_IRQ_CFG_SCI7_TXI);
#endif
#if (BSP_IRQ_CFG_SCI7_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI7_TEI_IRQn) = ELC_EVENT_SCI7_TEI;
    NVIC_SetPriority(SCI7_TEI_IRQn, BSP_IRQ_CFG_SCI7_TEI);
#endif
#if (BSP_IRQ_CFG_SCI7_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI7_ERI_IRQn) = ELC_EVENT_SCI7_ERI;
    NVIC_SetPriority(SCI7_ERI_IRQn, BSP_IRQ_CFG_SCI7_ERI);
#endif
#if (BSP_IRQ_CFG_SCI7_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI7_AM_IRQn) = ELC_EVENT_SCI7_AM;
    NVIC_SetPriority(SCI7_AM_IRQn, BSP_IRQ_CFG_SCI7_AM);
#endif
#if (BSP_IRQ_CFG_SCI8_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI8_RXI_IRQn) = ELC_EVENT_SCI8_RXI;
    NVIC_SetPriority(SCI8_RXI_IRQn, BSP_IRQ_CFG_SCI8_RXI);
#endif
#if (BSP_IRQ_CFG_SCI8_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI8_TXI_IRQn) = ELC_EVENT_SCI8_TXI;
    NVIC_SetPriority(SCI8_TXI_IRQn, BSP_IRQ_CFG_SCI8_TXI);
#endif
#if (BSP_IRQ_CFG_SCI8_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI8_TEI_IRQn) = ELC_EVENT_SCI8_TEI;
    NVIC_SetPriority(SCI8_TEI_IRQn, BSP_IRQ_CFG_SCI8_TEI);
#endif
#if (BSP_IRQ_CFG_SCI8_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI8_ERI_IRQn) = ELC_EVENT_SCI8_ERI;
    NVIC_SetPriority(SCI8_ERI_IRQn, BSP_IRQ_CFG_SCI8_ERI);
#endif
#if (BSP_IRQ_CFG_SCI8_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI8_AM_IRQn) = ELC_EVENT_SCI8_AM;
    NVIC_SetPriority(SCI8_AM_IRQn, BSP_IRQ_CFG_SCI8_AM);
#endif
#if (BSP_IRQ_CFG_SCI9_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI9_RXI_IRQn) = ELC_EVENT_SCI9_RXI;
    NVIC_SetPriority(SCI9_RXI_IRQn, BSP_IRQ_CFG_SCI9_RXI);
#endif
#if (BSP_IRQ_CFG_SCI9_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI9_TXI_IRQn) = ELC_EVENT_SCI9_TXI;
    NVIC_SetPriority(SCI9_TXI_IRQn, BSP_IRQ_CFG_SCI9_TXI);
#endif
#if (BSP_IRQ_CFG_SCI9_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI9_TEI_IRQn) = ELC_EVENT_SCI9_TEI;
    NVIC_SetPriority(SCI9_TEI_IRQn, BSP_IRQ_CFG_SCI9_TEI);
#endif
#if (BSP_IRQ_CFG_SCI9_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI9_ERI_IRQn) = ELC_EVENT_SCI9_ERI;
    NVIC_SetPriority(SCI9_ERI_IRQn, BSP_IRQ_CFG_SCI9_ERI);
#endif
#if (BSP_IRQ_CFG_SCI9_AM != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCI9_AM_IRQn) = ELC_EVENT_SCI9_AM;
    NVIC_SetPriority(SCI9_AM_IRQn, BSP_IRQ_CFG_SCI9_AM);
#endif
#if (BSP_IRQ_CFG_SPI0_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI0_RXI_IRQn) = ELC_EVENT_SPI0_RXI;
    NVIC_SetPriority(SPI0_RXI_IRQn, BSP_IRQ_CFG_SPI0_RXI);
#endif
#if (BSP_IRQ_CFG_SPI0_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI0_TXI_IRQn) = ELC_EVENT_SPI0_TXI;
    NVIC_SetPriority(SPI0_TXI_IRQn, BSP_IRQ_CFG_SPI0_TXI);
#endif
#if (BSP_IRQ_CFG_SPI0_IDLE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI0_IDLE_IRQn) = ELC_EVENT_SPI0_IDLE;
    NVIC_SetPriority(SPI0_IDLE_IRQn, BSP_IRQ_CFG_SPI0_IDLE);
#endif
#if (BSP_IRQ_CFG_SPI0_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI0_ERI_IRQn) = ELC_EVENT_SPI0_ERI;
    NVIC_SetPriority(SPI0_ERI_IRQn, BSP_IRQ_CFG_SPI0_ERI);
#endif
#if (BSP_IRQ_CFG_SPI0_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI0_TEI_IRQn) = ELC_EVENT_SPI0_TEI;
    NVIC_SetPriority(SPI0_TEI_IRQn, BSP_IRQ_CFG_SPI0_TEI);
#endif
#if (BSP_IRQ_CFG_SPI1_RXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI1_RXI_IRQn) = ELC_EVENT_SPI1_RXI;
    NVIC_SetPriority(SPI1_RXI_IRQn, BSP_IRQ_CFG_SPI1_RXI);
#endif
#if (BSP_IRQ_CFG_SPI1_TXI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI1_TXI_IRQn) = ELC_EVENT_SPI1_TXI;
    NVIC_SetPriority(SPI1_TXI_IRQn, BSP_IRQ_CFG_SPI1_TXI);
#endif
#if (BSP_IRQ_CFG_SPI1_IDLE != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI1_IDLE_IRQn) = ELC_EVENT_SPI1_IDLE;
    NVIC_SetPriority(SPI1_IDLE_IRQn, BSP_IRQ_CFG_SPI1_IDLE);
#endif
#if (BSP_IRQ_CFG_SPI1_ERI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI1_ERI_IRQn) = ELC_EVENT_SPI1_ERI;
    NVIC_SetPriority(SPI1_ERI_IRQn, BSP_IRQ_CFG_SPI1_ERI);
#endif
#if (BSP_IRQ_CFG_SPI1_TEI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SPI1_TEI_IRQn) = ELC_EVENT_SPI1_TEI;
    NVIC_SetPriority(SPI1_TEI_IRQn, BSP_IRQ_CFG_SPI1_TEI);
#endif
#if (BSP_IRQ_CFG_QSPI_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)QSPI_INT_IRQn) = ELC_EVENT_QSPI_INT;
    NVIC_SetPriority(QSPI_INT_IRQn, BSP_IRQ_CFG_QSPI_INT);
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_ACCS != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SDHIMMC0_ACCS_IRQn) = ELC_EVENT_SDHIMMC0_ACCS;
    NVIC_SetPriority(SDHIMMC0_ACCS_IRQn, BSP_IRQ_CFG_SDHIMMC0_ACCS);
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_SDIO != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SDHIMMC0_SDIO_IRQn) = ELC_EVENT_SDHIMMC0_SDIO;
    NVIC_SetPriority(SDHIMMC0_SDIO_IRQn, BSP_IRQ_CFG_SDHIMMC0_SDIO);
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_CARD != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SDHIMMC0_CARD_IRQn) = ELC_EVENT_SDHIMMC0_CARD;
    NVIC_SetPriority(SDHIMMC0_CARD_IRQn, BSP_IRQ_CFG_SDHIMMC0_CARD);
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_DMA_REQ != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SDHIMMC0_DMA_REQ_IRQn) = ELC_EVENT_SDHIMMC0_DMA_REQ;
    NVIC_SetPriority(SDHIMMC0_DMA_REQ_IRQn, BSP_IRQ_CFG_SDHIMMC0_DMA_REQ);
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_ACCS != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SDHIMMC1_ACCS_IRQn) = ELC_EVENT_SDHIMMC1_ACCS;
    NVIC_SetPriority(SDHIMMC1_ACCS_IRQn, BSP_IRQ_CFG_SDHIMMC1_ACCS);
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_SDIO != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SDHIMMC1_SDIO_IRQn) = ELC_EVENT_SDHIMMC1_SDIO;
    NVIC_SetPriority(SDHIMMC1_SDIO_IRQn, BSP_IRQ_CFG_SDHIMMC1_SDIO);
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_CARD != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SDHIMMC1_CARD_IRQn) = ELC_EVENT_SDHIMMC1_CARD;
    NVIC_SetPriority(SDHIMMC1_CARD_IRQn, BSP_IRQ_CFG_SDHIMMC1_CARD);
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_DMA_REQ != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SDHIMMC1_DMA_REQ_IRQn) = ELC_EVENT_SDHIMMC1_DMA_REQ;
    NVIC_SetPriority(SDHIMMC1_DMA_REQ_IRQn, BSP_IRQ_CFG_SDHIMMC1_DMA_REQ);
#endif
#if (BSP_IRQ_CFG_SCE_PROC_BUSY != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_PROC_BUSY_IRQn) = ELC_EVENT_SCE_PROC_BUSY;
    NVIC_SetPriority(SCE_PROC_BUSY_IRQn, BSP_IRQ_CFG_SCE_PROC_BUSY);
#endif
#if (BSP_IRQ_CFG_SCE_ROMOK != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_ROMOK_IRQn) = ELC_EVENT_SCE_ROMOK;
    NVIC_SetPriority(SCE_ROMOK_IRQn, BSP_IRQ_CFG_SCE_ROMOK);
#endif
#if (BSP_IRQ_CFG_SCE_LONG_PLG != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_LONG_PLG_IRQn) = ELC_EVENT_SCE_LONG_PLG;
    NVIC_SetPriority(SCE_LONG_PLG_IRQn, BSP_IRQ_CFG_SCE_LONG_PLG);
#endif
#if (BSP_IRQ_CFG_SCE_TEST_BUSY != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_TEST_BUSY_IRQn) = ELC_EVENT_SCE_TEST_BUSY;
    NVIC_SetPriority(SCE_TEST_BUSY_IRQn, BSP_IRQ_CFG_SCE_TEST_BUSY);
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_0 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_WRRDY_0_IRQn) = ELC_EVENT_SCE_WRRDY_0;
    NVIC_SetPriority(SCE_WRRDY_0_IRQn, BSP_IRQ_CFG_SCE_WRRDY_0);
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_WRRDY_1_IRQn) = ELC_EVENT_SCE_WRRDY_1;
    NVIC_SetPriority(SCE_WRRDY_1_IRQn, BSP_IRQ_CFG_SCE_WRRDY_1);
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_4 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_WRRDY_4_IRQn) = ELC_EVENT_SCE_WRRDY_4;
    NVIC_SetPriority(SCE_WRRDY_4_IRQn, BSP_IRQ_CFG_SCE_WRRDY_4);
#endif
#if (BSP_IRQ_CFG_SCE_RDRDY_0 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_RDRDY_0_IRQn) = ELC_EVENT_SCE_RDRDY_0;
    NVIC_SetPriority(SCE_RDRDY_0_IRQn, BSP_IRQ_CFG_SCE_RDRDY_0);
#endif
#if (BSP_IRQ_CFG_SCE_RDRDY_1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_RDRDY_1_IRQn) = ELC_EVENT_SCE_RDRDY_1;
    NVIC_SetPriority(SCE_RDRDY_1_IRQn, BSP_IRQ_CFG_SCE_RDRDY_1);
#endif
#if (BSP_IRQ_CFG_SCE_INTEGRATE_WRRDY != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_INTEGRATE_WRRDY_IRQn) = ELC_EVENT_SCE_INTEGRATE_WRRDY;
    NVIC_SetPriority(SCE_INTEGRATE_WRRDY_IRQn, BSP_IRQ_CFG_SCE_INTEGRATE_WRRDY);
#endif
#if (BSP_IRQ_CFG_SCE_INTEGRATE_RDRDY != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)SCE_INTEGRATE_RDRDY_IRQn) = ELC_EVENT_SCE_INTEGRATE_RDRDY;
    NVIC_SetPriority(SCE_INTEGRATE_RDRDY_IRQn, BSP_IRQ_CFG_SCE_INTEGRATE_RDRDY);
#endif
#if (BSP_IRQ_CFG_GLCDC_LINE_DETECT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GLCDC_LINE_DETECT_IRQn) = ELC_EVENT_GLCDC_LINE_DETECT;
    NVIC_SetPriority(GLCDC_LINE_DETECT_IRQn, BSP_IRQ_CFG_GLCDC_LINE_DETECT);
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_1 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GLCDC_UNDERFLOW_1_IRQn) = ELC_EVENT_GLCDC_UNDERFLOW_1;
    NVIC_SetPriority(GLCDC_UNDERFLOW_1_IRQn, BSP_IRQ_CFG_GLCDC_UNDERFLOW_1);
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_2 != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)GLCDC_UNDERFLOW_2_IRQn) = ELC_EVENT_GLCDC_UNDERFLOW_2;
    NVIC_SetPriority(GLCDC_UNDERFLOW_2_IRQn, BSP_IRQ_CFG_GLCDC_UNDERFLOW_2);
#endif
#if (BSP_IRQ_CFG_DRW_INT != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)DRW_INT_IRQn) = ELC_EVENT_DRW_INT;
    NVIC_SetPriority(DRW_INT_IRQn, BSP_IRQ_CFG_DRW_INT);
#endif
#if (BSP_IRQ_CFG_JPEG_JEDI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)JPEG_JEDI_IRQn) = ELC_EVENT_JPEG_JEDI;
    NVIC_SetPriority(JPEG_JEDI_IRQn, BSP_IRQ_CFG_JPEG_JEDI);
#endif
#if (BSP_IRQ_CFG_JPEG_JDTI != BSP_IRQ_DISABLED)
    *(base_addr + (uint32_t)JPEG_JDTI_IRQn) = ELC_EVENT_JPEG_JDTI;
    NVIC_SetPriority(JPEG_JDTI_IRQn, BSP_IRQ_CFG_JPEG_JDTI);
#endif
}

#endif



