/*******************************************************************************
* File Name: Timer_Data.h
* Version 2.70
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_60_Timer_Data_H)
#define CY_Timer_v2_60_Timer_Data_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Timer_Data_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define Timer_Data_Resolution                 32u
#define Timer_Data_UsingFixedFunction         0u
#define Timer_Data_UsingHWCaptureCounter      0u
#define Timer_Data_SoftwareCaptureMode        0u
#define Timer_Data_SoftwareTriggerMode        0u
#define Timer_Data_UsingHWEnable              0u
#define Timer_Data_EnableTriggerMode          0u
#define Timer_Data_InterruptOnCaptureCount    0u
#define Timer_Data_RunModeUsed                0u
#define Timer_Data_ControlRegRemoved          0u

#if defined(Timer_Data_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define Timer_Data_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (Timer_Data_UsingFixedFunction)
    #define Timer_Data_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define Timer_Data_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End Timer_Data_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!Timer_Data_UsingFixedFunction)

        uint32 TimerUdb;
        uint8 InterruptMaskValue;
        #if (Timer_Data_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!Timer_Data_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}Timer_Data_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    Timer_Data_Start(void) ;
void    Timer_Data_Stop(void) ;

void    Timer_Data_SetInterruptMode(uint8 interruptMode) ;
uint8   Timer_Data_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define Timer_Data_GetInterruptSource() Timer_Data_ReadStatusRegister()

#if(!Timer_Data_UDB_CONTROL_REG_REMOVED)
    uint8   Timer_Data_ReadControlRegister(void) ;
    void    Timer_Data_WriteControlRegister(uint8 control) ;
#endif /* (!Timer_Data_UDB_CONTROL_REG_REMOVED) */

uint32  Timer_Data_ReadPeriod(void) ;
void    Timer_Data_WritePeriod(uint32 period) ;
uint32  Timer_Data_ReadCounter(void) ;
void    Timer_Data_WriteCounter(uint32 counter) ;
uint32  Timer_Data_ReadCapture(void) ;
void    Timer_Data_SoftwareCapture(void) ;

#if(!Timer_Data_UsingFixedFunction) /* UDB Prototypes */
    #if (Timer_Data_SoftwareCaptureMode)
        void    Timer_Data_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!Timer_Data_UsingFixedFunction) */

    #if (Timer_Data_SoftwareTriggerMode)
        void    Timer_Data_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (Timer_Data_SoftwareTriggerMode) */

    #if (Timer_Data_EnableTriggerMode)
        void    Timer_Data_EnableTrigger(void) ;
        void    Timer_Data_DisableTrigger(void) ;
    #endif /* (Timer_Data_EnableTriggerMode) */


    #if(Timer_Data_InterruptOnCaptureCount)
        void    Timer_Data_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (Timer_Data_InterruptOnCaptureCount) */

    #if (Timer_Data_UsingHWCaptureCounter)
        void    Timer_Data_SetCaptureCount(uint8 captureCount) ;
        uint8   Timer_Data_ReadCaptureCount(void) ;
    #endif /* (Timer_Data_UsingHWCaptureCounter) */

    void Timer_Data_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void Timer_Data_Init(void)          ;
void Timer_Data_Enable(void)        ;
void Timer_Data_SaveConfig(void)    ;
void Timer_Data_RestoreConfig(void) ;
void Timer_Data_Sleep(void)         ;
void Timer_Data_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define Timer_Data__B_TIMER__CM_NONE 0
#define Timer_Data__B_TIMER__CM_RISINGEDGE 1
#define Timer_Data__B_TIMER__CM_FALLINGEDGE 2
#define Timer_Data__B_TIMER__CM_EITHEREDGE 3
#define Timer_Data__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define Timer_Data__B_TIMER__TM_NONE 0x00u
#define Timer_Data__B_TIMER__TM_RISINGEDGE 0x04u
#define Timer_Data__B_TIMER__TM_FALLINGEDGE 0x08u
#define Timer_Data__B_TIMER__TM_EITHEREDGE 0x0Cu
#define Timer_Data__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define Timer_Data_INIT_PERIOD             240009998u
#define Timer_Data_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << Timer_Data_CTRL_CAP_MODE_SHIFT))
#define Timer_Data_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << Timer_Data_CTRL_TRIG_MODE_SHIFT))
#if (Timer_Data_UsingFixedFunction)
    #define Timer_Data_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << Timer_Data_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << Timer_Data_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define Timer_Data_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << Timer_Data_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Timer_Data_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Timer_Data_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (Timer_Data_UsingFixedFunction) */
