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
* File Name    : sf_spi_cfg_ref.h
* Description  : Serial Peripheral Interface Framework (SF_SPI) Module configuration header file.
***********************************************************************************************************************/

#ifndef SF_SPI_CFG_H
#define SF_SPI_CFG_H

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SPI_CH_MAX (4)

/*******************************************************************************************************************//**
 * @ingroup SF_SPI
 * @defgroup SF_SPI_CFG Build Time Configurations
 * @{
 **********************************************************************************************************************/

/** Specify whether to include code for API parameter checking
*  Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
*  Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define SF_SPI_CFG_PARAM_CHECKING_ENABLE                (BSP_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * @} (end defgroup SF_SPI_CFG)
 **********************************************************************************************************************/

#define SF_HAL_SCI0               (0)
#define SF_HAL_SCI1               (1)
#define SF_HAL_SCI2               (2)
#define SF_HAL_SCI3               (3)
#define SF_HAL_SCI4               (4)
#define SF_HAL_SCI5               (5)
#define SF_HAL_SCI6               (6)
#define SF_HAL_SCI7               (7)
#define SF_HAL_SCI8               (8)
#define SF_HAL_SCI9               (9)

#define SF_UART_CH0               (SF_HAL_SCI9)
#define SF_UART_CH1               (SF_HAL_SCI1)

#endif /* SF_SPI_CFG_H */

