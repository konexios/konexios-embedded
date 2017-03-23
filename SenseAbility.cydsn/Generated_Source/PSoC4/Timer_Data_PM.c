/*******************************************************************************
* File Name: Timer_Data_PM.c
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

#include "Timer_Data.h"

static Timer_Data_backupStruct Timer_Data_backup;


/*******************************************************************************
* Function Name: Timer_Data_SaveConfig
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
*  Timer_Data_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_Data_SaveConfig(void) 
{
    #if (!Timer_Data_UsingFixedFunction)
        Timer_Data_backup.TimerUdb = Timer_Data_ReadCounter();
        Timer_Data_backup.InterruptMaskValue = Timer_Data_STATUS_MASK;
        #if (Timer_Data_UsingHWCaptureCounter)
            Timer_Data_backup.TimerCaptureCounter = Timer_Data_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_Data_UDB_CONTROL_REG_REMOVED)
            Timer_Data_backup.TimerControlRegister = Timer_Data_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_Data_RestoreConfig
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
*  Timer_Data_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_Data_RestoreConfig(void) 
{   
    #if (!Timer_Data_UsingFixedFunction)

        Timer_Data_WriteCounter(Timer_Data_backup.TimerUdb);
        Timer_Data_STATUS_MASK =Timer_Data_backup.InterruptMaskValue;
        #if (Timer_Data_UsingHWCaptureCounter)
            Timer_Data_SetCaptureCount(Timer_Data_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_Data_UDB_CONTROL_REG_REMOVED)
            Timer_Data_WriteControlRegister(Timer_Data_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_Data_Sleep
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
*  Timer_Data_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_Data_Sleep(void) 
{
    #if(!Timer_Data_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_Data_CTRL_ENABLE == (Timer_Data_CONTROL & Timer_Data_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_Data_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_Data_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_Data_Stop();
    Timer_Data_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_Data_Wakeup
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
*  Timer_Data_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_Data_Wakeup(void) 
{
    Timer_Data_RestoreConfig();
    #if(!Timer_Data_UDB_CONTROL_REG_REMOVED)
        if(Timer_Data_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_Data_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
