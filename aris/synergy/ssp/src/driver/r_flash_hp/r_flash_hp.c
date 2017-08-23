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
 * File Name    : r_flash_hp.c
 * Description  : HLD Interface for the High Performance FLASH on SC32 MCUs.
 **********************************************************************************************************************/




/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#if defined(BSP_MCU_GROUP_S7G2)

#include "r_flash_hp.h"
#include "hw\target\hw_flash_hp_private.h"
#include "r_flash_hp_private_api.h"
/* Configuration for this package. */
#include "r_flash_cfg.h"
#include "r_cgc_api.h"
#include "r_cgc.h"

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Macro for error logger. */
#ifndef FLASH_ERROR_RETURN
#define FLASH_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_flash_hp_version)
#endif

#define MINIMUM_SUPPORTED_FCLK_FREQ 4U            /// Minimum FCLK for Flash Operations

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static flash_block_info_t g_block_info;             /// Structure holding block info about an address.
bsp_lock_t                g_flash_Lock;             /// Flash commands software lock
static bool               g_flash_api_open = false; /// true once API has been successfully opened.

/** Structure that holds the parameters for current operations*/
current_parameters_t  g_current_parameters;

/** State variable for the Flash API. */
static flash_states_t g_flash_state = FLASH_STATE_UNINITIALIZED;

/** Internal functions. */
/*LDRA_INSPECTED 219 s - This is an allowed exception to LDRA standard 219 S "User name starts with underscore."*/
static ssp_err_t      flash_lock_state (flash_states_t new_state) PLACE_IN_RAM_SECTION;

/*LDRA_INSPECTED 219 s - This is an allowed exception to LDRA standard 219 S "User name starts with underscore."*/
static ssp_err_t      setup_for_pe_mode (flash_type_t flash_type, flash_states_t flash_state) PLACE_IN_RAM_SECTION;

static ssp_err_t      flash_setup (void) PLACE_IN_RAM_SECTION;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
const flash_api_t g_flash_on_flash_hp =
{
    .open              = R_FLASH_HP_Open,
    .close             = R_FLASH_HP_Close,
    .write             = R_FLASH_HP_Write,
    .read              = R_FLASH_HP_Read,
    .erase             = R_FLASH_HP_Erase,
    .blankCheck        = R_FLASH_HP_BlankCheck,
    .statusGet         = R_FLASH_HP_StatusGet,
    .accessWindowSet   = R_FLASH_HP_AccessWindowSet,
    .accessWindowClear = R_FLASH_HP_AccessWindowClear,
    .reset             = R_FLASH_HP_Reset,
    .startupAreaSelect = R_FLASH_HP_StartUpAreaSelect,
    .updateFlashClockFreq = R_FLASH_HP_UpdateFlashClockFreq,
    .versionGet        = R_FLASH_HP_VersionGet
};

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t g_flash_hp_version =
{
        .api_version_minor  = FLASH_API_VERSION_MINOR,
        .api_version_major  = FLASH_API_VERSION_MAJOR,
        .code_version_major = FLASH_HP_CODE_VERSION_MAJOR,
        .code_version_minor = FLASH_HP_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char          g_module_name[] = "r_flash_hp";
#endif

/** These two tables define the block size and numbers for this parts Code and Data Flash.
 *  flash_get_block_info() is called by various API functions to get information about a particular data or code flash
 * block.
 */
#define NUM_BLOCK_ROM_TABLE_ENTRIES 3

/** 8 blocks of 8K, followed by 126 blocks of 32K  */
static const flash_block_info_t g_flash_RomBlockSizes[NUM_BLOCK_ROM_TABLE_ENTRIES] =
{
        {.is_code_flash_addr = true, .num_blocks = (uint32_t)8, .block_section_end_addr = (uint32_t)(FLASH_CF_BLOCK_8 - 1), .this_block_end_addr = (uint32_t)0,
         .block_section_st_addr  = (uint32_t)FLASH_CF_BLOCK_0,  .block_size = (uint32_t)8192, .this_block_st_addr = (uint32_t)0, .this_block_number = (uint32_t)0},

        {.is_code_flash_addr = true, .num_blocks = (uint32_t)126, .block_section_end_addr = (uint32_t)FLASH_CF_BLOCK_END, .this_block_end_addr = (uint32_t)0,
          .block_section_st_addr  = (uint32_t)FLASH_CF_BLOCK_8,  .block_size = (uint32_t)32768, .this_block_st_addr = (uint32_t)0, .this_block_number = (uint32_t)0},

        {.is_code_flash_addr = false, .num_blocks = (uint32_t)0, .block_section_end_addr = (uint32_t)0, .this_block_end_addr = (uint32_t)0,
          .block_section_st_addr  = (uint32_t)0,  .block_size = (uint32_t)0, .this_block_st_addr = (uint32_t)0, .this_block_number = (uint32_t)0}

};


#define NUM_BLOCK_DATA_TABLE_ENTRIES 2

/** 1024 blocks of 64 bytes  */
static const flash_block_info_t g_flash_DataBlockSizes[NUM_BLOCK_DATA_TABLE_ENTRIES] =
{
        {.is_code_flash_addr = false, .num_blocks = (uint32_t)1024, .block_section_end_addr = (uint32_t)FLASH_DF_BLOCK_END, .this_block_end_addr = (uint32_t)0,
         .block_section_st_addr  = (uint32_t)FLASH_DF_BLOCK_0,  .block_size = (uint32_t)64, .this_block_st_addr = (uint32_t)0, .this_block_number = (uint32_t)0},

        {.is_code_flash_addr = false, .num_blocks = (uint32_t)0, .block_section_end_addr = (uint32_t)0, .this_block_end_addr = (uint32_t)0,
          .block_section_st_addr  = (uint32_t)0,  .block_size = (uint32_t)0, .this_block_st_addr = (uint32_t)0, .this_block_number = (uint32_t)0}
};


/*******************************************************************************************************************//**
 * @addtogroup FLASH_HP
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Initializes the flash peripheral. Implements flash_api_t::open.
 *
 * The Open function initializes the Flash. It first copies the FCU firmware to FCURAM
 * and sets the FCU Clock based on the current FCLK frequency. In addition, if Code Flash programming is enabled,
 * the API code responsible for Code Flash programming will be copied to RAM.
 *
 * This function must be called once prior to calling any other FLASH API functions.
 * If a user supplied callback function is supplied, then the Flash Ready and Error interrupts will be configured to
 * call the users callback routine with an Event type describing the source of the interrupt.
 *
 * @note Providing a callback function in the supplied p_cfg->callback field, automatically configures the
 *       Flash for Data Flash to operate in non-blocking (BGO) mode.
 *
 * Subsequent to a successful Open(), the Flash is ready to process additional Flash commands.
 *
 * @retval SSP_SUCCESS               Initialization was successful and timer has started.
 * @retval SSP_ERR_IN_USE            The Flash peripheral is busy with a prior on-going transaction.
 * @retval SSP_FLASH_ERR_            FAILURE Failed to successfully enter Programming/Erase mode.
 * @retval SSP_ERR_TIMEOUT           Timed out waiting for FCU to be ready.
 * @retval SSP_ERR_ASSERTION         NULL provided for p_ctrl or p_cfg.
 * @retval SSP_ERR_IRQ_BSP_DISABLED  Caller is requesting BGO but the Flash interrupts are not enabled.
 *
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_Open (flash_ctrl_t * const p_ctrl, flash_cfg_t const * const p_cfg)
{
    /** g_flash_hp_version is accessed by the ASSERT macro only, and so compiler toolchain can issue a
     *  warning that it is not accessed. The code below eliminates this warning and also ensures data
     *  structures are not optimized away. */
    SSP_PARAMETER_NOT_USED(g_flash_hp_version);

    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);
    SSP_PARAMETER_NOT_USED(g_flash_api_open);

    ssp_err_t err = SSP_SUCCESS;
