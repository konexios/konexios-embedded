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
 * File Name    : hw_flash_hp.c
 * Description  : LLD Interface for the FLASH peripheral using RV40F Flash technology on SC32 MCUs.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup FLASH_RV40F
 * @brief FLASH API
 *
 * This module supports the FLASH peripheral using RV40F Flash technology.
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

#if defined(BSP_MCU_GROUP_S7G2)
#include "r_flash_hp.h"
#include "hw_flash_hp_private.h"
#include "r_flash_cfg.h"
#include "r_cgc_api.h"
#include "r_cgc.h"

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** These typedefs are used for guaranteeing correct accesses to memory. When
 * working with the FCU sometimes byte or word accesses are required. */
typedef  volatile uint8_t *  FCU_BYTE_PTR;
typedef  volatile uint16_t * FCU_WORD_PTR;

typedef volatile struct
{
    union
    {
        unsigned char  BYTE;
        struct
        {
            unsigned char  FLWE : 2;
            unsigned char       : 6;
        }  BIT;
    }  FWEPROR;
} SC32_FWEPROR_TypeDef;

#define                SC32_FWEPROR_BASE (0x4001E416UL)                 /* ( FWEPROR   ) Base Address */
#define                SC32_FWEPROR      (*(SC32_FWEPROR_TypeDef         *) (SC32_FWEPROR_BASE))

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define FCU_COMMAND_AREA (0x00407E0000)
FCU_BYTE_PTR g_pfcu_cmd_area = (FCU_BYTE_PTR) FCU_COMMAND_AREA;

/** R_FACI Commands */
#define FLASH_FACI_CMD_PROGRAM       0xE8
#define FLASH_FACI_CMD_PROGRAM_CF    0x80
#define FLASH_FACI_CMD_PROGRAM_DF    0x02
#define FLASH_FACI_CMD_BLOCK_ERASE   0x20
#define FLASH_FACI_CMD_PE_SUSPEND    0xB0
#define FLASH_FACI_CMD_PE_RESUME     0xD0
#define FLASH_FACI_CMD_STATUS_CLEAR  0x50
#define FLASH_FACI_CMD_FORCED_STOP   0xB3
#define FLASH_FACI_CMD_BLANK_CHECK   0x71
#define FLASH_FACI_CMD_CONFIG_SET_1  0x40
#define FLASH_FACI_CMD_CONFIG_SET_2  0x08
#define FLASH_FACI_CMD_LOCK_BIT_PGM  0x77
#define FLASH_FACI_CMD_LOCK_BIT_READ 0x71
#define FLASH_FACI_CMD_FINAL         0xD0

/**  Configuration set Command offset*/
#define FCU_CONFIG_SET_ACCESS_STARTUP    (0x00000060)

#define FCU_CONFIG_SET_FAWS_OFFSET       2          /// Zero based offset into configurationSetArea[] for FAWS
#define FCU_CONFIG_SET_FSPR_OFFSET       1          /// Zero based offset into configurationSetArea[] for FSPR

#define FCU_CONFIG_SET_FAWE_BTFLG_OFFSET 3          /// Zero based offset into configurationSetArea[] for FAWE and
                                                    /// BTFLG
#define CONFIG_SET_ACCESS_WORD_CNT       8          /// 8 words need to be written

#define SAS_DEFAULT_AREA                 0x6602     /// Bit value for FSUAC register specifying DEFAULT area
#define SAS_ALTERNATE_AREA               0x6603     /// Bit value for FSUAC register specifying ALTERNATE area
#define SAS_PER_BTFLG_AREA               0x6600     /// Bit value for FSUAC register specifying use BTFLG area

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static flash_block_info_t   g_block_info;               /// Structure holding block info about an address.
static current_parameters_t * gp_flash_parameters;      /// passed in via flash_init()

static uint16_t             configurationSetArea[CONFIG_SET_ACCESS_WORD_CNT] =
{
    0,     ///< WD0
    0,     ///< WD1
    0,     ///< WD2 - contains FAWS
    0,     ///< WD3 - contains BTFLG and FAWE
    0,     ///< WD4
    0,     ///< WD5
    0,     ///< WD6
    0,     ///< WD7
};



/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
void fcu_fiferr_isr (void);
void fcu_frdyi_isr (void);

static ssp_err_t HW_FLASH_HP_configurationSet (void) PLACE_IN_RAM_SECTION;

#if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_DISABLED) && (BSP_IRQ_CFG_FCU_FIFERR != BSP_IRQ_DISABLED)
static void (* flash_ready_isr_handler)(flash_callback_args_t *cb);    ///< Function pointer for Flash Ready ISR
static void (* flash_error_isr_handler)(flash_callback_args_t *cb);    ///< Function pointer for Flash Error ISR
#endif

/** Insure that user is warned if the priority level is not the same for the Flash Rdy and error interrupts */
#if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_DISABLED) && (BSP_IRQ_CFG_FCU_FIFERR != BSP_IRQ_DISABLED)
#if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_CFG_FCU_FIFERR)
#warning FLASH Rdy and Error interrupt priorities are not the same. They should be.
#endif
#endif


/*******************************************************************************************************************//**
 * @brief   This function enables or disables the Flash interrupt. The flash interrupt, if enabled, is called when a
 *          Flash operation completes, or when an Error condition is detected. If the caller has provided a callback
 *          function as part of the provided p_cfg pointer, then that function will be called as a result of the
 *          interrupt.
 * @param[in]  state        Control for the FLASH device context.
 * @param[in]  p_cfg        Pointer to the FLASH configuration structure.
 * @retval SSP_SUCCESS              Successful.
 * @retval SSP_ERR_INVALID_PTR      p_cfg is null.
 * @retval SSP_ERR_IRQ_BSP_DISABLED Caller is requesting BGO but the Flash interrupts are not enabled.
 *
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_irq_cfg (bool state, flash_cfg_t const * const p_cfg)
{
    SSP_PARAMETER_NOT_USED(state);          /// Prevent compiler 'unused' warning

/** If BGO is being used then we require both interrupts to be enabled (RDY and ERR). If either one
 *  is not enabled then we are not going to be doing BGO and the user will get don't generate any ISR routines */
