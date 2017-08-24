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
 * File Name    : r_qspi.c
 * Description  : QSPI HAL API
 **********************************************************************************************************************/

#include "bsp_api.h"

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_qspi.h"
#include "r_qspi_private.h"
#include "r_qspi_private_api.h"

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
#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t s_qspi_version =
{
    .api_version_minor  = QSPI_API_VERSION_MINOR,
    .api_version_major  = QSPI_API_VERSION_MAJOR,
    .code_version_major = QSPI_CODE_VERSION_MAJOR,
    .code_version_minor = QSPI_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/*******************************************************************************************************************//**
 * @addtogroup QSPI
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
const qspi_api_t g_qspi_on_qspi =
{
    .open           = R_QSPI_Open,
    .close          = R_QSPI_Close,
    .read           = R_QSPI_Read,
    .pageProgram    = R_QSPI_PageProgram,
    .sectorErase    = R_QSPI_SectorErase,
    .statusGet      = R_QSPI_StatusGet,
    .bankSelect     = R_QSPI_BankSelect,
    .versionGet     = R_QSPI_VersionGet
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Open the QSPI driver module.
 *
 * Open the QSPI module driver in direct communication mode for the purposes of reading and writing flash memory via SPI
 * protocols.
 *
 * @retval SSP_SUCCESS             Configuration was successful.
 * @retval SSP_ERR_ASSERTION       p_cfg was NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_QSPI_Open (qspi_ctrl_t * p_ctrl, qspi_cfg_t const * const p_cfg)
{
#if QSPI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_cfg);
#endif

    /* Set the default bank to the first bank */
    HW_QSPI_BANK_SELECT(0);


    /* Get the configuration of the quad SPI flash device and remember it for subsequent operations */
    bsp_qspi_config_get(&(p_ctrl->manufacturer_id),
                        &(p_ctrl->memory_type),
                        &(p_ctrl->memory_capacity),
                        &(p_ctrl->max_eraseable_size),
                        &(p_ctrl->num_address_bytes),
                        &(p_ctrl->spi_mode),
                        &(p_ctrl->page_size),
						&(p_ctrl->xip_mode));

    /* all zeros in these fields mean the flash device is broken, miss configured, or not populated */
    if ((0 == p_ctrl->manufacturer_id) && (0 == p_ctrl->memory_type) && (0 == p_ctrl->memory_capacity))
    {
    	return SSP_ERR_UNSUPPORTED;
    }

    /* exit XIP mode while in Direct Communication mode */
    if (p_ctrl->xip_mode)
    {
    	bsp_qspi_xip_exit();
    }

    /* Place the QSPI block into Direct Communication mode */
    HW_QSPI_DIRECT_COMMUNICATION_ENTER();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Close the QSPI driver module.
 *
 * Return the QSPI module back to ROM access mode.
 *
 * @retval SSP_SUCCESS             Configuration was successful.
 * @retval SSP_ERR_ASSERTION       p_cfg was NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_QSPI_Close (qspi_ctrl_t * p_ctrl)
{
#if QSPI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    /* all zeros in these fields mean the flash device is broken, miss configured, or not populated */
    if ((0 == p_ctrl->manufacturer_id) && (0 == p_ctrl->memory_type) && (0 == p_ctrl->memory_capacity))
    {
    	return SSP_ERR_UNSUPPORTED;
    }

    /* Re-enter XIP mode if it was running in this mode before entering Direct Communication mode */
    if (p_ctrl->xip_mode)
    {
    	bsp_qspi_xip_enter();
    }

    /* Place the QSPI block into ROM access mode */
    HW_QSPI_DIRECT_COMMUNICATION_EXIT();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Read data from the flash.
 *
 * Read a block of data from a particular address on the SPI flash device.
 *
 * @retval SSP_SUCCESS             The flash was programmed successfully.
 * @retval SSP_ERR_INVALID_MODE    The QSPI controller is not in direct communication mode.
 *
 **********************************************************************************************************************/
ssp_err_t R_QSPI_Read (qspi_ctrl_t * p_ctrl,
                       uint8_t     * p_device_address,
                       uint8_t     * p_memory_address,
                       uint32_t    byte_count)
{
    uint32_t chip_address = (uint32_t) p_device_address - BSP_PRV_QSPI_DEVICE_PHYSICAL_ADDRESS;

#if QSPI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_device_address);
    SSP_ASSERT(p_memory_address);
#endif

    if (4 == p_ctrl->num_address_bytes)
    {
        /* Send command to read data */
        HW_QSPI_BYTE_WRITE(QSPI_COMMAND_4BYTE_READ);

        /* Send the MSByte of the address */
        HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 24));
    }
    else
    {
        /* Send command to read data */
        HW_QSPI_BYTE_WRITE(QSPI_COMMAND_READ);
    }

    /* Send the remaining bytes of the address */
    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 16));
    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 8));
    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address));

    /* Read the device memory into the passed in buffer */
    while (byte_count--)
    {
        *p_memory_address++ = HW_QSPI_BYTE_READ();
    }

    /* Close the SPI bus cycle */
    HW_QSPI_DIRECT_COMMUNICATION_ENTER();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Program a page of data to the flash.
 *
 * @retval SSP_SUCCESS             The flash was programmed successfully.
 * @retval SSP_ERR_INVALID_MODE    The QSPI controller is not in direct communication mode.
 *
 **********************************************************************************************************************/
