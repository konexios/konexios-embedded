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
* File Name    : startup_S7G2.c
* Description  : Startup for the S7G2
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

/* Only build this file if this board is chosen. */
#if defined(BSP_MCU_GROUP_S7G2)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Defines function pointers to be used with vector table. */
typedef void (* exc_ptr_t)(void);

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
void Reset_Handler(void);
void Default_Handler(void);
int  main(void);

/***********************************************************************************************************************
* Function Name: Reset_Handler
* Description  : MCU starts executing here out of reset. Main stack pointer is setup already.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void Reset_Handler (void)
{
    /* Initialize system using BSP. */
    SystemInit();

    /* Call user application. */
    main();

    while (1)
    {
        /* Infinite Loop. */
    }
}

/***********************************************************************************************************************
* Function Name: Default_Handler
* Description  : Default exception handler.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void Default_Handler (void)
{
    while(1);
}

/* Stacks. */
/* Main stack */
/* IAR requires pragma for setting alignment. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t g_main_stack[BSP_CFG_STACK_MAIN_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_STACK) \
                                                      BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);

/* Process stack */
#if (BSP_CFG_STACK_PROCESS_BYTES > 0)
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
BSP_DONT_REMOVE static uint8_t g_process_stack[BSP_CFG_STACK_PROCESS_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_STACK) \
                                                                            BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif

/* Heap */
#if (BSP_CFG_HEAP_BYTES > 0)
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
BSP_DONT_REMOVE static uint8_t g_heap[BSP_CFG_HEAP_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_HEAP) \
                                                          BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif

/* All system exceptions in the vector table are weak references to Default_Handler. If the user wishes to handle
 * these exceptions in their code they should define their own function with the same name.
 */
#if defined(__ICCARM__)
#define WEAK_REF_ATTRIBUTE

