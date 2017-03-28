/*******************************************************************************
* File Name: .h
* Version 3.0
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_I2CM_H)
#define CY_SCB_PVT_I2CM_H

#include "I2CM.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define I2CM_SetI2CExtClkInterruptMode(interruptMask) I2CM_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define I2CM_ClearI2CExtClkInterruptSource(interruptMask) I2CM_CLEAR_INTR_I2C_EC(interruptMask)
#define I2CM_GetI2CExtClkInterruptSource()                (I2CM_INTR_I2C_EC_REG)
#define I2CM_GetI2CExtClkInterruptMode()                  (I2CM_INTR_I2C_EC_MASK_REG)
#define I2CM_GetI2CExtClkInterruptSourceMasked()          (I2CM_INTR_I2C_EC_MASKED_REG)

#if (!I2CM_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define I2CM_SetSpiExtClkInterruptMode(interruptMask) \
                                                                I2CM_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define I2CM_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                I2CM_CLEAR_INTR_SPI_EC(interruptMask)
    #define I2CM_GetExtSpiClkInterruptSource()                 (I2CM_INTR_SPI_EC_REG)
    #define I2CM_GetExtSpiClkInterruptMode()                   (I2CM_INTR_SPI_EC_MASK_REG)
    #define I2CM_GetExtSpiClkInterruptSourceMasked()           (I2CM_INTR_SPI_EC_MASKED_REG)
#endif /* (!I2CM_CY_SCBIP_V1) */

#if(I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void I2CM_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (I2CM_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_I2CM_CUSTOM_INTR_HANDLER)
    extern cyisraddress I2CM_customIntrHandler;
#endif /* !defined (CY_REMOVE_I2CM_CUSTOM_INTR_HANDLER) */
#endif /* (I2CM_SCB_IRQ_INTERNAL) */

extern I2CM_BACKUP_STRUCT I2CM_backup;

#if(I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 I2CM_scbMode;
    extern uint8 I2CM_scbEnableWake;
    extern uint8 I2CM_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 I2CM_mode;
    extern uint8 I2CM_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * I2CM_rxBuffer;
    extern uint8   I2CM_rxDataBits;
    extern uint32  I2CM_rxBufferSize;

    extern volatile uint8 * I2CM_txBuffer;
    extern uint8   I2CM_txDataBits;
    extern uint32  I2CM_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 I2CM_numberOfAddr;
    extern uint8 I2CM_subAddrSize;
#endif /* (I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*        Conditional Macro
****************************************/

#if(I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define I2CM_SCB_MODE_I2C_RUNTM_CFG     (I2CM_SCB_MODE_I2C      == I2CM_scbMode)
    #define I2CM_SCB_MODE_SPI_RUNTM_CFG     (I2CM_SCB_MODE_SPI      == I2CM_scbMode)
    #define I2CM_SCB_MODE_UART_RUNTM_CFG    (I2CM_SCB_MODE_UART     == I2CM_scbMode)
    #define I2CM_SCB_MODE_EZI2C_RUNTM_CFG   (I2CM_SCB_MODE_EZI2C    == I2CM_scbMode)
    #define I2CM_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (I2CM_SCB_MODE_UNCONFIG == I2CM_scbMode)

    /* Defines wakeup enable */
    #define I2CM_SCB_WAKE_ENABLE_CHECK       (0u != I2CM_scbEnableWake)
#endif /* (I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!I2CM_CY_SCBIP_V1)
    #define I2CM_SCB_PINS_NUMBER    (7u)
#else
    #define I2CM_SCB_PINS_NUMBER    (2u)
#endif /* (!I2CM_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_I2CM_H) */


/* [] END OF FILE */