#if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_DISABLED) && (BSP_IRQ_CFG_FCU_FIFERR != BSP_IRQ_DISABLED)

    /*Enable the Interrupts*/
    if (true == state)
    {
        /* Assign the callback if not NULL*/
        if (NULL == p_cfg->p_callback)
        {
            return SSP_ERR_INVALID_POINTER;
        }

        flash_ready_isr_handler = p_cfg->p_callback;
        flash_error_isr_handler = p_cfg->p_callback;

        /** Enable FCU interrupts if callback is not null, clear the Interrupt Request bit */
        R_FACI->FRDYIE = 0x01UL;
        NVIC_ClearPendingIRQ(FCU_FRDYI_IRQn);
        R_BSP_IrqStatusClear(FCU_FRDYI_IRQn);
        NVIC_EnableIRQ(FCU_FRDYI_IRQn);

        R_FACI->FAEINT = 0x99UL;
        NVIC_ClearPendingIRQ(FCU_FIFERR_IRQn);
        R_BSP_IrqStatusClear(FCU_FIFERR_IRQn);
        NVIC_EnableIRQ(FCU_FIFERR_IRQn);
    }
    /* Disable the Interrupts */
    else if (false == state)
    {
        /** Disable interrupt in ICU*/
        NVIC_DisableIRQ(FCU_FRDYI_IRQn);

        /** Disable Flash Rdy interrupt in the FLASH peripheral*/
        R_FACI->FRDYIE = 0x00UL;

        /** Clear the Interrupt Request bit */
        R_BSP_IrqStatusClear(FCU_FRDYI_IRQn);

        /** Disable interrupt in ICU*/
        NVIC_DisableIRQ(FCU_FIFERR_IRQn);

        /** Disable Flash Error interrupt in the FLASH peripheral*/
        R_FACI->FAEINT = 0x00UL;

        /** Clear the Interrupt Request bit */
        R_BSP_IrqStatusClear(FCU_FIFERR_IRQn);
    }
    else
    {
        /* Do nothing */
    }
    return SSP_SUCCESS;
#else
   /** The Flash Interrupts have not been enabled. If the caller is requesting BGO then we'll flag this as an error. */
   if (p_cfg->data_flash_bgo == true)
       return SSP_ERR_IRQ_BSP_DISABLED;
   else
       return SSP_SUCCESS;
#endif
}