#pragma weak HardFault_Handler                        = Default_Handler
#pragma weak MemManage_Handler                        = Default_Handler
#pragma weak BusFault_Handler                         = Default_Handler
#pragma weak UsageFault_Handler                       = Default_Handler
#pragma weak SVC_Handler                              = Default_Handler
#pragma weak DebugMon_Handler                         = Default_Handler
#pragma weak PendSV_Handler                           = Default_Handler
#pragma weak SysTick_Handler                          = Default_Handler
#pragma weak icu_irq0_isr                             = Default_Handler
#pragma weak icu_irq1_isr                             = Default_Handler
#pragma weak icu_irq2_isr                             = Default_Handler
#pragma weak icu_irq3_isr                             = Default_Handler
#pragma weak icu_irq4_isr                             = Default_Handler
#pragma weak icu_irq5_isr                             = Default_Handler
#pragma weak icu_irq6_isr                             = Default_Handler
#pragma weak icu_irq7_isr                             = Default_Handler
#pragma weak icu_irq8_isr                             = Default_Handler
#pragma weak icu_irq9_isr                             = Default_Handler
#pragma weak icu_irq10_isr                            = Default_Handler
#pragma weak icu_irq11_isr                            = Default_Handler
#pragma weak icu_irq12_isr                            = Default_Handler
#pragma weak icu_irq13_isr                            = Default_Handler
#pragma weak icu_irq14_isr                            = Default_Handler
#pragma weak icu_irq15_isr                            = Default_Handler
#pragma weak dmac0_int_isr                            = Default_Handler
#pragma weak dmac1_int_isr                            = Default_Handler
#pragma weak dmac2_int_isr                            = Default_Handler
#pragma weak dmac3_int_isr                            = Default_Handler
#pragma weak dmac4_int_isr                            = Default_Handler
#pragma weak dmac5_int_isr                            = Default_Handler
#pragma weak dmac6_int_isr                            = Default_Handler
#pragma weak dmac7_int_isr                            = Default_Handler
#pragma weak dtc_complete_isr                         = Default_Handler
#pragma weak dtc_end_isr                              = Default_Handler
#pragma weak icu_snooze_cancel_isr                    = Default_Handler
#pragma weak fcu_fiferr_isr                           = Default_Handler
#pragma weak fcu_frdyi_isr                            = Default_Handler
#pragma weak lvd_lvd1_isr                             = Default_Handler
#pragma weak lvd_lvd2_isr                             = Default_Handler
#pragma weak cgc_mosc_stop_isr                        = Default_Handler
#pragma weak lpm_snooze_request_isr                   = Default_Handler
#pragma weak agt0_int_isr                             = Default_Handler
#pragma weak agt0_compare_a_isr                       = Default_Handler
#pragma weak agt0_compare_b_isr                       = Default_Handler
#pragma weak agt1_int_isr                             = Default_Handler
#pragma weak agt1_compare_a_isr                       = Default_Handler
#pragma weak agt1_compare_b_isr                       = Default_Handler
#pragma weak iwdt_underflow_isr                       = Default_Handler
#pragma weak wdt_underflow_isr                        = Default_Handler
#pragma weak rtc_alarm_isr                            = Default_Handler
#pragma weak rtc_period_isr                           = Default_Handler
#pragma weak rtc_carry_isr                            = Default_Handler
#pragma weak adc0_scan_end_isr                        = Default_Handler
#pragma weak adc0_scan_end_b_isr                      = Default_Handler
#pragma weak adc0_window_a_isr                        = Default_Handler
#pragma weak adc0_window_b_isr                        = Default_Handler
#pragma weak adc0_compare_match_isr                   = Default_Handler
#pragma weak adc0_compare_mismatch_isr                = Default_Handler
#pragma weak adc1_scan_end_isr                        = Default_Handler
#pragma weak adc1_scan_end_b_isr                      = Default_Handler
#pragma weak adc1_window_a_isr                        = Default_Handler
#pragma weak adc1_window_b_isr                        = Default_Handler
#pragma weak adc1_compare_match_isr                   = Default_Handler
#pragma weak adc1_compare_mismatch_isr                = Default_Handler
#pragma weak comp_hs_0_isr                            = Default_Handler
#pragma weak comp_hs_1_isr                            = Default_Handler
#pragma weak comp_hs_2_isr                            = Default_Handler
#pragma weak comp_hs_3_isr                            = Default_Handler
#pragma weak comp_hs_4_isr                            = Default_Handler
#pragma weak comp_hs_5_isr                            = Default_Handler
#pragma weak usbfs_fifo_0_isr                         = Default_Handler
#pragma weak usbfs_fifo_1_isr                         = Default_Handler
#pragma weak usbfs_int_isr                            = Default_Handler
#pragma weak usbfs_resume_isr                         = Default_Handler
#pragma weak iic0_rxi_isr                             = Default_Handler
#pragma weak iic0_txi_isr                             = Default_Handler
#pragma weak iic0_tei_isr                             = Default_Handler
#pragma weak iic0_eri_isr                             = Default_Handler
#pragma weak iic0_wui_isr                             = Default_Handler
#pragma weak iic1_rxi_isr                             = Default_Handler
#pragma weak iic1_txi_isr                             = Default_Handler
#pragma weak iic1_tei_isr                             = Default_Handler
#pragma weak iic1_eri_isr                             = Default_Handler
#pragma weak iic2_rxi_isr                             = Default_Handler
#pragma weak iic2_txi_isr                             = Default_Handler
#pragma weak iic2_tei_isr                             = Default_Handler
#pragma weak iic2_eri_isr                             = Default_Handler
#pragma weak ssi0_txi_isr                             = Default_Handler
#pragma weak ssi0_rxi_isr                             = Default_Handler
#pragma weak ssi0_int_isr                             = Default_Handler
#pragma weak ssi1_txi_rxi_isr                         = Default_Handler
#pragma weak ssi1_int_isr                             = Default_Handler
#pragma weak src_input_fifo_empty_isr                 = Default_Handler
#pragma weak src_output_fifo_full_isr                 = Default_Handler
#pragma weak src_output_fifo_overflow_isr             = Default_Handler
#pragma weak src_output_fifo_underflow_isr            = Default_Handler
#pragma weak src_conversion_end_isr                   = Default_Handler
#pragma weak pdc_receive_data_ready_isr               = Default_Handler
#pragma weak pdc_frame_end_isr                        = Default_Handler
#pragma weak pdc_int_isr                              = Default_Handler
#pragma weak ctsu_write_isr                           = Default_Handler
#pragma weak ctsu_read_isr                            = Default_Handler
#pragma weak ctsu_end_isr                             = Default_Handler
#pragma weak key_int_isr                              = Default_Handler
#pragma weak doc_int_isr                              = Default_Handler
#pragma weak cac_frequency_error_isr                  = Default_Handler
#pragma weak cac_measurement_end_isr                  = Default_Handler
#pragma weak cac_overflow_isr                         = Default_Handler
#pragma weak can0_error_isr                           = Default_Handler
#pragma weak can0_fifo_rx_isr                         = Default_Handler
#pragma weak can0_fifo_tx_isr                         = Default_Handler
#pragma weak can0_mailbox_rx_isr                      = Default_Handler
#pragma weak can0_mailbox_tx_isr                      = Default_Handler
#pragma weak can1_error_isr                           = Default_Handler
#pragma weak can1_fifo_rx_isr                         = Default_Handler
#pragma weak can1_fifo_tx_isr                         = Default_Handler
#pragma weak can1_mailbox_rx_isr                      = Default_Handler
#pragma weak can1_mailbox_tx_isr                      = Default_Handler
#pragma weak ioport_event_1_isr                       = Default_Handler
#pragma weak ioport_event_2_isr                       = Default_Handler
#pragma weak ioport_event_3_isr                       = Default_Handler
#pragma weak ioport_event_4_isr                       = Default_Handler
#pragma weak elc_software_event_0_isr                 = Default_Handler
#pragma weak elc_software_event_1_isr                 = Default_Handler
#pragma weak poeg0_event_isr                          = Default_Handler
#pragma weak poeg1_event_isr                          = Default_Handler
#pragma weak poeg2_event_isr                          = Default_Handler
#pragma weak poeg3_event_isr                          = Default_Handler
#pragma weak gpt0_capture_compare_a_isr               = Default_Handler
#pragma weak gpt0_capture_compare_b_isr               = Default_Handler
#pragma weak gpt0_compare_c_isr                       = Default_Handler
#pragma weak gpt0_compare_d_isr                       = Default_Handler
#pragma weak gpt0_compare_e_isr                       = Default_Handler
#pragma weak gpt0_compare_f_isr                       = Default_Handler
#pragma weak gpt0_counter_overflow_isr                = Default_Handler
#pragma weak gpt0_counter_underflow_isr               = Default_Handler
#pragma weak gpt0_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt0_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt1_capture_compare_a_isr               = Default_Handler
#pragma weak gpt1_capture_compare_b_isr               = Default_Handler
#pragma weak gpt1_compare_c_isr                       = Default_Handler
#pragma weak gpt1_compare_d_isr                       = Default_Handler
#pragma weak gpt1_compare_e_isr                       = Default_Handler
#pragma weak gpt1_compare_f_isr                       = Default_Handler
#pragma weak gpt1_counter_overflow_isr                = Default_Handler
#pragma weak gpt1_counter_underflow_isr               = Default_Handler
#pragma weak gpt1_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt1_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt2_capture_compare_a_isr               = Default_Handler
#pragma weak gpt2_capture_compare_b_isr               = Default_Handler
#pragma weak gpt2_compare_c_isr                       = Default_Handler
#pragma weak gpt2_compare_d_isr                       = Default_Handler
#pragma weak gpt2_compare_e_isr                       = Default_Handler
#pragma weak gpt2_compare_f_isr                       = Default_Handler
#pragma weak gpt2_counter_overflow_isr                = Default_Handler
#pragma weak gpt2_counter_underflow_isr               = Default_Handler
#pragma weak gpt2_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt2_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt3_capture_compare_a_isr               = Default_Handler
#pragma weak gpt3_capture_compare_b_isr               = Default_Handler
#pragma weak gpt3_compare_c_isr                       = Default_Handler
#pragma weak gpt3_compare_d_isr                       = Default_Handler
#pragma weak gpt3_compare_e_isr                       = Default_Handler
#pragma weak gpt3_compare_f_isr                       = Default_Handler
#pragma weak gpt3_counter_overflow_isr                = Default_Handler
#pragma weak gpt3_counter_underflow_isr               = Default_Handler
#pragma weak gpt3_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt3_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt4_capture_compare_a_isr               = Default_Handler
#pragma weak gpt4_capture_compare_b_isr               = Default_Handler
#pragma weak gpt4_compare_c_isr                       = Default_Handler
#pragma weak gpt4_compare_d_isr                       = Default_Handler
#pragma weak gpt4_compare_e_isr                       = Default_Handler
#pragma weak gpt4_compare_f_isr                       = Default_Handler
#pragma weak gpt4_counter_overflow_isr                = Default_Handler
#pragma weak gpt4_counter_underflow_isr               = Default_Handler
#pragma weak gpt4_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt4_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt5_capture_compare_a_isr               = Default_Handler
#pragma weak gpt5_capture_compare_b_isr               = Default_Handler
#pragma weak gpt5_compare_c_isr                       = Default_Handler
#pragma weak gpt5_compare_d_isr                       = Default_Handler
#pragma weak gpt5_compare_e_isr                       = Default_Handler
#pragma weak gpt5_compare_f_isr                       = Default_Handler
#pragma weak gpt5_counter_overflow_isr                = Default_Handler
#pragma weak gpt5_counter_underflow_isr               = Default_Handler
#pragma weak gpt5_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt5_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt6_capture_compare_a_isr               = Default_Handler
#pragma weak gpt6_capture_compare_b_isr               = Default_Handler
#pragma weak gpt6_compare_c_isr                       = Default_Handler
#pragma weak gpt6_compare_d_isr                       = Default_Handler
#pragma weak gpt6_compare_e_isr                       = Default_Handler
#pragma weak gpt6_compare_f_isr                       = Default_Handler
#pragma weak gpt6_counter_overflow_isr                = Default_Handler
#pragma weak gpt6_counter_underflow_isr               = Default_Handler
#pragma weak gpt6_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt6_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt7_capture_compare_a_isr               = Default_Handler
#pragma weak gpt7_capture_compare_b_isr               = Default_Handler
#pragma weak gpt7_compare_c_isr                       = Default_Handler
#pragma weak gpt7_compare_d_isr                       = Default_Handler
#pragma weak gpt7_compare_e_isr                       = Default_Handler
#pragma weak gpt7_compare_f_isr                       = Default_Handler
#pragma weak gpt7_counter_overflow_isr                = Default_Handler
#pragma weak gpt7_counter_underflow_isr               = Default_Handler
#pragma weak gpt7_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt7_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt8_capture_compare_a_isr               = Default_Handler
#pragma weak gpt8_capture_compare_b_isr               = Default_Handler
#pragma weak gpt8_compare_c_isr                       = Default_Handler
#pragma weak gpt8_compare_d_isr                       = Default_Handler
#pragma weak gpt8_compare_e_isr                       = Default_Handler
#pragma weak gpt8_compare_f_isr                       = Default_Handler
#pragma weak gpt8_counter_overflow_isr                = Default_Handler
#pragma weak gpt8_counter_underflow_isr               = Default_Handler
#pragma weak gpt9_capture_compare_a_isr               = Default_Handler
#pragma weak gpt9_capture_compare_b_isr               = Default_Handler
#pragma weak gpt9_compare_c_isr                       = Default_Handler
#pragma weak gpt9_compare_d_isr                       = Default_Handler
#pragma weak gpt9_compare_e_isr                       = Default_Handler
#pragma weak gpt9_compare_f_isr                       = Default_Handler
#pragma weak gpt9_counter_overflow_isr                = Default_Handler
#pragma weak gpt9_counter_underflow_isr               = Default_Handler
#pragma weak gpt10_capture_compare_a_isr              = Default_Handler
#pragma weak gpt10_capture_compare_b_isr              = Default_Handler
#pragma weak gpt10_compare_c_isr                      = Default_Handler
#pragma weak gpt10_compare_d_isr                      = Default_Handler
#pragma weak gpt10_compare_e_isr                      = Default_Handler
#pragma weak gpt10_compare_f_isr                      = Default_Handler
#pragma weak gpt10_counter_overflow_isr               = Default_Handler
#pragma weak gpt10_counter_underflow_isr              = Default_Handler
#pragma weak gpt11_capture_compare_a_isr              = Default_Handler
#pragma weak gpt11_capture_compare_b_isr              = Default_Handler
#pragma weak gpt11_compare_c_isr                      = Default_Handler
#pragma weak gpt11_compare_d_isr                      = Default_Handler
#pragma weak gpt11_compare_e_isr                      = Default_Handler
#pragma weak gpt11_compare_f_isr                      = Default_Handler
#pragma weak gpt11_counter_overflow_isr               = Default_Handler
#pragma weak gpt11_counter_underflow_isr              = Default_Handler
#pragma weak gpt12_capture_compare_a_isr              = Default_Handler
#pragma weak gpt12_capture_compare_b_isr              = Default_Handler
#pragma weak gpt12_compare_c_isr                      = Default_Handler
#pragma weak gpt12_compare_d_isr                      = Default_Handler
#pragma weak gpt12_compare_e_isr                      = Default_Handler
#pragma weak gpt12_compare_f_isr                      = Default_Handler
#pragma weak gpt12_counter_overflow_isr               = Default_Handler
#pragma weak gpt12_counter_underflow_isr              = Default_Handler
#pragma weak gpt13_capture_compare_a_isr              = Default_Handler
#pragma weak gpt13_capture_compare_b_isr              = Default_Handler
#pragma weak gpt13_compare_c_isr                      = Default_Handler
#pragma weak gpt13_compare_d_isr                      = Default_Handler
#pragma weak gpt13_compare_e_isr                      = Default_Handler
#pragma weak gpt13_compare_f_isr                      = Default_Handler
#pragma weak gpt13_counter_overflow_isr               = Default_Handler
#pragma weak gpt13_counter_underflow_isr              = Default_Handler
#pragma weak ops_uvw_edge_isr                         = Default_Handler
#pragma weak eptpc_ipls_isr                           = Default_Handler
#pragma weak eptpc_mint_isr                           = Default_Handler
#pragma weak eptpc_pint_isr                           = Default_Handler
#pragma weak edmac0_eint_isr                          = Default_Handler
#pragma weak edmac1_eint_isr                          = Default_Handler
#pragma weak eptpc_timer0_rise_isr                    = Default_Handler
#pragma weak eptpc_timer1_rise_isr                    = Default_Handler
#pragma weak eptpc_timer2_rise_isr                    = Default_Handler
#pragma weak eptpc_timer3_rise_isr                    = Default_Handler
#pragma weak eptpc_timer4_rise_isr                    = Default_Handler
#pragma weak eptpc_timer5_rise_isr                    = Default_Handler
#pragma weak eptpc_timer0_fall_isr                    = Default_Handler
#pragma weak eptpc_timer1_fall_isr                    = Default_Handler
#pragma weak eptpc_timer2_fall_isr                    = Default_Handler
#pragma weak eptpc_timer3_fall_isr                    = Default_Handler
#pragma weak eptpc_timer4_fall_isr                    = Default_Handler
#pragma weak eptpc_timer5_fall_isr                    = Default_Handler
#pragma weak usbhs_fifo_0_isr                         = Default_Handler
#pragma weak usbhs_fifo_1_isr                         = Default_Handler
#pragma weak usbhs_usb_int_resume_isr                 = Default_Handler
#pragma weak sci0_rxi_isr                             = Default_Handler
#pragma weak sci0_txi_isr                             = Default_Handler
#pragma weak sci0_tei_isr                             = Default_Handler
#pragma weak sci0_eri_isr                             = Default_Handler
#pragma weak sci0_am_isr                              = Default_Handler
#pragma weak sci0_rxi_or_eri_isr                      = Default_Handler
#pragma weak sci1_rxi_isr                             = Default_Handler
#pragma weak sci1_txi_isr                             = Default_Handler
#pragma weak sci1_tei_isr                             = Default_Handler
#pragma weak sci1_eri_isr                             = Default_Handler
#pragma weak sci1_am_isr                              = Default_Handler
#pragma weak sci2_rxi_isr                             = Default_Handler
#pragma weak sci2_txi_isr                             = Default_Handler
#pragma weak sci2_tei_isr                             = Default_Handler
#pragma weak sci2_eri_isr                             = Default_Handler
#pragma weak sci2_am_isr                              = Default_Handler
#pragma weak sci3_rxi_isr                             = Default_Handler
#pragma weak sci3_txi_isr                             = Default_Handler
#pragma weak sci3_tei_isr                             = Default_Handler
#pragma weak sci3_eri_isr                             = Default_Handler
#pragma weak sci3_am_isr                              = Default_Handler
#pragma weak sci4_rxi_isr                             = Default_Handler
#pragma weak sci4_txi_isr                             = Default_Handler
#pragma weak sci4_tei_isr                             = Default_Handler
#pragma weak sci4_eri_isr                             = Default_Handler
#pragma weak sci4_am_isr                              = Default_Handler
#pragma weak sci5_rxi_isr                             = Default_Handler
#pragma weak sci5_txi_isr                             = Default_Handler
#pragma weak sci5_tei_isr                             = Default_Handler
#pragma weak sci5_eri_isr                             = Default_Handler
#pragma weak sci5_am_isr                              = Default_Handler
#pragma weak sci6_rxi_isr                             = Default_Handler
#pragma weak sci6_txi_isr                             = Default_Handler
#pragma weak sci6_tei_isr                             = Default_Handler
#pragma weak sci6_eri_isr                             = Default_Handler
#pragma weak sci6_am_isr                              = Default_Handler
#pragma weak sci7_rxi_isr                             = Default_Handler
#pragma weak sci7_txi_isr                             = Default_Handler
#pragma weak sci7_tei_isr                             = Default_Handler
#pragma weak sci7_eri_isr                             = Default_Handler
#pragma weak sci7_am_isr                              = Default_Handler
#pragma weak sci8_rxi_isr                             = Default_Handler
#pragma weak sci8_txi_isr                             = Default_Handler
#pragma weak sci8_tei_isr                             = Default_Handler
#pragma weak sci8_eri_isr                             = Default_Handler
#pragma weak sci8_am_isr                              = Default_Handler
#pragma weak sci9_rxi_isr                             = Default_Handler
#pragma weak sci9_txi_isr                             = Default_Handler
#pragma weak sci9_tei_isr                             = Default_Handler
#pragma weak sci9_eri_isr                             = Default_Handler
#pragma weak sci9_am_isr                              = Default_Handler
#pragma weak spi0_rxi_isr                             = Default_Handler
#pragma weak spi0_txi_isr                             = Default_Handler
#pragma weak spi0_idle_isr                            = Default_Handler
#pragma weak spi0_eri_isr                             = Default_Handler
#pragma weak spi0_tei_isr                             = Default_Handler
#pragma weak spi1_rxi_isr                             = Default_Handler
#pragma weak spi1_txi_isr                             = Default_Handler
#pragma weak spi1_idle_isr                            = Default_Handler
#pragma weak spi1_eri_isr                             = Default_Handler
#pragma weak spi1_tei_isr                             = Default_Handler
#pragma weak qspi_int_isr                             = Default_Handler
#pragma weak sdhimmc0_accs_isr                        = Default_Handler
#pragma weak sdhimmc0_sdio_isr                        = Default_Handler
#pragma weak sdhimmc0_card_isr                        = Default_Handler
#pragma weak sdhimmc0_dma_req_isr                     = Default_Handler
#pragma weak sdhimmc1_accs_isr                        = Default_Handler
#pragma weak sdhimmc1_sdio_isr                        = Default_Handler
#pragma weak sdhimmc1_card_isr                        = Default_Handler
#pragma weak sdhimmc1_dma_req_isr                     = Default_Handler
#pragma weak sce_proc_busy_isr                        = Default_Handler
#pragma weak sce_romok_isr                            = Default_Handler
#pragma weak sce_long_plg_isr                         = Default_Handler
#pragma weak sce_test_busy_isr                        = Default_Handler
#pragma weak sce_wrrdy_0_isr                          = Default_Handler
#pragma weak sce_wrrdy_1_isr                          = Default_Handler
#pragma weak sce_wrrdy_4_isr                          = Default_Handler
#pragma weak sce_rdrdy_0_isr                          = Default_Handler
#pragma weak sce_rdrdy_1_isr                          = Default_Handler
#pragma weak sce_integrate_wrrdy_isr                  = Default_Handler
#pragma weak sce_integrate_rdrdy_isr                  = Default_Handler
#pragma weak glcdc_line_detect_isr                    = Default_Handler
#pragma weak glcdc_underflow_1_isr                    = Default_Handler
#pragma weak glcdc_underflow_2_isr                    = Default_Handler
#pragma weak drw_int_isr                              = Default_Handler
#pragma weak jpeg_jedi_isr                            = Default_Handler
#pragma weak jpeg_jdti_isr                            = Default_Handler
#elif defined(__GNUC__)
#define WEAK_REF_ATTRIBUTE      __attribute__ ((weak, alias("Default_Handler")))
#endif

