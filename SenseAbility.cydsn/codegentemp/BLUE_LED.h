/*******************************************************************************
* File Name: BLUE_LED.h  
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

#if !defined(CY_PINS_BLUE_LED_H) /* Pins BLUE_LED_H */
#define CY_PINS_BLUE_LED_H

#include "cytypes.h"
#include "cyfitter.h"
#include "BLUE_LED_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    BLUE_LED_Write(uint8 value) ;
void    BLUE_LED_SetDriveMode(uint8 mode) ;
uint8   BLUE_LED_ReadDataReg(void) ;
uint8   BLUE_LED_Read(void) ;
uint8   BLUE_LED_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define BLUE_LED_DRIVE_MODE_BITS        (3)
#define BLUE_LED_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - BLUE_LED_DRIVE_MODE_BITS))

#define BLUE_LED_DM_ALG_HIZ         (0x00u)
#define BLUE_LED_DM_DIG_HIZ         (0x01u)
#define BLUE_LED_DM_RES_UP          (0x02u)
#define BLUE_LED_DM_RES_DWN         (0x03u)
#define BLUE_LED_DM_OD_LO           (0x04u)
#define BLUE_LED_DM_OD_HI           (0x05u)
#define BLUE_LED_DM_STRONG          (0x06u)
#define BLUE_LED_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define BLUE_LED_MASK               BLUE_LED__MASK
#define BLUE_LED_SHIFT              BLUE_LED__SHIFT
#define BLUE_LED_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BLUE_LED_PS                     (* (reg32 *) BLUE_LED__PS)
/* Port Configuration */
#define BLUE_LED_PC                     (* (reg32 *) BLUE_LED__PC)
/* Data Register */
#define BLUE_LED_DR                     (* (reg32 *) BLUE_LED__DR)
/* Input Buffer Disable Override */
#define BLUE_LED_INP_DIS                (* (reg32 *) BLUE_LED__PC2)


#if defined(BLUE_LED__INTSTAT)  /* Interrupt Registers */

    #define BLUE_LED_INTSTAT                (* (reg32 *) BLUE_LED__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define BLUE_LED_DRIVE_MODE_SHIFT       (0x00u)
#define BLUE_LED_DRIVE_MODE_MASK        (0x07u << BLUE_LED_DRIVE_MODE_SHIFT)


#endif /* End Pins BLUE_LED_H */


/* [] END OF FILE */
