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
* File Name    : bsp_irq_cfg_ref.h
* Description  : This configuration header file determines which ELC events are mapped to trigger interrupts in the
*                NVIC.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_CONFIG_ARIS
 * @defgroup BSP_CONFIG_ARIS_INTERRUPTS Build Time Configurations - Interrupts
 *
 * Each ELC event that can be configured to trigger an interrupt is listed below. If the user wishes for a particular
 * event to trigger an interrupt then they will need to change the macro's definition from BSP IRQ DISABLED to
 * the interrupt priority level they wish to use for the interrupt. The valid priority range for this MCU is
 * 0 to 15 with lower numbers having a higher priority (0 = max priority, 15 = min priority).
 * The BSP will take care of putting the proper function in the vector table, will set the interrupt priority level,
 * and will configure the ELC event to generate an interrupt after reset and before the user's application is called.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_IRQ_CFG_REF_H_
#define BSP_IRQ_CFG_REF_H_

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define BSP_IRQ_CFG_ICU_IRQ0                          (BSP_IRQ_DISABLED) ///< PORT0 IRQ
#define BSP_IRQ_CFG_ICU_IRQ1                          (BSP_IRQ_DISABLED) ///< PORT1 IRQ
#define BSP_IRQ_CFG_ICU_IRQ2                          (BSP_IRQ_DISABLED) ///< PORT2 IRQ
#define BSP_IRQ_CFG_ICU_IRQ3                          (BSP_IRQ_DISABLED) ///< PORT3 IRQ
#define BSP_IRQ_CFG_ICU_IRQ4                          (BSP_IRQ_DISABLED) ///< PORT4 IRQ
#define BSP_IRQ_CFG_ICU_IRQ5                          (BSP_IRQ_DISABLED) ///< PORT5 IRQ
#define BSP_IRQ_CFG_ICU_IRQ6                          (BSP_IRQ_DISABLED) ///< PORT6 IRQ
#define BSP_IRQ_CFG_ICU_IRQ7                          (BSP_IRQ_DISABLED) ///< PORT7 IRQ
#define BSP_IRQ_CFG_ICU_IRQ8                          (BSP_IRQ_DISABLED) ///< PORT8 IRQ
#define BSP_IRQ_CFG_ICU_IRQ9                          (BSP_IRQ_DISABLED) ///< PORT9 IRQ
#define BSP_IRQ_CFG_ICU_IRQ10                         (BSP_IRQ_DISABLED) ///< PORT10 IRQ
#define BSP_IRQ_CFG_ICU_IRQ11                         (BSP_IRQ_DISABLED) ///< PORT11 IRQ
#define BSP_IRQ_CFG_ICU_IRQ12                         (BSP_IRQ_DISABLED) ///< PORT12 IRQ
#define BSP_IRQ_CFG_ICU_IRQ13                         (BSP_IRQ_DISABLED) ///< PORT13 IRQ
#define BSP_IRQ_CFG_ICU_IRQ14                         (BSP_IRQ_DISABLED) ///< PORT14 IRQ
#define BSP_IRQ_CFG_ICU_IRQ15                         (BSP_IRQ_DISABLED) ///< PORT15 IRQ
#define BSP_IRQ_CFG_DMAC0_INT                         (BSP_IRQ_DISABLED) ///< DMAC0 DMAC
#define BSP_IRQ_CFG_DMAC1_INT                         (BSP_IRQ_DISABLED) ///< DMAC1 DMAC
#define BSP_IRQ_CFG_DMAC2_INT                         (BSP_IRQ_DISABLED) ///< DMAC2 DMAC
#define BSP_IRQ_CFG_DMAC3_INT                         (BSP_IRQ_DISABLED) ///< DMAC3 DMAC
#define BSP_IRQ_CFG_DMAC4_INT                         (BSP_IRQ_DISABLED) ///< DMAC4 DMAC
#define BSP_IRQ_CFG_DMAC5_INT                         (BSP_IRQ_DISABLED) ///< DMAC5 DMAC
#define BSP_IRQ_CFG_DMAC6_INT                         (BSP_IRQ_DISABLED) ///< DMAC6 DMAC
#define BSP_IRQ_CFG_DMAC7_INT                         (BSP_IRQ_DISABLED) ///< DMAC7 DMAC
#define BSP_IRQ_CFG_DTC_COMPLETE                       (BSP_IRQ_DISABLED) ///< DTC COMPLETE
#define BSP_IRQ_CFG_DTC_END                        (BSP_IRQ_DISABLED) ///< DTC DTC END
#define BSP_IRQ_CFG_ICU_SNOOZE_CANCEL          (BSP_IRQ_DISABLED) ///< ICU CANCELING SNOOZE MODE
#define BSP_IRQ_CFG_FCU_FIFERR                         (BSP_IRQ_DISABLED) ///< FCU FIFERR
#define BSP_IRQ_CFG_FCU_FRDYI                          (BSP_IRQ_DISABLED) ///< FCU FRDYI
#define BSP_IRQ_CFG_LVD_LVD1                          (BSP_IRQ_DISABLED) ///< LVD1 LVD1
#define BSP_IRQ_CFG_LVD_LVD2                          (BSP_IRQ_DISABLED) ///< LVD2 LVD2
#define BSP_IRQ_CFG_CGC_MOSC_STOP                      (BSP_IRQ_DISABLED) ///< MOSC OSC STOP
#define BSP_IRQ_CFG_LPM_SNOOZE_REQUEST      (BSP_IRQ_DISABLED) ///< CPUSYS SNOOZE MODE ENTRY FLAG
#define BSP_IRQ_CFG_AGT0_INT                          (BSP_IRQ_DISABLED) ///< AGT0 AGTI
#define BSP_IRQ_CFG_AGT0_COMPARE_A                       (BSP_IRQ_DISABLED) ///< AGT0 AGTCMAI
#define BSP_IRQ_CFG_AGT0_COMPARE_B                       (BSP_IRQ_DISABLED) ///< AGT0 AGTCMBI
#define BSP_IRQ_CFG_AGT1_INT                          (BSP_IRQ_DISABLED) ///< AGT1 AGTI
#define BSP_IRQ_CFG_AGT1_COMPARE_A                       (BSP_IRQ_DISABLED) ///< AGT1 AGTCMAI
#define BSP_IRQ_CFG_AGT1_COMPARE_B                       (BSP_IRQ_DISABLED) ///< AGT1 AGTCMBI
#define BSP_IRQ_CFG_IWDT_UNDERFLOW                     (BSP_IRQ_DISABLED) ///< IWDT NMIUNDF N
#define BSP_IRQ_CFG_WDT_UNDERFLOW                     (BSP_IRQ_DISABLED) ///< CWDT NMIUNDF N
#define BSP_IRQ_CFG_RTC_ALARM                            (BSP_IRQ_DISABLED) ///< RTC ALM
#define BSP_IRQ_CFG_RTC_PERIOD                            (BSP_IRQ_DISABLED) ///< RTC PRD
#define BSP_IRQ_CFG_RTC_CARRY                            (BSP_IRQ_DISABLED) ///< RTC CUP
#define BSP_IRQ_CFG_ADC0_SCAN_END                         (BSP_IRQ_DISABLED) ///< S12AD0 ADI
#define BSP_IRQ_CFG_ADC0_SCAN_END_B                       (BSP_IRQ_DISABLED) ///< S12AD0 GBADI
#define BSP_IRQ_CFG_ADC0_WINDOW_A                       (BSP_IRQ_DISABLED) ///< S12AD0 CMPAI
#define BSP_IRQ_CFG_ADC0_WINDOW_B                       (BSP_IRQ_DISABLED) ///< S12AD0 CMPBI
#define BSP_IRQ_CFG_ADC0_COMPARE_MATCH               (BSP_IRQ_DISABLED) ///< S12AD0 COMPARE MATCH
#define BSP_IRQ_CFG_ADC0_COMPARE_MISMATCH            (BSP_IRQ_DISABLED) ///< S12AD0 COMPARE MISMATCH
#define BSP_IRQ_CFG_ADC1_SCAN_END                         (BSP_IRQ_DISABLED) ///< S12AD1 ADI
#define BSP_IRQ_CFG_ADC1_SCAN_END_B                       (BSP_IRQ_DISABLED) ///< S12AD1 GBADI
#define BSP_IRQ_CFG_ADC1_WINDOW_A                       (BSP_IRQ_DISABLED) ///< S12AD1 CMPAI
#define BSP_IRQ_CFG_ADC1_WINDOW_B                       (BSP_IRQ_DISABLED) ///< S12AD1 CMPBI
#define BSP_IRQ_CFG_ADC1_COMPARE_MATCH               (BSP_IRQ_DISABLED) ///< S12AD1 COMPARE MATCH
#define BSP_IRQ_CFG_ADC1_COMPARE_MISMATCH            (BSP_IRQ_DISABLED) ///< S12AD1 COMPARE MISMATCH
#define BSP_IRQ_CFG_COMP_HS_0                  (BSP_IRQ_DISABLED) ///< COMP OC0 COMP IRQ
#define BSP_IRQ_CFG_COMP_HS_1                  (BSP_IRQ_DISABLED) ///< COMP RD1 COMP IRQ
#define BSP_IRQ_CFG_COMP_HS_2                  (BSP_IRQ_DISABLED) ///< COMP RD2 COMP IRQ
#define BSP_IRQ_CFG_COMP_HS_3                  (BSP_IRQ_DISABLED) ///< COMP RD3 COMP IRQ
#define BSP_IRQ_CFG_COMP_HS_4                  (BSP_IRQ_DISABLED) ///< COMP RD4 COMP IRQ
#define BSP_IRQ_CFG_COMP_HS_5                  (BSP_IRQ_DISABLED) ///< COMP RD5 COMP IRQ
#define BSP_IRQ_CFG_USBFS_FIFO_0                       (BSP_IRQ_DISABLED) ///< USBFS D0FIFO
#define BSP_IRQ_CFG_USBFS_FIFO_1                       (BSP_IRQ_DISABLED) ///< USBFS D1FIFO
#define BSP_IRQ_CFG_USBFS_INT                         (BSP_IRQ_DISABLED) ///< USBFS USBI
#define BSP_IRQ_CFG_USBFS_RESUME                         (BSP_IRQ_DISABLED) ///< USBFS USBR
#define BSP_IRQ_CFG_IIC0_RXI                          (BSP_IRQ_DISABLED) ///< RIIC0 RXI
#define BSP_IRQ_CFG_IIC0_TXI                          (BSP_IRQ_DISABLED) ///< RIIC0 TXI
#define BSP_IRQ_CFG_IIC0_TEI                          (BSP_IRQ_DISABLED) ///< RIIC0 TEI
#define BSP_IRQ_CFG_IIC0_ERI                          (BSP_IRQ_DISABLED) ///< RIIC0 EEI
#define BSP_IRQ_CFG_IIC0_WUI                          (BSP_IRQ_DISABLED) ///< RIIC0 WUI
#define BSP_IRQ_CFG_IIC1_RXI                          (BSP_IRQ_DISABLED) ///< RIIC1 RXI
#define BSP_IRQ_CFG_IIC1_TXI                          (BSP_IRQ_DISABLED) ///< RIIC1 TXI
#define BSP_IRQ_CFG_IIC1_TEI                          (BSP_IRQ_DISABLED) ///< RIIC1 TEI
#define BSP_IRQ_CFG_IIC1_ERI                          (BSP_IRQ_DISABLED) ///< RIIC1 EEI
#define BSP_IRQ_CFG_IIC2_RXI                          (BSP_IRQ_DISABLED) ///< RIIC2 RXI
#define BSP_IRQ_CFG_IIC2_TXI                          (BSP_IRQ_DISABLED) ///< RIIC2 TXI
#define BSP_IRQ_CFG_IIC2_TEI                          (BSP_IRQ_DISABLED) ///< RIIC2 TEI
#define BSP_IRQ_CFG_IIC2_ERI                          (BSP_IRQ_DISABLED) ///< RIIC2 EEI
#define BSP_IRQ_CFG_SSI0_TXI                        (BSP_IRQ_DISABLED) ///< SSI0 SSITXI
#define BSP_IRQ_CFG_SSI0_RXI                        (BSP_IRQ_DISABLED) ///< SSI0 SSIRXI
#define BSP_IRQ_CFG_SSI0_INT                          (BSP_IRQ_DISABLED) ///< SSI0 SSIF
#define BSP_IRQ_CFG_SSI1_TXI_RXI                         (BSP_IRQ_DISABLED) ///< SSI1 SSIRT
#define BSP_IRQ_CFG_SSI1_INT                          (BSP_IRQ_DISABLED) ///< SSI1 SSIF
#define BSP_IRQ_CFG_SRC_INPUT_FIFO_EMPTY                           (BSP_IRQ_DISABLED) ///< SRC IDEI
#define BSP_IRQ_CFG_SRC_OUTPUT_FIFO_FULL                           (BSP_IRQ_DISABLED) ///< SRC ODFI
#define BSP_IRQ_CFG_SRC_OUTPUT_FIFO_OVERFLOW                            (BSP_IRQ_DISABLED) ///< SRC OVF
#define BSP_IRQ_CFG_SRC_OUTPUT_FIFO_UNDERFLOW                            (BSP_IRQ_DISABLED) ///< SRC UDF
#define BSP_IRQ_CFG_SRC_CONVERSION_END                            (BSP_IRQ_DISABLED) ///< SRC CEF
#define BSP_IRQ_CFG_PDC_RECEIVE_DATA_READY                          (BSP_IRQ_DISABLED) ///< PDC PCDFI
#define BSP_IRQ_CFG_PDC_FRAME_END                          (BSP_IRQ_DISABLED) ///< PDC PCFEI
#define BSP_IRQ_CFG_PDC_INT                          (BSP_IRQ_DISABLED) ///< PDC PCERI
#define BSP_IRQ_CFG_CTSU_WRITE                        (BSP_IRQ_DISABLED) ///< CTSU CTSUWR
#define BSP_IRQ_CFG_CTSU_READ                        (BSP_IRQ_DISABLED) ///< CTSU CTSURD
#define BSP_IRQ_CFG_CTSU_END                        (BSP_IRQ_DISABLED) ///< CTSU CTSUFN
#define BSP_IRQ_CFG_KEY_INT                          (BSP_IRQ_DISABLED) ///< KEY INTKR
#define BSP_IRQ_CFG_DOC_INT                          (BSP_IRQ_DISABLED) ///< DOC DOPCF
#define BSP_IRQ_CFG_CAC_FREQUENCY_ERROR                          (BSP_IRQ_DISABLED) ///< CAC FERRF
#define BSP_IRQ_CFG_CAC_MEASUREMENT_END                          (BSP_IRQ_DISABLED) ///< CAC MENDF
#define BSP_IRQ_CFG_CAC_OVERFLOW                           (BSP_IRQ_DISABLED) ///< CAC OVFF
#define BSP_IRQ_CFG_CAN0_ERROR                         (BSP_IRQ_DISABLED) ///< RCAN20 ERS
#define BSP_IRQ_CFG_CAN0_FIFO_RX                         (BSP_IRQ_DISABLED) ///< RCAN20 RXF
#define BSP_IRQ_CFG_CAN0_FIFO_TX                         (BSP_IRQ_DISABLED) ///< RCAN20 TXF
#define BSP_IRQ_CFG_CAN0_MAILBOX_RX                         (BSP_IRQ_DISABLED) ///< RCAN20 RXM
#define BSP_IRQ_CFG_CAN0_MAILBOX_TX                         (BSP_IRQ_DISABLED) ///< RCAN20 TXM
#define BSP_IRQ_CFG_CAN1_ERROR                         (BSP_IRQ_DISABLED) ///< RCAN21 ERS
#define BSP_IRQ_CFG_CAN1_FIFO_RX                         (BSP_IRQ_DISABLED) ///< RCAN21 RXF
#define BSP_IRQ_CFG_CAN1_FIFO_TX                         (BSP_IRQ_DISABLED) ///< RCAN21 TXF
#define BSP_IRQ_CFG_CAN1_MAILBOX_RX                         (BSP_IRQ_DISABLED) ///< RCAN21 RXM
#define BSP_IRQ_CFG_CAN1_MAILBOX_TX                         (BSP_IRQ_DISABLED) ///< RCAN21 TXM
#define BSP_IRQ_CFG_IOPORT_EVENT_1                  (BSP_IRQ_DISABLED) ///< GPIO PORT GROUP A
#define BSP_IRQ_CFG_IOPORT_EVENT_2                  (BSP_IRQ_DISABLED) ///< GPIO PORT GROUP B
#define BSP_IRQ_CFG_IOPORT_EVENT_3                  (BSP_IRQ_DISABLED) ///< GPIO PORT GROUP C
#define BSP_IRQ_CFG_IOPORT_EVENT_4                  (BSP_IRQ_DISABLED) ///< GPIO PORT GROUP D
#define BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0                (BSP_IRQ_DISABLED) ///< ELC0 SOFTWARE EVENT
#define BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_1                (BSP_IRQ_DISABLED) ///< ELC1 SOFTWARE EVENT
#define BSP_IRQ_CFG_POEG0_EVENT                  (BSP_IRQ_DISABLED) ///< POEG GROUP EVENT0
#define BSP_IRQ_CFG_POEG1_EVENT                  (BSP_IRQ_DISABLED) ///< POEG GROUP EVENT1
#define BSP_IRQ_CFG_POEG2_EVENT                  (BSP_IRQ_DISABLED) ///< POEG GROUP EVENT2
#define BSP_IRQ_CFG_POEG3_EVENT                  (BSP_IRQ_DISABLED) ///< POEG GROUP EVENT3
#define BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT0 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT0 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT0_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT0 COMPARE INT C
#define BSP_IRQ_CFG_GPT0_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT0 COMPARE INT D
#define BSP_IRQ_CFG_GPT0_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT0 COMPARE INT E
#define BSP_IRQ_CFG_GPT0_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT0 COMPARE INT F
#define BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT0 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT0_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT0 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT0_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT0 AD TRIG A
#define BSP_IRQ_CFG_GPT0_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT0 AD TRIG B
#define BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT1 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT1 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT1_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT1 COMPARE INT C
#define BSP_IRQ_CFG_GPT1_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT1 COMPARE INT D
#define BSP_IRQ_CFG_GPT1_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT1 COMPARE INT E
#define BSP_IRQ_CFG_GPT1_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT1 COMPARE INT F
#define BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT1 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT1_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT1 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT1_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT1 AD TRIG A
#define BSP_IRQ_CFG_GPT1_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT1 AD TRIG B
#define BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT2 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT2 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT2_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT2 COMPARE INT C
#define BSP_IRQ_CFG_GPT2_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT2 COMPARE INT D
#define BSP_IRQ_CFG_GPT2_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT2 COMPARE INT E
#define BSP_IRQ_CFG_GPT2_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT2 COMPARE INT F
#define BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT2 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT2_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT2 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT2_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT2 AD TRIG A
#define BSP_IRQ_CFG_GPT2_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT2 AD TRIG B
#define BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT3 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT3 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT3_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT3 COMPARE INT C
#define BSP_IRQ_CFG_GPT3_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT3 COMPARE INT D
#define BSP_IRQ_CFG_GPT3_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT3 COMPARE INT E
#define BSP_IRQ_CFG_GPT3_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT3 COMPARE INT F
#define BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT3 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT3_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT3 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT3_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT3 AD TRIG A
#define BSP_IRQ_CFG_GPT3_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT3 AD TRIG B
#define BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT4 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT4 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT4_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT4 COMPARE INT C
#define BSP_IRQ_CFG_GPT4_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT4 COMPARE INT D
#define BSP_IRQ_CFG_GPT4_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT4 COMPARE INT E
#define BSP_IRQ_CFG_GPT4_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT4 COMPARE INT F
#define BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT4 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT4_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT4 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT4_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT4 AD TRIG A
#define BSP_IRQ_CFG_GPT4_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT4 AD TRIG B
#define BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT5 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT5 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT5_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT5 COMPARE INT C
#define BSP_IRQ_CFG_GPT5_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT5 COMPARE INT D
#define BSP_IRQ_CFG_GPT5_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT5 COMPARE INT E
#define BSP_IRQ_CFG_GPT5_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT5 COMPARE INT F
#define BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT5 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT5_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT5 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT5_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT5 AD TRIG A
#define BSP_IRQ_CFG_GPT5_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT5 AD TRIG B
#define BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT6 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT6 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT6_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT6 COMPARE INT C
#define BSP_IRQ_CFG_GPT6_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT6 COMPARE INT D
#define BSP_IRQ_CFG_GPT6_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT6 COMPARE INT E
#define BSP_IRQ_CFG_GPT6_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT6 COMPARE INT F
#define BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT6 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT6_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT6 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT6_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT6 AD TRIG A
#define BSP_IRQ_CFG_GPT6_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT6 AD TRIG B
#define BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT7 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT7 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT7_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT7 COMPARE INT C
#define BSP_IRQ_CFG_GPT7_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT7 COMPARE INT D
#define BSP_IRQ_CFG_GPT7_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT7 COMPARE INT E
#define BSP_IRQ_CFG_GPT7_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT7 COMPARE INT F
#define BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT7 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT7_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT7 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT7_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT7 AD TRIG A
#define BSP_IRQ_CFG_GPT7_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT7 AD TRIG B
#define BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT8 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT8 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT8_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT8 COMPARE INT C
#define BSP_IRQ_CFG_GPT8_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT8 COMPARE INT D
#define BSP_IRQ_CFG_GPT8_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT8 COMPARE INT E
#define BSP_IRQ_CFG_GPT8_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT8 COMPARE INT F
#define BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT8 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT8_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT8 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT8_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT8 AD TRIG A
#define BSP_IRQ_CFG_GPT8_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT8 AD TRIG B
#define BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_A         (BSP_IRQ_DISABLED) ///< GPT9 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_B         (BSP_IRQ_DISABLED) ///< GPT9 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT9_COMPARE_C                 (BSP_IRQ_DISABLED) ///< GPT9 COMPARE INT C
#define BSP_IRQ_CFG_GPT9_COMPARE_D                 (BSP_IRQ_DISABLED) ///< GPT9 COMPARE INT D
#define BSP_IRQ_CFG_GPT9_COMPARE_E                 (BSP_IRQ_DISABLED) ///< GPT9 COMPARE INT E
#define BSP_IRQ_CFG_GPT9_COMPARE_F                 (BSP_IRQ_DISABLED) ///< GPT9 COMPARE INT F
#define BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW              (BSP_IRQ_DISABLED) ///< GPT9 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT9_COUNTER_UNDERFLOW             (BSP_IRQ_DISABLED) ///< GPT9 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT9_AD_TRIG_A                     (BSP_IRQ_DISABLED) ///< GPT9 AD TRIG A
#define BSP_IRQ_CFG_GPT9_AD_TRIG_B                     (BSP_IRQ_DISABLED) ///< GPT9 AD TRIG B
#define BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_A        (BSP_IRQ_DISABLED) ///< GPT10 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_B        (BSP_IRQ_DISABLED) ///< GPT10 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT10_COMPARE_C                (BSP_IRQ_DISABLED) ///< GPT10 COMPARE INT C
#define BSP_IRQ_CFG_GPT10_COMPARE_D                (BSP_IRQ_DISABLED) ///< GPT10 COMPARE INT D
#define BSP_IRQ_CFG_GPT10_COMPARE_E                (BSP_IRQ_DISABLED) ///< GPT10 COMPARE INT E
#define BSP_IRQ_CFG_GPT10_COMPARE_F                (BSP_IRQ_DISABLED) ///< GPT10 COMPARE INT F
#define BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW             (BSP_IRQ_DISABLED) ///< GPT10 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT10_COUNTER_UNDERFLOW            (BSP_IRQ_DISABLED) ///< GPT10 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT10_AD_TRIG_A                    (BSP_IRQ_DISABLED) ///< GPT10 AD TRIG A
#define BSP_IRQ_CFG_GPT10_AD_TRIG_B                    (BSP_IRQ_DISABLED) ///< GPT10 AD TRIG B
#define BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_A        (BSP_IRQ_DISABLED) ///< GPT11 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_B        (BSP_IRQ_DISABLED) ///< GPT11 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT11_COMPARE_C                (BSP_IRQ_DISABLED) ///< GPT11 COMPARE INT C
#define BSP_IRQ_CFG_GPT11_COMPARE_D                (BSP_IRQ_DISABLED) ///< GPT11 COMPARE INT D
#define BSP_IRQ_CFG_GPT11_COMPARE_E                (BSP_IRQ_DISABLED) ///< GPT11 COMPARE INT E
#define BSP_IRQ_CFG_GPT11_COMPARE_F                (BSP_IRQ_DISABLED) ///< GPT11 COMPARE INT F
#define BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW             (BSP_IRQ_DISABLED) ///< GPT11 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT11_COUNTER_UNDERFLOW            (BSP_IRQ_DISABLED) ///< GPT11 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT11_AD_TRIG_A                    (BSP_IRQ_DISABLED) ///< GPT11 AD TRIG A
#define BSP_IRQ_CFG_GPT11_AD_TRIG_B                    (BSP_IRQ_DISABLED) ///< GPT11 AD TRIG B
#define BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_A        (BSP_IRQ_DISABLED) ///< GPT12 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_B        (BSP_IRQ_DISABLED) ///< GPT12 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT12_COMPARE_C                (BSP_IRQ_DISABLED) ///< GPT12 COMPARE INT C
#define BSP_IRQ_CFG_GPT12_COMPARE_D                (BSP_IRQ_DISABLED) ///< GPT12 COMPARE INT D
#define BSP_IRQ_CFG_GPT12_COMPARE_E                (BSP_IRQ_DISABLED) ///< GPT12 COMPARE INT E
#define BSP_IRQ_CFG_GPT12_COMPARE_F                (BSP_IRQ_DISABLED) ///< GPT12 COMPARE INT F
#define BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW             (BSP_IRQ_DISABLED) ///< GPT12 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT12_COUNTER_UNDERFLOW            (BSP_IRQ_DISABLED) ///< GPT12 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT12_AD_TRIG_A                    (BSP_IRQ_DISABLED) ///< GPT12 AD TRIG A
#define BSP_IRQ_CFG_GPT12_AD_TRIG_B                    (BSP_IRQ_DISABLED) ///< GPT12 AD TRIG B
#define BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_A        (BSP_IRQ_DISABLED) ///< GPT13 CAPTURE COMPARE INT A
#define BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_B        (BSP_IRQ_DISABLED) ///< GPT13 CAPTURE COMPARE INT B
#define BSP_IRQ_CFG_GPT13_COMPARE_C                (BSP_IRQ_DISABLED) ///< GPT13 COMPARE INT C
#define BSP_IRQ_CFG_GPT13_COMPARE_D                (BSP_IRQ_DISABLED) ///< GPT13 COMPARE INT D
#define BSP_IRQ_CFG_GPT13_COMPARE_E                (BSP_IRQ_DISABLED) ///< GPT13 COMPARE INT E
#define BSP_IRQ_CFG_GPT13_COMPARE_F                (BSP_IRQ_DISABLED) ///< GPT13 COMPARE INT F
#define BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW             (BSP_IRQ_DISABLED) ///< GPT13 COUNTER OVERFLOW
#define BSP_IRQ_CFG_GPT13_COUNTER_UNDERFLOW            (BSP_IRQ_DISABLED) ///< GPT13 COUNTER UNDERFLOW
#define BSP_IRQ_CFG_GPT13_AD_TRIG_A                    (BSP_IRQ_DISABLED) ///< GPT13 AD TRIG A
#define BSP_IRQ_CFG_GPT13_AD_TRIG_B                    (BSP_IRQ_DISABLED) ///< GPT13 AD TRIG B
#define BSP_IRQ_CFG_OPS_UVW_EDGE                       (BSP_IRQ_DISABLED) ///< GPT UVW EDGE
#define BSP_IRQ_CFG_EPTPC_IPLS                         (BSP_IRQ_DISABLED) ///< ETHER IPLS
#define BSP_IRQ_CFG_EPTPC_MINT                         (BSP_IRQ_DISABLED) ///< ETHER MINT
#define BSP_IRQ_CFG_EPTPC_PINT                         (BSP_IRQ_DISABLED) ///< ETHER PINT
#define BSP_IRQ_CFG_EDMAC0_EINT                        (BSP_IRQ_DISABLED) ///< ETHER EINT0
#define BSP_IRQ_CFG_EDMAC1_EINT                        (BSP_IRQ_DISABLED) ///< ETHER EINT1
#define BSP_IRQ_CFG_EPTPC_TIMER0_RISE                  (BSP_IRQ_DISABLED) ///< ETHER ETHER0 RISE
#define BSP_IRQ_CFG_EPTPC_TIMER1_RISE                  (BSP_IRQ_DISABLED) ///< ETHER ETHER1 RISE
#define BSP_IRQ_CFG_EPTPC_TIMER2_RISE                  (BSP_IRQ_DISABLED) ///< ETHER ETHER2 RISE
#define BSP_IRQ_CFG_EPTPC_TIMER3_RISE                  (BSP_IRQ_DISABLED) ///< ETHER ETHER3 RISE
#define BSP_IRQ_CFG_EPTPC_TIMER4_RISE                  (BSP_IRQ_DISABLED) ///< ETHER ETHER4 RISE
#define BSP_IRQ_CFG_EPTPC_TIMER5_RISE                  (BSP_IRQ_DISABLED) ///< ETHER ETHER5 RISE
#define BSP_IRQ_CFG_EPTPC_TIMER0_FALL                  (BSP_IRQ_DISABLED) ///< ETHER ETHER0 FALL
#define BSP_IRQ_CFG_EPTPC_TIMER1_FALL                  (BSP_IRQ_DISABLED) ///< ETHER ETHER1 FALL
#define BSP_IRQ_CFG_EPTPC_TIMER2_FALL                  (BSP_IRQ_DISABLED) ///< ETHER ETHER2 FALL
#define BSP_IRQ_CFG_EPTPC_TIMER3_FALL                  (BSP_IRQ_DISABLED) ///< ETHER ETHER3 FALL
#define BSP_IRQ_CFG_EPTPC_TIMER4_FALL                  (BSP_IRQ_DISABLED) ///< ETHER ETHER4 FALL
#define BSP_IRQ_CFG_EPTPC_TIMER5_FALL                  (BSP_IRQ_DISABLED) ///< ETHER ETHER5 FALL
#define BSP_IRQ_CFG_USBHS_FIFO_0                       (BSP_IRQ_DISABLED) ///< USBHS D0FIFO
#define BSP_IRQ_CFG_USBHS_FIFO_1                       (BSP_IRQ_DISABLED) ///< USBHS D1FIFO
#define BSP_IRQ_CFG_USBHS_USB_INT_RESUME                        (BSP_IRQ_DISABLED) ///< USBHS USBIR
#define BSP_IRQ_CFG_SCI0_RXI                           (BSP_IRQ_DISABLED) ///< SCI0 RXI
#define BSP_IRQ_CFG_SCI0_TXI                           (BSP_IRQ_DISABLED) ///< SCI0 TXI
#define BSP_IRQ_CFG_SCI0_TEI                           (BSP_IRQ_DISABLED) ///< SCI0 TEI
#define BSP_IRQ_CFG_SCI0_ERI                           (BSP_IRQ_DISABLED) ///< SCI0 ERI
#define BSP_IRQ_CFG_SCI0_AM                            (BSP_IRQ_DISABLED) ///< SCI0 AM
#define BSP_IRQ_CFG_SCI0_RXI_OR_ERI                    (BSP_IRQ_DISABLED) ///< SCI0 RXI OR ERI
#define BSP_IRQ_CFG_SCI1_RXI                           (BSP_IRQ_DISABLED) ///< SCI1 RXI
#define BSP_IRQ_CFG_SCI1_TXI                           (BSP_IRQ_DISABLED) ///< SCI1 TXI
#define BSP_IRQ_CFG_SCI1_TEI                           (BSP_IRQ_DISABLED) ///< SCI1 TEI
#define BSP_IRQ_CFG_SCI1_ERI                           (BSP_IRQ_DISABLED) ///< SCI1 ERI
#define BSP_IRQ_CFG_SCI1_AM                            (BSP_IRQ_DISABLED) ///< SCI1 AM
#define BSP_IRQ_CFG_SCI2_RXI                           (BSP_IRQ_DISABLED) ///< SCI2 RXI
#define BSP_IRQ_CFG_SCI2_TXI                           (BSP_IRQ_DISABLED) ///< SCI2 TXI
#define BSP_IRQ_CFG_SCI2_TEI                           (BSP_IRQ_DISABLED) ///< SCI2 TEI
#define BSP_IRQ_CFG_SCI2_ERI                           (BSP_IRQ_DISABLED) ///< SCI2 ERI
#define BSP_IRQ_CFG_SCI2_AM                            (BSP_IRQ_DISABLED) ///< SCI2 AM
#define BSP_IRQ_CFG_SCI3_RXI                           (BSP_IRQ_DISABLED) ///< SCI3 RXI
#define BSP_IRQ_CFG_SCI3_TXI                           (BSP_IRQ_DISABLED) ///< SCI3 TXI
#define BSP_IRQ_CFG_SCI3_TEI                           (BSP_IRQ_DISABLED) ///< SCI3 TEI
#define BSP_IRQ_CFG_SCI3_ERI                           (BSP_IRQ_DISABLED) ///< SCI3 ERI
#define BSP_IRQ_CFG_SCI3_AM                            (BSP_IRQ_DISABLED) ///< SCI3 AM
#define BSP_IRQ_CFG_SCI4_RXI                           (BSP_IRQ_DISABLED) ///< SCI4 RXI
#define BSP_IRQ_CFG_SCI4_TXI                           (BSP_IRQ_DISABLED) ///< SCI4 TXI
#define BSP_IRQ_CFG_SCI4_TEI                           (BSP_IRQ_DISABLED) ///< SCI4 TEI
#define BSP_IRQ_CFG_SCI4_ERI                           (BSP_IRQ_DISABLED) ///< SCI4 ERI
#define BSP_IRQ_CFG_SCI4_AM                            (BSP_IRQ_DISABLED) ///< SCI4 AM
#define BSP_IRQ_CFG_SCI5_RXI                           (BSP_IRQ_DISABLED) ///< SCI5 RXI
#define BSP_IRQ_CFG_SCI5_TXI                           (BSP_IRQ_DISABLED) ///< SCI5 TXI
#define BSP_IRQ_CFG_SCI5_TEI                           (BSP_IRQ_DISABLED) ///< SCI5 TEI
#define BSP_IRQ_CFG_SCI5_ERI                           (BSP_IRQ_DISABLED) ///< SCI5 ERI
#define BSP_IRQ_CFG_SCI5_AM                            (BSP_IRQ_DISABLED) ///< SCI5 AM
#define BSP_IRQ_CFG_SCI6_RXI                           (BSP_IRQ_DISABLED) ///< SCI6 RXI
#define BSP_IRQ_CFG_SCI6_TXI                           (BSP_IRQ_DISABLED) ///< SCI6 TXI
#define BSP_IRQ_CFG_SCI6_TEI                           (BSP_IRQ_DISABLED) ///< SCI6 TEI
#define BSP_IRQ_CFG_SCI6_ERI                           (BSP_IRQ_DISABLED) ///< SCI6 ERI
#define BSP_IRQ_CFG_SCI6_AM                            (BSP_IRQ_DISABLED) ///< SCI6 AM
#define BSP_IRQ_CFG_SCI7_RXI                           (BSP_IRQ_DISABLED) ///< SCI7 RXI
#define BSP_IRQ_CFG_SCI7_TXI                           (BSP_IRQ_DISABLED) ///< SCI7 TXI
#define BSP_IRQ_CFG_SCI7_TEI                           (BSP_IRQ_DISABLED) ///< SCI7 TEI
#define BSP_IRQ_CFG_SCI7_ERI                           (BSP_IRQ_DISABLED) ///< SCI7 ERI
#define BSP_IRQ_CFG_SCI7_AM                            (BSP_IRQ_DISABLED) ///< SCI7 AM
#define BSP_IRQ_CFG_SCI8_RXI                           (BSP_IRQ_DISABLED) ///< SCI8 RXI
#define BSP_IRQ_CFG_SCI8_TXI                           (BSP_IRQ_DISABLED) ///< SCI8 TXI
#define BSP_IRQ_CFG_SCI8_TEI                           (BSP_IRQ_DISABLED) ///< SCI8 TEI
#define BSP_IRQ_CFG_SCI8_ERI                           (BSP_IRQ_DISABLED) ///< SCI8 ERI
#define BSP_IRQ_CFG_SCI8_AM                            (BSP_IRQ_DISABLED) ///< SCI8 AM
#define BSP_IRQ_CFG_SCI9_RXI                           (BSP_IRQ_DISABLED) ///< SCI9 RXI
#define BSP_IRQ_CFG_SCI9_TXI                           (BSP_IRQ_DISABLED) ///< SCI9 TXI
#define BSP_IRQ_CFG_SCI9_TEI                           (BSP_IRQ_DISABLED) ///< SCI9 TEI
#define BSP_IRQ_CFG_SCI9_ERI                           (BSP_IRQ_DISABLED) ///< SCI9 ERI
#define BSP_IRQ_CFG_SCI9_AM                            (BSP_IRQ_DISABLED) ///< SCI9 AM
#define BSP_IRQ_CFG_SPI0_RXI                         (BSP_IRQ_DISABLED) ///< RSPI0 SPRI
#define BSP_IRQ_CFG_SPI0_TXI                         (BSP_IRQ_DISABLED) ///< RSPI0 SPTI
#define BSP_IRQ_CFG_SPI0_IDLE                         (BSP_IRQ_DISABLED) ///< RSPI0 SPII
#define BSP_IRQ_CFG_SPI0_ERI                         (BSP_IRQ_DISABLED) ///< RSPI0 SPEI
#define BSP_IRQ_CFG_SPI0_TEI                   (BSP_IRQ_DISABLED) ///< RSPI0 SP ELCTEND
#define BSP_IRQ_CFG_SPI1_RXI                         (BSP_IRQ_DISABLED) ///< RSPI1 SPRI
#define BSP_IRQ_CFG_SPI1_TXI                         (BSP_IRQ_DISABLED) ///< RSPI1 SPTI
#define BSP_IRQ_CFG_SPI1_IDLE                         (BSP_IRQ_DISABLED) ///< RSPI1 SPII
#define BSP_IRQ_CFG_SPI1_ERI                         (BSP_IRQ_DISABLED) ///< RSPI1 SPEI
#define BSP_IRQ_CFG_SPI1_TEI                   (BSP_IRQ_DISABLED) ///< RSPI1 SP ELCTEND
#define BSP_IRQ_CFG_QSPI_INT                          (BSP_IRQ_DISABLED) ///< QSPI INTR
#define BSP_IRQ_CFG_SDHIMMC0_ACCS                     (BSP_IRQ_DISABLED) ///< SDHI MMC0 ACCS
#define BSP_IRQ_CFG_SDHIMMC0_SDIO                     (BSP_IRQ_DISABLED) ///< SDHI MMC0 SDIO
#define BSP_IRQ_CFG_SDHIMMC0_CARD                     (BSP_IRQ_DISABLED) ///< SDHI MMC0 CARD
#define BSP_IRQ_CFG_SDHIMMC0_DMA_REQ                (BSP_IRQ_DISABLED) ///< SDHI MMC0 ODMSDBREQ
#define BSP_IRQ_CFG_SDHIMMC1_ACCS                     (BSP_IRQ_DISABLED) ///< SDHI MMC1 ACCS
#define BSP_IRQ_CFG_SDHIMMC1_SDIO                     (BSP_IRQ_DISABLED) ///< SDHI MMC1 SDIO
#define BSP_IRQ_CFG_SDHIMMC1_CARD                     (BSP_IRQ_DISABLED) ///< SDHI MMC1 CARD
#define BSP_IRQ_CFG_SDHIMMC1_DMA_REQ                (BSP_IRQ_DISABLED) ///< SDHI MMC1 ODMSDBREQ
#define BSP_IRQ_CFG_DIVIDER_INT                  (BSP_IRQ_DISABLED) ///< EXT DIVIDER INTMD
#define BSP_IRQ_CFG_SCE_PROC_BUSY                   (BSP_IRQ_DISABLED) ///< TSIP PROC BUSY N
#define BSP_IRQ_CFG_SCE_ROMOK                       (BSP_IRQ_DISABLED) ///< TSIP ROMOK N
#define BSP_IRQ_CFG_SCE_LONG_PLG                    (BSP_IRQ_DISABLED) ///< TSIP LONG PLG N
#define BSP_IRQ_CFG_SCE_TEST_BUSY                   (BSP_IRQ_DISABLED) ///< TSIP TEST BUSY N
#define BSP_IRQ_CFG_SCE_WRRDY_0                     (BSP_IRQ_DISABLED) ///< TSIP WRRDY 0 N
#define BSP_IRQ_CFG_SCE_WRRDY_1                     (BSP_IRQ_DISABLED) ///< TSIP WRRDY 1 N
#define BSP_IRQ_CFG_SCE_WRRDY_4                     (BSP_IRQ_DISABLED) ///< TSIP WRRDY 4 N
#define BSP_IRQ_CFG_SCE_RDRDY_0                     (BSP_IRQ_DISABLED) ///< TSIP RDRDY 0 N
#define BSP_IRQ_CFG_SCE_RDRDY_1                     (BSP_IRQ_DISABLED) ///< TSIP RDRDY 1 N
#define BSP_IRQ_CFG_SCE_INTEGRATE_WRRDY             (BSP_IRQ_DISABLED) ///< TSIP INTEGRATE WRRDY N
#define BSP_IRQ_CFG_SCE_INTEGRATE_RDRDY             (BSP_IRQ_DISABLED) ///< TSIP INTEGRATE RDRDY N
#define BSP_IRQ_CFG_GLCDC_LINE_DETECT                  (BSP_IRQ_DISABLED) ///< LCDC LCDC LEVEL 0
#define BSP_IRQ_CFG_GLCDC_UNDERFLOW_1                  (BSP_IRQ_DISABLED) ///< LCDC LCDC LEVEL 1
#define BSP_IRQ_CFG_GLCDC_UNDERFLOW_2                  (BSP_IRQ_DISABLED) ///< LCDC LCDC LEVEL 2
#define BSP_IRQ_CFG_DRW_INT                    (BSP_IRQ_DISABLED) ///< TWOD ENGINE IRQ
#define BSP_IRQ_CFG_JPEG_JEDI                          (BSP_IRQ_DISABLED) ///< JPEG JEDI
#define BSP_IRQ_CFG_JPEG_JDTI                          (BSP_IRQ_DISABLED) ///< JPEG JDTI

/** @} (end defgroup BSP_CONFIG_ARIS_INTERRUPTS) */

#endif /* BSP_IRQ_CFG_REF_H  */
