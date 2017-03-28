/*******************************************************************************
* File Name: TH_AL.h  
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

#if !defined(CY_PINS_TH_AL_H) /* Pins TH_AL_H */
#define CY_PINS_TH_AL_H

#include "cytypes.h"
#include "cyfitter.h"
#include "TH_AL_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    TH_AL_Write(uint8 value) ;
void    TH_AL_SetDriveMode(uint8 mode) ;
uint8   TH_AL_ReadDataReg(void) ;
uint8   TH_AL_Read(void) ;
uint8   TH_AL_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define TH_AL_DRIVE_MODE_BITS        (3)
#define TH_AL_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - TH_AL_DRIVE_MODE_BITS))

#define TH_AL_DM_ALG_HIZ         (0x00u)
#define TH_AL_DM_DIG_HIZ         (0x01u)
#define TH_AL_DM_RES_UP          (0x02u)
#define TH_AL_DM_RES_DWN         (0x03u)
#define TH_AL_DM_OD_LO           (0x04u)
#define TH_AL_DM_OD_HI           (0x05u)
#define TH_AL_DM_STRONG          (0x06u)
#define TH_AL_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define TH_AL_MASK               TH_AL__MASK
#define TH_AL_SHIFT              TH_AL__SHIFT
#define TH_AL_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TH_AL_PS                     (* (reg32 *) TH_AL__PS)
/* Port Configuration */
#define TH_AL_PC                     (* (reg32 *) TH_AL__PC)
/* Data Register */
#define TH_AL_DR                     (* (reg32 *) TH_AL__DR)
/* Input Buffer Disable Override */
#define TH_AL_INP_DIS                (* (reg32 *) TH_AL__PC2)


#if defined(TH_AL__INTSTAT)  /* Interrupt Registers */

    #define TH_AL_INTSTAT                (* (reg32 *) TH_AL__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define TH_AL_DRIVE_MODE_SHIFT       (0x00u)
#define TH_AL_DRIVE_MODE_MASK        (0x07u << TH_AL_DRIVE_MODE_SHIFT)


#endif /* End Pins TH_AL_H */


/* [] END OF FILE */
