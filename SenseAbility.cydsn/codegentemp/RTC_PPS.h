/*******************************************************************************
* File Name: RTC_PPS.h
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

#if !defined(CY_Timer_v2_60_RTC_PPS_H)
#define CY_Timer_v2_60_RTC_PPS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 RTC_PPS_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define RTC_PPS_Resolution                 32u
#define RTC_PPS_UsingFixedFunction         0u
#define RTC_PPS_UsingHWCaptureCounter      0u
#define RTC_PPS_SoftwareCaptureMode        0u
#define RTC_PPS_SoftwareTriggerMode        0u
#define RTC_PPS_UsingHWEnable              0u
#define RTC_PPS_EnableTriggerMode          0u
#define RTC_PPS_InterruptOnCaptureCount    0u
#define RTC_PPS_RunModeUsed                0u
#define RTC_PPS_ControlRegRemoved          0u

#if defined(RTC_PPS_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define RTC_PPS_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (RTC_PPS_UsingFixedFunction)
    #define RTC_PPS_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define RTC_PPS_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End RTC_PPS_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!RTC_PPS_UsingFixedFunction)

        uint32 TimerUdb;
        uint8 InterruptMaskValue;
        #if (RTC_PPS_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!RTC_PPS_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}RTC_PPS_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    RTC_PPS_Start(void) ;
void    RTC_PPS_Stop(void) ;

void    RTC_PPS_SetInterruptMode(uint8 interruptMode) ;
uint8   RTC_PPS_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define RTC_PPS_GetInterruptSource() RTC_PPS_ReadStatusRegister()

#if(!RTC_PPS_UDB_CONTROL_REG_REMOVED)
    uint8   RTC_PPS_ReadControlRegister(void) ;
    void    RTC_PPS_WriteControlRegister(uint8 control) ;
#endif /* (!RTC_PPS_UDB_CONTROL_REG_REMOVED) */

uint32  RTC_PPS_ReadPeriod(void) ;
void    RTC_PPS_WritePeriod(uint32 period) ;
uint32  RTC_PPS_ReadCounter(void) ;
void    RTC_PPS_WriteCounter(uint32 counter) ;
uint32  RTC_PPS_ReadCapture(void) ;
void    RTC_PPS_SoftwareCapture(void) ;

#if(!RTC_PPS_UsingFixedFunction) /* UDB Prototypes */
    #if (RTC_PPS_SoftwareCaptureMode)
        void    RTC_PPS_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!RTC_PPS_UsingFixedFunction) */

    #if (RTC_PPS_SoftwareTriggerMode)
        void    RTC_PPS_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (RTC_PPS_SoftwareTriggerMode) */

    #if (RTC_PPS_EnableTriggerMode)
        void    RTC_PPS_EnableTrigger(void) ;
        void    RTC_PPS_DisableTrigger(void) ;
    #endif /* (RTC_PPS_EnableTriggerMode) */


    #if(RTC_PPS_InterruptOnCaptureCount)
        void    RTC_PPS_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (RTC_PPS_InterruptOnCaptureCount) */

    #if (RTC_PPS_UsingHWCaptureCounter)
        void    RTC_PPS_SetCaptureCount(uint8 captureCount) ;
        uint8   RTC_PPS_ReadCaptureCount(void) ;
    #endif /* (RTC_PPS_UsingHWCaptureCounter) */

    void RTC_PPS_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void RTC_PPS_Init(void)          ;
void RTC_PPS_Enable(void)        ;
void RTC_PPS_SaveConfig(void)    ;
void RTC_PPS_RestoreConfig(void) ;
void RTC_PPS_Sleep(void)         ;
void RTC_PPS_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define RTC_PPS__B_TIMER__CM_NONE 0
#define RTC_PPS__B_TIMER__CM_RISINGEDGE 1
#define RTC_PPS__B_TIMER__CM_FALLINGEDGE 2
#define RTC_PPS__B_TIMER__CM_EITHEREDGE 3
#define RTC_PPS__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define RTC_PPS__B_TIMER__TM_NONE 0x00u
#define RTC_PPS__B_TIMER__TM_RISINGEDGE 0x04u
#define RTC_PPS__B_TIMER__TM_FALLINGEDGE 0x08u
#define RTC_PPS__B_TIMER__TM_EITHEREDGE 0x0Cu
#define RTC_PPS__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define RTC_PPS_INIT_PERIOD             47999999u
#define RTC_PPS_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << RTC_PPS_CTRL_CAP_MODE_SHIFT))
#define RTC_PPS_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << RTC_PPS_CTRL_TRIG_MODE_SHIFT))
#if (RTC_PPS_UsingFixedFunction)
    #define RTC_PPS_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << RTC_PPS_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << RTC_PPS_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define RTC_PPS_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << RTC_PPS_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << RTC_PPS_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << RTC_PPS_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (RTC_PPS_UsingFixedFunction) */
