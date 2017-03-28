/*******************************************************************************
* File Name: MAG_SW_IN.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_MAG_SW_IN_H) /* Pins MAG_SW_IN_H */
#define CY_PINS_MAG_SW_IN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "MAG_SW_IN_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    MAG_SW_IN_Write(uint8 value) ;
void    MAG_SW_IN_SetDriveMode(uint8 mode) ;
uint8   MAG_SW_IN_ReadDataReg(void) ;
uint8   MAG_SW_IN_Read(void) ;
uint8   MAG_SW_IN_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define MAG_SW_IN_DRIVE_MODE_BITS        (3)
#define MAG_SW_IN_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - MAG_SW_IN_DRIVE_MODE_BITS))

#define MAG_SW_IN_DM_ALG_HIZ         (0x00u)
#define MAG_SW_IN_DM_DIG_HIZ         (0x01u)
#define MAG_SW_IN_DM_RES_UP          (0x02u)
#define MAG_SW_IN_DM_RES_DWN         (0x03u)
#define MAG_SW_IN_DM_OD_LO           (0x04u)
#define MAG_SW_IN_DM_OD_HI           (0x05u)
#define MAG_SW_IN_DM_STRONG          (0x06u)
#define MAG_SW_IN_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define MAG_SW_IN_MASK               MAG_SW_IN__MASK
#define MAG_SW_IN_SHIFT              MAG_SW_IN__SHIFT
#define MAG_SW_IN_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MAG_SW_IN_PS                     (* (reg32 *) MAG_SW_IN__PS)
/* Port Configuration */
#define MAG_SW_IN_PC                     (* (reg32 *) MAG_SW_IN__PC)
/* Data Register */
#define MAG_SW_IN_DR                     (* (reg32 *) MAG_SW_IN__DR)
/* Input Buffer Disable Override */
#define MAG_SW_IN_INP_DIS                (* (reg32 *) MAG_SW_IN__PC2)


#if defined(MAG_SW_IN__INTSTAT)  /* Interrupt Registers */

    #define MAG_SW_IN_INTSTAT                (* (reg32 *) MAG_SW_IN__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define MAG_SW_IN_DRIVE_MODE_SHIFT       (0x00u)
#define MAG_SW_IN_DRIVE_MODE_MASK        (0x07u << MAG_SW_IN_DRIVE_MODE_SHIFT)


#endif /* End Pins MAG_SW_IN_H */


/* [] END OF FILE */