#define Timer_Data_INIT_CAPTURE_COUNT      (2u)
#define Timer_Data_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << Timer_Data_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (Timer_Data_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define Timer_Data_STATUS         (*(reg8 *) Timer_Data_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define Timer_Data_STATUS_MASK    (*(reg8 *) Timer_Data_TimerHW__SR0 )
    #define Timer_Data_CONTROL        (*(reg8 *) Timer_Data_TimerHW__CFG0)
    #define Timer_Data_CONTROL2       (*(reg8 *) Timer_Data_TimerHW__CFG1)
    #define Timer_Data_CONTROL2_PTR   ( (reg8 *) Timer_Data_TimerHW__CFG1)
    #define Timer_Data_RT1            (*(reg8 *) Timer_Data_TimerHW__RT1)
    #define Timer_Data_RT1_PTR        ( (reg8 *) Timer_Data_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define Timer_Data_CONTROL3       (*(reg8 *) Timer_Data_TimerHW__CFG2)
        #define Timer_Data_CONTROL3_PTR   ( (reg8 *) Timer_Data_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define Timer_Data_GLOBAL_ENABLE  (*(reg8 *) Timer_Data_TimerHW__PM_ACT_CFG)
    #define Timer_Data_GLOBAL_STBY_ENABLE  (*(reg8 *) Timer_Data_TimerHW__PM_STBY_CFG)

    #define Timer_Data_CAPTURE_LSB         (* (reg16 *) Timer_Data_TimerHW__CAP0 )
    #define Timer_Data_CAPTURE_LSB_PTR       ((reg16 *) Timer_Data_TimerHW__CAP0 )
    #define Timer_Data_PERIOD_LSB          (* (reg16 *) Timer_Data_TimerHW__PER0 )
    #define Timer_Data_PERIOD_LSB_PTR        ((reg16 *) Timer_Data_TimerHW__PER0 )
    #define Timer_Data_COUNTER_LSB         (* (reg16 *) Timer_Data_TimerHW__CNT_CMP0 )
    #define Timer_Data_COUNTER_LSB_PTR       ((reg16 *) Timer_Data_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define Timer_Data_BLOCK_EN_MASK                     Timer_Data_TimerHW__PM_ACT_MSK
    #define Timer_Data_BLOCK_STBY_EN_MASK                Timer_Data_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define Timer_Data_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define Timer_Data_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define Timer_Data_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define Timer_Data_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define Timer_Data_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define Timer_Data_CTRL_ENABLE                        ((uint8)((uint8)0x01u << Timer_Data_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Timer_Data_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define Timer_Data_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << Timer_Data_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define Timer_Data_CTRL_MODE_SHIFT                 0x01u
        #define Timer_Data_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << Timer_Data_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define Timer_Data_CTRL_RCOD_SHIFT        0x02u
        #define Timer_Data_CTRL_ENBL_SHIFT        0x00u
        #define Timer_Data_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define Timer_Data_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << Timer_Data_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define Timer_Data_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << Timer_Data_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define Timer_Data_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << Timer_Data_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define Timer_Data_CTRL_RCOD       ((uint8)((uint8)0x03u << Timer_Data_CTRL_RCOD_SHIFT))
        #define Timer_Data_CTRL_ENBL       ((uint8)((uint8)0x80u << Timer_Data_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define Timer_Data_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define Timer_Data_RT1_MASK                        ((uint8)((uint8)0x03u << Timer_Data_RT1_SHIFT))
    #define Timer_Data_SYNC                            ((uint8)((uint8)0x03u << Timer_Data_RT1_SHIFT))
    #define Timer_Data_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define Timer_Data_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << Timer_Data_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define Timer_Data_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << Timer_Data_SYNCDSI_SHIFT))

    #define Timer_Data_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << Timer_Data_CTRL_MODE_SHIFT))
    #define Timer_Data_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << Timer_Data_CTRL_MODE_SHIFT))
    #define Timer_Data_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << Timer_Data_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Timer_Data_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Timer_Data_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Timer_Data_STATUS_TC_INT_MASK_SHIFT        (Timer_Data_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Timer_Data_STATUS_CAPTURE_INT_MASK_SHIFT   (Timer_Data_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define Timer_Data_STATUS_TC                       ((uint8)((uint8)0x01u << Timer_Data_STATUS_TC_SHIFT))
    #define Timer_Data_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << Timer_Data_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define Timer_Data_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << Timer_Data_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define Timer_Data_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << Timer_Data_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define Timer_Data_STATUS              (* (reg8 *) Timer_Data_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define Timer_Data_STATUS_MASK         (* (reg8 *) Timer_Data_TimerUDB_rstSts_stsreg__MASK_REG)
    #define Timer_Data_STATUS_AUX_CTRL     (* (reg8 *) Timer_Data_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Timer_Data_CONTROL             (* (reg8 *) Timer_Data_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(Timer_Data_Resolution <= 8u) /* 8-bit Timer */
        #define Timer_Data_CAPTURE_LSB         (* (reg8 *) Timer_Data_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Timer_Data_CAPTURE_LSB_PTR       ((reg8 *) Timer_Data_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Timer_Data_PERIOD_LSB          (* (reg8 *) Timer_Data_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Timer_Data_PERIOD_LSB_PTR        ((reg8 *) Timer_Data_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Timer_Data_COUNTER_LSB         (* (reg8 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define Timer_Data_COUNTER_LSB_PTR       ((reg8 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
    #elif(Timer_Data_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define Timer_Data_CAPTURE_LSB         (* (reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Timer_Data_CAPTURE_LSB_PTR       ((reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Timer_Data_PERIOD_LSB          (* (reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Timer_Data_PERIOD_LSB_PTR        ((reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Timer_Data_COUNTER_LSB         (* (reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define Timer_Data_COUNTER_LSB_PTR       ((reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define Timer_Data_CAPTURE_LSB         (* (reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define Timer_Data_CAPTURE_LSB_PTR       ((reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define Timer_Data_PERIOD_LSB          (* (reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define Timer_Data_PERIOD_LSB_PTR        ((reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define Timer_Data_COUNTER_LSB         (* (reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
            #define Timer_Data_COUNTER_LSB_PTR       ((reg16 *) Timer_Data_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(Timer_Data_Resolution <= 24u)/* 24-bit Timer */
        #define Timer_Data_CAPTURE_LSB         (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Timer_Data_CAPTURE_LSB_PTR       ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Timer_Data_PERIOD_LSB          (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Timer_Data_PERIOD_LSB_PTR        ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Timer_Data_COUNTER_LSB         (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define Timer_Data_COUNTER_LSB_PTR       ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define Timer_Data_CAPTURE_LSB         (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Timer_Data_CAPTURE_LSB_PTR       ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Timer_Data_PERIOD_LSB          (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Timer_Data_PERIOD_LSB_PTR        ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Timer_Data_COUNTER_LSB         (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define Timer_Data_COUNTER_LSB_PTR       ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define Timer_Data_CAPTURE_LSB         (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define Timer_Data_CAPTURE_LSB_PTR       ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define Timer_Data_PERIOD_LSB          (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define Timer_Data_PERIOD_LSB_PTR        ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define Timer_Data_COUNTER_LSB         (* (reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
            #define Timer_Data_COUNTER_LSB_PTR       ((reg32 *) Timer_Data_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define Timer_Data_COUNTER_LSB_PTR_8BIT       ((reg8 *) Timer_Data_TimerUDB_sT32_timerdp_u0__A0_REG )
    
    #if (Timer_Data_UsingHWCaptureCounter)
        #define Timer_Data_CAP_COUNT              (*(reg8 *) Timer_Data_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Timer_Data_CAP_COUNT_PTR          ( (reg8 *) Timer_Data_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Timer_Data_CAPTURE_COUNT_CTRL     (*(reg8 *) Timer_Data_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define Timer_Data_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) Timer_Data_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (Timer_Data_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define Timer_Data_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define Timer_Data_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define Timer_Data_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define Timer_Data_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define Timer_Data_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define Timer_Data_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << Timer_Data_CTRL_INTCNT_SHIFT))
    #define Timer_Data_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << Timer_Data_CTRL_TRIG_MODE_SHIFT))
    #define Timer_Data_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << Timer_Data_CTRL_TRIG_EN_SHIFT))
    #define Timer_Data_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << Timer_Data_CTRL_CAP_MODE_SHIFT))
    #define Timer_Data_CTRL_ENABLE                    ((uint8)((uint8)0x01u << Timer_Data_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define Timer_Data_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define Timer_Data_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define Timer_Data_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define Timer_Data_STATUS_TC_INT_MASK_SHIFT       Timer_Data_STATUS_TC_SHIFT
    #define Timer_Data_STATUS_CAPTURE_INT_MASK_SHIFT  Timer_Data_STATUS_CAPTURE_SHIFT
    #define Timer_Data_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define Timer_Data_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define Timer_Data_STATUS_FIFOFULL_INT_MASK_SHIFT Timer_Data_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define Timer_Data_STATUS_TC                      ((uint8)((uint8)0x01u << Timer_Data_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define Timer_Data_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << Timer_Data_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Timer_Data_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << Timer_Data_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Timer_Data_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << Timer_Data_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define Timer_Data_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << Timer_Data_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define Timer_Data_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << Timer_Data_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Timer_Data_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << Timer_Data_STATUS_FIFOFULL_SHIFT))

    #define Timer_Data_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define Timer_Data_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define Timer_Data_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define Timer_Data_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define Timer_Data_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define Timer_Data_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_Timer_Data_H */


/* [] END OF FILE */
