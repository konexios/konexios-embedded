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
* File Name    : r_riic.c
* Description  : I2C Master mode driver implementation on the RIIC peripheral.
***********************************************************************************************************************/


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_riic.h"
#include "r_riic_private.h"
#include "r_riic_private_api.h"
#include "r_cgc.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Macro for error logger. */
#ifndef RIIC_ERROR_RETURN
#define RIIC_ERROR_RETURN(a, err)  SSP_ERROR_RETURN((a), (err), "riic", &g_riic_master_version)
#endif

#define DEBUG 0

#if DEBUG
char msg [0x80] ;
#define PRNF(x, ...) { sprintf (msg, x, ##__VA_ARGS__) ; puts (msg) ; }

char *sequence[1000];
uint16_t err_sequence[100];

char g_open_str[] = "OPEN";
char g_event_ISR_str[] = "Event_ISR";
char g_error_ISR_str[] = "Error_ISR";
char g_txend_ISR_str[] = "TXEND_ISR";
char g_txdataempty_ISR_str[] = "TXEmpty_ISR";
char g_rxdatafull_ISR_str[] = "RXFULL_ISR";
char g_error_str[] = "error";
char g_remains_str[] = "remains";
char g_continuationtx_str[] = "continuationtx";
char g_newtx_str[] = "new_tx";
char g_close_str[] = "close";
char g_fullcfgcomplete_str[] = "full cfg complete";
char g_clkrecfg_str[] = "clk reconfig";
char g_fullcfg_str[] = "full cfg";
char g_abort_str[] = "abort";
char g_notify_str[] = "notify";
char g_RESTART_str[] = "READ RESTART";
char g_WRITE_NEW_str[] = "WRITE NEW";
char g_WRITE_RESTART_str[] = "WRITE RESTART";
char g_NEW_str[] = "READ NEW";
char g_IN_USE_str[] = "IN USE";
char g_READ_str[] = "READ";
char g_WRITE_str[] = "WRITE";
char g_RESET_str[] = "RESET";
char g_TXEND_ISR_more_address_or_data_left_force_ISR_str[] = "TXEND_ISR_more_address_or_data_left_force_ISR_str";
char g_TXEND_ISR_send_stop_str[] = "TXEND_ISR_send_stop_str";
char g_TXEND_ISR_send_restart_str[] = "TXEND_ISR_send_restart_str";
char g_TXEND_ISR_no_data_bytes_left_to_send_str[] = "TXEND_ISR_no_data_bytes_left_to_send_str";
char g_TXEND_ISR_no_errors_str[] = "TXEND_ISR_no_errors_str";
char g_TXEND_ISR_data_bytes_left_to_send_str[] = "TXEND_ISR_data_bytes_left_to_send_str";
char g_TXEND_ISR_no_address_bytes_left_to_send_str[] = "TXEND_ISR_no_address_bytes_left_to_send_str";
char g_TXEND_ISR_address_bytes_yet_to_be_sent_str[] = "TXEND_ISR_address_bytes_yet_to_be_sent_str";
char g_TXEmpty_ISR_bytes_loaded_for_transfer_str[] = "TXEmpty_ISR_bytes_loaded_for_transfer_str";
char g_TXEmpty_ISR_load_byte_for_transfer_str[] = "TXEmpty_ISR_load_byte_for_transfer_str";
char g_TXEmpty_ISR_issue_10_bit_slave_address_str[] = "TXEmpty_ISR_issue_10_bit_slave_address_str";
char g_TXEmpty_ISR_issue_7_bit_slave_address_str[] = "TXEmpty_ISR_issue_7_bit_slave_address_str";
char g_TXEmpty_ISR_issue_slave_address_str[] = "TXEmpty_ISR_issue_slave_address_str";
char g_RXFULL_ISR_first_byte_dummy_read_str[] = "RXFULL_ISR_first_byte_dummy_read_str";
char g_RXFULL_ISR_last_byte_set_ACK_str[] = "RXFULL_ISR_last_byte_set_ACK_str";
char g_RXFULL_ISR_send_stop_str[] = "RXFULL_ISR_send_stop_str";
char g_RXFULL_ISR_send_restart_str[] = "RXFULL_ISR_send_restart_str";
char g_RXFULL_ISR_no_bytes_left_str[] = "RXFULL_ISR_no_bytes_left_str";
char g_RXFULL_ISR_no_errors_str[] = "RXFULL_ISR_no_errors_str";
char g_RXFULL_ISR_read_to_buff_str[] = "RXFULL_ISR_read_to_buff_str";
char g_RXFULL_ISR_2_byte_left_set_wait_str[] = "RXFULL_ISR_2_byte_left_set_wait_str";
char g_RXFULL_ISR_first[] = "RXFULL_ISR_first";
char g_error_ISR_abort_str[] = "g_error_ISR_abort_str";
char g_error_ISR_abort_timeout_str[] = "error_ISR_abort_timeout_str";

char **p_seq = sequence;
uint16_t *p_errseq = err_sequence;
#endif  //DEBUG


#define I2C_CODE_READ   (0x01)
#define I2C_CODE_10BIT  (0xF0)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** Data transfer record */
typedef struct st_riic_transfer
{
    i2c_ctrl_t      * p_ctrl;                /**< Holds the channel and callback information */
    uint8_t         * p_buff;                /**< Holds the data associated with the transfer */
    volatile uint32_t total;                 /**< Holds the total number of data bytes to transfer */
    volatile uint32_t remain;                /**< Tracks the remaining data bytes to transfer */
    volatile uint32_t loaded;                /**< Tracks the number of data bytes written to the register */

    uint8_t           addr_low;              /**< Holds the last address byte to issue */
    uint8_t           addr_high;             /**< Holds the first address byte to issue in 10-bit mode  */
    volatile uint8_t  addr_total;            /**< Holds the total number of address bytes to transfer */
    volatile uint8_t  addr_remain;           /**< Tracks the remaining address bytes to transfer */
    volatile uint8_t  addr_loaded;           /**< Tracks the number of address bytes written to the register */

    volatile bool     read;                  /**< Holds the direction of the data byte transfer */
    volatile bool     restart;               /**< Holds whether or not the restart should be issued when done */
    volatile bool     err;                   /**< Tracks if an error occurred during processing */
    volatile bool     restarted;             /**< Tracks if a restart was issued during the previous transfer */
    volatile bool     transaction_completed; /**< Tracks if the transaction started earlier was completed  */
} riic_transfer_t;
 
/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/
/** Internal helper functions */
__STATIC_INLINE void riic_notify                (riic_transfer_t  * const p_transfer,
                                                 i2c_event_t        const event)                 INLINE_ATTRIBUTE;
__STATIC_INLINE void riic_clock_settings        (uint32_t         * const p_rate,
                                                 uint8_t          * const p_brh,
                                                 uint8_t          * const p_brl,
                                                 uint8_t          * const p_cks)                 INLINE_ATTRIBUTE;