void NMI_Handler(void);                      //NMI has many sources and is handled by BSP
void HardFault_Handler                       (void) WEAK_REF_ATTRIBUTE;
void MemManage_Handler                       (void) WEAK_REF_ATTRIBUTE;
void BusFault_Handler                        (void) WEAK_REF_ATTRIBUTE;
void UsageFault_Handler                      (void) WEAK_REF_ATTRIBUTE;
void SVC_Handler                             (void) WEAK_REF_ATTRIBUTE;
void DebugMon_Handler                        (void) WEAK_REF_ATTRIBUTE;
void PendSV_Handler                          (void) WEAK_REF_ATTRIBUTE;
void SysTick_Handler                         (void) WEAK_REF_ATTRIBUTE;
void icu_irq0_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq1_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq2_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq3_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq4_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq5_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq6_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq7_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq8_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq9_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq10_isr                           (void) WEAK_REF_ATTRIBUTE;
void icu_irq11_isr                           (void) WEAK_REF_ATTRIBUTE;
void icu_irq12_isr                           (void) WEAK_REF_ATTRIBUTE;
void icu_irq13_isr                           (void) WEAK_REF_ATTRIBUTE;
void icu_irq14_isr                           (void) WEAK_REF_ATTRIBUTE;
void icu_irq15_isr                           (void) WEAK_REF_ATTRIBUTE;
void dmac0_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void dmac1_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void dmac2_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void dmac3_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void dmac4_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void dmac5_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void dmac6_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void dmac7_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void dtc_complete_isr                        (void) WEAK_REF_ATTRIBUTE;
void dtc_end_isr                             (void) WEAK_REF_ATTRIBUTE;
void icu_snooze_cancel_isr                   (void) WEAK_REF_ATTRIBUTE;
void fcu_fiferr_isr                          (void) WEAK_REF_ATTRIBUTE;
void fcu_frdyi_isr                           (void) WEAK_REF_ATTRIBUTE;
void lvd_lvd1_isr                            (void) WEAK_REF_ATTRIBUTE;
void lvd_lvd2_isr                            (void) WEAK_REF_ATTRIBUTE;
void cgc_mosc_stop_isr                       (void) WEAK_REF_ATTRIBUTE;
void lpm_snooze_request_isr                  (void) WEAK_REF_ATTRIBUTE;
void agt0_int_isr                            (void) WEAK_REF_ATTRIBUTE;
void agt0_compare_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void agt0_compare_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void agt1_int_isr                            (void) WEAK_REF_ATTRIBUTE;
void agt1_compare_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void agt1_compare_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void iwdt_underflow_isr                      (void) WEAK_REF_ATTRIBUTE;
void wdt_underflow_isr                       (void) WEAK_REF_ATTRIBUTE;
void rtc_alarm_isr                           (void) WEAK_REF_ATTRIBUTE;
void rtc_period_isr                          (void) WEAK_REF_ATTRIBUTE;
void rtc_carry_isr                           (void) WEAK_REF_ATTRIBUTE;
void adc0_scan_end_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc0_scan_end_b_isr                     (void) WEAK_REF_ATTRIBUTE;
void adc0_window_a_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc0_window_b_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc0_compare_match_isr                  (void) WEAK_REF_ATTRIBUTE;
void adc0_compare_mismatch_isr               (void) WEAK_REF_ATTRIBUTE;
void adc1_scan_end_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc1_scan_end_b_isr                     (void) WEAK_REF_ATTRIBUTE;
void adc1_window_a_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc1_window_b_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc1_compare_match_isr                  (void) WEAK_REF_ATTRIBUTE;
void adc1_compare_mismatch_isr               (void) WEAK_REF_ATTRIBUTE;
void comp_hs_0_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_hs_1_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_hs_2_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_hs_3_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_hs_4_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_hs_5_isr                           (void) WEAK_REF_ATTRIBUTE;
void usbfs_fifo_0_isr                        (void) WEAK_REF_ATTRIBUTE;
void usbfs_fifo_1_isr                        (void) WEAK_REF_ATTRIBUTE;
void usbfs_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void usbfs_resume_isr                        (void) WEAK_REF_ATTRIBUTE;
void iic0_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic0_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic0_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic0_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic0_wui_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic1_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic1_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic1_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic1_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic2_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic2_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic2_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic2_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void ssi0_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void ssi0_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void ssi0_int_isr                            (void) WEAK_REF_ATTRIBUTE;
void ssi1_txi_rxi_isr                        (void) WEAK_REF_ATTRIBUTE;
void ssi1_int_isr                            (void) WEAK_REF_ATTRIBUTE;
void src_input_fifo_empty_isr                (void) WEAK_REF_ATTRIBUTE;
void src_output_fifo_full_isr                (void) WEAK_REF_ATTRIBUTE;
void src_output_fifo_overflow_isr            (void) WEAK_REF_ATTRIBUTE;
void src_output_fifo_underflow_isr           (void) WEAK_REF_ATTRIBUTE;
void src_conversion_end_isr                  (void) WEAK_REF_ATTRIBUTE;
void pdc_receive_data_ready_isr              (void) WEAK_REF_ATTRIBUTE;
void pdc_frame_end_isr                       (void) WEAK_REF_ATTRIBUTE;
void pdc_int_isr                             (void) WEAK_REF_ATTRIBUTE;
void ctsu_write_isr                          (void) WEAK_REF_ATTRIBUTE;
void ctsu_read_isr                           (void) WEAK_REF_ATTRIBUTE;
void ctsu_end_isr                            (void) WEAK_REF_ATTRIBUTE;
void key_int_isr                             (void) WEAK_REF_ATTRIBUTE;
void doc_int_isr                             (void) WEAK_REF_ATTRIBUTE;
void cac_frequency_error_isr                 (void) WEAK_REF_ATTRIBUTE;
void cac_measurement_end_isr                 (void) WEAK_REF_ATTRIBUTE;
void cac_overflow_isr                        (void) WEAK_REF_ATTRIBUTE;
void can0_error_isr                          (void) WEAK_REF_ATTRIBUTE;
void can0_fifo_rx_isr                        (void) WEAK_REF_ATTRIBUTE;
void can0_fifo_tx_isr                        (void) WEAK_REF_ATTRIBUTE;
void can0_mailbox_rx_isr                     (void) WEAK_REF_ATTRIBUTE;
void can0_mailbox_tx_isr                     (void) WEAK_REF_ATTRIBUTE;
void can1_error_isr                          (void) WEAK_REF_ATTRIBUTE;
void can1_fifo_rx_isr                        (void) WEAK_REF_ATTRIBUTE;
void can1_fifo_tx_isr                        (void) WEAK_REF_ATTRIBUTE;
void can1_mailbox_rx_isr                     (void) WEAK_REF_ATTRIBUTE;
void can1_mailbox_tx_isr                     (void) WEAK_REF_ATTRIBUTE;
void ioport_event_1_isr                      (void) WEAK_REF_ATTRIBUTE;
void ioport_event_2_isr                      (void) WEAK_REF_ATTRIBUTE;
void ioport_event_3_isr                      (void) WEAK_REF_ATTRIBUTE;
void ioport_event_4_isr                      (void) WEAK_REF_ATTRIBUTE;
void elc_software_event_0_isr                (void) WEAK_REF_ATTRIBUTE;
void elc_software_event_1_isr                (void) WEAK_REF_ATTRIBUTE;
void poeg0_event_isr                         (void) WEAK_REF_ATTRIBUTE;
void poeg1_event_isr                         (void) WEAK_REF_ATTRIBUTE;
void poeg2_event_isr                         (void) WEAK_REF_ATTRIBUTE;
void poeg3_event_isr                         (void) WEAK_REF_ATTRIBUTE;
void gpt0_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt0_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt0_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt0_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt0_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt0_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt1_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt1_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt1_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt2_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt2_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt2_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt3_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt3_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt3_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt4_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt4_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt4_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt5_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt5_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt5_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt6_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt6_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt6_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt7_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt7_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt7_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt7_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt7_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt7_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt7_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt7_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt7_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt7_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt8_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt8_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt8_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt8_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt8_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt8_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt8_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt8_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt9_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt9_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt9_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt9_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt9_compare_e_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt9_compare_f_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt9_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt9_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt10_capture_compare_a_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt10_capture_compare_b_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt10_compare_c_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt10_compare_d_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt10_compare_e_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt10_compare_f_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt10_counter_overflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt10_counter_underflow_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt11_capture_compare_a_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt11_capture_compare_b_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt11_compare_c_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt11_compare_d_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt11_compare_e_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt11_compare_f_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt11_counter_overflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt11_counter_underflow_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt12_capture_compare_a_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt12_capture_compare_b_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt12_compare_c_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt12_compare_d_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt12_compare_e_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt12_compare_f_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt12_counter_overflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt12_counter_underflow_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt13_capture_compare_a_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt13_capture_compare_b_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt13_compare_c_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt13_compare_d_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt13_compare_e_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt13_compare_f_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt13_counter_overflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt13_counter_underflow_isr             (void) WEAK_REF_ATTRIBUTE;
void ops_uvw_edge_isr                        (void) WEAK_REF_ATTRIBUTE;
void eptpc_ipls_isr                          (void) WEAK_REF_ATTRIBUTE;
void eptpc_mint_isr                          (void) WEAK_REF_ATTRIBUTE;
void eptpc_pint_isr                          (void) WEAK_REF_ATTRIBUTE;
void edmac0_eint_isr                         (void) WEAK_REF_ATTRIBUTE;
void edmac1_eint_isr                         (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer0_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer1_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer2_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer3_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer4_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer5_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer0_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer1_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer2_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer3_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer4_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void eptpc_timer5_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void usbhs_fifo_0_isr                        (void) WEAK_REF_ATTRIBUTE;
void usbhs_fifo_1_isr                        (void) WEAK_REF_ATTRIBUTE;
void usbhs_usb_int_resume_isr                (void) WEAK_REF_ATTRIBUTE;
void sci0_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci0_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci0_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci0_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci0_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci0_rxi_or_eri_isr                     (void) WEAK_REF_ATTRIBUTE;
void sci1_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci1_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci1_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci1_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci1_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci2_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci2_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci2_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci2_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci2_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci3_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci3_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci3_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci3_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci3_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci4_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci4_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci4_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci4_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci4_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci5_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci5_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci5_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci5_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci5_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci6_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci6_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci6_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci6_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci6_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci7_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci7_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci7_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci7_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci7_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci8_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci8_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci8_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci8_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci8_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci9_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void spi0_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi0_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi0_idle_isr                           (void) WEAK_REF_ATTRIBUTE;
void spi0_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi0_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi1_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi1_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi1_idle_isr                           (void) WEAK_REF_ATTRIBUTE;
void spi1_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi1_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void qspi_int_isr                            (void) WEAK_REF_ATTRIBUTE;
void sdhimmc0_accs_isr                       (void) WEAK_REF_ATTRIBUTE;
void sdhimmc0_sdio_isr                       (void) WEAK_REF_ATTRIBUTE;
void sdhimmc0_card_isr                       (void) WEAK_REF_ATTRIBUTE;
void sdhimmc0_dma_req_isr                    (void) WEAK_REF_ATTRIBUTE;
void sdhimmc1_accs_isr                       (void) WEAK_REF_ATTRIBUTE;
void sdhimmc1_sdio_isr                       (void) WEAK_REF_ATTRIBUTE;
void sdhimmc1_card_isr                       (void) WEAK_REF_ATTRIBUTE;
void sdhimmc1_dma_req_isr                    (void) WEAK_REF_ATTRIBUTE;
void sce_proc_busy_isr                       (void) WEAK_REF_ATTRIBUTE;
void sce_romok_isr                           (void) WEAK_REF_ATTRIBUTE;
void sce_long_plg_isr                        (void) WEAK_REF_ATTRIBUTE;
void sce_test_busy_isr                       (void) WEAK_REF_ATTRIBUTE;
void sce_wrrdy_0_isr                         (void) WEAK_REF_ATTRIBUTE;
void sce_wrrdy_1_isr                         (void) WEAK_REF_ATTRIBUTE;
void sce_wrrdy_4_isr                         (void) WEAK_REF_ATTRIBUTE;
void sce_rdrdy_0_isr                         (void) WEAK_REF_ATTRIBUTE;
void sce_rdrdy_1_isr                         (void) WEAK_REF_ATTRIBUTE;
void sce_integrate_wrrdy_isr                 (void) WEAK_REF_ATTRIBUTE;
void sce_integrate_rdrdy_isr                 (void) WEAK_REF_ATTRIBUTE;
void glcdc_line_detect_isr                   (void) WEAK_REF_ATTRIBUTE;
void glcdc_underflow_1_isr                   (void) WEAK_REF_ATTRIBUTE;
void glcdc_underflow_2_isr                   (void) WEAK_REF_ATTRIBUTE;
void drw_int_isr                             (void) WEAK_REF_ATTRIBUTE;
void jpeg_jedi_isr                           (void) WEAK_REF_ATTRIBUTE;
void jpeg_jdti_isr                           (void) WEAK_REF_ATTRIBUTE;

/* Vector table. */
BSP_DONT_REMOVE const exc_ptr_t __Vectors[BSP_VECTOR_TABLE_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_VECTOR) =
{
    (exc_ptr_t)(&g_main_stack[0] + BSP_CFG_STACK_MAIN_BYTES),           /*      Initial Stack Pointer     */
    Reset_Handler,                                                      /*      Reset Handler             */
    NMI_Handler,                                                        /*      NMI Handler               */
    HardFault_Handler,                                                  /*      Hard Fault Handler        */
    MemManage_Handler,                                                  /*      MPU Fault Handler         */
    BusFault_Handler,                                                   /*      Bus Fault Handler         */
    UsageFault_Handler,                                                 /*      Usage Fault Handler       */
    0,                                                                  /*      Reserved                  */
    0,                                                                  /*      Reserved                  */
    0,                                                                  /*      Reserved                  */
    0,                                                                  /*      Reserved                  */
    SVC_Handler,                                                        /*      SVCall Handler            */
    DebugMon_Handler,                                                   /*      Debug Monitor Handler     */
    0,                                                                  /*      Reserved                  */
    PendSV_Handler,                                                     /*      PendSV Handler            */
    SysTick_Handler,                                                    /*      SysTick Handler           */
#if (BSP_IRQ_CFG_ICU_IRQ0 != BSP_IRQ_DISABLED)
    icu_irq0_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ1 != BSP_IRQ_DISABLED)
    icu_irq1_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ2 != BSP_IRQ_DISABLED)
    icu_irq2_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ3 != BSP_IRQ_DISABLED)
    icu_irq3_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ4 != BSP_IRQ_DISABLED)
    icu_irq4_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ5 != BSP_IRQ_DISABLED)
    icu_irq5_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ6 != BSP_IRQ_DISABLED)
    icu_irq6_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ7 != BSP_IRQ_DISABLED)
    icu_irq7_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ8 != BSP_IRQ_DISABLED)
    icu_irq8_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ9 != BSP_IRQ_DISABLED)
    icu_irq9_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ10 != BSP_IRQ_DISABLED)
    icu_irq10_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ11 != BSP_IRQ_DISABLED)
    icu_irq11_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ12 != BSP_IRQ_DISABLED)
    icu_irq12_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ13 != BSP_IRQ_DISABLED)
    icu_irq13_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ14 != BSP_IRQ_DISABLED)
    icu_irq14_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ15 != BSP_IRQ_DISABLED)
    icu_irq15_isr,