#define RTC_PPS_INIT_CAPTURE_COUNT      (2u)
#define RTC_PPS_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << RTC_PPS_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (RTC_PPS_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define RTC_PPS_STATUS         (*(reg8 *) RTC_PPS_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define RTC_PPS_STATUS_MASK    (*(reg8 *) RTC_PPS_TimerHW__SR0 )
    #define RTC_PPS_CONTROL        (*(reg8 *) RTC_PPS_TimerHW__CFG0)
    #define RTC_PPS_CONTROL2       (*(reg8 *) RTC_PPS_TimerHW__CFG1)
    #define RTC_PPS_CONTROL2_PTR   ( (reg8 *) RTC_PPS_TimerHW__CFG1)
    #define RTC_PPS_RT1            (*(reg8 *) RTC_PPS_TimerHW__RT1)
    #define RTC_PPS_RT1_PTR        ( (reg8 *) RTC_PPS_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define RTC_PPS_CONTROL3       (*(reg8 *) RTC_PPS_TimerHW__CFG2)
        #define RTC_PPS_CONTROL3_PTR   ( (reg8 *) RTC_PPS_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define RTC_PPS_GLOBAL_ENABLE  (*(reg8 *) RTC_PPS_TimerHW__PM_ACT_CFG)
    #define RTC_PPS_GLOBAL_STBY_ENABLE  (*(reg8 *) RTC_PPS_TimerHW__PM_STBY_CFG)

    #define RTC_PPS_CAPTURE_LSB         (* (reg16 *) RTC_PPS_TimerHW__CAP0 )
    #define RTC_PPS_CAPTURE_LSB_PTR       ((reg16 *) RTC_PPS_TimerHW__CAP0 )
    #define RTC_PPS_PERIOD_LSB          (* (reg16 *) RTC_PPS_TimerHW__PER0 )
    #define RTC_PPS_PERIOD_LSB_PTR        ((reg16 *) RTC_PPS_TimerHW__PER0 )
    #define RTC_PPS_COUNTER_LSB         (* (reg16 *) RTC_PPS_TimerHW__CNT_CMP0 )
    #define RTC_PPS_COUNTER_LSB_PTR       ((reg16 *) RTC_PPS_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define RTC_PPS_BLOCK_EN_MASK                     RTC_PPS_TimerHW__PM_ACT_MSK
    #define RTC_PPS_BLOCK_STBY_EN_MASK                RTC_PPS_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define RTC_PPS_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define RTC_PPS_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define RTC_PPS_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define RTC_PPS_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define RTC_PPS_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define RTC_PPS_CTRL_ENABLE                        ((uint8)((uint8)0x01u << RTC_PPS_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define RTC_PPS_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define RTC_PPS_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << RTC_PPS_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define RTC_PPS_CTRL_MODE_SHIFT                 0x01u
        #define RTC_PPS_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << RTC_PPS_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define RTC_PPS_CTRL_RCOD_SHIFT        0x02u
        #define RTC_PPS_CTRL_ENBL_SHIFT        0x00u
        #define RTC_PPS_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define RTC_PPS_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << RTC_PPS_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define RTC_PPS_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << RTC_PPS_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define RTC_PPS_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << RTC_PPS_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define RTC_PPS_CTRL_RCOD       ((uint8)((uint8)0x03u << RTC_PPS_CTRL_RCOD_SHIFT))
        #define RTC_PPS_CTRL_ENBL       ((uint8)((uint8)0x80u << RTC_PPS_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define RTC_PPS_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define RTC_PPS_RT1_MASK                        ((uint8)((uint8)0x03u << RTC_PPS_RT1_SHIFT))
    #define RTC_PPS_SYNC                            ((uint8)((uint8)0x03u << RTC_PPS_RT1_SHIFT))
    #define RTC_PPS_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define RTC_PPS_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << RTC_PPS_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define RTC_PPS_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << RTC_PPS_SYNCDSI_SHIFT))

    #define RTC_PPS_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << RTC_PPS_CTRL_MODE_SHIFT))
    #define RTC_PPS_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << RTC_PPS_CTRL_MODE_SHIFT))
    #define RTC_PPS_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << RTC_PPS_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define RTC_PPS_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define RTC_PPS_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define RTC_PPS_STATUS_TC_INT_MASK_SHIFT        (RTC_PPS_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define RTC_PPS_STATUS_CAPTURE_INT_MASK_SHIFT   (RTC_PPS_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define RTC_PPS_STATUS_TC                       ((uint8)((uint8)0x01u << RTC_PPS_STATUS_TC_SHIFT))
    #define RTC_PPS_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << RTC_PPS_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define RTC_PPS_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << RTC_PPS_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define RTC_PPS_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << RTC_PPS_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define RTC_PPS_STATUS              (* (reg8 *) RTC_PPS_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define RTC_PPS_STATUS_MASK         (* (reg8 *) RTC_PPS_TimerUDB_rstSts_stsreg__MASK_REG)
    #define RTC_PPS_STATUS_AUX_CTRL     (* (reg8 *) RTC_PPS_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define RTC_PPS_CONTROL             (* (reg8 *) RTC_PPS_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(RTC_PPS_Resolution <= 8u) /* 8-bit Timer */
        #define RTC_PPS_CAPTURE_LSB         (* (reg8 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define RTC_PPS_CAPTURE_LSB_PTR       ((reg8 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define RTC_PPS_PERIOD_LSB          (* (reg8 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define RTC_PPS_PERIOD_LSB_PTR        ((reg8 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define RTC_PPS_COUNTER_LSB         (* (reg8 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define RTC_PPS_COUNTER_LSB_PTR       ((reg8 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
    #elif(RTC_PPS_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define RTC_PPS_CAPTURE_LSB         (* (reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define RTC_PPS_CAPTURE_LSB_PTR       ((reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define RTC_PPS_PERIOD_LSB          (* (reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define RTC_PPS_PERIOD_LSB_PTR        ((reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define RTC_PPS_COUNTER_LSB         (* (reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define RTC_PPS_COUNTER_LSB_PTR       ((reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define RTC_PPS_CAPTURE_LSB         (* (reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define RTC_PPS_CAPTURE_LSB_PTR       ((reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define RTC_PPS_PERIOD_LSB          (* (reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define RTC_PPS_PERIOD_LSB_PTR        ((reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define RTC_PPS_COUNTER_LSB         (* (reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
            #define RTC_PPS_COUNTER_LSB_PTR       ((reg16 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(RTC_PPS_Resolution <= 24u)/* 24-bit Timer */
        #define RTC_PPS_CAPTURE_LSB         (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define RTC_PPS_CAPTURE_LSB_PTR       ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define RTC_PPS_PERIOD_LSB          (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define RTC_PPS_PERIOD_LSB_PTR        ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define RTC_PPS_COUNTER_LSB         (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define RTC_PPS_COUNTER_LSB_PTR       ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define RTC_PPS_CAPTURE_LSB         (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define RTC_PPS_CAPTURE_LSB_PTR       ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define RTC_PPS_PERIOD_LSB          (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define RTC_PPS_PERIOD_LSB_PTR        ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define RTC_PPS_COUNTER_LSB         (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define RTC_PPS_COUNTER_LSB_PTR       ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define RTC_PPS_CAPTURE_LSB         (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define RTC_PPS_CAPTURE_LSB_PTR       ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define RTC_PPS_PERIOD_LSB          (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define RTC_PPS_PERIOD_LSB_PTR        ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define RTC_PPS_COUNTER_LSB         (* (reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
            #define RTC_PPS_COUNTER_LSB_PTR       ((reg32 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define RTC_PPS_COUNTER_LSB_PTR_8BIT       ((reg8 *) RTC_PPS_TimerUDB_sT32_timerdp_u0__A0_REG )
    
    #if (RTC_PPS_UsingHWCaptureCounter)
        #define RTC_PPS_CAP_COUNT              (*(reg8 *) RTC_PPS_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define RTC_PPS_CAP_COUNT_PTR          ( (reg8 *) RTC_PPS_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define RTC_PPS_CAPTURE_COUNT_CTRL     (*(reg8 *) RTC_PPS_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define RTC_PPS_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) RTC_PPS_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (RTC_PPS_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define RTC_PPS_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define RTC_PPS_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define RTC_PPS_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define RTC_PPS_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define RTC_PPS_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define RTC_PPS_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << RTC_PPS_CTRL_INTCNT_SHIFT))
    #define RTC_PPS_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << RTC_PPS_CTRL_TRIG_MODE_SHIFT))
    #define RTC_PPS_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << RTC_PPS_CTRL_TRIG_EN_SHIFT))
    #define RTC_PPS_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << RTC_PPS_CTRL_CAP_MODE_SHIFT))
    #define RTC_PPS_CTRL_ENABLE                    ((uint8)((uint8)0x01u << RTC_PPS_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define RTC_PPS_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define RTC_PPS_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define RTC_PPS_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define RTC_PPS_STATUS_TC_INT_MASK_SHIFT       RTC_PPS_STATUS_TC_SHIFT
    #define RTC_PPS_STATUS_CAPTURE_INT_MASK_SHIFT  RTC_PPS_STATUS_CAPTURE_SHIFT
    #define RTC_PPS_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define RTC_PPS_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define RTC_PPS_STATUS_FIFOFULL_INT_MASK_SHIFT RTC_PPS_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define RTC_PPS_STATUS_TC                      ((uint8)((uint8)0x01u << RTC_PPS_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define RTC_PPS_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << RTC_PPS_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define RTC_PPS_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << RTC_PPS_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define RTC_PPS_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << RTC_PPS_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define RTC_PPS_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << RTC_PPS_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define RTC_PPS_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << RTC_PPS_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define RTC_PPS_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << RTC_PPS_STATUS_FIFOFULL_SHIFT))

    #define RTC_PPS_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define RTC_PPS_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define RTC_PPS_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define RTC_PPS_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define RTC_PPS_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define RTC_PPS_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_RTC_PPS_H */


/* [] END OF FILE */