__STATIC_INLINE ssp_err_t riic_abort_seq_master (i2c_ctrl_t const * const p_ctrl,
                                                 bool       const         all)                   INLINE_ATTRIBUTE;

/** Functions that manipulate hardware */
__STATIC_INLINE ssp_err_t riic_open_hw_master  (i2c_ctrl_t       * const p_ctrl)                 INLINE_ATTRIBUTE;
__STATIC_INLINE void      riic_close_hw_master (i2c_ctrl_t       * const p_ctrl)                 INLINE_ATTRIBUTE;
__STATIC_INLINE void      riic_cfg_ints_master (i2c_ctrl_t       * const p_ctrl)                 INLINE_ATTRIBUTE;
__STATIC_INLINE void      riic_abort_hw_master (i2c_ctrl_t const * const p_ctrl)                 INLINE_ATTRIBUTE;
static          ssp_err_t riic_run_hw_master   (riic_transfer_t  * const p_transfer);

/** Interrupt handlers */
static void riic_rxi_master (uint32_t const channel);
static void riic_txi_master (uint32_t const channel);
static void riic_tei_master (uint32_t const channel);
static void riic_err_master (uint32_t const channel,
                             uint8_t  const errors);

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/
/** Open device count per channel */
static uint32_t g_ch_opened[RIIC_MAX_CH];

/** Lowest stored speed per channel */
static uint32_t g_ch_speeds[RIIC_MAX_CH];