#endif
#if (BSP_IRQ_CFG_DMAC0_INT != BSP_IRQ_DISABLED)
    dmac0_int_isr,
#endif
#if (BSP_IRQ_CFG_DMAC1_INT != BSP_IRQ_DISABLED)
    dmac1_int_isr,
#endif
#if (BSP_IRQ_CFG_DMAC2_INT != BSP_IRQ_DISABLED)
    dmac2_int_isr,
#endif
#if (BSP_IRQ_CFG_DMAC3_INT != BSP_IRQ_DISABLED)
    dmac3_int_isr,
#endif
#if (BSP_IRQ_CFG_DMAC4_INT != BSP_IRQ_DISABLED)
    dmac4_int_isr,
#endif
#if (BSP_IRQ_CFG_DMAC5_INT != BSP_IRQ_DISABLED)
    dmac5_int_isr,
#endif
#if (BSP_IRQ_CFG_DMAC6_INT != BSP_IRQ_DISABLED)
    dmac6_int_isr,
#endif
#if (BSP_IRQ_CFG_DMAC7_INT != BSP_IRQ_DISABLED)
    dmac7_int_isr,
#endif
#if (BSP_IRQ_CFG_DTC_COMPLETE != BSP_IRQ_DISABLED)
    dtc_complete_isr,