/*******************************************************************************************************************//**
 * @brief   This function will initialize the FCU, copy the FCU firmware to FCURAM and
 *         set the FCU Clock based on the current FCLK frequency.
 * @param[in, out]  p_current_parameters        Pointer to the g_current_parameters structure created by the HLD layer.
 * @retval SSP_SUCCESS        Successful.
 * @retval SSP_ERR_TIMEOUT  Timed out while attempting to switch to P/E mode or
 *                            while trying to issue a STOP or an ongoing flash operation timed out.
 * @retval FLASH_ERR_LOCKED   Switch to Read mode timed out and STOP was attempted to recover. Stop operation failed.
 *                            Flash Peripheral in locked state.
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_init (current_parameters_t * p_current_parameters)
{
    int32_t  expression;
    ssp_err_t err = SSP_SUCCESS;

    gp_flash_parameters                    = p_current_parameters; // our copy from now on

    gp_flash_parameters->current_operation = FLASH_OPERATION_FCU_INIT;

    /*Allow Access to the Flash registers*/
    SC32_FWEPROR.FWEPROR.BYTE = 0x01;       // Not currently available in the register set.

    /* Set the Clock */
    R_FACI->FPCKAR = (uint16_t)((0x1E00UL) + gp_flash_parameters->flash_clock_freq);

    if (SSP_SUCCESS == err)
    {
        gp_flash_parameters->flash_clock_freq  = gp_flash_parameters->flash_clock_freq / 1000000;  // Scale it down
        gp_flash_parameters->system_clock_freq = gp_flash_parameters->system_clock_freq / 1000000; // Scale it down

        /* Common expression for 60 MHz calculations */
        expression =
            (int32_t) ((60.0 / (gp_flash_parameters->flash_clock_freq)) * (gp_flash_parameters->system_clock_freq));


        /*  According to HW Manual the Max Programming Time for 256 bytes (ROM)
         *  is 7.2ms.  This is with a FCLK of 60MHz. The calculation below
         *  calculates the number of ICLK ticks needed for the timeout delay.
         *  The 8ms number is adjusted linearly depending on the FCLK frequency.
         */
        gp_flash_parameters->wait_max_write_cf =    (uint32_t) (8000 * expression);

        /*  According to HW Manual the Max Programming Time for 4 bytes
         *  (Data Flash) is 1.7ms.  This is with a FCLK of 60MHz. The calculation
         *  below calculates the number of ICLK ticks needed for the timeout delay.
         *  The 1.7ms number is adjusted linearly depending on the FCLK frequency.
         */
        gp_flash_parameters->wait_max_write_df =    (uint32_t) (1700 * expression);

        /*  According to HW Manual the Max Blank Check time for 4 bytes
         *  (Data Flash) is 30 usec.  This is with a FCLK of 60MHz. The calculation
         *  below calculates the number of ICLK ticks needed for the timeout delay.
         *  The 30 usec number is adjusted linearly depending on the FCLK frequency.
         */
        gp_flash_parameters->wait_max_blank_check = (uint32_t) (30 *   expression);

        /*  According to HW Manual the max timeout value when using the peripheral
         *  clock notification command is 60us. This is with a FCLK of 50MHz. The
         *  calculation below calculates the number of ICLK ticks needed for the
         *  timeout delay. The 60us number is adjusted linearly depending on
         *  the FCLK frequency.
         */
        gp_flash_parameters->wait_max_notify_fcu_clock =

                    ((uint32_t) (60.0 *  (50.0 / (gp_flash_parameters->flash_clock_freq))) * (gp_flash_parameters->system_clock_freq));

        /*  According to HW Manual the max timeout value when using the configuration set
         *  command is 70 ms. This is with a FCLK of 20MHz. The
         *  calculation below calculates the number of ICLK ticks needed for the
         *  timeout delay. The 70 ms number is adjusted linearly depending on
         *  the FCLK frequency.
         */
        gp_flash_parameters->wait_max_configuration_set =
        ((uint32_t) (70000.0 *
                    (20.0 / (gp_flash_parameters->flash_clock_freq))) * (gp_flash_parameters->system_clock_freq));

        /*  According to HW Manual the Max Erasure Time for a 32KB block is
         *  around 480ms.  This is with a FCLK of 60MHz. The calculation below
         *  calculates the number of ICLK ticks needed for the timeout delay.
         *  The 480ms number is adjusted linearly depending on the FCLK frequency.
         */
        gp_flash_parameters->wait_max_erase_cf_large_block = (uint32_t)(480000 * expression);

        /*  According to HW Manual the Max Erasure Time for a 8KB block is
         *  around 144ms.  This is with a FCLK of 60MHz. The calculation below
         *  calculates the number of ICLK ticks needed for the timeout delay.
         *  The 150ms number is adjusted linearly depending on the FCLK frequency.
         */
        gp_flash_parameters->wait_max_erase_df_block = (uint32_t) (150000 * expression);

        /*  According to HW Manual the Max Erasure Time for a 64 byte Data Flash block is
         *  around 10ms.  This is with a FCLK of 60MHz. The calculation below
         *  calculates the number of ICLK ticks needed for the timeout delay.
         *  The 10ms number is adjusted linearly depending on the FCLK frequency.
         */
        gp_flash_parameters->wait_max_erase_cf_small_block = (uint32_t) (10000 * expression);
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   This function erases a specified number of Code or Data Flash blocks
 * @param[in]   block_address   The starting address of the first block to erase.
 * @param[in]   num_blocks      The number of blocks to erase.
 * @retval SSP_SUCCESS            Successfully erased (non-BGO) mode or operation successfully started (BGO).
 * @retval SSP_ERR_ERASE_FAILED    Status is indicating a Erase error.
 * @retval SSP_ERR_CMD_LOCKED   FCU is in locked state, typically as a result of having received an illegal
 *                                    command.
 * @retval SSP_ERR_WRITE_FAILED  Status is indicating a Programming error for the requested operation.
 * @retval SSP_ERR_TIMEOUT Timed out waiting for the FCU to become ready.
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_erase (uint32_t block_address, uint32_t num_blocks)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Set current operation parameters */
    gp_flash_parameters->dest_addr   = block_address;
    gp_flash_parameters->total_count = num_blocks;
    gp_flash_parameters->wait_cnt    = gp_flash_parameters->wait_max_erase_cf_large_block;

    /* Set Erasure Priority Mode*/
    R_FACI->FCPSR = (uint16_t)0x0001;

    for (gp_flash_parameters->current_count = 0UL;
         gp_flash_parameters->current_count    < gp_flash_parameters->total_count;
         gp_flash_parameters->current_count++)
    {
        /** Set block start address*/
        R_FACI->FSADDR = gp_flash_parameters->dest_addr;

        /* Issue two part Block Erase commands */
        *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_BLOCK_ERASE;
        *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_FINAL;

        /* if in BGO mode, exit here; remaining processing if any will be done in ISR */
        if (gp_flash_parameters->current_operation == FLASH_OPERATION_DF_BGO_ERASE)
        {
            return err;
        }

        /* Read FRDY bit until it has been set to 1 indicating that the current
         * operation is complete.*/
        while (1 != R_FACI->FSTATR_b.FRDY)
        {
            /* Wait until FRDY is 1 unless timeout occurs. */
            if (gp_flash_parameters->wait_cnt == 0U)
            {
                /* if FRDY is not set to 1 after max timeout, issue the stop command */
                HW_FLASH_HP_stop();
                return SSP_ERR_TIMEOUT; ///< Need to indicate an error occured
            }
            gp_flash_parameters->wait_cnt--;
        }

        /* Check if there were any errors */
        if (0 != R_FACI->FASTAT_b.CMDLK)
        {
            /* Check if status is indicating a Programming error */
            if ((0x0002 & R_FACI->FPESTAT) && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
            {
                err = SSP_ERR_WRITE_FAILED;
            }
            /* Check if status is indicating a Erase error */
            else if ((0x0012 & R_FACI->FPESTAT) && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
            {
                err = SSP_ERR_ERASE_FAILED;
            }
            else
            {
                err = SSP_ERR_CMD_LOCKED;
            }

            return err;
        }

        /* Get the block information about this address */
        if ((flash_get_block_info(gp_flash_parameters->dest_addr, &g_block_info)) != true)
        {
            return SSP_ERR_INVALID_ADDRESS;
        }

        /* increment to the next block starting address by adding the appropriate block size */
        gp_flash_parameters->dest_addr += g_block_info.block_size;

        if (R_FACI->FENTRYR == 0x0080UL)
        {
            gp_flash_parameters->wait_cnt = gp_flash_parameters->wait_max_erase_df_block;
        }
        else if (R_FACI->FENTRYR == 0x0001UL)
        {
            if (g_block_info.block_size > FLASH_CODE_SMALL_BLOCK_SZ)
            {
                gp_flash_parameters->wait_cnt = gp_flash_parameters->wait_max_erase_cf_large_block;
            }
            else
            {
                gp_flash_parameters->wait_cnt = gp_flash_parameters->wait_max_erase_cf_small_block;
            }
        }
        else
        {
            return SSP_ERR_ERASE_FAILED; // should never get here
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   This function writes a specified number of bytes to Code or Data Flash.
 * @param[in]   src_start_address   The starting address of the first byte (source) to write.
 * @param[in]   dest_start_address  The starting address of the Flash (destination) to write.
 * @retval SSP_SUCCESS            Data successfully written (non-BGO) mode or operation successfully started (BGO).
 * @retval SSP_ERR_ERASE_FAILED    Status is indicating an erase error. Possibly from a prior operation.
 * @retval SSP_ERR_CMD_LOCKED   FCU is in locked state, typically as a result of having received an illegal
 *                                    command.
 * @retval SSP_ERR_WRITE_FAILED  Status is indicating a Programming error for the requested operation. This
 *                                    may be returned if the requested Flash area is not blank.
 * @retval SSP_ERR_TIMEOUT Timed out waiting for the FCU to become ready.
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_write (uint32_t const * src_start_address, uint32_t * dest_start_address, uint32_t num_bytes)
{
    ssp_err_t err      = SSP_SUCCESS;
    uint32_t  wait_cnt = FLASH_DBFULL_TIMEOUT;

    gp_flash_parameters->total_count   = (num_bytes) >> 1; //Since two bytes will be written at a time
    gp_flash_parameters->dest_addr     = *dest_start_address;
    gp_flash_parameters->src_addr      = *src_start_address;
    gp_flash_parameters->current_count = (uint32_t)0;

    /* Iterate through the number of data bytes */
    while (gp_flash_parameters->total_count > (uint32_t)0)
    {
        /* Set block start address */
        R_FACI->FSADDR = gp_flash_parameters->dest_addr;

        /* Issue two part Write commands */
        *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_PROGRAM;
        *g_pfcu_cmd_area = (uint8_t) gp_flash_parameters->min_pgm_size;

        /* Write one line (FLASH_MIN_PGM_SIZE_DF bytes for DF, FLASH_MIN_PGM_SIZE_CF bytes for CF) */
        while (gp_flash_parameters->current_count < gp_flash_parameters->min_pgm_size)
        {
            gp_flash_parameters->current_count++;
            /* Copy data from source address to destination area */
            *(FCU_WORD_PTR) g_pfcu_cmd_area = *(uint16_t *) gp_flash_parameters->src_addr;

            while (R_FACI->FSTATR_b.DBFULL == (uint32_t)1)
            {
                /* Wait until DBFULL is 0 unless timeout occurs. */
                if (wait_cnt <= (uint32_t)0)
                {

                    /* if DBFULL is not set to 0 after max timeout, issue the stop command*/
                    HW_FLASH_HP_stop();
                    return SSP_ERR_TIMEOUT; ///< Need to indicate an error occured
                }
                wait_cnt--;
           }

            gp_flash_parameters->src_addr  += 2;
            gp_flash_parameters->dest_addr += 2;
            gp_flash_parameters->total_count--;
        }

        /* Reset line count */
        gp_flash_parameters->current_count = (uint32_t)0;

        /* Issue write end command */
        *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_FINAL;

        /* if in BGO mode, exit here; remaining processing if any will be done in ISR */
        if (gp_flash_parameters->current_operation == FLASH_OPERATION_DF_BGO_WRITE)
        {
            return err;
        }

        /* Read FRDY bit until it has been set to 1 indicating that the current
         * operation is complete.*/
        while (1UL != R_FACI->FSTATR_b.FRDY)
        {
            /* Wait until FRDY is 1 unless timeout occurs. */
            if (gp_flash_parameters->wait_cnt <= (uint32_t)0)
            {
                /* if FRDY is not set to 1 after max timeout, issue the stop command */
                HW_FLASH_HP_stop();
                return SSP_ERR_TIMEOUT; ///< Need to indicate an error occured
            }
            gp_flash_parameters->wait_cnt--;
        }

        /* Check if there were any errors */
        if (0UL != R_FACI->FASTAT_b.CMDLK)
        {
            /* Check if status is indicating a Programming error */
            if ((0x0002 & R_FACI->FPESTAT)
                && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
            {
                err = SSP_ERR_WRITE_FAILED;
            }
            /* Check if status is indicating a Erase error */
            else if ((0x0012 & R_FACI->FPESTAT)
                     && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
            {
                err = SSP_ERR_ERASE_FAILED;
            }
            else
            {
                err = SSP_ERR_CMD_LOCKED;
            }

            return err;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   This function checks if the specified Data Flash address range is blank.
 * @param[in]   src_start_address  The starting address of the Flash area to blank check.
 * @param[in]   num_bytes          Specifies the number of bytes that need to be checked.
 * @param[out]  result             Pointer that will be populated by the API with the results of the blank check
 *                                 operation in non-BGO (blocking) mode. In this case the blank check operation
 *                                 completes here and the result is returned. In Data Flash BGO mode the blank check
 *                                 operation is only started here and the result obtained later when the
 *                                 supplied callback routine is called.
 * @retval SSP_SUCCESS             Blankcheck operation completed with result in result,
 *                                 or blankcheck started and in-progess (BGO mode).
 * @retval SSP_ERR_ERASE_FAILED     Status is indicating an erase error. Possibly from a prior operation.
 * @retval SSP_ERR_CMD_LOCKED   FCU is in locked state, typically as a result of having received an illegal
 * command.
 * @retval SSP_ERR_WRITE_FAILED  Status is indicating a Programming error for the requested operation.
 * @retval SSP_ERR_TIMEOUT Timed out waiting for the FCU to become ready.
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_blankcheck (uint32_t start_address, uint32_t num_bytes, flash_result_t * result)
{
    ssp_err_t err = SSP_SUCCESS;

    /*Set the mode to incremental*/
    R_FACI->FBCCNT = 0x00UL;

    /*Set the start address for blank checking*/
    R_FACI->FSADDR = start_address;

    /*Set the end address for blank checking*/
    R_FACI->FEADDR = (start_address + num_bytes) - 1UL;

    /*Issue two part Blank Check command*/
    *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_BLANK_CHECK;
    *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_FINAL;

    /* if in DF BGO mode, exit here; remaining processing if any will be done in ISR */
    if (gp_flash_parameters->bgo_enabled_df == true)
    {
        *result = FLASH_RESULT_BGO_ACTIVE;
        return err;
    }

    /* Read FRDY bit until it has been set to 1 indicating that the current
     * operation is complete. */
    while (1UL != R_FACI->FSTATR_b.FRDY)
    {
        /* Wait until FRDY is 1 unless timeout occurs. */
        if (gp_flash_parameters->wait_cnt <= (uint32_t)0)
        {
            /* if FRDY is not set to 1 after max timeout, issue the stop command */
            HW_FLASH_HP_stop();
            return SSP_ERR_TIMEOUT; ///< Need to indicate an error occured
        }
        gp_flash_parameters->wait_cnt--;
    }

    /* Check if there were any errors */
    if (0UL != R_FACI->FASTAT_b.CMDLK)
    {
        /* Check if status is indicating a Programming error */
        if ((0x0002 & R_FACI->FPESTAT)
            && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
        {
            err = SSP_ERR_WRITE_FAILED;
        }
        /* Check if status is indicating a Erase error */
        else if ((0x0012 & R_FACI->FPESTAT)
                 && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
        {
            err = SSP_ERR_ERASE_FAILED;
        }
        else
        {
            err = SSP_ERR_CMD_LOCKED;
        }

        return err;
    }

    if (R_FACI->FBCSTAT == 0x01UL)
    {
        *result = FLASH_RESULT_NOT_BLANK;
    }
    else
    {
        *result = FLASH_RESULT_BLANK;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   This function switches the peripheral to P/E mode for Code Flash or Data Flash.
 * @param[in]   flash_type_t : Specifies Code or Data Flash.
 *                             Valid selections are: FLASH_TYPE_CODE_FLASH or FLASH_TYPE_DATA_FLASH
 * @retval SSP_SUCCESS                 Successfully entered P/E mode.
 * @retval SSP_ERR_PE_FAILURE    Failed to enter P/E mode
 * @retval SSP_ERR_INVALID_ARGUMENT    Supplied flash_type was something other than FLASH_TYPE_CODE_FLASH or
 *                                     FLASH_TYPE_DATA_FLASH.
 *
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_pe_mode_enter (flash_type_t flash_type)
{
    ssp_err_t err = SSP_SUCCESS;

    if (flash_type == FLASH_TYPE_DATA_FLASH)
    {
        R_FACI->FENTRYR = 0xAA80; //Transition to DF P/E mode
        if (R_FACI->FENTRYR == 0x0080UL)
        {
            err = SSP_SUCCESS;
        }
        else
        {
            err = SSP_ERR_PE_FAILURE;
        }
    }
#if (FLASH_CFG_PARAM_CODE_FLASH_PROGRAMMING_ENABLE == 1)
    else if (flash_type == FLASH_TYPE_CODE_FLASH)
    {
        R_FACI->FENTRYR = 0xAA01; //Transition to CF P/E mode
        if (R_FACI->FENTRYR == 0x0001UL)
        {
            err = SSP_SUCCESS;
        }
        else
        {
            err = SSP_ERR_PE_FAILURE;
        }
    }
#endif /* if (FLASH_CFG_PARAM_CODE_FLASH_PROGRAMMING_ENABLE == 1) */
    else
    {
        err = SSP_ERR_INVALID_ARGUMENT;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   This function switches the peripheral from P/E mode for Code Flash or Data Flash to Read mode.
 * @param[in]     none
 * @param[out]    none
 * @retval SSP_SUCCESS                 Successfully entered P/E mode.
 * @retval SSP_ERR_PE_FAILURE    Failed to exited P/E mode
 * @retval SSP_FLASH_ERR_PARAMETERS    Supplied flash_type was something other than FLASH_TYPE_CODE_FLASH or
 *                                     FLASH_TYPE_DATA_FLASH.
 *
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_pe_mode_exit (void)
{
    ssp_err_t         err      = SSP_SUCCESS;
    ssp_err_t         temp_err = SSP_SUCCESS;

    /* Timeout counter. */
    volatile uint32_t wait_cnt = FLASH_FRDY_CMD_TIMEOUT;

    /* Read FRDY bit until it has been set to 1 indicating that the current
     * operation is complete.*/
    while (1 != R_FACI->FSTATR_b.FRDY)
    {
        /* Wait until FRDY is 1 unless timeout occurs. */
        if (wait_cnt <= (uint32_t)0)
        {
            /* if FRDY is not set to 1 after max timeout, return timeout status*/
            return SSP_ERR_TIMEOUT;
        }
        wait_cnt--;
    }

    /*Check if Command Lock bit is set*/
    if (0 != R_FACI->FASTAT_b.CMDLK)
    {
        /*Issue a Status Clear to clear Command Locked state*/
        *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_STATUS_CLEAR;
        temp_err         = SSP_ERR_CMD_LOCKED;
    }

    /*Transition to Read mode*/
    R_FACI->FENTRYR = 0xAA00;

    if (R_FACI->FENTRYR == 0x0000UL)
    {
        err = SSP_SUCCESS;
    }
    else
    {
        err = SSP_ERR_PE_FAILURE;
    }

    /*If a command locked state was detected earlier, then return that error*/
    if (SSP_ERR_CMD_LOCKED == temp_err)
    {
        return temp_err;
    }
    else
    {
        return err;
    }
}

/*******************************************************************************************************************//**
 * @brief   This function resets the Flash peripheral.
 * @param[in]     none
 * @param[out]    none
 * @retval SSP_SUCCESS                 Successful reset.
 * @retval SSP_ERR_TIMEOUT       Timeout executing flash_stop.Failed to exited P/E mode
 *
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_reset (void)
{
    ssp_err_t res = SSP_SUCCESS;

    /*Issue a Flash Stop to stop any ongoing operation*/
    HW_FLASH_HP_stop();
    /*Read and clear any DF or CF access violation errors*/
    if (R_FACI->FASTAT_b.CFAE == 1UL)
    {
        R_FACI->FASTAT_b.CFAE = (uint8_t)0;
    }

    if (R_FACI->FASTAT_b.DFAE == 1UL)
    {
        R_FACI->FASTAT_b.DFAE = (uint8_t)0;
    }

    /*Issue a Flash Stop again for a complete RESET*/
    res = HW_FLASH_HP_stop();
    /*Transition to Read mode*/
    HW_FLASH_HP_pe_mode_exit();
    return res;
}

/*******************************************************************************************************************//**
 * @brief   This function issues the STOP command and checks the state of the Command Lock bit..
 * @param[in]     none
 * @param[out]    none
 * @retval SSP_SUCCESS                 Successful stop.
 * @retval SSP_ERR_TIMEOUT       Timeout executing flash_stop.Failed to exited P/E mode
 * @retval SSP_ERR_CMD_LOCKED    Peripheral in command locked state
 *
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_stop (void)
{
    /* Timeout counter. */
    volatile uint32_t wait_cnt = FLASH_FRDY_CMD_TIMEOUT;

    /*Issue stop command to flash command area*/
    *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_FORCED_STOP;

    /* Read FRDY bit until it has been set to 1 indicating that the current
     * operation is complete.*/
    while (1 != R_FACI->FSTATR_b.FRDY)
    {
        /* Wait until FRDY is 1 unless timeout occurs. */
        if (wait_cnt <= (uint32_t)0)
        {
            /* This should not happen normally.
             * FRDY should get set in 15-20 ICLK cycles on STOP command*/
            return SSP_ERR_TIMEOUT;
        }
        wait_cnt--;
   }

    /*Check that Command Lock bit is cleared*/
    if (0 != R_FACI->FASTAT_b.CMDLK)
    {
        return SSP_ERR_CMD_LOCKED;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure an access window for the Code Flash memory using the provided start and end address. An access
 *         window defines a contiguous area in Code Flash for which programming/erase is enabled. This area is
 *         on block boundaries.
 *         The block containing start_addr is the first block. The block containing end_addr is the last block.
 *         The access window then becomes first block - last block inclusive. Anything outside this range
 *         of Code Flash is then write protected.
 *         This command DOES modify the configuration via The Configuration Set command to update the FAWS and FAWE.
 *
 * @param[in]  start_addr Determines the Starting block for the Code Flash access window.
 * @param[in]  end_addr   Determines the Ending block for the Code Flash access window.
 *
 * @retval SSP_SUCCESS                Access window successfully configured.
 * @retval SSP_ERR_ERASE_FAILED        Status is indicating an erase error. Possibly from a prior operation.
 * @retval SSP_ERR_CMD_LOCKED   FCU is in locked state, typically as a result of having received an illegal
 * command.
 * @retval SSP_ERR_WRITE_FAILED  Status is indicating a Programming error for the requested operation.
 * @retval SSP_ERR_TIMEOUT Timed out waiting for the FCU to become ready.
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_access_window_set (uint32_t const start_addr, uint32_t const end_addr)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Prepare the configuration words */
    configurationSetArea[FCU_CONFIG_SET_FAWE_BTFLG_OFFSET] = (uint16_t)0;
    configurationSetArea[FCU_CONFIG_SET_FSPR_OFFSET]       = (uint16_t)0;

    /* Prepare the configuration words */
    configurationSetArea[FCU_CONFIG_SET_FAWS_OFFSET]       = (uint16_t)(start_addr >> 13);  // consists of the address bits from 13
                                                                                // to 23 ([23:13]).
    configurationSetArea[FCU_CONFIG_SET_FAWE_BTFLG_OFFSET] = (uint16_t)(end_addr  >> 13);   // consists of the address bits from 13
                                                                                // to 23 ([23:13]).

    /* Preserve the BTFLG bit */
    configurationSetArea[FCU_CONFIG_SET_FAWE_BTFLG_OFFSET] |= (uint16_t)((R_FACI->FAWMON_b.BTFLG << 15));

    err                                                     = HW_FLASH_HP_configurationSet(); // Execute the
                                                                                              // Configuration set
    // sequence

    return err;
}

/*******************************************************************************************************************//**
 * @brief  Remove any access window that is currently configured in the Code Flash.
 *         This command DOES modify the configuration via The Configuration Set command to update the FAWS and FAWE.
 *         Subsequent to this call all Code Flash is writable.
 *
 * @retval SSP_SUCCESS              Access window successfully removed.
 * @retval SSP_ERR_ERASE_FAILED      Status is indicating an erase error. Possibly from a prior operation.
 * @retval SSP_ERR_CMD_LOCKED   FCU is in locked state, typically as a result of having received an illegal
 * command.
 * @retval SSP_ERR_WRITE_FAILED  Status is indicating a Programming error for the requested operation.
 * @retval SSP_ERR_TIMEOUT      Timed out waiting for the FCU to become ready.
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_access_window_clear (void)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Prepare the configuration words */
    configurationSetArea[FCU_CONFIG_SET_FAWE_BTFLG_OFFSET] = (uint16_t)0;
    configurationSetArea[FCU_CONFIG_SET_FSPR_OFFSET]       = (uint16_t)0;
    configurationSetArea[FCU_CONFIG_SET_FAWS_OFFSET]       = (uint16_t)0;

    /* Preserve the BTFLG bit */
    configurationSetArea[FCU_CONFIG_SET_FAWE_BTFLG_OFFSET] |= (uint16_t)((R_FACI->FAWMON_b.BTFLG << 15));

    err                                                     = HW_FLASH_HP_configurationSet(); // Execute the
                                                                                              // Configuration set
    // sequence

    return err;
}

/*******************************************************************************************************************//**
 * @brief  Execute the Set Configuration sequence using the configurationSetArea structure set up the caller.
 *
 * @retval SSP_SUCCESS                Set Configuration successful
 * @retval SSP_ERR_ERASE_FAILED        Status is indicating an erase error. Possibly from a prior operation.
 * @retval SSP_ERR_CMD_LOCKED   FCU is in locked state, typically as a result of having received an illegal
 * command.
 * @retval SSP_ERR_WRITE_FAILED  Status is indicating a Programming error for the requested operation.
 * @retval SSP_ERR_TIMEOUT Timed out waiting for the FCU to become ready.
 *
 **********************************************************************************************************************/
static ssp_err_t HW_FLASH_HP_configurationSet (void)
{
    ssp_err_t err   = SSP_SUCCESS;
    uint8_t   index = 0;

    /** Prepare our worst case timeout */
    gp_flash_parameters->wait_cnt    = gp_flash_parameters->wait_max_configuration_set;

    R_FACI->FSADDR   = FCU_CONFIG_SET_ACCESS_STARTUP;
    *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_CONFIG_SET_1;
    *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_CONFIG_SET_2;

    for (index = (uint8_t)0; index < (uint8_t)CONFIG_SET_ACCESS_WORD_CNT; index++)
    {
        /*  There are 8 16 bit words that must be written to accomplish a configuration set */
        *(FCU_WORD_PTR) g_pfcu_cmd_area = configurationSetArea[index];
    }

    /* Issue end command */
    *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_FINAL;

    /* Read FRDY bit until it has been set to 1 indicating that the current
     * operation is complete.*/
    while (1 != R_FACI->FSTATR_b.FRDY)
    {
        /* Wait until FRDY is 1 unless timeout occurs. */
        if (gp_flash_parameters->wait_cnt <= (uint32_t)0)
        {
            /* if FRDY is not set to 1 after max timeout, issue the stop command */
            HW_FLASH_HP_stop();
            return SSP_ERR_TIMEOUT; ///< Need to indicate an error occured
        }
        gp_flash_parameters->wait_cnt--;
    }

    /* Check if there were any errors */
    if (0 != R_FACI->FASTAT_b.CMDLK)
    {
        /* Check if status is indicating a Programming error */
        if ((0x0002 & R_FACI->FPESTAT)
            && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
        {
            err = SSP_ERR_WRITE_FAILED;
        }
        /* Check if status is indicating a Erase error */
        else if ((0x0012 & R_FACI->FPESTAT)
                 && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
        {
            err = SSP_ERR_ERASE_FAILED;
        }
        else
        {
            err = SSP_ERR_CMD_LOCKED;
        }

    }
    return err;
}

/*******************************************************************************************************************//**
 * @brief Temporarily sets the startup area to use the DEFAULT or ALTERNATE area as requested.
 * On the next subsequent reset, the startup area will be determined by the state of the BTFLG.
 * This command does NOT modify the configuration via The Configuration Set command, hence these changes are
 * only in effect until the next reset.
 *
 * @retval SSP_SUCCESS                StartUp area temporarily modified.
 * @note FSUACR register must be set using the full word. Setting the KEY bitfield and SAS bitfields separately will
 * not work.
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_set_startup_area_temporary (flash_startup_area_swap_t swap_type)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Set the SAS bit as requested */
    if (FLASH_STARTUP_AREA_BLOCK0 == swap_type)
    {
        R_FACI->FSUACR = SAS_DEFAULT_AREA;
    }
    else if (FLASH_STARTUP_AREA_BLOCK1 == swap_type)
    {
        R_FACI->FSUACR = SAS_ALTERNATE_AREA;
    }
    else
    {
        R_FACI->FSUACR = SAS_PER_BTFLG_AREA;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief Modifies the start-up program swap flag (BTFLG) based on the supplied parameters. These changes will
 *        take effect on the next reset. This command DOES modify the configuration via The Configuration
 *        Set command to update the BTFLG.
 *
 * @retval SSP_SUCCESS                Access window successfully removed.
 * @retval SSP_ERR_ERASE_FAILED        Status is indicating an erase error. Possibly from a prior operation.
 * @retval SSP_ERR_CMD_LOCKED   FCU is in locked state, typically as a result of having received an illegal
 * command.
 * @retval SSP_ERR_WRITE_FAILED  Status is indicating a Programming error for the requested operation.
 * @retval SSP_ERR_TIMEOUT Timed out waiting for the FCU to become ready.
 *
 **********************************************************************************************************************/
ssp_err_t HW_FLASH_HP_set_startup_area_boot (flash_startup_area_swap_t swap_type)
{
    ssp_err_t err   = SSP_SUCCESS;
    uint16_t  btflg = (uint16_t) ((uint16_t)swap_type << 15);

    configurationSetArea[FCU_CONFIG_SET_FAWE_BTFLG_OFFSET] = (uint16_t)0;

    /* Prepare the configuration words. Preserve the current FAWE and FAWS */
    configurationSetArea[FCU_CONFIG_SET_FAWS_OFFSET] = R_FACI->FAWMON_b.FAWS;

    /* Set the BTFLG bit to 1 or 0 accordingly */
    configurationSetArea[FCU_CONFIG_SET_FAWE_BTFLG_OFFSET] = (uint16_t)(R_FACI->FAWMON_b.FAWE | btflg);

    err                                                    = HW_FLASH_HP_configurationSet(); // Execute the
                                                                                             // Configuration set
    // sequence

    return err;
}

/** If BGO is being used then we require both interrupts to be enabled (RDY and ERR). If either one
 *  is not enabled then don't generate any ISR routines */
 #if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_DISABLED) && (BSP_IRQ_CFG_FCU_FIFERR != BSP_IRQ_DISABLED)
/*******************************************************************************************************************//**
 * @brief  FLASH error interrupt routine.
 *
 * @details This function implements the FLASH error isr. The function clears the interrupt request source on entry
 *          populates the callback structure with the FLASH_IRQ_EVENT_ERR_ECC event, and providing
 *          a callback routine has been provided, calls the callback function with the event. <br>
 *
 * @param[in]     none
 * @param[out]    none
 * @retval        none
 **********************************************************************************************************************/
void fcu_fiferr_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE
    flash_callback_args_t cb_data;

    /*Check if Command Lock bit is set*/
    if (1 == R_FACI->FASTAT_b.CMDLK)
    {
        if (1 == R_FACI->FASTAT_b.DFAE)
        {
            cb_data.event = FLASH_EVENT_ERR_DF_ACCESS;
        }
        else if (1 == R_FACI->FASTAT_b.CFAE)
        {
            cb_data.event = FLASH_EVENT_ERR_CF_ACCESS;
        }

        if ((0x0002 & R_FACI->FPESTAT)
            && ((R_FACI->FSTATR_b.PRGERR) || (R_FACI->FSTATR_b.ERSERR)))
        {
            cb_data.event = FLASH_EVENT_ERR_FAILURE;
        }
        else
        {
            cb_data.event = FLASH_EVENT_ERR_CMD_LOCKED;
        }
    }

    /*Reset the FCU: This will stop any existing processes and exit PE mode*/
    HW_FLASH_HP_reset();
    flash_ReleaseState();

    /** Insure that if we do get a Rdy interrupt subsequent to this error interrupt, that
     *  we don't try to proceed with the operation in progress */
    gp_flash_parameters->current_operation = FLASH_OPERATION_FCU_INIT;

    // Clear any pending Rdy interrupt
    NVIC_ClearPendingIRQ(FCU_FRDYI_IRQn);

    /** Clear the Interrupt Request*/
    R_BSP_IrqStatusClear(FCU_FRDYI_IRQn);

    // Clear any pending Err interrupt
    NVIC_ClearPendingIRQ(FCU_FIFERR_IRQn);

    /** Clear the Interrupt Request*/
    R_BSP_IrqStatusClear(FCU_FIFERR_IRQn);

    if (NULL != flash_error_isr_handler)
    {
        flash_error_isr_handler(&cb_data);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}    /* End of function fcu_fiferr_isr() */


/*******************************************************************************************************************//**
 * @brief  FLASH ready interrupt routine.
 *
 * @details This function implements the FLASH ready isr. The function clears the interrupt request source on entry
 *          populates the callback structure with the relevant event, and providing
 *          a callback routine has been provided, calls the callback function with the event. <br>
 *
 * @param[in]     none
 * @param[out]    none
 * @retval        none
 **********************************************************************************************************************/
void fcu_frdyi_isr (void)
{
    SF_CONTEXT_SAVE
    /*Wait counter used for DBFULL flag*/
    uint32_t             wait_cnt = FLASH_DBFULL_TIMEOUT;
    flash_block_info_t   block_info;
    flash_callback_args_t cb_data;

    /** Clear the Interrupt Request*/
    R_BSP_IrqStatusClear(FCU_FRDYI_IRQn);

    if (FLASH_OPERATION_DF_BGO_WRITE == gp_flash_parameters->current_operation)
    {
        /*If there are still bytes to write*/
        if (gp_flash_parameters->total_count > (uint32_t)0)
        {
            /*Set block start address*/
            R_FACI->FSADDR   = gp_flash_parameters->dest_addr;
            /*Issue two part Write commands*/
            *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_PROGRAM;
            *g_pfcu_cmd_area = (uint8_t) gp_flash_parameters->min_pgm_size;
            /* Write one line (256 for CF, 4 for DF) */
            while (gp_flash_parameters->current_count < gp_flash_parameters->min_pgm_size)
            {
                gp_flash_parameters->current_count++;
                /* Copy data from source address to destination area */
                *(FCU_WORD_PTR) g_pfcu_cmd_area =   *(uint16_t *) gp_flash_parameters->src_addr;
                while (R_FACI->FSTATR_b.DBFULL == 1UL)
                {
                    /* Wait until DBFULL is 0 unless timeout occurs. */
                    if (wait_cnt <= (uint32_t)0)
                    {
                        /* if DBFULL is not set to 0 after max timeout, reset thr FCU*/
                        HW_FLASH_HP_reset();
                        cb_data.event = FLASH_EVENT_ERR_FAILURE;
                        SF_CONTEXT_RESTORE
                        return;
                    }
                    wait_cnt--;
               }

                gp_flash_parameters->src_addr  += 2;
                gp_flash_parameters->dest_addr += 2;
                gp_flash_parameters->total_count--;
            }

            /*Reset line count*/
            gp_flash_parameters->current_count = (uint32_t)0;
            /*Issue write end command*/
            *g_pfcu_cmd_area                   = (uint8_t) FLASH_FACI_CMD_FINAL;
            /*Exit ISR until next FRDY interrupt to continue operation*/
           SF_CONTEXT_RESTORE
           return;
        }
        /*Done writing all bytes*/
        else
        {
            cb_data.event = FLASH_EVENT_WRITE_COMPLETE;
        }
    }
    else if ((FLASH_OPERATION_DF_BGO_ERASE == gp_flash_parameters->current_operation))
    {
        gp_flash_parameters->current_count++;
        if (gp_flash_parameters->current_count
            < gp_flash_parameters->total_count)
        {
            /*If current mode is Data Flash PE, increment to the next block starting address
             * by adding 64 (DF Block Size) else decrement to the next flash block start address*/
            if (R_FACI->FENTRYR == 0x0080UL)
            {
                gp_flash_parameters->dest_addr += FLASH_DATA_BLOCK_SZ;
            }
            else if (R_FACI->FENTRYR == 0x0001UL)
            {
                /* Get the block information about this address */
                if ((flash_get_block_info(gp_flash_parameters->dest_addr, &block_info)) != true)
                {
                    /* Should not get a failure for getting address info...*/
                    HW_FLASH_HP_reset();
                    cb_data.event = FLASH_EVENT_ERR_FAILURE;
                    SF_CONTEXT_RESTORE
                    return;
                }

                gp_flash_parameters->dest_addr += block_info.block_size;
            }

            /*Set block start address*/
            R_FACI->FSADDR   = gp_flash_parameters->dest_addr;
            /*Issue two part Block Erase commands*/
            *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_BLOCK_ERASE;
            *g_pfcu_cmd_area = (uint8_t) FLASH_FACI_CMD_FINAL;
            /*Exit ISR until next FRDY interrupt to continue operation*/
            SF_CONTEXT_RESTORE
            return;
        }
        /*If all blocks are erased*/
        else
        {
            cb_data.event = FLASH_EVENT_ERASE_COMPLETE;
        }
    }
    else if (FLASH_OPERATION_DF_BGO_BLANKCHECK == gp_flash_parameters->current_operation)
    {
        if (R_FACI->FBCSTAT == 0x01UL)
        {
            cb_data.event = FLASH_EVENT_NOT_BLANK;
        }
        else
        {
            cb_data.event = FLASH_EVENT_BLANK;
        }
    }
    else
    {
        /** This case could only be reached by a memory corruption of some type. */
        cb_data.event = FLASH_EVENT_ERR_FAILURE;
    }

    /*finished current operation. Exit P/E mode*/
    HW_FLASH_HP_pe_mode_exit();
    /*Release lock and Set current state to Idle*/
    flash_ReleaseState();

    if (NULL != flash_ready_isr_handler)
    {
        /** Set data to identify callback to user, then call user callback. */
        flash_ready_isr_handler(&cb_data);
    }

    SF_CONTEXT_RESTORE
}  /* End of function fcu_frdyi_isr() */
#endif
#endif /* if defined(BSP_MCU_GROUP_S7G2) */

/*******************************************************************************************************************//**
 * @} (end defgroup FLASH_RV40F)
 **********************************************************************************************************************/
