/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : hw_qspi_common.h
 * Description  : Quad Serial Peripheral Interface (QSPI) Module hardware common header file.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup QSPI
 * @{
 **********************************************************************************************************************/

#ifndef HW_QSPI_COMMON_H
#define HW_QSPI_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Remove mock reference */
#if !BSP_MCU_MOCKED

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Powers on the ELC block.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_QSPI_PowerOn (void)
{
}

/*******************************************************************************************************************//**
 * Powers off the ELC block.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_QSPI_PowerOff (void)
{
}

/*******************************************************************************************************************//**
 * Set the BANK Switching Address
 **********************************************************************************************************************/
__STATIC_INLINE void HW_QSPI_BANK_SELECT (uint32_t bank)
{
    R_QSPI->SFMCNT1_b.QSPI_EXT = (uint8_t)(0x3F & bank);
}

/*******************************************************************************************************************//**
 * Place the QSPI block in direct communication mode for the purposes of writing or erasing the flash device
 **********************************************************************************************************************/
__STATIC_INLINE void HW_QSPI_DIRECT_COMMUNICATION_ENTER (void)
{
    R_QSPI->SFMCMD_b.DCOM = 1;
}

/*******************************************************************************************************************//**
 * Exit direct communication mode and place the block in ROM access mode
 **********************************************************************************************************************/
__STATIC_INLINE void HW_QSPI_DIRECT_COMMUNICATION_EXIT (void)
{
    R_QSPI->SFMCMD_b.DCOM = 0;
}

/*******************************************************************************************************************//**
 * Read a byte of data over the direct communications channel
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_QSPI_BYTE_READ (void)
{
    return R_QSPI->SFMCOM_b.SFMD;
}

/*******************************************************************************************************************//**
 * Write a byte of data over the direct communications channel
 **********************************************************************************************************************/
__STATIC_INLINE void HW_QSPI_BYTE_WRITE (uint8_t byte)
{
    R_QSPI->SFMCOM = byte;
}
#endif /* BSP_MCU_MOCKED */
#endif /* HW_QSPI_COMMON_H */