#endif
#if (BSP_IRQ_CFG_DTC_END != BSP_IRQ_DISABLED)
    dtc_end_isr,
#endif
#if (BSP_IRQ_CFG_ICU_SNOOZE_CANCEL != BSP_IRQ_DISABLED)
    icu_snooze_cancel_isr,
#endif
#if (BSP_IRQ_CFG_FCU_FIFERR != BSP_IRQ_DISABLED)
    fcu_fiferr_isr,
#endif
#if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_DISABLED)
    fcu_frdyi_isr,
#endif
#if (BSP_IRQ_CFG_LVD_LVD1 != BSP_IRQ_DISABLED)
    lvd_lvd1_isr,
#endif
#if (BSP_IRQ_CFG_LVD_LVD2 != BSP_IRQ_DISABLED)
    lvd_lvd2_isr,
#endif
#if (BSP_IRQ_CFG_CGC_MOSC_STOP != BSP_IRQ_DISABLED)
    cgc_mosc_stop_isr,
#endif
#if (BSP_IRQ_CFG_LPM_SNOOZE_REQUEST != BSP_IRQ_DISABLED)
    lpm_snooze_request_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED)
    agt0_int_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_COMPARE_A != BSP_IRQ_DISABLED)
    agt0_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_COMPARE_B != BSP_IRQ_DISABLED)
    agt0_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED)
    agt1_int_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_COMPARE_A != BSP_IRQ_DISABLED)
    agt1_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_COMPARE_B != BSP_IRQ_DISABLED)
    agt1_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_IWDT_UNDERFLOW != BSP_IRQ_DISABLED)
    iwdt_underflow_isr,