#if (FLASH_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_ctrl);
#endif

    /* Take the hardware lock for the Flash */
    FLASH_ERROR_RETURN(!(SSP_SUCCESS != R_BSP_HardwareLock(BSP_HW_LOCK_FACI)), SSP_ERR_HW_LOCKED);

    /** Allow Initialization if not initialized or if no operation is ongoing and re-initialization is desired */
    if ((FLASH_STATE_UNINITIALIZED == g_flash_state) || (FLASH_STATE_READY == g_flash_state))
    {
        /** Check that API is not already Open */
        FLASH_ERROR_RETURN(!(SSP_SUCCESS != flash_lock_state(FLASH_STATE_INITIALIZATION)), SSP_ERR_IN_USE);
    }

    /** Set the parameters struct based on the user supplied settings */
    g_current_parameters.bgo_enabled_df = p_cfg->data_flash_bgo;

    err                                 = flash_setup(); /// Check FCLK, calculate timeout values

    if (g_current_parameters.bgo_enabled_df == true)
    {
        /** Setup the Flash interrupt callback based on the caller's info. If both Flash interrupts are
         * not enabled in the BSP then this will return SSP_ERR_IRQ_BSP_DISABLED */
        err = HW_FLASH_HP_irq_cfg((bool) (p_cfg->p_callback != NULL), p_cfg);
    }
    else
    {
        /** Make sure Flash interrupts are disabled, they are only used in BGO mode */
        HW_FLASH_HP_irq_cfg(false, p_cfg);
    }

    /* Release state so other operations can be performed. */
    flash_ReleaseState();

    /** If we failed to successfully open then return the hardware lock */
    if (err != SSP_SUCCESS)
    {
        /* Return the hardware lock for the Flash */
        R_BSP_HardwareUnlock(BSP_HW_LOCK_FACI);
    }
    else
    {
        g_flash_api_open = true;         /// API is now open
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief  Writes to the specified Code or Data Flash memory area. Implements flash_api_t::write.
 *
 * @retval SSP_SUCCESS              Operation successful. If BGO is enabled this means the operation was started
 *                                  successfully.
 * @retval SSP_ERR_IN_USE       The Flash peripheral is busy with a prior on-going transaction.
 * @retval SSP_FLASH_ERR_FAILURE    Operation failed. Possibly destination address is under access window control.
 * @retval SSP_ERR_TIMEOUT    Timed out waiting for FCU operation to complete.
 * @retval SSP_ERR_INVALID_SIZE      Number of bytes provided was not a multiple of the programming size or exceeded
 *                                  the maximum range.
 * @retval SSP_ERR_INVALID_ADDRESS    Invalid address was input or address not on programming boundary.
 * @retval SSP_ERR_ASSERTION        NULL provided for p_ctrl.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_Write (flash_ctrl_t * const p_ctrl,
                            uint32_t const       src_address,
                            uint32_t             flash_address,
                            uint32_t const       num_bytes)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err = SSP_SUCCESS;

    /* Get the block information for this address */
    FLASH_ERROR_RETURN((flash_get_block_info(flash_address, &g_block_info)), SSP_ERR_INVALID_ADDRESS);

#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure num_bytes > 0 */
    FLASH_ERROR_RETURN((0 != num_bytes), SSP_ERR_INVALID_SIZE);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);

    /* Is this a request to Write Code Flash? */
    if (g_block_info.is_code_flash_addr == true)
    {
#if (FLASH_CFG_PARAM_CODE_FLASH_PROGRAMMING_ENABLE == 1)
        /* Ensure start address is valid and on a programming boundary*/
        if (((int32_t)flash_address >= FLASH_CF_BLOCK_0) && (flash_address <= FLASH_CF_BLOCK_END) && (!(flash_address & (FLASH_MIN_PGM_SIZE_CF - 1))))
        {
            /*Check if there is enough space in the destination, and that num_bytes is a multiple of programming size*/
            if ((((num_bytes - 1) + flash_address) > (uint32_t) FLASH_CF_BLOCK_END) || (num_bytes & (FLASH_MIN_PGM_SIZE_CF - 1)))
            {
                FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_SIZE);
            }
        }
        else
        {
            FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_ADDRESS);
        }
