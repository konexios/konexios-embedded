/*******************************************************************************
* File Name: RTC_PPS_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "RTC_PPS.h"

static RTC_PPS_backupStruct RTC_PPS_backup;


/*******************************************************************************
* Function Name: RTC_PPS_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RTC_PPS_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void RTC_PPS_SaveConfig(void) 
{
    #if (!RTC_PPS_UsingFixedFunction)
        RTC_PPS_backup.TimerUdb = RTC_PPS_ReadCounter();
        RTC_PPS_backup.InterruptMaskValue = RTC_PPS_STATUS_MASK;
        #if (RTC_PPS_UsingHWCaptureCounter)
            RTC_PPS_backup.TimerCaptureCounter = RTC_PPS_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!RTC_PPS_UDB_CONTROL_REG_REMOVED)
            RTC_PPS_backup.TimerControlRegister = RTC_PPS_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: RTC_PPS_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RTC_PPS_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RTC_PPS_RestoreConfig(void) 
{   
    #if (!RTC_PPS_UsingFixedFunction)

        RTC_PPS_WriteCounter(RTC_PPS_backup.TimerUdb);
        RTC_PPS_STATUS_MASK =RTC_PPS_backup.InterruptMaskValue;
        #if (RTC_PPS_UsingHWCaptureCounter)
            RTC_PPS_SetCaptureCount(RTC_PPS_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!RTC_PPS_UDB_CONTROL_REG_REMOVED)
            RTC_PPS_WriteControlRegister(RTC_PPS_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: RTC_PPS_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RTC_PPS_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void RTC_PPS_Sleep(void) 
{
    #if(!RTC_PPS_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(RTC_PPS_CTRL_ENABLE == (RTC_PPS_CONTROL & RTC_PPS_CTRL_ENABLE))
        {
            /* Timer is enabled */
            RTC_PPS_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            RTC_PPS_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    RTC_PPS_Stop();
    RTC_PPS_SaveConfig();
}


/*******************************************************************************
* Function Name: RTC_PPS_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RTC_PPS_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RTC_PPS_Wakeup(void) 
{
    RTC_PPS_RestoreConfig();
    #if(!RTC_PPS_UDB_CONTROL_REG_REMOVED)
        if(RTC_PPS_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                RTC_PPS_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