#endif
#if (BSP_IRQ_CFG_WDT_UNDERFLOW != BSP_IRQ_DISABLED)
    wdt_underflow_isr,
#endif
#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
    rtc_alarm_isr,
#endif
#if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED)
    rtc_period_isr,
#endif
#if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED)
    rtc_carry_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END != BSP_IRQ_DISABLED)
    adc0_scan_end_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
    adc0_scan_end_b_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_WINDOW_A != BSP_IRQ_DISABLED)
    adc0_window_a_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_WINDOW_B != BSP_IRQ_DISABLED)
    adc0_window_b_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_COMPARE_MATCH != BSP_IRQ_DISABLED)
    adc0_compare_match_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_COMPARE_MISMATCH != BSP_IRQ_DISABLED)
    adc0_compare_mismatch_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_SCAN_END != BSP_IRQ_DISABLED)
    adc1_scan_end_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_SCAN_END_B != BSP_IRQ_DISABLED)
    adc1_scan_end_b_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_WINDOW_A != BSP_IRQ_DISABLED)
    adc1_window_a_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_WINDOW_B != BSP_IRQ_DISABLED)
    adc1_window_b_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_COMPARE_MATCH != BSP_IRQ_DISABLED)
    adc1_compare_match_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_COMPARE_MISMATCH != BSP_IRQ_DISABLED)
    adc1_compare_mismatch_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_0 != BSP_IRQ_DISABLED)
    comp_hs_0_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_1 != BSP_IRQ_DISABLED)
    comp_hs_1_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_2 != BSP_IRQ_DISABLED)
    comp_hs_2_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_3 != BSP_IRQ_DISABLED)
    comp_hs_3_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_4 != BSP_IRQ_DISABLED)
    comp_hs_4_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_5 != BSP_IRQ_DISABLED)
    comp_hs_5_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_FIFO_0 != BSP_IRQ_DISABLED)
    usbfs_fifo_0_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_FIFO_1 != BSP_IRQ_DISABLED)
    usbfs_fifo_1_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_INT != BSP_IRQ_DISABLED)
    usbfs_int_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_RESUME != BSP_IRQ_DISABLED)
    usbfs_resume_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_RXI != BSP_IRQ_DISABLED)
    iic0_rxi_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_TXI != BSP_IRQ_DISABLED)
    iic0_txi_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_TEI != BSP_IRQ_DISABLED)
    iic0_tei_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_ERI != BSP_IRQ_DISABLED)
    iic0_eri_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_WUI != BSP_IRQ_DISABLED)
    iic0_wui_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_RXI != BSP_IRQ_DISABLED)
    iic1_rxi_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_TXI != BSP_IRQ_DISABLED)
    iic1_txi_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_TEI != BSP_IRQ_DISABLED)
    iic1_tei_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_ERI != BSP_IRQ_DISABLED)
    iic1_eri_isr,
#endif
#if (BSP_IRQ_CFG_IIC2_RXI != BSP_IRQ_DISABLED)
    iic2_rxi_isr,
#endif
#if (BSP_IRQ_CFG_IIC2_TXI != BSP_IRQ_DISABLED)
    iic2_txi_isr,
#endif
#if (BSP_IRQ_CFG_IIC2_TEI != BSP_IRQ_DISABLED)
    iic2_tei_isr,
#endif
#if (BSP_IRQ_CFG_IIC2_ERI != BSP_IRQ_DISABLED)
    iic2_eri_isr,
#endif
#if (BSP_IRQ_CFG_SSI0_TXI != BSP_IRQ_DISABLED)
    ssi0_txi_isr,
#endif
#if (BSP_IRQ_CFG_SSI0_RXI != BSP_IRQ_DISABLED)
    ssi0_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SSI0_INT != BSP_IRQ_DISABLED)
    ssi0_int_isr,
#endif
#if (BSP_IRQ_CFG_SSI1_TXI_RXI != BSP_IRQ_DISABLED)
    ssi1_txi_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SSI1_INT != BSP_IRQ_DISABLED)
    ssi1_int_isr,
#endif
#if (BSP_IRQ_CFG_SRC_INPUT_FIFO_EMPTY != BSP_IRQ_DISABLED)
    src_input_fifo_empty_isr,
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_FULL != BSP_IRQ_DISABLED)
    src_output_fifo_full_isr,
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_OVERFLOW != BSP_IRQ_DISABLED)
    src_output_fifo_overflow_isr,
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_UNDERFLOW != BSP_IRQ_DISABLED)
    src_output_fifo_underflow_isr,
#endif
#if (BSP_IRQ_CFG_SRC_CONVERSION_END != BSP_IRQ_DISABLED)
    src_conversion_end_isr,
#endif
#if (BSP_IRQ_CFG_PDC_RECEIVE_DATA_READY != BSP_IRQ_DISABLED)
    pdc_receive_data_ready_isr,
#endif
#if (BSP_IRQ_CFG_PDC_FRAME_END != BSP_IRQ_DISABLED)
    pdc_frame_end_isr,
#endif
#if (BSP_IRQ_CFG_PDC_INT != BSP_IRQ_DISABLED)
    pdc_int_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_WRITE != BSP_IRQ_DISABLED)
    ctsu_write_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_READ != BSP_IRQ_DISABLED)
    ctsu_read_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_END != BSP_IRQ_DISABLED)
    ctsu_end_isr,
#endif
#if (BSP_IRQ_CFG_KEY_INT != BSP_IRQ_DISABLED)
    key_int_isr,