#else
        FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);
#endif
    }
    else
    {
        /* This is a request to write Data Flash */
        /* Ensure start address is valid */
        /* Check address validity and that it is on a Data Flash programming boundary */
        if ((flash_address >= (uint32_t)FLASH_DF_BLOCK_0) && (flash_address <= (uint32_t)FLASH_DF_BLOCK_END) &&
            (!(flash_address & (uint32_t)(FLASH_MIN_PGM_SIZE_DF - 1))))
        {
            /*Check if there is enough space in the destination, and that num_bytes is a multiple of programming size*/
            if ((((num_bytes - 1) + flash_address) > (uint32_t)FLASH_DF_BLOCK_END) || (num_bytes & (FLASH_MIN_PGM_SIZE_DF - 1)))
            {
                FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_SIZE);
            }
        }
        else
        {
            FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_ADDRESS);
        }
    }
#endif /* if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1) */

    /** Update Flash state and enter the respective Code or Data Flash P/E mode */
    if (g_block_info.is_code_flash_addr == true)
    {
        err = setup_for_pe_mode(FLASH_TYPE_CODE_FLASH, FLASH_STATE_WRITING);
    }
    else
    {
        err = setup_for_pe_mode(FLASH_TYPE_DATA_FLASH, FLASH_STATE_WRITING);
    }

    /* Still good to go? */
    if (SSP_SUCCESS == err)
    {
        /* Is this a request to Erase Code Flash? */
        if (g_block_info.is_code_flash_addr == true)
        {
            g_current_parameters.wait_cnt          = g_current_parameters.wait_max_write_cf;
            g_current_parameters.current_operation = FLASH_OPERATION_CF_WRITE;
            g_current_parameters.min_pgm_size      = (FLASH_MIN_PGM_SIZE_CF >> 1);
        }
        else
        {
            /* This is a request to erase Data Flash */
            g_current_parameters.wait_cnt     = g_current_parameters.wait_max_write_df;
            g_current_parameters.min_pgm_size = (FLASH_MIN_PGM_SIZE_DF >> 1);
            if (g_current_parameters.bgo_enabled_df == false)
            {
                g_current_parameters.current_operation = FLASH_OPERATION_DF_WRITE;
            }
            else
            {
                g_current_parameters.current_operation = FLASH_OPERATION_DF_BGO_WRITE;
            }
        }

        /*Write the data*/
        err = HW_FLASH_HP_write(&src_address, &flash_address, num_bytes);
        if (SSP_SUCCESS == err)
        {
            /*If in non-BGO mode, then current operation is completed.
             * Exit from PE mode and return status*/
            if (g_current_parameters.current_operation != FLASH_OPERATION_DF_BGO_WRITE)
            {
                /*Return to read mode*/
                err = HW_FLASH_HP_pe_mode_exit();
            }
        }
    }

    if (SSP_SUCCESS != err)
    {
        /*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
        HW_FLASH_HP_reset();
    }

    if (g_current_parameters.current_operation != FLASH_OPERATION_DF_BGO_WRITE)
    {
        flash_ReleaseState();
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief  Reads the requested number of bytes from the supplied Data or Code Flash memory address.
 *         Implements flash_api_t::read.
 *         @note This function is provided simply for the purposes of maintaining a complete interface.
 *         It is possible (and recommended), to read Flash memory directly.
 *
 * @retval SSP_SUCCESS              Operation successful.
 * @retval SSP_ERR_INVALID_ADDRESS    Invalid Flash address was supplied.
 * @retval SSP_ERR_ASSERTION        NULL provided for p_ctrl or p_dest_address
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_Read  (flash_ctrl_t * const p_ctrl, uint8_t * p_dest_address, uint32_t const flash_address,
                            uint32_t const num_bytes)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err           = SSP_SUCCESS;
    uint32_t  index;
    uint8_t   * p_flash_ptr = (uint8_t *) flash_address;

#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)
    SSP_ASSERT(NULL != p_dest_address);
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure num_bytes > 0 */
    FLASH_ERROR_RETURN((0 != num_bytes), SSP_ERR_INVALID_SIZE);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);

    FLASH_ERROR_RETURN(!(((int32_t)flash_address < FLASH_CF_BLOCK_0) || ((int32_t)flash_address > (int32_t)FLASH_DF_BLOCK_END)),
                       SSP_ERR_INVALID_ADDRESS);