/** In-progress transfer per channel */
static riic_transfer_t g_in_progress[RIIC_MAX_CH];

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static ssp_version_t const g_riic_master_version =
{
    .api_version_minor  = I2C_MASTER_API_VERSION_MINOR,
    .api_version_major  = I2C_MASTER_API_VERSION_MAJOR,
    .code_version_major = RIIC_MASTER_CODE_VERSION_MAJOR,
    .code_version_minor = RIIC_MASTER_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif


/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/

/** RIIC Implementation of I2C device master interface */
i2c_api_master_t const g_i2c_master_on_riic =
{
    .open       = R_RIIC_MasterOpen,
    .close      = R_RIIC_MasterClose,
    .read       = R_RIIC_MasterRead,
    .write      = R_RIIC_MasterWrite,
    .reset      = R_RIIC_MasterReset,
    .versionGet = R_RIIC_MasterVersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup RIIC
 * @{
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   Gets version information and stores it in the provided version struct.
 *
 * @retval  SSP_SUCCESS                 Successful version get.
 * @retval  SSP_ERR_INVALID_POINTER     p_version is NULL.
***********************************************************************************************************************/
ssp_err_t R_RIIC_MasterVersionGet   (ssp_version_t          * const p_version)
{
#if RIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_version != NULL);
#endif

    p_version->version_id = g_riic_master_version.version_id;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief   Opens the I2C device. May power on IIC peripheral and perform initialization described in hardware manual.
 *
 *  This function will reconfigure the clock settings of the peripheral when a device with a lower rate than
 *  previously configured is opened.
 *
 * @retval  SSP_SUCCESS           Requested clock rate was set exactly.
 *                                Opened identical configuration of already open instance.
 * @retval  SSP_ERR_IN_USE        Attempted to open an already open device instance.
 * @retval  SSP_ERR_INVALID_RATE  The requested rate cannot be set.
***********************************************************************************************************************/
ssp_err_t R_RIIC_MasterOpen         (i2c_ctrl_t             * const p_ctrl,
                                     i2c_cfg_t      const   * const p_cfg)
{
#if RIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
    SSP_ASSERT(p_cfg != NULL);
    SSP_ASSERT(p_cfg->channel < RIIC_MAX_CH);
    SSP_ASSERT(p_cfg->rate <= I2C_RATE_FASTPLUS);
    SSP_ASSERT(riic_eei[p_cfg->channel] != BSP_MAX_NUM_IRQn);
    SSP_ASSERT(riic_rxi[p_cfg->channel] != BSP_MAX_NUM_IRQn);
    SSP_ASSERT(riic_tei[p_cfg->channel] != BSP_MAX_NUM_IRQn);
    SSP_ASSERT(riic_txi[p_cfg->channel] != BSP_MAX_NUM_IRQn);

    /** p_extend not supported currently*/
    SSP_ASSERT(p_cfg->p_extend == NULL);
#endif

    ssp_err_t err;

    /* Get the transfer record associated with the channel */
    riic_transfer_t * p_transfer;

    /* Check if this device was already opened */
    if (p_ctrl->open)
    {
        err = SSP_ERR_IN_USE;
    }
    else
    {
        /* Record the configuration on the device for use later */
        p_ctrl->info = *p_cfg;
#if (DEBUG)
        *p_seq++ = g_open_str;
#endif
        /* Open the hardware in master mode */
        err = riic_open_hw_master(p_ctrl);

        /* Now handle any fatal errors that may have occurred */
        if (SSP_SUCCESS == err)
        {
            /* Finally, we can consider the device opened */
            p_ctrl->open = true;

            /* We need to track the number of unique devices open on the channel as well */
            g_ch_opened[p_ctrl->info.channel]++;

            p_transfer = &g_in_progress[p_ctrl->info.channel];

            p_transfer->transaction_completed = true;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   Closes the I2C device. May power down IIC peripheral.
 *
 *  This function will safely terminate any in-progress I2C transfer with the device. If a transfer is aborted, the user
 *  will be notified via callback with an abort event. Since the callback is optional, this function will also return
 *  a specific error code in this situation.
 *
 *  @note   Since the device may be opened by multiple users, the only time a transfer will be aborted is if there are
 *          no more users of the device.
 *
 * @retval  SSP_SUCCESS      Device closed without issue.
 * @retval  SSP_ERR_ABORTED  Device was closed while a transfer was in progress.
***********************************************************************************************************************/
ssp_err_t R_RIIC_MasterClose        (i2c_ctrl_t             * const p_ctrl)
{
#if RIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
#endif

    ssp_err_t err = SSP_SUCCESS;

    /* Check if the device is even open, return an error if not */
    if (!p_ctrl->open)
    {
        err = SSP_ERR_NOT_OPEN;
    }
    else
    {
        /* Abort an in-progress transfer with this device only */
        err = riic_abort_seq_master(p_ctrl, false);

        /* The device is now considered closed */
        p_ctrl->open = false;

        /* Remove this device from the record of unique devices opened on the channel */
        g_ch_opened[p_ctrl->info.channel]--;

        /* De-configure everything if there are no devices open on the channel */
        if (0 == g_ch_opened[p_ctrl->info.channel])
        {
            riic_close_hw_master(p_ctrl);
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   Performs a read from the I2C device.
 *
 *  This function will fail if there is already an in-progress I2C transfer on the associated channel. Otherwise, the
 *  I2C read operation will begin. When no callback is provided by the user, this function performs a blocking read.
 *  Otherwise, the read operation is non-blocking and the caller will be notified when the operation has finished by
 *  an SF_EVENT_RX_COMPLETE in the callback.
 *
 * @retval  SSP_SUCCESS      Function executed without issue; if no callback was provided, the process was kicked off.
 * @retval  SSP_ERR_IN_USE   Another transfer was in progress.
 * @retval  SSP_ERR_ABORTED  The transfer failed.
***********************************************************************************************************************/
ssp_err_t R_RIIC_MasterRead         (i2c_ctrl_t             * const p_ctrl,
                                     uint8_t                * const p_dest,
                                     uint32_t                 const bytes,
                                     bool                     const restart)
{
#if RIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
    SSP_ASSERT(p_dest != NULL);
    SSP_ASSERT(bytes != 0);
#endif

    ssp_err_t err = SSP_SUCCESS;

    RIIC_ERROR_RETURN(p_ctrl->open != 0, SSP_ERR_NOT_OPEN);

    /* Get the transfer record associated with the channel */
    riic_transfer_t * const p_transfer = &g_in_progress[p_ctrl->info.channel];
#if (DEBUG)
    *p_seq++ = g_READ_str;
#endif
    /* Fail if there is already a transfer in progress */
    if (false == p_transfer->transaction_completed)
    {
#if (DEBUG)
         *p_seq++ = g_IN_USE_str;
#endif
        err = SSP_ERR_IN_USE;
    }
    else
    {
        /* Record the new information about this transfer */
        p_transfer->p_ctrl  = p_ctrl;
        p_transfer->p_buff  = p_dest;
        p_transfer->total   = bytes;
        p_transfer->restart = restart;
        p_transfer->read    = true;

        /* Handle the different addressing modes */
        if (p_ctrl->info.addr_mode == I2C_ADDR_MODE_7BIT)
        {
            /* Set the address bytes according to a 7-bit slave read command */
            p_transfer->addr_high  = 0;
            p_transfer->addr_low   = (uint8_t)(p_ctrl->info.slave << 1) | I2C_CODE_READ;
            p_transfer->addr_total = 1;
        }
        else
        {
            /* Check if this is a new transaction or a continuation */
            if (!p_transfer->restarted)
            {
#if (DEBUG)
                 *p_seq++ = g_NEW_str;
#endif
                /* Set the address bytes according to a 10-bit slave read command */
                p_transfer->addr_high   = (uint8_t)((p_ctrl->info.slave >> 7) | I2C_CODE_10BIT) | I2C_CODE_READ;
                p_transfer->addr_low    = (uint8_t)p_ctrl->info.slave;
                /* Note: The address must be issued slightly differently when reading a slave with a 10-bit address */
                p_transfer->addr_total  = 3;
            }
            else
            {
#if (DEBUG)
                 *p_seq++ = g_RESTART_str;
#endif
                HW_RIIC_TimeoutEnable(p_ctrl->info.channel, true);
                /* We will only need to issue the high address byte, but store it like a 7-bit read */
                p_transfer->addr_low    = (uint8_t)((p_ctrl->info.slave >> 7) | (uint8_t)I2C_CODE_10BIT) | (uint8_t)I2C_CODE_READ;
                p_transfer->addr_total  = 1;
            }
        }

        /* Kickoff the read operation as a master */
        err = riic_run_hw_master(p_transfer);
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   Performs a write to the I2C device.
 *
 *  This function will fail if there is already an in-progress I2C transfer on the associated channel. Otherwise, the
 *  I2C write operation will begin. When no callback is provided by the user, this function performs a blocking write.
 *  Otherwise, the write operation is non-blocking and the caller will be notified when the operation has finished by
 *  an SF_EVENT_TX_COMPLETE in the callback.
 *
 * @retval  SSP_SUCCESS      Function executed without issue; if no callback was provided, the process was kicked off.
 * @retval  SSP_ERR_IN_USE   Another transfer was in progress.
 * @retval  SSP_ERR_ABORTED  The transfer failed.
***********************************************************************************************************************/
ssp_err_t R_RIIC_MasterWrite        (i2c_ctrl_t             * const p_ctrl,
                                     uint8_t                * const p_src,
                                     uint32_t                 const bytes,
                                     bool                     const restart)
{
#if RIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
    SSP_ASSERT(p_src != NULL);
#endif

    ssp_err_t err = SSP_SUCCESS;

    RIIC_ERROR_RETURN(p_ctrl->open != 0, SSP_ERR_NOT_OPEN);
#if (DEBUG)
     *p_seq++ = g_WRITE_str;
#endif
    /* Get the transfer record associated with the channel */
    riic_transfer_t * const p_transfer = &g_in_progress[p_ctrl->info.channel];

    /* Fail if there is already a transfer in progress */
    if (false == p_transfer->transaction_completed)
    {
#if (DEBUG)
     *p_seq++ = g_IN_USE_str;
#endif
        err = SSP_ERR_IN_USE;
    }
    else
    {
        /* Record the new information about this transfer */
        p_transfer->p_ctrl  = p_ctrl;
        p_transfer->p_buff  = p_src;
        p_transfer->total   = bytes;
        p_transfer->remain  = bytes;
        p_transfer->restart = restart;
        p_transfer->read    = false;

        /* Handle the different addressing modes */
        if (p_ctrl->info.addr_mode == I2C_ADDR_MODE_7BIT)
        {
            /* Set the address bytes according to a 7-bit slave write command */
            p_transfer->addr_high  = 0;
            p_transfer->addr_low   = (uint8_t)(p_ctrl->info.slave << 1) & ~I2C_CODE_READ;
            p_transfer->addr_total = 1;
        }
        else
        {
            /* Check if this is a new transaction or a continuation */
            if (!p_transfer->restarted)
            {
#if (DEBUG)
             *p_seq++ = g_WRITE_NEW_str;
#endif
                /* Set the address bytes according to a 10-bit slave write command */
                p_transfer->addr_high   = (uint8_t)((p_ctrl->info.slave >> 7) | I2C_CODE_10BIT) & (uint8_t)~I2C_CODE_READ;
                p_transfer->addr_low    = (uint8_t)(uint8_t)p_ctrl->info.slave;
                p_transfer->addr_total  = 2;
            }
            else
            {
#if (DEBUG)
             *p_seq++ = g_WRITE_RESTART_str;
#endif
                /* We will only need to issue the high address byte, but store it like a 7-bit write */
                p_transfer->addr_low    = (uint8_t)((p_ctrl->info.slave >> 7) | I2C_CODE_10BIT) & (uint8_t)~I2C_CODE_READ;
                p_transfer->addr_total  = 1;
            }
        }

        /* Kickoff the write operation as a master */
        err = riic_run_hw_master(p_transfer);
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   Aborts any in-progress transfer and forces the IIC peripheral into a ready state.
 *
 *  This function will safely terminate any in-progress I2C transfer with the device. If a transfer is aborted, the user
 *  will be notified via callback with an abort event. Since the callback is optional, this function will also return
 *  a specific error code in this situation.
 *
 * @retval  SSP_SUCCESS                 Channel was reset without issue.
 * @retval  SSP_ERR_ABORTED             A transfer was aborted while resetting the hardware.
***********************************************************************************************************************/
ssp_err_t R_RIIC_MasterReset        (i2c_ctrl_t             * const p_ctrl)
{
#if RIIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl != NULL);
#endif

    RIIC_ERROR_RETURN(p_ctrl->open != 0, SSP_ERR_NOT_OPEN);

    /* Abort any transfer happening on the channel */
    ssp_err_t err = riic_abort_seq_master(p_ctrl, true);
#if (DEBUG)
    if (SSP_ERR_ABORTED == err)
    {
        *p_seq++ = g_RESET_str;
    }
#endif
    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup RIIC)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   Single point for managing the logic around notifying a transfer has finished.
 *
 * @param[in]       p_transfer  Pointer to transfer that is ending.
 * @param[in]       event       The event code to pass to the callback.
***********************************************************************************************************************/
__STATIC_INLINE void        riic_notify             (riic_transfer_t    * const p_transfer,
                                                     i2c_event_t        const           event)
{
#if (DEBUG)
    *p_seq++ = g_notify_str;
#endif
    /* Check if we can notify the caller of the abort via callback */
    if (NULL != p_transfer->p_ctrl->info.p_callback)
    {
        /* Fill in the argument to the callback */
        uint32_t total_bytes = p_transfer->total;
        i2c_callback_args_t args =
        {
            .p_context  = p_transfer->p_ctrl->info.p_context,
            .bytes      = total_bytes - p_transfer->remain,
            .event      = event
        };

        /* Now do the callback here */
        p_transfer->p_ctrl->info.p_callback(&args);
    }

    /* Turn off all the interrupts here since this always means the transfer is over */
    HW_RIIC_DisableInts(p_transfer->p_ctrl->info.channel);

    /** Set the flag indicating that the transaction is completed*/
    p_transfer->transaction_completed = true;
}

/*******************************************************************************************************************//**
 * @brief   Returns the register settings for getting the closest value less than the requested rate.
 *
 * @param[in,out]   p_rate  Pointer to desired rate (in) and calculated rate (out)
 * @param[out]      p_brh   Pointer to bit rate high-level setting
 * @param[out]      p_brl   Pointer to bit rate low-level setting
 * @param[out]      p_cks   Pointer to clock select setting
***********************************************************************************************************************/
__STATIC_INLINE void        riic_clock_settings     (uint32_t           * const p_rate,
                                                     uint8_t            * const p_brh,
                                                     uint8_t            * const p_brl,
                                                     uint8_t            * const p_cks)
{
    uint32_t divisor = 0;
    uint32_t period_count = 0;
    uint64_t period_count64 = 0;
    uint64_t pclk = 0;

    uint32_t const precision = 100;

    /* Note: using divisor here for convenience since we need a u32 */
    g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKB, &divisor);
    pclk   = (uint64_t)divisor * precision;
    *p_cks = 0;

    /* Note: When requesting 400 kHz, observed rate is ~365 kHz, while this algorithm suggests we should be able to achieve ~394736kHz */

    /* Find the correct settings starting from the highest rate */
    divisor = 1;
    while (divisor <= RIIC_MAX_DIVISOR)
    {
        /* Calculate how many counts (at the reference clock frequency) it would take to get close to the requested rate */
        period_count64 = (pclk / divisor) / *p_rate;

        /* Always round up */
        period_count = (uint32_t)((period_count64 + (precision-1)) / precision);

        /* If we can store this number of counts, we are close enough, exit the loop */
        if (period_count <= (2 << RIIC_MAX_BITS_BR_REGS))
        {
            divisor = RIIC_MAX_DIVISOR + 1;
        }
        else
        {
            /* Keep track of the appropriate divisor setting and continue looping */
            (*p_cks)++;

            /* Proceed to the next divisor to try */
            divisor <<= 1;
        }
    }

    /* Return 0's if we could not find good settings, otherwise return the settings we found */
    if (divisor != (RIIC_MAX_DIVISOR + 1))
    {
        *p_rate = 0;
        *p_brh  = 0;
        *p_brl  = 0;
    }
    else
    {

        *p_rate = (uint32_t)((pclk / (uint64_t)(1 << (*p_cks))) / period_count) / precision;

        /* Stick to a 50/50 duty cycle */
        *p_brl  = (uint8_t) period_count / 2;
        *p_brh  = (uint8_t)((uint8_t)period_count - (uint8_t)*p_brl);

        /* adjust for register settings */
        (*p_brl)--;
        (*p_brh)--;
    }
}

/*******************************************************************************************************************//**
 * @brief   Single point for managing the logic around aborting a transfer when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control struct of specific device
***********************************************************************************************************************/
__STATIC_INLINE ssp_err_t riic_abort_seq_master     (i2c_ctrl_t const   * const p_ctrl,
                                                     bool       const           all_devices)
{
#if (DEBUG)
    *p_seq++ = g_error_ISR_abort_str;
#endif
    ssp_err_t err = SSP_SUCCESS;

    /* Get the transfer record associated with the channel */
    riic_transfer_t * const p_transfer = &g_in_progress[p_ctrl->info.channel];

    /* Determines whether we abort for the entire channel, or only this device */
    bool match = (p_ctrl == p_transfer->p_ctrl) || all_devices;

    /* Check if there is an in-progress transfer associated with the match */
    if (match && (0 != p_transfer->remain))
    {
        /* Safely stop the hardware from operating */
        riic_abort_hw_master(p_ctrl);

        /* Notify anyone waiting that the transfer is finished */
        riic_notify(p_transfer, I2C_EVENT_ABORTED);

        /* Update the transfer descriptor to show no longer in-progress and an error */
        p_transfer->remain = 0;
        p_transfer->err    = true;

        /* Update the transfer descriptor to make sure interrupts no longer process */
        p_transfer->addr_loaded = p_transfer->addr_total;
        p_transfer->loaded      = p_transfer->total;

        /* Notify the caller via error code since the callback is optional */
        err = SSP_ERR_ABORTED;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   Performs the hardware initialization sequence when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control struct of specific device
***********************************************************************************************************************/
__STATIC_INLINE ssp_err_t riic_open_hw_master       (i2c_ctrl_t         * const p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Figure out if the hardware channel hasn't yet been configured */
    bool do_full_cfg    = !g_ch_opened[p_ctrl->info.channel];
    bool do_clock_cfg   = do_full_cfg;
    if (do_full_cfg)
    {
#if (DEBUG)
         *p_seq++ = g_fullcfg_str;
#endif
        /* Perform the first part of the initialization sequence */
        HW_RIIC_Power(p_ctrl->info.channel, true);
        HW_RIIC_Output(p_ctrl->info.channel, false);
        HW_RIIC_Reset(p_ctrl->info.channel, true);
        HW_RIIC_ClearIrqs(p_ctrl->info.channel);
        HW_RIIC_Output(p_ctrl->info.channel, true);
    }
    else
    {
        /* Do no re-configure the hardware if it is currently being used */
        if (0 != g_in_progress[p_ctrl->info.channel].remain)
        {
            return SSP_ERR_IN_USE;
        }
#if (DEBUG)
        *p_seq++ = g_clkrecfg_str;
#endif
        /* Re-configure the clock rate if this device is slower than the current clock rate */
        do_clock_cfg = (p_ctrl->info.rate < g_ch_speeds[p_ctrl->info.channel]);
    }

    /* May need to configure a new clock rate for the channel */
    uint32_t    best_match;
    uint8_t     high_count;
    uint8_t     low_count;
    uint8_t     ref_clock;
    if (do_clock_cfg)
    {
        /* We will be looking for the closest rate less-than-or-equal-to the requested rate */
        best_match = p_ctrl->info.rate;
        riic_clock_settings(&best_match, &high_count, &low_count, &ref_clock);

        /* Abort if we could not find good settings */
        if (best_match == 0)
        {
            err = SSP_ERR_INVALID_RATE;

            /* Undo the initialization if we were in the middle of it */
            if (do_full_cfg)
            {
                HW_RIIC_Power(p_ctrl->info.channel, false);
            }

            return err;
        }

        /* Record the found clock speed */
        g_ch_speeds[p_ctrl->info.channel] = best_match;

        HW_RIIC_ClockCfg(p_ctrl->info.channel, high_count, low_count, ref_clock, (best_match > I2C_RATE_FAST));

        /* Allow timeouts to be generated */
        HW_RIIC_TimeoutCfg(p_ctrl->info.channel);
    }

    /* Finish the initialization sequence, if this is its first opening */
    if (do_full_cfg)
    {
        /* Configure the peripheral into master mode */
        HW_RIIC_SetupMaster(p_ctrl->info.channel);

        /* Setup the interrupts for master mode */
        riic_cfg_ints_master(p_ctrl);

        /* Ensure features that are not supported are turned off */
        HW_RIIC_NoSupport(p_ctrl->info.channel);

        /* Release the peripheral from internal reset */
        HW_RIIC_Reset(p_ctrl->info.channel, false);
#if (DEBUG)
        *p_seq++ = g_fullcfgcomplete_str;
#endif
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   Performs the hardware initialization sequence when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control struct of specific device
***********************************************************************************************************************/
__STATIC_INLINE void        riic_close_hw_master    (i2c_ctrl_t         * const p_ctrl)
{
    /* De-configure the interrupts used in master mode */
    HW_RIIC_DisableInts(p_ctrl->info.channel);

    /* Shutdown the hardware to save some power */
    HW_RIIC_Power(p_ctrl->info.channel, false);
#if (DEBUG)
    *p_seq++ = g_close_str;
#endif
}

/*******************************************************************************************************************//**
 * @brief   Enables and assigns the interrupts to be used in master mode.
 *
 * @param[in]       p_ctrl  Pointer to control struct of specific device
***********************************************************************************************************************/
__STATIC_INLINE void        riic_cfg_ints_master    (i2c_ctrl_t         * const p_ctrl)
{
    /* These are the interrupts we will accept */
    HW_RIIC_ErrIntEn(p_ctrl->info.channel, true);
    HW_RIIC_RxFullIntEn(p_ctrl->info.channel, true);
    HW_RIIC_TxEndIntEn(p_ctrl->info.channel, true);
    HW_RIIC_TxEmptyIntEn(p_ctrl->info.channel, true);
}

/*******************************************************************************************************************//**
 * @brief   Safely stops the current data transfer when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control struct of specific device
***********************************************************************************************************************/
__STATIC_INLINE void        riic_abort_hw_master    (i2c_ctrl_t const   * const p_ctrl)
{
    /* Disable channel interrupts */
    HW_RIIC_DisableInts(p_ctrl->info.channel);

    /* Check if we are the active master on the bus right now */
    if (HW_RIIC_ActiveMaster(p_ctrl->info.channel))
    {
        /* Check if the error happened in the middle of a transfer */
        uint8_t bits_pending = HW_RIIC_BitsPending(p_ctrl->info.channel);
        /*HW_RIIC_BitsPending returns value 9 at the end of a data transfer including the acknowledge bit.
        (values of the BC[2:0] bits return to 000b (ie, 9 bits) at the end of a data transfer as per HM)*/
        if (9 != bits_pending)
        {
            /* Manually issue enough clocks to finish the transfer */
            while (bits_pending--)
            {
                HW_RIIC_ExtraClock(p_ctrl->info.channel);
                while (!HW_RIIC_ExtraClockDone(p_ctrl->info.channel))
                {
                    /* Waiting for clock cycle to finish */
                }
            }

            /* Check if the peripheral is holding the line low */
            if (HW_RIIC_SDALow(p_ctrl->info.channel))
            {
                /* Release the line manually */
                HW_RIIC_SDARelease(p_ctrl->info.channel);
            }
        }

        /* Try to issue the stop condition now */
        HW_RIIC_SendStop(p_ctrl->info.channel);

        /* Make sure we can timeout again if the slave doesn't release the line */
        HW_RIIC_ResetTimeout(p_ctrl->info.channel);

        /* Wait until the stop condition is sent or the hardware detects the line is stuck low */
        volatile bool timed_out = false;
        while (HW_RIIC_StopPending(p_ctrl->info.channel) && !timed_out)
        {
            timed_out = HW_RIIC_TimedOut(p_ctrl->info.channel);
        }

        if (timed_out)
        {
#if (DEBUG)
        *p_seq++ = g_error_ISR_abort_timeout_str;
#endif
            /* What can we do? */
            timed_out = false;
        }
    }

    /* Put peripheral into internal reset */
    HW_RIIC_Reset(p_ctrl->info.channel, true);

    /* Release peripheral from internal reset */
    HW_RIIC_Reset(p_ctrl->info.channel, false);

    /* Make sure there are no retained interrupt requests, as per manual */
    HW_RIIC_Output(p_ctrl->info.channel, false);
    HW_RIIC_ClearIrqs(p_ctrl->info.channel);
    HW_RIIC_Output(p_ctrl->info.channel, true);
}

/*******************************************************************************************************************//**
 * @brief   Performs the data transfer described by the parameters when operating as a master.
 *
 * @param[in]       p_transfer  Pointer to transfer that needs to be done.
***********************************************************************************************************************/
static          ssp_err_t   riic_run_hw_master      (riic_transfer_t    * const p_transfer)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Initialize fields used during transfer */
    p_transfer->addr_loaded = 0;
    p_transfer->loaded      = 0;
    p_transfer->remain      = p_transfer->total;
    p_transfer->addr_remain = p_transfer->addr_total;
    p_transfer->err         = false;
    /** Set the flag indicating that the transaction is not completed*/
    p_transfer->transaction_completed = false;

    /* Check if this is a new transaction or a continuation */
    if (!p_transfer->restarted)
    {
#if (DEBUG)
        *p_seq++ = g_newtx_str;
#endif
        /* Clear and then turn on the interrupts just before starting the transfer */
        HW_RIIC_ClearIrqs(p_transfer->p_ctrl->info.channel);
        riic_cfg_ints_master(p_transfer->p_ctrl);

        /* Set the RIIC into master mode and start interrupt processing */
        HW_RIIC_SendStart(p_transfer->p_ctrl->info.channel);
    }
    else
    {
#if (DEBUG)
        *p_seq++ = g_continuationtx_str;
#endif
        /* Clear for next transfer */
        p_transfer->restarted = false;

        /* Turn on the interrupts which will start the transfer */
        riic_cfg_ints_master(p_transfer->p_ctrl);
    }

    /* Check if we must block until the transfer is done */
    if (NULL == p_transfer->p_ctrl->info.p_callback)
    {
        /* Note: There is a hardware timeout that will allow this loop to exit */
        while (false == p_transfer->transaction_completed)
        {
#if (DEBUG)
            if (remains_count == 0)
            {
                *p_seq++ = g_remains_str;
            }
            remains_count++;
#endif
            /* The transfer descriptor is updated during interrupt processing */
        }
#if (DEBUG)
        *p_errseq++ = remains_count;
#endif
        if (p_transfer->err)
        {
#if (DEBUG)
             *p_seq++ = g_error_str;
#endif
            err = SSP_ERR_ABORTED;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief   Handles the receive data full interrupt when operating as a master.
 *
 * @param[in]       channel     The target RIIC block's channel number.
***********************************************************************************************************************/
static          void        riic_rxi_master         (uint32_t   const           channel)
{
    /* Get the in-progress transfer */
    riic_transfer_t * p_transfer = &g_in_progress[channel];

    /* First receive interrupt: Handle the special case of 1 or 2 byte read here */
    if (0 == p_transfer->loaded)
    {
        /* Enable WAIT for 1 or 2 byte read */
        if(2 <= p_transfer->total)
        {
            HW_RIIC_SetWait(channel);
        }

        /*Enable NACK for 1 byte read*/
        if(1 == p_transfer->remain)
        {
            HW_RIIC_SetAck(channel);
        }

        /* do a dummy read to clock the data into the ICDRR */
        volatile uint8_t dummy = HW_RIIC_ReadByte(channel);
        (void)dummy;

        /* Update the counter */
        p_transfer->loaded = 1;

    }
    /* ICDRR contain valid received data */
    else if(0 < p_transfer->remain)
    {
        /* if next data = (final byte - 2), enable WAIT */
        if(3 == p_transfer->remain)
        {
            HW_RIIC_SetWait(channel);
        }

        /* if next data = (final byte - 1), enable NACK  */
        else if(2 == p_transfer->remain)
        {
            HW_RIIC_SetAck(channel);
        }

        /* if next data = final byte, send STOP or RESTART */
        else if (1 == p_transfer->remain)
        {
            if (p_transfer->restart)
            {
                HW_RIIC_SendRestart(channel);

                /* Remember that we issued a restart when doing the next transfer */
                p_transfer->restarted = true;
            }
            else
            {
                /* Clear STOP flag and set SP */
                HW_RIIC_ClrStop(channel);
                HW_RIIC_SendStop(channel);
            }
        }

        /* Read the data into the buffer */
        p_transfer->p_buff[p_transfer->loaded - 1] = HW_RIIC_ReadByte(channel);

        /* if we are done with the transmission, clear the WAIT bit */
        if(1 == p_transfer->remain)
        {
            HW_RIIC_ClrWait(channel);
        }

        p_transfer->loaded++;
        p_transfer->remain--;
    }
}

/*******************************************************************************************************************//**
 * @brief   Handles the transmit data empty interrupt when operating as a master.
 *
 * @param[in]       channel     The target RIIC block's channel number.
***********************************************************************************************************************/
static          void        riic_txi_master         (uint32_t   const           channel)
{
    /* Force sequencing between tx end and next byte load by disabling tx empty here,
       tx end will re-enable the tx empty interrupt */
    NVIC_DisableIRQ(riic_txi[channel]);

    /* Get the in-progress transfer */
    riic_transfer_t * p_transfer = &g_in_progress[channel];

    /* Check if we are issuing the slave address */
    uint8_t addr_total = p_transfer->addr_total;
    if (addr_total != p_transfer->addr_loaded)
    {
#if (DEBUG)
         *p_seq++ = g_TXEmpty_ISR_issue_slave_address_str;
#endif
        /* If 7-bit address mode, issue the single address byte */
        if (1 == p_transfer->addr_total)
        {
#if (DEBUG)
         *p_seq++ = g_TXEmpty_ISR_issue_7_bit_slave_address_str;
#endif
            HW_RIIC_WriteByte(channel, p_transfer->addr_low);
        }
        else
        {
#if (DEBUG)
         *p_seq++ = g_TXEmpty_ISR_issue_10_bit_slave_address_str;
#endif
            /* 10-bit addressing mode, address issuance changes between read and write */
            if (p_transfer->read)
            {
                /* The address must be issued once with the write code, and again with the read code */
                switch (p_transfer->addr_loaded)
                {
                    case 0:
                        HW_RIIC_WriteByte(channel, p_transfer->addr_high & (uint8_t)~I2C_CODE_READ);
                        break;
                    case 1:
                        HW_RIIC_WriteByte(channel, p_transfer->addr_low);
                        break;
                    case 2:
                    default:
                        HW_RIIC_WriteByte(channel, p_transfer->addr_high);
                        break;
                }
            }
            else
            {
                /* The address must be issued only once with the write code */
                if (0 == p_transfer->addr_loaded)
                {
                    HW_RIIC_WriteByte(channel, p_transfer->addr_high);
                }
                else
                {
                    HW_RIIC_WriteByte(channel, p_transfer->addr_low);
                }
            }
        }

        /* Update the number of address bytes loaded for next pass */
        p_transfer->addr_loaded++;
    }
    else
    {
#if (DEBUG)
     *p_seq++ = g_TXEmpty_ISR_load_byte_for_transfer_str;
#endif
        /* Check if there are any bytes remaining to load for transfer */
        uint32_t total = p_transfer->total;
        if (!p_transfer->read && (total != p_transfer->loaded))
        {
#if (DEBUG)
         *p_seq++ = g_TXEmpty_ISR_bytes_loaded_for_transfer_str;
#endif
            HW_RIIC_WriteByte(channel, p_transfer->p_buff[p_transfer->loaded]);

            /* Update the number of bytes loaded for next pass */
            p_transfer->loaded++;
        }
    }
}

/*******************************************************************************************************************//**
 * @brief   Handles the transmit end interrupt when operating as a master.
 *
 * @param[in]       channel     The target RIIC block's channel number.
***********************************************************************************************************************/
static void riic_tei_master (uint32_t   const           channel)
{
    HW_RIIC_ClearTxEnd(channel);

    /* Get the in-progress transfer */
    riic_transfer_t * p_transfer = &g_in_progress[channel];

    /* Check if we sent an address byte */
    if (0 != p_transfer->addr_remain)
    {
#if (DEBUG)
     *p_seq++ = g_TXEND_ISR_address_bytes_yet_to_be_sent_str;
#endif
        /* Update tracker that we have completed an address byte */
        p_transfer->addr_remain--;
    }
    else
    {
#if (DEBUG)
     *p_seq++ = g_TXEND_ISR_no_address_bytes_left_to_send_str;
#endif
        /* Check if there are more data bytes to send */
        if (0 != p_transfer->remain)
        {
#if (DEBUG)
             *p_seq++ = g_TXEND_ISR_data_bytes_left_to_send_str;
#endif
            /* Update tracker that we have completed a data byte */
            p_transfer->remain--;
        }
    }

    /* Make sure there were no errors during processing */
    if (!p_transfer->err)
    {
#if (DEBUG)
         *p_seq++ = g_TXEND_ISR_no_errors_str;
#endif
        /* Note: There should never be a situation where there are more address bytes and
                 no data bytes, so we don't even bother checking */
        if (0 == p_transfer->remain)
        {
#if (DEBUG)
             *p_seq++ = g_TXEND_ISR_no_data_bytes_left_to_send_str;
#endif
            /* Notify anyone waiting that the transfer is finished */
            //MT modified riic_notify(p_transfer, I2C_EVENT_TX_COMPLETE);

            /* End the data transfer according to the desired protocol */
            if (p_transfer->restart)
            {
#if (DEBUG)
                 *p_seq++ = g_TXEND_ISR_send_restart_str;
#endif
                HW_RIIC_SendRestart(channel);
                HW_RIIC_TimeoutEnable(channel, false);

                /* Remember that we issued a restart for the next transfer */
                p_transfer->restarted = true;
            }
            else
            {
#if (DEBUG)
                 *p_seq++ = g_TXEND_ISR_send_stop_str;
#endif
                HW_RIIC_SendStop(channel);
            }
        }
        else
        {
            /* Check if there are more address or data bytes to output */
            if (!p_transfer->read || (0 != p_transfer->addr_remain))
            {
#if (DEBUG)
                 *p_seq++ = g_TXEND_ISR_more_address_or_data_left_force_ISR_str;
#endif
                /* We are forcing sequencing between tx end and tx empty, so we must re-enable it here */
                NVIC_EnableIRQ(riic_txi[channel]);
            }
        }
    }
}

/*******************************************************************************************************************//**
 * @brief   Handles the error interrupts when operating as a master.
***********************************************************************************************************************/
static void riic_err_master (uint32_t   const           channel,
                             uint8_t    const           errors)
{
    SSP_PARAMETER_NOT_USED(errors);
    /** Get the in-progress transfer */
    riic_transfer_t * p_transfer = &g_in_progress[channel];

    /** Abort an in-progress transfer with the current device */
    riic_abort_seq_master(p_transfer->p_ctrl, false);

}

/***********************************************************************************************************************
Interrupt Vectors
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   Receive data full interrupt routine for channel 0.
 *
 * This function implements the RIIC Channel 0 Receive buffer full ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC0_RXI != BSP_IRQ_DISABLED)
void iic0_rxi_isr  (void);
void iic0_rxi_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    riic_rxi_master(0);
    R_BSP_IrqStatusClear (riic_rxi[0]);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
/*******************************************************************************************************************//**
 * @brief   Receive data full interrupt routine for channel 1.
 *
 * This function implements the RIIC Channel 1 Receive buffer full ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC1_RXI != BSP_IRQ_DISABLED)
void iic1_rxi_isr  (void);
void iic1_rxi_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE
#if (DEBUG)
    *p_seq++ = g_rxdatafull_ISR_str;
#endif
    riic_rxi_master(1);
    R_BSP_IrqStatusClear (riic_rxi[1]);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
#if (BSP_CFG_MCU_PART_SERIES != 1)
/*******************************************************************************************************************//**
 * @brief   Receive data full interrupt routine for channel 2.
 *
 * This function implements the RIIC Channel 2 Receive buffer full ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC2_RXI != BSP_IRQ_DISABLED)
void iic2_rxi_isr  (void);
void iic2_rxi_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    riic_rxi_master(2);
    R_BSP_IrqStatusClear (riic_rxi[2]);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
#endif
/*******************************************************************************************************************//**
 * @brief   Transmit data empty interrupt routine for channel 0.
 *
 * This function implements the RIIC Channel 0 Transmit buffer empty ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC0_TXI != BSP_IRQ_DISABLED)
void iic0_txi_isr  (void);
void iic0_txi_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    riic_txi_master(0);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
/*******************************************************************************************************************//**
 * @brief   Transmit data empty interrupt routine for channel 1.
 *
 * This function implements the RIIC Channel 1 Transmit buffer empty ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC1_TXI != BSP_IRQ_DISABLED)
void iic1_txi_isr  (void);
void iic1_txi_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE
#if (DEBUG)
    *p_seq++ = g_txdataempty_ISR_str;
#endif
    riic_txi_master(1);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
#if (BSP_CFG_MCU_PART_SERIES != 1)
/*******************************************************************************************************************//**
 * @brief   Transmit data empty interrupt routine for channel 2.
 *
 * This function implements the RIIC Channel 2 Transmit buffer empty ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC2_TXI != BSP_IRQ_DISABLED)
void iic2_txi_isr  (void);
void iic2_txi_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    riic_txi_master(2);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
#endif
/*******************************************************************************************************************//**
 * @brief   Transmit end interrupt routine for channel 0.
 *
 * This function implements the RIIC Channel 0 Transmission End ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC0_TEI != BSP_IRQ_DISABLED)
void iic0_tei_isr  (void);
void iic0_tei_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    riic_tei_master(0);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
/*******************************************************************************************************************//**
 * @brief   Transmit end interrupt routine for channel 1.
 *
 * This function implements the RIIC Channel 1 Transmission End ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC1_TEI != BSP_IRQ_DISABLED)
void iic1_tei_isr  (void);
void iic1_tei_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE
#if (DEBUG)
    *p_seq++ = g_txend_ISR_str;
#endif
    riic_tei_master(1);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
#if (BSP_CFG_MCU_PART_SERIES != 1)
/*******************************************************************************************************************//**
 * @brief   Transmit end interrupt routine for channel 2.
 *
 * This function implements the RIIC Channel 2 Transmission End ISR routine.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC2_TEI != BSP_IRQ_DISABLED)
void iic2_tei_isr  (void);
void iic2_tei_isr  (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    riic_tei_master(2);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
#endif
/*******************************************************************************************************************//**
 * @brief   Error and event interrupt routine for channel 0.
 *
 * This function implements the RIIC Channel 0 Event/Error.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC0_ERI != BSP_IRQ_DISABLED)
void iic0_eri_isr (void);
void iic0_eri_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /** Clear the error flags in the ICU */
    R_BSP_IrqStatusClear(riic_eei[0]);

    riic_transfer_t * p_transfer = &g_in_progress[0];
    /** Clear the error flags in the peripheral */
    volatile uint8_t errs_events = HW_RIIC_GetAndClearErrsEvents(0);

    /** If the event was an error event, then handle it */
    if (errs_events & (RIIC_ERR_EVENT_TIMEOUT | RIIC_ERR_EVENT_ARBITRATION_LOSS | RIIC_ERR_EVENT_NACK))
    {
#if (DEBUG)
        *p_seq++ = g_error_ISR_str;
        *p_errseq++ = errs_events;
#endif
        riic_err_master(0, errs_events);
    }
    /** else if the event was a start/restart or stop condition*/
    else if (errs_events & (RIIC_ERR_EVENT_START | RIIC_ERR_EVENT_STOP))
    {
    	/** HM: Clear Stop and Nackf bits for processing for next transfer operation */
    	HW_RIIC_ClrStopNackf(0);

        /** If there are zero bytes remaining for a read or write transaction, then this means
         * that this is a RESTART condition as opposed to a normal START condition which is also
         * detected by the hardware as the same event, but the "remain" filed would not be 0 in
         * that case */
        if ((0 == p_transfer->total) && (errs_events & RIIC_ERR_EVENT_STOP))
        {
            /** Notify anyone waiting that the transfer is finished */
    	    riic_notify(p_transfer, I2C_EVENT_TX_COMPLETE);
        }
        else if (0 == p_transfer->remain)
        {
    	    /** If this was a read transaction, then it is complete now */
            if (true == p_transfer->read)
            {
    	        /** Notify anyone waiting that the transfer is finished */
    	        riic_notify(p_transfer, I2C_EVENT_RX_COMPLETE);
            }
    	        /** Else this was a write transaction and is complete now */
    	    else if ((false == p_transfer->read) && (0 != p_transfer->total))
    	    {
    	        /** Notify anyone waiting that the transfer is finished */
    	        riic_notify(p_transfer, I2C_EVENT_TX_COMPLETE);
    	    }
    	}
        else
        {
            /** Do nothing if more than 0 bytes remain because this indicates that this is a START
             * condition and not a RESTART*/
        }
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif

/*******************************************************************************************************************//**
 * @brief   Error and event interrupt routine for channel 1.
 *
 * This function implements the RIIC Channel 1 Event/Error.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC1_ERI != BSP_IRQ_DISABLED)
void iic1_eri_isr (void);
void iic1_eri_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /** Clear the error flags in the ICU */
    R_BSP_IrqStatusClear(riic_eei[1]);

    riic_transfer_t * p_transfer = &g_in_progress[1];
    /** Clear the error flags in the peripheral */
    volatile uint8_t errs_events = HW_RIIC_GetAndClearErrsEvents(1);

    /** If the event was an error event, then handle it */
    if (errs_events & (RIIC_ERR_EVENT_TIMEOUT | RIIC_ERR_EVENT_ARBITRATION_LOSS | RIIC_ERR_EVENT_NACK))
    {
#if (DEBUG)
        *p_seq++ = g_error_ISR_str;
        *p_errseq++ = errs_events;
#endif
        riic_err_master(1, errs_events);
    }
    /** else if the event was a start/restart or stop condition*/
    else if (errs_events & (RIIC_ERR_EVENT_START | RIIC_ERR_EVENT_STOP))
    {
    	/** HM: Clear Stop and Nackf bits for processing for next transfer operation */
    	HW_RIIC_ClrStopNackf(1);

        /** If there are zero bytes remaining for a read or write transaction, then this means
         * that this is a RESTART condition as opposed to a normal START condition which is also
         * detected by the hardware as the same event, but the "remain" filed would not be 0 in
         * that case */
        if ((0 == p_transfer->total) && (errs_events & RIIC_ERR_EVENT_STOP))
        {
            /** Notify anyone waiting that the transfer is finished */
    	    riic_notify(p_transfer, I2C_EVENT_TX_COMPLETE);
        }
        else if (0 == p_transfer->remain)
        {
    	    /** If this was a read transaction, then it is complete now */
            if (true == p_transfer->read)
            {
    	        /** Notify anyone waiting that the transfer is finished */
    	        riic_notify(p_transfer, I2C_EVENT_RX_COMPLETE);
            }
    	        /** Else this was a write transaction and is complete now */
    	    else if ((false == p_transfer->read) && (0 != p_transfer->total))
    	    {
    	        /** Notify anyone waiting that the transfer is finished */
    	        riic_notify(p_transfer, I2C_EVENT_TX_COMPLETE);
    	    }
    	}
        else
        {
            /** Do nothing if more than 0 bytes remain because this indicates that this is a START
             * condition and not a RESTART*/
        }
    }
    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
#if (BSP_CFG_MCU_PART_SERIES != 1)
/*******************************************************************************************************************//**
 * @brief   Error and event interrupt routine for channel 2.
 *
 * This function implements the RIIC Channel 2 Event/Error.
 *
***********************************************************************************************************************/
#if (BSP_IRQ_CFG_IIC2_ERI != BSP_IRQ_DISABLED)
void iic2_eri_isr (void);
void iic2_eri_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /** Clear the error flags in the ICU */
    R_BSP_IrqStatusClear(riic_eei[2]);

    riic_transfer_t * p_transfer = &g_in_progress[2];
    /** Clear the error flags in the peripheral */
    volatile uint8_t errs_events = HW_RIIC_GetAndClearErrsEvents(2);

    /** If the event was an error event, then handle it */
    if (errs_events & (RIIC_ERR_EVENT_TIMEOUT | RIIC_ERR_EVENT_ARBITRATION_LOSS | RIIC_ERR_EVENT_NACK))
    {
#if (DEBUG)
        *p_seq++ = g_error_ISR_str;
        *p_errseq++ = errs_events;
#endif
        riic_err_master(2, errs_events);
    }
    /** else if the event was a start/restart or stop condition*/
    else if (errs_events & (RIIC_ERR_EVENT_START | RIIC_ERR_EVENT_STOP))
    {
    	/** HM: Clear Stop and Nackf bits for processing for next transfer operation */
    	HW_RIIC_ClrStopNackf(2);

        /** If there are zero bytes remaining for a read or write transaction, then this means
         * that this is a RESTART condition as opposed to a normal START condition which is also
         * detected by the hardware as the same event, but the "remain" filed would not be 0 in
         * that case */
        if ((0 == p_transfer->total) && (errs_events & RIIC_ERR_EVENT_STOP))
        {
            /** Notify anyone waiting that the transfer is finished */
    	    riic_notify(p_transfer, I2C_EVENT_TX_COMPLETE);
        }
        else if (0 == p_transfer->remain)
        {
    	    /** If this was a read transaction, then it is complete now */
            if (true == p_transfer->read)
            {
    	        /** Notify anyone waiting that the transfer is finished */
    	        riic_notify(p_transfer, I2C_EVENT_RX_COMPLETE);
            }
    	        /** Else this was a write transaction and is complete now */
    	    else if ((false == p_transfer->read) && (0 != p_transfer->total))
    	    {
    	        /** Notify anyone waiting that the transfer is finished */
    	        riic_notify(p_transfer, I2C_EVENT_TX_COMPLETE);
    	    }
    	}
        else
        {
            /** Do nothing if more than 0 bytes remain because this indicates that this is a START
             * condition and not a RESTART*/
        }
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif
#endif