#endif
#if (BSP_IRQ_CFG_DOC_INT != BSP_IRQ_DISABLED)
    doc_int_isr,
#endif
#if (BSP_IRQ_CFG_CAC_FREQUENCY_ERROR != BSP_IRQ_DISABLED)
    cac_frequency_error_isr,
#endif
#if (BSP_IRQ_CFG_CAC_MEASUREMENT_END != BSP_IRQ_DISABLED)
    cac_measurement_end_isr,
#endif
#if (BSP_IRQ_CFG_CAC_OVERFLOW != BSP_IRQ_DISABLED)
    cac_overflow_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_ERROR != BSP_IRQ_DISABLED)
    can0_error_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_FIFO_RX != BSP_IRQ_DISABLED)
    can0_fifo_rx_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_FIFO_TX != BSP_IRQ_DISABLED)
    can0_fifo_tx_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_MAILBOX_RX != BSP_IRQ_DISABLED)
    can0_mailbox_rx_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_MAILBOX_TX != BSP_IRQ_DISABLED)
    can0_mailbox_tx_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_ERROR != BSP_IRQ_DISABLED)
    can1_error_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_FIFO_RX != BSP_IRQ_DISABLED)
    can1_fifo_rx_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_FIFO_TX != BSP_IRQ_DISABLED)
    can1_fifo_tx_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_MAILBOX_RX != BSP_IRQ_DISABLED)
    can1_mailbox_rx_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_MAILBOX_TX != BSP_IRQ_DISABLED)
    can1_mailbox_tx_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_1 != BSP_IRQ_DISABLED)
    ioport_event_1_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_2 != BSP_IRQ_DISABLED)
    ioport_event_2_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_3 != BSP_IRQ_DISABLED)
    ioport_event_3_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_4 != BSP_IRQ_DISABLED)
    ioport_event_4_isr,
#endif
#if (BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0 != BSP_IRQ_DISABLED)
    elc_software_event_0_isr,
#endif
#if (BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_1 != BSP_IRQ_DISABLED)
    elc_software_event_1_isr,
#endif
#if (BSP_IRQ_CFG_POEG0_EVENT != BSP_IRQ_DISABLED)
    poeg0_event_isr,
#endif
#if (BSP_IRQ_CFG_POEG1_EVENT != BSP_IRQ_DISABLED)
    poeg1_event_isr,
#endif
#if (BSP_IRQ_CFG_POEG2_EVENT != BSP_IRQ_DISABLED)
    poeg2_event_isr,
#endif
#if (BSP_IRQ_CFG_POEG3_EVENT != BSP_IRQ_DISABLED)
    poeg3_event_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt0_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt0_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_C != BSP_IRQ_DISABLED)
    gpt0_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_D != BSP_IRQ_DISABLED)
    gpt0_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_E != BSP_IRQ_DISABLED)
    gpt0_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_F != BSP_IRQ_DISABLED)
    gpt0_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt0_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt0_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt0_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt0_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt1_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt1_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_C != BSP_IRQ_DISABLED)
    gpt1_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_D != BSP_IRQ_DISABLED)
    gpt1_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_E != BSP_IRQ_DISABLED)
    gpt1_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_F != BSP_IRQ_DISABLED)
    gpt1_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt1_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt1_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt1_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt1_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt2_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt2_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_C != BSP_IRQ_DISABLED)
    gpt2_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_D != BSP_IRQ_DISABLED)
    gpt2_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_E != BSP_IRQ_DISABLED)
    gpt2_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_F != BSP_IRQ_DISABLED)
    gpt2_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt2_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt2_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt2_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt2_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt3_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt3_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_C != BSP_IRQ_DISABLED)
    gpt3_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_D != BSP_IRQ_DISABLED)
    gpt3_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_E != BSP_IRQ_DISABLED)
    gpt3_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_F != BSP_IRQ_DISABLED)
    gpt3_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt3_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt3_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt3_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt3_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt4_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt4_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_C != BSP_IRQ_DISABLED)
    gpt4_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_D != BSP_IRQ_DISABLED)
    gpt4_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_E != BSP_IRQ_DISABLED)
    gpt4_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_F != BSP_IRQ_DISABLED)
    gpt4_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt4_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt4_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt4_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt4_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt5_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt5_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_C != BSP_IRQ_DISABLED)
    gpt5_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_D != BSP_IRQ_DISABLED)
    gpt5_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_E != BSP_IRQ_DISABLED)
    gpt5_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_F != BSP_IRQ_DISABLED)
    gpt5_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt5_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt5_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt5_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt5_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt6_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt6_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_C != BSP_IRQ_DISABLED)
    gpt6_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_D != BSP_IRQ_DISABLED)
    gpt6_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_E != BSP_IRQ_DISABLED)
    gpt6_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_F != BSP_IRQ_DISABLED)
    gpt6_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt6_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt6_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt6_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt6_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt7_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt7_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_C != BSP_IRQ_DISABLED)
    gpt7_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_D != BSP_IRQ_DISABLED)
    gpt7_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_E != BSP_IRQ_DISABLED)
    gpt7_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_F != BSP_IRQ_DISABLED)
    gpt7_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt7_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt7_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt7_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt7_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt8_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt8_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_C != BSP_IRQ_DISABLED)
    gpt8_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_D != BSP_IRQ_DISABLED)
    gpt8_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_E != BSP_IRQ_DISABLED)
    gpt8_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_F != BSP_IRQ_DISABLED)
    gpt8_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt8_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt8_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt9_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt9_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_C != BSP_IRQ_DISABLED)
    gpt9_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_D != BSP_IRQ_DISABLED)
    gpt9_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_E != BSP_IRQ_DISABLED)
    gpt9_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_F != BSP_IRQ_DISABLED)
    gpt9_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt9_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt9_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt10_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt10_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_C != BSP_IRQ_DISABLED)
    gpt10_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_D != BSP_IRQ_DISABLED)
    gpt10_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_E != BSP_IRQ_DISABLED)
    gpt10_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_F != BSP_IRQ_DISABLED)
    gpt10_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt10_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt10_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt11_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt11_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_C != BSP_IRQ_DISABLED)
    gpt11_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_D != BSP_IRQ_DISABLED)
    gpt11_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_E != BSP_IRQ_DISABLED)
    gpt11_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_F != BSP_IRQ_DISABLED)
    gpt11_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt11_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt11_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt12_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt12_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_C != BSP_IRQ_DISABLED)
    gpt12_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_D != BSP_IRQ_DISABLED)
    gpt12_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_E != BSP_IRQ_DISABLED)
    gpt12_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_F != BSP_IRQ_DISABLED)
    gpt12_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt12_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt12_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt13_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt13_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_C != BSP_IRQ_DISABLED)
    gpt13_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_D != BSP_IRQ_DISABLED)
    gpt13_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_E != BSP_IRQ_DISABLED)
    gpt13_compare_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_F != BSP_IRQ_DISABLED)
    gpt13_compare_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt13_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt13_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_OPS_UVW_EDGE != BSP_IRQ_DISABLED)
    ops_uvw_edge_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_IPLS != BSP_IRQ_DISABLED)
    eptpc_ipls_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_MINT != BSP_IRQ_DISABLED)
    eptpc_mint_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_PINT != BSP_IRQ_DISABLED)
    eptpc_pint_isr,
#endif
#if (BSP_IRQ_CFG_EDMAC0_EINT != BSP_IRQ_DISABLED)
    edmac0_eint_isr,
#endif
#if (BSP_IRQ_CFG_EDMAC1_EINT != BSP_IRQ_DISABLED)
    edmac1_eint_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER0_RISE != BSP_IRQ_DISABLED)
    eptpc_timer0_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER1_RISE != BSP_IRQ_DISABLED)
    eptpc_timer1_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER2_RISE != BSP_IRQ_DISABLED)
    eptpc_timer2_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER3_RISE != BSP_IRQ_DISABLED)
    eptpc_timer3_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER4_RISE != BSP_IRQ_DISABLED)
    eptpc_timer4_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER5_RISE != BSP_IRQ_DISABLED)
    eptpc_timer5_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER0_FALL != BSP_IRQ_DISABLED)
    eptpc_timer0_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER1_FALL != BSP_IRQ_DISABLED)
    eptpc_timer1_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER2_FALL != BSP_IRQ_DISABLED)
    eptpc_timer2_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER3_FALL != BSP_IRQ_DISABLED)
    eptpc_timer3_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER4_FALL != BSP_IRQ_DISABLED)
    eptpc_timer4_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER5_FALL != BSP_IRQ_DISABLED)
    eptpc_timer5_fall_isr,