#endif /* if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1) */

    for (index = (uint32_t)0; index < num_bytes; index++)
    {
        p_dest_address[index] = p_flash_ptr[index];
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief  Erases the specified Code or Data Flash blocks. Implements flash_api_t::erase
 *                                     by the block_erase_address.
 * @retval SSP_SUCCESS                 Successful open.
 * @retval SSP_ERR_INVALID_BLOCKS        Invalid number of blocks specified
 * @retval SSP_ERR_INVALID_ADDRESS       Invalid address specified
 * @retval SSP_ERR_IN_USE          Other flash operation in progress, or API not initialized
 * @retval SSP_FLASH_ERR_FAILURE       Other flash operation failure. Flash Control Unit has been reset.
 * @retval SSP_ERR_ASSERTION           NULL provided for p_ctrl
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_Erase (flash_ctrl_t * const p_ctrl, uint32_t const address, uint32_t const num_blocks)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err = SSP_SUCCESS;

    /* Get the block information for this address */
    FLASH_ERROR_RETURN((flash_get_block_info(address, &g_block_info)), SSP_ERR_INVALID_ADDRESS);

#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);

    /* Is this a request to Erase Code Flash? */
    if (g_block_info.is_code_flash_addr == true)
    {
#if (FLASH_CFG_PARAM_CODE_FLASH_PROGRAMMING_ENABLE == 1)
      if ((num_blocks > FLASH_NUM_BLOCKS_CF) || (num_blocks > (FLASH_NUM_BLOCKS_CF - g_block_info.this_block_number)) ||
          (num_blocks <= (uint32_t)0))
        {
            FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_BLOCKS);
        }
#else
      FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);
#endif
    }
    else
    {
        /* This is a request to erase Data Flash */
        if ((num_blocks > FLASH_NUM_BLOCKS_DF) ||
            (num_blocks > (FLASH_NUM_BLOCKS_DF - g_block_info.this_block_number)) ||
            (num_blocks <= (uint32_t)0))
        {
            FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_BLOCKS);
        }
    }
#endif /* if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1) */

    /** Update Flash state and enter the respective Code or Data Flash P/E mode */
    if (g_block_info.is_code_flash_addr == true)
    {
        err = setup_for_pe_mode(FLASH_TYPE_CODE_FLASH, FLASH_STATE_WRITING);
    }
    else
    {
        err = setup_for_pe_mode(FLASH_TYPE_DATA_FLASH, FLASH_STATE_WRITING);
    }

    /* Still good to go? */
    if (SSP_SUCCESS == err)
    {
        /* Is this a request to Erase Code Flash? */
        if (g_block_info.is_code_flash_addr == true)
        {
            g_current_parameters.current_operation = FLASH_OPERATION_CF_ERASE;
        }
        else
        {
            /* This is a request to erase Data Flash */
            if (g_current_parameters.bgo_enabled_df == false)
            {
                g_current_parameters.current_operation = FLASH_OPERATION_DF_ERASE;
            }
            else
            {
                g_current_parameters.current_operation = FLASH_OPERATION_DF_BGO_ERASE;
            }
        }

        /* Erase the Blocks, give this function the details about this block */
        err = HW_FLASH_HP_erase(g_block_info.this_block_st_addr, num_blocks);
        if (SSP_SUCCESS == err)
        {
            /*If in non-BGO mode, then current operation is completed.
             * Exit from PE mode and return status*/
            if (g_current_parameters.current_operation != FLASH_OPERATION_DF_BGO_ERASE)
            {
                /*Return to read mode*/
                err = HW_FLASH_HP_pe_mode_exit();
            }
        }
    }

    if (SSP_SUCCESS != err)
    {
        /*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
        HW_FLASH_HP_reset();
    }

    if (g_current_parameters.current_operation != FLASH_OPERATION_DF_BGO_ERASE)
    {
        flash_ReleaseState();
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief  Performs a blank check on the specified address area. Implements flash_api_t::blankCheck.
 * @retval SSP_SUCCESS              Blankcheck operation completed with result in p_blank_check_result,
 *                                  or blankcheck started and in-progess (BGO mode).
 * @retval SSP_FLASH_ERR_FAILURE    Operation Failed for some reason.
 * @retval SSP_ERR_INVALID_ADDRESS    Invalid data flash address was input.
 * @retval SSP_ERR_INVALID_SIZE      'num_bytes' was either too large or not aligned for the CF/DF boundary size.
 * @retval SSP_ERR_IN_USE       Other flash operation in progress or API not initialized.
 * @retval SSP_ERR_ASSERTION        NULL provided for p_ctrl.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_BlankCheck (flash_ctrl_t * const p_ctrl, uint32_t const address, uint32_t num_bytes,
                                 flash_result_t * p_blank_check_result)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err         = SSP_SUCCESS;
    uint32_t  index;
    uint8_t   * p_address = (uint8_t *) address;

    /* Get the block information for this address */
    FLASH_ERROR_RETURN((flash_get_block_info(address, &g_block_info)), SSP_ERR_INVALID_ADDRESS);

#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);

    /* Is this a request to Blank Check Code Flash? */
    if (g_block_info.is_code_flash_addr == true)
    {
#if (FLASH_CFG_PARAM_CODE_FLASH_PROGRAMMING_ENABLE == 1)
        if ((((int32_t)address >= FLASH_CF_BLOCK_0) && ((int32_t)address <= (int32_t)FLASH_CF_BLOCK_END)))
        {
            /* Check if the range is valid, num_bytes is larger than 0 - up to and including the last CF byte */
            if (((num_bytes + address) > (uint32_t)(FLASH_CF_BLOCK_END + 1)) || (num_bytes == (uint32_t)0))
            {
                FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_SIZE);
            }
        }
        else
        {
            FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_ADDRESS);
        }
