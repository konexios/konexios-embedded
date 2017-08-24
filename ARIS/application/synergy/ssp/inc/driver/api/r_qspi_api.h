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
 * File Name    : r_qspi_api.h
 * Description  : QSPI Interface
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup QSPI_API Quad SPI Flash Interface
 * @brief Interface for accessing external SPI flash devices.
 *
 * @section QSPI_API_SUMMARY Summary
 * The QSPI module provides an interface that writes and erases sectors in quad SPI flash devices connected to the QSPI
 * interface.
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * QSPI Interface description: @ref HALQSPIInterface
 * @{
 **********************************************************************************************************************/

#ifndef DRV_QSPI_API_H
#define DRV_QSPI_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define QSPI_API_VERSION_MAJOR (1)
#define QSPI_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** User configuration structure used by the open function */
typedef struct st_qspi_cfg
{
    void  * p_extend;           ///< place holder for future development
} qspi_cfg_t;

/** Driver handle */
typedef struct st_qspi_ctrl
{
    uint8_t   manufacturer_id;    ///< Manufacturer ID
    uint8_t   memory_type;        ///< Memory type
    uint8_t   memory_capacity;    ///< Memory capacity (in MByte)
    uint32_t  max_eraseable_size; ///< Largest eraseable sector size in kbytes. Used to determine buffer size for
                                  // partial sector erases.
    uint32_t  num_address_bytes;  ///< Number of bytes used to represent the address
    uint32_t  spi_mode;           ///< SPI mode - 0 = Extended, 1 = Dual, 2 = Quad
    uint32_t  page_size;          ///< Number of bytes in a programmable page
    bool      xip_mode;           ///< 0 = run in read mode, 1 = run in XIP mode
} qspi_ctrl_t;

/** QSPI functions implemented at the HAL layer follow this API. */
typedef struct st_qspi_api
{
    /** Open the QSPI driver module.
     * @par Implemented as
     * - R_QSPI_Open()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_cfg                Pointer to a configuration structure
     **/
    ssp_err_t (* open)(qspi_ctrl_t * p_ctrl, qspi_cfg_t const * const p_cfg);

    /** Close the QSPI driver module.
     * @par Implemented as
     * - R_QSPI_Close()
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    ssp_err_t (* close)(qspi_ctrl_t * p_ctrl);

    /** Read a block of data from the flash.
     * @par Implemented as
     * - R_QSPI_Read()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_device_address     The location in the flash device address space to read
     * @param[in] p_memory_address     The memory address of a buffer to place the read data in
     * @param[in] byte_count           The number of bytes to read
     **/
    ssp_err_t (* read)(qspi_ctrl_t * p_ctrl, uint8_t * p_device_address, uint8_t * p_memory_address,
                       uint32_t byte_count);

    /** Program a page of data to the flash.
     * @par Implemented as
     * - R_QSPI_PageProgram()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_device_address     The location in the flash device address space to write the data to
     * @param[in] p_memory_address     The memory address of the data to write to the flash device
     * @param[in] byte_count           The number of bytes to write
     **/
    ssp_err_t (* pageProgram)(qspi_ctrl_t * p_ctrl, uint8_t * p_device_address, uint8_t * p_memory_address,
                              uint32_t byte_count);

    /** Erase a sector of the flash.
     * @par Implemented as
     * - R_QSPI_SectorErase()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_device_address     The location in the flash device address space to start the erase from
     **/
    ssp_err_t (* sectorErase)(qspi_ctrl_t * p_ctrl, uint8_t * p_device_address);

    /** Get the write or erase status of the flash.
     * @par Implemented as
     * - R_QSPI_StatusGet()
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_write_in_progress  The write or erase status  - TRUE = write/erase in progress
     **/
    ssp_err_t (* statusGet)(qspi_ctrl_t * p_ctrl, bool * p_write_in_progress);

    /** Select the bank to access.
     * @par Implemented as
     * - R_QSPI_BankSelect()
     *
     * @param[in] bank                 The bank number
     **/
    ssp_err_t (* bankSelect)(uint32_t bank);

    /** Get the driver version based on compile time macros.
     * @par Implemented as
     * - R_QSPI_VersionGet()
     * @param[out]  p_version  Code and API version used.
     **/
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);
} qspi_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_qspi_instance
{
    qspi_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    qspi_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    qspi_api_t const * p_api;     ///< Pointer to the API structure for this instance
} qspi_instance_t;

#endif /* DRV_QSPI_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup QSPI_API)
 **********************************************************************************************************************/