#endif
#if (BSP_IRQ_CFG_USBHS_FIFO_0 != BSP_IRQ_DISABLED)
    usbhs_fifo_0_isr,
#endif
#if (BSP_IRQ_CFG_USBHS_FIFO_1 != BSP_IRQ_DISABLED)
    usbhs_fifo_1_isr,
#endif
#if (BSP_IRQ_CFG_USBHS_USB_INT_RESUME != BSP_IRQ_DISABLED)
    usbhs_usb_int_resume_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_RXI != BSP_IRQ_DISABLED)
    sci0_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_TXI != BSP_IRQ_DISABLED)
    sci0_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_TEI != BSP_IRQ_DISABLED)
    sci0_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_ERI != BSP_IRQ_DISABLED)
    sci0_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_AM != BSP_IRQ_DISABLED)
    sci0_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_RXI_OR_ERI != BSP_IRQ_DISABLED)
    sci0_rxi_or_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_RXI != BSP_IRQ_DISABLED)
    sci1_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_TXI != BSP_IRQ_DISABLED)
    sci1_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_TEI != BSP_IRQ_DISABLED)
    sci1_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_ERI != BSP_IRQ_DISABLED)
    sci1_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_AM != BSP_IRQ_DISABLED)
    sci1_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_RXI != BSP_IRQ_DISABLED)
    sci2_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_TXI != BSP_IRQ_DISABLED)
    sci2_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_TEI != BSP_IRQ_DISABLED)
    sci2_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_ERI != BSP_IRQ_DISABLED)
    sci2_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_AM != BSP_IRQ_DISABLED)
    sci2_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_RXI != BSP_IRQ_DISABLED)
    sci3_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_TXI != BSP_IRQ_DISABLED)
    sci3_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_TEI != BSP_IRQ_DISABLED)
    sci3_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_ERI != BSP_IRQ_DISABLED)
    sci3_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_AM != BSP_IRQ_DISABLED)
    sci3_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_RXI != BSP_IRQ_DISABLED)
    sci4_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_TXI != BSP_IRQ_DISABLED)
    sci4_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_TEI != BSP_IRQ_DISABLED)
    sci4_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_ERI != BSP_IRQ_DISABLED)
    sci4_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_AM != BSP_IRQ_DISABLED)
    sci4_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_RXI != BSP_IRQ_DISABLED)
    sci5_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_TXI != BSP_IRQ_DISABLED)
    sci5_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_TEI != BSP_IRQ_DISABLED)
    sci5_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_ERI != BSP_IRQ_DISABLED)
    sci5_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_AM != BSP_IRQ_DISABLED)
    sci5_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_RXI != BSP_IRQ_DISABLED)
    sci6_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_TXI != BSP_IRQ_DISABLED)
    sci6_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_TEI != BSP_IRQ_DISABLED)
    sci6_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_ERI != BSP_IRQ_DISABLED)
    sci6_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_AM != BSP_IRQ_DISABLED)
    sci6_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_RXI != BSP_IRQ_DISABLED)
    sci7_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_TXI != BSP_IRQ_DISABLED)
    sci7_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_TEI != BSP_IRQ_DISABLED)
    sci7_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_ERI != BSP_IRQ_DISABLED)
    sci7_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_AM != BSP_IRQ_DISABLED)
    sci7_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_RXI != BSP_IRQ_DISABLED)
    sci8_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_TXI != BSP_IRQ_DISABLED)
    sci8_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_TEI != BSP_IRQ_DISABLED)
    sci8_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_ERI != BSP_IRQ_DISABLED)
    sci8_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_AM != BSP_IRQ_DISABLED)
    sci8_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_RXI != BSP_IRQ_DISABLED)
    sci9_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_TXI != BSP_IRQ_DISABLED)
    sci9_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_TEI != BSP_IRQ_DISABLED)
    sci9_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_ERI != BSP_IRQ_DISABLED)
    sci9_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_AM != BSP_IRQ_DISABLED)
    sci9_am_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_RXI != BSP_IRQ_DISABLED)
    spi0_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_TXI != BSP_IRQ_DISABLED)
    spi0_txi_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_IDLE != BSP_IRQ_DISABLED)
    spi0_idle_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_ERI != BSP_IRQ_DISABLED)
    spi0_eri_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_TEI != BSP_IRQ_DISABLED)
    spi0_tei_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_RXI != BSP_IRQ_DISABLED)
    spi1_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_TXI != BSP_IRQ_DISABLED)
    spi1_txi_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_IDLE != BSP_IRQ_DISABLED)
    spi1_idle_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_ERI != BSP_IRQ_DISABLED)
    spi1_eri_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_TEI != BSP_IRQ_DISABLED)
    spi1_tei_isr,
#endif
#if (BSP_IRQ_CFG_QSPI_INT != BSP_IRQ_DISABLED)
    qspi_int_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_ACCS != BSP_IRQ_DISABLED)
    sdhimmc0_accs_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_SDIO != BSP_IRQ_DISABLED)
    sdhimmc0_sdio_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_CARD != BSP_IRQ_DISABLED)
    sdhimmc0_card_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_DMA_REQ != BSP_IRQ_DISABLED)
    sdhimmc0_dma_req_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_ACCS != BSP_IRQ_DISABLED)
    sdhimmc1_accs_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_SDIO != BSP_IRQ_DISABLED)
    sdhimmc1_sdio_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_CARD != BSP_IRQ_DISABLED)
    sdhimmc1_card_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_DMA_REQ != BSP_IRQ_DISABLED)
    sdhimmc1_dma_req_isr,
#endif
#if (BSP_IRQ_CFG_SCE_PROC_BUSY != BSP_IRQ_DISABLED)
    sce_proc_busy_isr,
#endif
#if (BSP_IRQ_CFG_SCE_ROMOK != BSP_IRQ_DISABLED)
    sce_romok_isr,
#endif
#if (BSP_IRQ_CFG_SCE_LONG_PLG != BSP_IRQ_DISABLED)
    sce_long_plg_isr,
#endif
#if (BSP_IRQ_CFG_SCE_TEST_BUSY != BSP_IRQ_DISABLED)
    sce_test_busy_isr,
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_0 != BSP_IRQ_DISABLED)
    sce_wrrdy_0_isr,
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_1 != BSP_IRQ_DISABLED)
    sce_wrrdy_1_isr,
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_4 != BSP_IRQ_DISABLED)
    sce_wrrdy_4_isr,
#endif
#if (BSP_IRQ_CFG_SCE_RDRDY_0 != BSP_IRQ_DISABLED)
    sce_rdrdy_0_isr,
#endif
#if (BSP_IRQ_CFG_SCE_RDRDY_1 != BSP_IRQ_DISABLED)
    sce_rdrdy_1_isr,
#endif
#if (BSP_IRQ_CFG_SCE_INTEGRATE_WRRDY != BSP_IRQ_DISABLED)
    sce_integrate_wrrdy_isr,
#endif
#if (BSP_IRQ_CFG_SCE_INTEGRATE_RDRDY != BSP_IRQ_DISABLED)
    sce_integrate_rdrdy_isr,
#endif
#if (BSP_IRQ_CFG_GLCDC_LINE_DETECT != BSP_IRQ_DISABLED)
    glcdc_line_detect_isr,
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_1 != BSP_IRQ_DISABLED)
    glcdc_underflow_1_isr,
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_2 != BSP_IRQ_DISABLED)
    glcdc_underflow_2_isr,
#endif
#if (BSP_IRQ_CFG_DRW_INT != BSP_IRQ_DISABLED)
    drw_int_isr,
#endif
#if (BSP_IRQ_CFG_JPEG_JEDI != BSP_IRQ_DISABLED)
    jpeg_jedi_isr,
#endif
#if (BSP_IRQ_CFG_JPEG_JDTI != BSP_IRQ_DISABLED)
    jpeg_jdti_isr,
#endif
};

#endif