#else
        FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);
#endif
    }
    else
    {
        if (((address >= (uint32_t)FLASH_DF_BLOCK_0) && (address <= (uint32_t)FLASH_DF_BLOCK_END)))
        {
            /*Check if the range is valid, num_bytes is a multiple of 4, is larger than 0 and less than
             * FLASH_DATA_BLANK_CHECK_MAX - up to and including the last DF byte*/
            if (
                ((num_bytes + address) > (int32_t)(FLASH_DF_BLOCK_END + 1)) ||
                (num_bytes & (FLASH_MIN_PGM_SIZE_DF - 1)) ||
                (num_bytes == (uint32_t)0) ||
                (num_bytes > FLASH_DATA_BLANK_CHECK_MAX)
                )
            {
                FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_SIZE);
            }
        }
        else
        {
            FLASH_ERROR_RETURN(0, SSP_ERR_INVALID_ADDRESS);
        }
    }
#endif /* if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1) */

    /* Check if API is busy*/
    if (g_flash_state != FLASH_STATE_READY)
    {
        /* API not initialized or busy with another operation*/
        err = SSP_ERR_IN_USE;
    }

    /* Grab the current flash state*/
    if (SSP_SUCCESS != flash_lock_state(FLASH_STATE_BLANK_CHECK))
    {
        /* API busy with another operation*/
        err = SSP_ERR_IN_USE;
    }

    /* Still good to go? */
    if (SSP_SUCCESS == err)
    {
        /* Is this a request to Blank check Code Flash? */
        if (g_block_info.is_code_flash_addr == true)
        {
            /// Blank checking for Code Flash does not require any FCU operations. The specified address area
            /// Can simply be checked for non 0xFF.
            g_current_parameters.current_operation = FLASH_OPERATION_CF_BLANKCHECK;
            *p_blank_check_result                  = FLASH_RESULT_BLANK; /// Assume blank until we know otherwise
            for (index = (uint32_t)0; index < num_bytes; index++)
            {
                if (p_address[index] != 0xFF)
                {
                    *p_blank_check_result = FLASH_RESULT_NOT_BLANK;
                    break;
                }
            }
        }
        else
        {
            /* This is a request to Blank check Data Flash */
            /*No errors in parameters. Enter Data Flash PE mode*/
            err = HW_FLASH_HP_pe_mode_enter(FLASH_TYPE_DATA_FLASH);
            if (g_current_parameters.bgo_enabled_df == false)
            {
                g_current_parameters.current_operation = FLASH_OPERATION_DF_BLANKCHECK;
            }
            else
            {
                g_current_parameters.current_operation = FLASH_OPERATION_DF_BGO_BLANKCHECK;
            }
        }

        /* If the request was to blank check Code Flash then we've already done it above and set the result */
        if (g_block_info.is_code_flash_addr == false)
        {

            /* Start the Blank check operation. If BGO is enabled then the result of the Blank check will be
             * available when the interrupt occurs and p_blank_check_result will contain FLASH_RESULT_BGO_ACTIVE */

            /*wait_max_blank_check specifies the wait time for a 4 byte blank check,
             * num_bytes is divided by 4 and then multiplied to calculate the wait time for the entire operation*/
            if (num_bytes < (uint32_t)4)
            {
                g_current_parameters.wait_cnt = g_current_parameters.wait_max_blank_check * num_bytes;
            }
            else
            {
                g_current_parameters.wait_cnt = (g_current_parameters.wait_max_blank_check * (num_bytes >> 2));
            }

            err = HW_FLASH_HP_blankcheck(address, num_bytes, p_blank_check_result);

            if (SSP_SUCCESS == err)
            {
                /*If in non-BGO mode, then current operation is completed.
                 * Exit from PE mode and return status*/
                if (g_current_parameters.current_operation != FLASH_OPERATION_DF_BGO_BLANKCHECK)
                {
                    /*Return to read mode*/
                    err                                    = HW_FLASH_HP_pe_mode_exit();
                    g_current_parameters.current_operation = FLASH_OPERATION_IDLE;
                }
            }
        }
    }

    if (SSP_SUCCESS != err)
    {
        /*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
        HW_FLASH_HP_reset();
    }

    if (g_current_parameters.current_operation != FLASH_OPERATION_DF_BGO_BLANKCHECK)
    {
        flash_ReleaseState();
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief  Query the FLASH peripheral for its status. Implements flash_api_t::statusGet.
 *
 * @retval SSP_SUCCESS               FLASH peripheral is ready to use.
 * @retval SSP_ERR_IN_USE        FLASH peripheral is busy with a prior operation.
 * @retval SSP_ERR_ASSERTION         NULL provided for p_ctrl.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_StatusGet (flash_ctrl_t * const p_ctrl)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err = SSP_SUCCESS;
#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);
#endif

    /* Return flash status */
    if (g_flash_state == FLASH_STATE_READY)
    {
        err = SSP_SUCCESS;
    }
    else
    {
        err = SSP_ERR_IN_USE;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief  Configure an access window for the Code Flash memory using the provided start and end address. An access
 *         window defines a contiguous area in Code Flash for which programming/erase is enabled. This area is
 *         on block boundaries.
 *         The block containing start_addr is the first block. The block containing end_addr is the last block.
 *         The access window then becomes first block --> last block inclusive. Anything outside this range
 *         of Code Flash is then write protected.
 *         @note If the start address and end address are set to the same value, then the access window
 *         is effectively removed. This accomplishes the same functionality as R_FLASH_HP_AccessWindowClear().
 *
 *         Implements flash_api_t::accessWindowSet.
 *
 * @retval SSP_SUCCESS              Access window successfully configured.
 * @retval SSP_ERR_INVALID_ADDRESS    Invalid settings for start_addr and/or end_addr.
 * @retval SSP_ERR_IN_USE       FLASH peripheral is busy with a prior operation.
 * @retval SSP_ERR_ASSERTION        NULL provided for p_ctrl.
 * @retval SSP_ERR_INVALID_ARGUMENT Code Flash Programming is not enabled.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_AccessWindowSet (flash_ctrl_t * const p_ctrl, uint32_t const start_addr,
                                      uint32_t const end_addr)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    SSP_PARAMETER_NOT_USED(start_addr);      /// Remove warnings generated when Code Flash code is not compiled in.
    SSP_PARAMETER_NOT_USED(end_addr);

    ssp_err_t err = SSP_SUCCESS;

#if (FLASH_CFG_PARAM_CODE_FLASH_PROGRAMMING_ENABLE == 1)
#if (FLASH_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);
#endif

    if (((int32_t)start_addr >= FLASH_CF_BLOCK_0) && (end_addr <= (uint32_t)(FLASH_CF_BLOCK_END)) && (start_addr <= end_addr))
    {
        /** Update Flash state and enter Data Flash P/E mode */
        err = setup_for_pe_mode(FLASH_TYPE_DATA_FLASH, FLASH_STATE_ACCESS_WINDOW);

        /* Still good to go? */
        if (SSP_SUCCESS == err)
        {
            err = HW_FLASH_HP_access_window_set(start_addr, end_addr);
        }

        if (SSP_SUCCESS == err)
        {
            /*Return to read mode*/
            err = HW_FLASH_HP_pe_mode_exit();
        }
        else
        {
            /*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
            HW_FLASH_HP_reset();
        }

        flash_ReleaseState();
    }
    else
    {
        err = SSP_ERR_INVALID_ADDRESS;
    }

#else
    err = SSP_ERR_INVALID_ARGUMENT;     ///< For consistency with _LP API we return error if Code Flash not enabled
#endif
    return err;
}

/*******************************************************************************************************************//**
 * @brief  Remove any access window that is currently configured in the Code Flash.
 *         Subsequent to this call all Code Flash is writable.
 *         Implements flash_api_t::accessWindowClear.
 *
 * @retval SSP_SUCCESS          Access window successfully removed.
 * @retval SSP_ERR_IN_USE       FLASH peripheral is busy with a prior operation.
 * @retval SSP_ERR_ASSERTION    NULL provided for p_ctrl.
 * @retval SSP_ERR_INVALID_ARGUMENT Code Flash Programming is not enabled.
 *
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_AccessWindowClear (flash_ctrl_t * const p_ctrl)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err = SSP_SUCCESS;

#if (FLASH_CFG_PARAM_CODE_FLASH_PROGRAMMING_ENABLE == 1)

#if (FLASH_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);
#endif

    /** Update Flash state and enter Data Flash P/E mode */
    err = setup_for_pe_mode(FLASH_TYPE_DATA_FLASH, FLASH_STATE_ACCESS_WINDOW);

    /* Still good to go? */
    if (SSP_SUCCESS == err)
    {
        err = HW_FLASH_HP_access_window_clear();
    }
    if (SSP_SUCCESS == err)
    {
        /*Return to read mode*/
        err = HW_FLASH_HP_pe_mode_exit();
    }
    else
    {
        /*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
        HW_FLASH_HP_reset();
    }

    flash_ReleaseState();
#else
    err = SSP_ERR_INVALID_ARGUMENT;     ///< For consistency with _LP API we return error if Code Flash not enabled
#endif
    return err;
}

/*******************************************************************************************************************//**
 * @brief  Resets the FLASH peripheral. Implements flash_api_t::reset.
 * No attempt is made to grab the Flash software lock before executing the reset since the assumption is that a reset
 * will terminate any existing operation.
 *
 * @retval SSP_SUCCESS       Flash circuit successfully reset.
 * @retval SSP_ERR_ASSERTION  NULL provided for p_ctrl.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_Reset (flash_ctrl_t * const p_ctrl)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err = SSP_SUCCESS;

#if (FLASH_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);
#endif

    err = HW_FLASH_HP_reset();
    flash_ReleaseState();

    return err;
}

/******************************************************************************************************************//**
 * @brief  Selects which block - Default (Block 0) or Alternate (Block 1) is used as the startup area block.
 *         The provided parameters determine which block will become the active startup block and whether that
 *         action will be immediate (but temporary), or permanent subsequent to the next reset.
 *         Doing a temporary switch might appear to have limited usefulness. If there is an access window
 *         in place such that Block 0 is write protected, then one could do a temporary switch, update the
 *         block and switch them back without having to touch the access window.
 *         Implements flash_api_t::startupAreaSelect.
 *
 * @retval SSP_SUCCESS              Start-up area successfully toggled.
 * @retval SSP_ERR_HW_LOCKED        FLASH peripheral has already been initialized and is in use.
 * @retval SSP_ERR_IN_USE       FLASH peripheral is busy with a prior operation.
 * @retval SSP_ERR_ASSERTION        NULL provided for p_ctrl.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_StartUpAreaSelect (flash_ctrl_t * const      p_ctrl,
                                        flash_startup_area_swap_t swap_type,
                                        bool                      is_temporary)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err = SSP_SUCCESS;

#if (FLASH_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);
#endif

    /** This is already the normal reset behavior. There's nothing to do */
    if ((swap_type == FLASH_STARTUP_AREA_BTFLG) && (is_temporary == false))
    {
        return err;
    }

    /** Update Flash state and enter the respective Code or Data Flash P/E mode based on whether or not we will be c
     * calling Configuration Set */
    if (is_temporary == true)
    {
        err = setup_for_pe_mode(FLASH_TYPE_CODE_FLASH, FLASH_STATE_STARTUP_AREA);
        if (err == SSP_SUCCESS)
        {
            err = HW_FLASH_HP_set_startup_area_temporary(swap_type);
        }
    }
    else
    {
        err = setup_for_pe_mode(FLASH_TYPE_DATA_FLASH, FLASH_STATE_STARTUP_AREA);
        if (err == SSP_SUCCESS)
        {
            err = HW_FLASH_HP_set_startup_area_boot(swap_type);
        }
    }

    /* Success? */
    if (SSP_SUCCESS == err)
    {
        /*Return to read mode*/
        err = HW_FLASH_HP_pe_mode_exit();
    }

    if (SSP_SUCCESS != err)
    {
        /*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
        HW_FLASH_HP_reset();
    }

    flash_ReleaseState();
    return err;
}

/******************************************************************************************************************//**
 * @brief  Indicate to the already open Flash API, that the FCLK has changed since the Open(). This could be the case if
 *         the application has changed the system clock, and therefore the FCLK. Failure to call this function subsequent
 *         to changing the FCLK could result in damage to the flash macro. This function uses R_CGC_SystemClockFreqGet()
 *         to get the current FCLK frequency.
 *         Implements flash_api_t::updateFlashClockFreq.
 *
 * @retval SSP_SUCCESS              Start-up area successfully toggled.
 * @retval SSP_ERR_IN_USE           Flash is busy with an on-going operation.
 * @retval SSP_ERR_ASSERTION        NULL provided for p_ctrl
 * @retval SSP_ERR_NOT_OPEN         Flash API has not yet been opened.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_UpdateFlashClockFreq (flash_ctrl_t * const  p_ctrl)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err = SSP_SUCCESS;

#if (FLASH_CFG_PARAM_CHECKING_ENABLE)
    SSP_ASSERT(NULL != p_ctrl);

    /** Insure API has been opened */
    FLASH_ERROR_RETURN(!(true != g_flash_api_open), SSP_ERR_NOT_OPEN);
#endif

    /** Grab the current flash state*/
    if (SSP_SUCCESS != flash_lock_state(FLASH_UPDATE_FCLK))
    {
        /** API already in this state*/
        err = SSP_ERR_IN_USE;
    }

    err = flash_setup();    ///< Check FCLK, calculate timeout values

    flash_ReleaseState();

    return err;
}
/*******************************************************************************************************************//**
 * @brief      Releases any resources that were allocated by the Open() or any subsequent Flash operations. Implements
 *             flash_api_t::close.
 *
 * @retval     SSP_SUCCESS       Successful close.
 * @retval SSP_ERR_ASSERTION     NULL provided for p_ctrl or p_cfg.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_Close (flash_ctrl_t * const p_ctrl)
{
    /** Eliminate warning if parameter checking is disabled. */
    SSP_PARAMETER_NOT_USED(p_ctrl);

    ssp_err_t err = SSP_SUCCESS;

#if FLASH_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif
    /* Return the hardware lock for the Flash */
    R_BSP_HardwareUnlock(BSP_HW_LOCK_FACI);

    HW_FLASH_HP_irq_cfg(false, NULL);

    /** Release the lock */
    flash_ReleaseState();

    return err;
}

/*******************************************************************************************************************//**
 * @brief   This function gets FLASH HAL driver version
 * @retval  SSP_SUCCESS - operation performed successfully
 * @note This function is reentrant.
 **********************************************************************************************************************/
ssp_err_t R_FLASH_HP_VersionGet (ssp_version_t * const p_version)
{
#if FLASH_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id = g_flash_hp_version.version_id;
    return SSP_SUCCESS;
}  /* End of function R_FLASH_HP_VersionGet() */

/*******************************************************************************************************************//**
 * @} (end FLASH_HP)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   This function attempts to change the flash state to the new requested state.
 *          This can only happen if we are able to take the FLASH lock. If the lock is currently in use then
 *          we will return FLASH_ERR_BUSY, otherwise we will take the lock and change the state.
 * @param[in]  state        Which state to attempt to transition to.
 * @retval SSP_SUCCESS             Successful.
 * @retval SSP_ERR_IN_USE      Flash is busy with another operation.
 **********************************************************************************************************************/
static ssp_err_t flash_lock_state (flash_states_t new_state)
{
    /* Check to see if lock was successfully taken */
    if (R_BSP_SoftwareLock(&g_flash_Lock) == SSP_SUCCESS)
    {
        /* Lock taken, we can change state */
        g_flash_state = new_state;

        /* Return success */
        return SSP_SUCCESS;
    }
    else
    {
        /* Another operation is on-going */
        return SSP_ERR_IN_USE;
    }
}

/*******************************************************************************************************************//**
 * @brief   This function releases the flash state so another flash operation can take place.
 *          This function is called by both the HLD and LLD layers (interrupt routines).
 * @retval None
 **********************************************************************************************************************/
void flash_ReleaseState (void)
{
    /* Done with current operation */
    g_flash_state = FLASH_STATE_READY;

    /* Release hold on lock */
    R_BSP_SoftwareUnlock(&g_flash_Lock);
}

/*******************************************************************************************************************//**
 * @brief   This function places the flash in the requested Code or Data P/E mode.
 * @param[in]   flash_type          FLASH_TYPE_CODE_FLASH or FLASH_TYPE_DATA_FLASH.
 * @param[in]   p_ctrl              Desired Flash state to transition into (ie FLASH_STATE_WRITING).
 * @retval SSP_SUCCESS             Successful.
 * @retval SSP_ERR_IN_USE      Flash is busy with another operation.
 **********************************************************************************************************************/
static ssp_err_t setup_for_pe_mode (flash_type_t flash_type, flash_states_t flash_state)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Check if API is busy*/
    if (g_flash_state != FLASH_STATE_READY)
    {
        /* API not initialized or busy with another operation*/
        err = SSP_ERR_IN_USE;
    }

    /* Grab the current flash state*/
    if (SSP_SUCCESS != flash_lock_state(flash_state))
    {
        /* API busy with another operation*/
        err = SSP_ERR_IN_USE;
    }

    /* Still good to go? */
    if (SSP_SUCCESS == err)
    {
        /* Configuration Set command requires Data Flash P/E mode */
        err = HW_FLASH_HP_pe_mode_enter(flash_type);
    }

    return err;
}

bool flash_get_block_info (uint32_t addr, flash_block_info_t * p_block_info)
{
    uint16_t                 num_sections = 0;
    uint32_t                 bnum;
    uint32_t                 bcount       = 0;
    uint16_t                 num_tbl_entries;
    const flash_block_info_t * pBInfo;

    /// Determine if this is a Code or Data Flash address, or something else (invalid)
    if (((int32_t)addr >= FLASH_CF_BLOCK_0) && (addr <= FLASH_CF_BLOCK_END))
    {
        p_block_info->is_code_flash_addr = true;
        num_tbl_entries                  = NUM_BLOCK_ROM_TABLE_ENTRIES;
        pBInfo                           = &g_flash_RomBlockSizes[0];
    }
    else if ((addr >= FLASH_DF_BLOCK_0) && (addr <= FLASH_DF_BLOCK_END))
    {
        p_block_info->is_code_flash_addr = false;
        num_tbl_entries                  = NUM_BLOCK_DATA_TABLE_ENTRIES;
        pBInfo                           = &g_flash_DataBlockSizes[0];
    }
    else
    {
        return false;
    }

    while (num_sections < num_tbl_entries)
    {
        num_sections++;
        /// Is this address within this section of blocks?
        if ((addr >= pBInfo->block_section_st_addr)  && (addr <= pBInfo->block_section_end_addr))
        {
            bnum                                 = ((addr - pBInfo->block_section_st_addr) / pBInfo->block_size);

            p_block_info->this_block_st_addr     = pBInfo->block_section_st_addr + (bnum * pBInfo->block_size);
            p_block_info->this_block_end_addr    = (p_block_info->this_block_st_addr + pBInfo->block_size) - 1;
            p_block_info->this_block_number      = bnum + bcount;
            p_block_info->block_section_st_addr  = pBInfo->block_section_st_addr;
            p_block_info->block_section_end_addr = pBInfo->block_section_end_addr;
            p_block_info->block_size             = pBInfo->block_size;
            p_block_info->num_blocks             = pBInfo->num_blocks;
            break;
        }
        else
        {
            bcount += pBInfo->num_blocks;           /// add in # of blocks in this section
            pBInfo++;
        }
    }

    return true;
}

/*******************************************************************************************************************//**
 * @brief   This function verifies that FCLK falls within the allowable range and calculates the timeout values
 *          based on the current FCLK frequency.
 *                            in by this function.
 * @retval false    Supplied address is neither a valid Code or Data Flash address for this part.
 * @retval true     Supplied address is valid and p_block_infocontains the details on this addresses block
 **********************************************************************************************************************/
static ssp_err_t flash_setup (void)
{
    ssp_err_t err;

    /** We need clock frequencies to calculate the worst case timeout values. */
    err = g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_FCLK, &g_current_parameters.flash_clock_freq);

    if (SSP_SUCCESS == err)
    {
        /** FCLK must be a minimum of 4 MHz for Flash operations */
        if (g_current_parameters.flash_clock_freq < MINIMUM_SUPPORTED_FCLK_FREQ)
        {
            /* Release state so other operations can be performed. */
            err = SSP_ERR_FCLK;
        }
        else
        {
            err = g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_ICLK, &g_current_parameters.system_clock_freq);
        }
    }

    if (SSP_SUCCESS == err)
    {
        /* Initialize the FCU */
        HW_FLASH_HP_init(&g_current_parameters);
    }

    return err;
}
#endif /* if defined(BSP_MCU_GROUP_S7G2) */


