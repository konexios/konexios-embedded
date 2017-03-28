/*******************************************************************************
* File Name: TH_AL.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "TH_AL.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        TH_AL_PC =   (TH_AL_PC & \
                                (uint32)(~(uint32)(TH_AL_DRIVE_MODE_IND_MASK << (TH_AL_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (TH_AL_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: TH_AL_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void TH_AL_Write(uint8 value) 
{
    uint8 drVal = (uint8)(TH_AL_DR & (uint8)(~TH_AL_MASK));
    drVal = (drVal | ((uint8)(value << TH_AL_SHIFT) & TH_AL_MASK));
    TH_AL_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: TH_AL_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  TH_AL_DM_STRONG     Strong Drive 
*  TH_AL_DM_OD_HI      Open Drain, Drives High 
*  TH_AL_DM_OD_LO      Open Drain, Drives Low 
*  TH_AL_DM_RES_UP     Resistive Pull Up 
*  TH_AL_DM_RES_DWN    Resistive Pull Down 
*  TH_AL_DM_RES_UPDWN  Resistive Pull Up/Down 
*  TH_AL_DM_DIG_HIZ    High Impedance Digital 
*  TH_AL_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void TH_AL_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(TH_AL__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: TH_AL_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro TH_AL_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 TH_AL_Read(void) 
{
    return (uint8)((TH_AL_PS & TH_AL_MASK) >> TH_AL_SHIFT);
}


/*******************************************************************************
* Function Name: TH_AL_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 TH_AL_ReadDataReg(void) 
{
    return (uint8)((TH_AL_DR & TH_AL_MASK) >> TH_AL_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(TH_AL_INTSTAT) 

    /*******************************************************************************
    * Function Name: TH_AL_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 TH_AL_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(TH_AL_INTSTAT & TH_AL_MASK);
		TH_AL_INTSTAT = maskedStatus;
        return maskedStatus >> TH_AL_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