ssp_err_t R_QSPI_PageProgram (qspi_ctrl_t * p_ctrl,
                              uint8_t     * p_device_address,
                              uint8_t     * p_memory_address,
                              uint32_t    byte_count)
{
    uint32_t chip_address      = (uint32_t) p_device_address - BSP_PRV_QSPI_DEVICE_PHYSICAL_ADDRESS;

#if QSPI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_device_address);
    SSP_ASSERT(p_memory_address);
#endif

    /* Send command to enable writing */
    HW_QSPI_BYTE_WRITE(QSPI_COMMAND_WRITE_ENABLE);

    /* Close the SPI bus cycle */
    HW_QSPI_DIRECT_COMMUNICATION_ENTER();

    if (4 == p_ctrl->num_address_bytes)
    {
        /* Send command to write data */
        HW_QSPI_BYTE_WRITE(QSPI_COMMAND_4BYTE_PAGE_PROGRAM);

        /* Send the MSByte of the address */
        HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 24));
    }
    else
    {
        /* Send command to write data */
        HW_QSPI_BYTE_WRITE(QSPI_COMMAND_PAGE_PROGRAM);
    }

    /* Send the remaining bytes of the address */
    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 16));
    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 8));
    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address));

    while (byte_count--)
    {
        /* Read the device memory into the passed in buffer */
        HW_QSPI_BYTE_WRITE(*p_memory_address++);
    }

    /* Close the SPI bus cycle */
    HW_QSPI_DIRECT_COMMUNICATION_ENTER();

    /* Send command to disable writing */
    HW_QSPI_BYTE_WRITE(QSPI_COMMAND_WRITE_DISABLE);

    /* Close the SPI bus cycle */
    HW_QSPI_DIRECT_COMMUNICATION_ENTER();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Erase a sector on the flash.
 *
 * Erase one sector on the SPI flash device. Any passed in address within the sector to be erased is acceptable.
 *
 * @retval SSP_SUCCESS             The command to erase the sector of flash was executed successfully.
 * @retval SSP_ERR_INVALID_MODE    The QSPI controller is not in direct communication mode.
 *
 **********************************************************************************************************************/
ssp_err_t R_QSPI_SectorErase (qspi_ctrl_t * p_ctrl, uint8_t * p_device_address)
{
    uint32_t chip_address = (uint32_t) p_device_address - BSP_PRV_QSPI_DEVICE_PHYSICAL_ADDRESS;

#if QSPI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_device_address);
#endif

    /* Send command to enable writing */
    HW_QSPI_BYTE_WRITE(QSPI_COMMAND_WRITE_ENABLE);

    /* Close the SPI bus cycle */
    HW_QSPI_DIRECT_COMMUNICATION_ENTER();

    if (4 == p_ctrl->num_address_bytes)
    {
        /* Send command to erase */
        HW_QSPI_BYTE_WRITE(QSPI_COMMAND_4BYTE_SECTOR_ERASE);

        /* Send command to write data */
        HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 24));
    }
    else
    {
        /* Send command to erase */
        HW_QSPI_BYTE_WRITE(QSPI_COMMAND_SECTOR_ERASE);
    }

    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 16));
    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address >> 8));
    HW_QSPI_BYTE_WRITE((uint8_t)(chip_address));

    /* Close the SPI bus cycle */
    HW_QSPI_DIRECT_COMMUNICATION_ENTER();

    /* Send command to disable writing */
    HW_QSPI_BYTE_WRITE(QSPI_COMMAND_WRITE_DISABLE);

    /* Close the SPI bus cycle */
    HW_QSPI_DIRECT_COMMUNICATION_ENTER();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Get the write or erase status of the flash.
 *
 * Return the write status of the flash. This is most useful for determining if erases are complete.
 *
 * @retval SSP_SUCCESS             The write status is correct.
 * @retval SSP_ERR_INVALID_MODE    The QSPI controller is not in direct communication mode.
 *
 **********************************************************************************************************************/
ssp_err_t R_QSPI_StatusGet (qspi_ctrl_t * p_ctrl, bool * p_write_in_progress)
{
#if QSPI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_write_in_progress);
#endif

    /* Get the write status from the device */
    bsp_qspi_status_get(p_write_in_progress);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Select the bank to access.
 *
 * A bank is a 64MB sliding access window into the flash memory space. This function sets the current bank.
 *
 * @retval SSP_SUCCESS             Bank successfully selected.
 *
 **********************************************************************************************************************/
ssp_err_t R_QSPI_BankSelect (uint32_t bank)
{
    /* Return back to ROM access mode */
    HW_QSPI_BANK_SELECT(bank);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Get the driver version based on compile time macros.
 *
 * @retval     SSP_SUCCESS          Successful close.
 * @retval     SSP_ERR_INVALID_PTR  p_version is NULL.
 *
 **********************************************************************************************************************/
ssp_err_t R_QSPI_VersionGet (ssp_version_t * const p_version)
{
#if QSPI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_version);
#endif

    p_version->version_id = s_qspi_version.version_id;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup QSPI)
 **********************************************************************************************************************/
