/*******************************************************************************
* File Name: TH_AH.c  
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
#include "TH_AH.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        TH_AH_PC =   (TH_AH_PC & \
                                (uint32)(~(uint32)(TH_AH_DRIVE_MODE_IND_MASK << (TH_AH_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (TH_AH_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: TH_AH_Write
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
void TH_AH_Write(uint8 value) 
{
    uint8 drVal = (uint8)(TH_AH_DR & (uint8)(~TH_AH_MASK));
    drVal = (drVal | ((uint8)(value << TH_AH_SHIFT) & TH_AH_MASK));
    TH_AH_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: TH_AH_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  TH_AH_DM_STRONG     Strong Drive 
*  TH_AH_DM_OD_HI      Open Drain, Drives High 
*  TH_AH_DM_OD_LO      Open Drain, Drives Low 
*  TH_AH_DM_RES_UP     Resistive Pull Up 
*  TH_AH_DM_RES_DWN    Resistive Pull Down 
*  TH_AH_DM_RES_UPDWN  Resistive Pull Up/Down 
*  TH_AH_DM_DIG_HIZ    High Impedance Digital 
*  TH_AH_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void TH_AH_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(TH_AH__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: TH_AH_Read
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
*  Macro TH_AH_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 TH_AH_Read(void) 
{
    return (uint8)((TH_AH_PS & TH_AH_MASK) >> TH_AH_SHIFT);
}


/*******************************************************************************
* Function Name: TH_AH_ReadDataReg
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
uint8 TH_AH_ReadDataReg(void) 
{
    return (uint8)((TH_AH_DR & TH_AH_MASK) >> TH_AH_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(TH_AH_INTSTAT) 

    /*******************************************************************************
    * Function Name: TH_AH_ClearInterrupt
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
    uint8 TH_AH_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(TH_AH_INTSTAT & TH_AH_MASK);
		TH_AH_INTSTAT = maskedStatus;
        return maskedStatus >> TH_AH_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
