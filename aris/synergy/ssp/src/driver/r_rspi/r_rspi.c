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
 * File Name    : r_rspi.c
 * Description  : This module contains API functions and HLD layer functions for RSPI module.
 *                The API functions includes an open() function to initialize and power on the SPI bus,
 *                a close() function to power down and shut off the channel, read(), Write() and Write/read()
 *                function to access and transfer data to and from the SPI bus.
 *                HDL layer functions includes functions like rspi_baud_set() to set the baud rate,
 *                rspi_default_config_set(), rspi_common_config_set(), rspi_extended_config_set() functions for
 *                setting configuration, a rspi_bitrate_check() function for checking the bit rate range and
 *                rspi_tx_rx_common() function as the transmit and reception handler.
 ********************************************************************************************************************/


/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "r_rspi.h"
#include "r_spi_api.h"
#include "r_rspi_private.h"
#include "./hw/hw_rspi_private.h"
#include "r_rspi_private_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"

/*********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#ifndef RSPI_ERROR_RETURN
#define RSPI_ERROR_RETURN(a, err)       SSP_ERROR_RETURN((a), (err), "spi", &module_version)
#endif

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** SPI HAL module version data structure */
static const ssp_version_t module_version =
{
    .api_version_major  = SPI_API_VERSION_MAJOR,
    .api_version_minor  = SPI_API_VERSION_MINOR,
    .code_version_major = RSPI_CODE_VERSION_MAJOR,
    .code_version_minor = RSPI_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
typedef enum e_rspi_transfer_operation
{
    // Values will be uses as bit flags.
    RSPI_DO_TX    = 0x1,
    RSPI_DO_RX    = 0x2,
    RSPI_DO_TX_RX = 0x3
} rspi_transfer_operation_t;

typedef struct rspi_tcb_s
{
    void                       * psrc;
    void                       * pdest;
    uint16_t                   tx_count;
    uint16_t                   rx_count;
    uint32_t                   xfr_length;
    uint8_t                    bytes_per_transfer;  /* Source buffer bytes per transfer: 1, 2, or 4. */
    bool                       do_rx_now;           /* State flag for valid read data available. */
    bool                       do_tx;               /* State flag for transmit operation. */
    rspi_transfer_operation_t  transfer_mode;       /* Transmit only, receive only, or transmit-receive. */
    uint16_t                   ext_delay;           /* Extra delay for any slow SPI device */
    bsp_lock_t                 resource_lock_tx_rx; /**< Resource lock for transmission/reception */
    void                       * p_context;         /**< Pointer to the function level device context */
} rspi_tcb_t;

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
/* Array of channel control. One for each physical RSPI channel on the device. */
static struct st_spi_ctrl g_rspi_controls[RSPI_NUM_CHANNELS];

#if (BSP_CFG_MCU_PART_SERIES == 1)
static volatile uint16_t g_rxdata[RSPI_NUM_CHANNELS]; /* Space for fast read of RSPI RX data register. */
#else
static volatile uint32_t g_rxdata[RSPI_NUM_CHANNELS]; /* Space for fast read of RSPI RX data register. */
#endif

/* Allocate transfer control block for all channels. */
static struct rspi_tcb_s g_rspi_tcb[RSPI_NUM_CHANNELS] = { { 0 }, { 0 } };

const spi_api_t          g_spi_on_rspi =
{
    .open      = R_RSPI_Open,
    .read      = R_RSPI_Read,
    .write     = R_RSPI_Write,
    .writeRead = R_RSPI_WriteRead,
    .close     = R_RSPI_Close,
    .versionGet= R_RSPI_VersionGet
};

/*********************************************************************************************************************
 * Private function declarations
 ********************************************************************************************************************/
/* Common routine used by RSPI API write or read functions. */
static ssp_err_t rspi_write_read_common (spi_ctrl_t                 * const p_ctrl,
                                         void const                         * p_src,
                                         void const                         * p_dest,
                                         uint32_t const                     length,
                                         spi_bit_width_t const              bit_width,
                                         rspi_transfer_operation_t          tx_rx_mode);

/* Sets the baud rate registers for a given frequency. */
static uint32_t rspi_baud_set (uint8_t channel, uint32_t baud_target);

/* Common subroutine for transmitting. */
static void     rspi_tx_rx_common (uint8_t channel, rspi_int_t which_irq);

/* Set the default configuration for the registers */
static void     rspi_default_config_set (uint8_t channel);

/* Set the basic configuration (simple SPI) from the user's configuration */
static void     rspi_common_config_set (uint8_t channel, spi_cfg_t  const * const p_cfg);

/* Set the extended configuration (RSPI) from the user's extended configuration */
static void     rspi_extended_config_set (uint8_t channel, spi_cfg_t  const * const p_cfg);

/* Configures SCI SPI related transfer drivers (if enabled). */
static ssp_err_t    rspi_transfer_open     (spi_cfg_t const * const p_cfg);

/* Service functions for various interrupts*/
void spi0_eri_isr (void);
void spi0_rxi_isr (void);
void spi0_txi_isr (void);
void spi1_eri_isr (void);
void spi1_rxi_isr (void);
void spi1_txi_isr (void);


/*****************************************************************************************************************//**
 * @addtogroup SPI
 * @{
 *********************************************************************************************************************/

/***************************************************************************************************************
 * Functions
 ****************************************************************************************************************/

/*************************************************************************************************************//**
 * @brief   This functions initializes a channel for SPI communication mode.
 *
 * Implements spi_api_t::open
 *          This function performs the following tasks:<BR>
 *          Performs parameter checking and processes error conditions.<BR>
 *          Applies power to the SPI channel.<BR>
 *          Disables interrupts.<BR>
 *          Initializes the associated registers with some default value and the user-configurable options.<BR>
 *          Provides the channel control for use with other API functions.<BR>
 *          Updates user-configurable file if necessary.<BR>
 * @retval  SSP_SUCCESS               Channel initialized successfully.
 * @retval  SSP_ERR_INVALID_ARGUMENT  Channel number invalid.
 * @retval  SSP_ERR_IN_USE  Channel   Currently in operation; Close channel first.
 * @retval  SSP_ERR_INVALID_POINTER   The p_cfg pointer or p_ctrl pointer is NULL.
 * @retval  SSP_ERR_INVALID_ARGUMENT  An element of the r_spi_cfg_t structure contains an invalid value.
 * @retval  SSP_ERR_INVALID_ARGUMENT  The parameters is out of range.
 * @retval   SSP_ERR_HW_LOCKED         The lock could not be acquired. The channel is busy.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
ssp_err_t R_RSPI_Open (spi_ctrl_t            * p_ctrl,
                       spi_cfg_t            const * const p_cfg)
{
	ssp_err_t        result = SSP_SUCCESS;
    uint8_t channel = (uint8_t)p_cfg->channel;

#if RSPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_ctrl);
    /* Check channel number */
    if (RSPI_NUM_CHANNELS < p_cfg->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
#endif /* if RSPI_CFG_PARAM_CHECKING_ENABLE */

    /* Check to see if the peripheral has already been initialized. */
    if (true == g_rspi_controls[channel].channel_opened)
    {
        /* This channel has already been initialized. */
        return SSP_ERR_NOT_OPEN;
    }

    /* Attempt to acquire lock for this RSPI channel. Prevents re-entrance conflict. */
    /* All RSPI channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    result = R_BSP_HardwareLock((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_RSPI0 + channel));
    RSPI_ERROR_RETURN((SSP_SUCCESS == result), SSP_ERR_HW_LOCKED);

    result = rspi_transfer_open(p_cfg);
    RSPI_ERROR_RETURN((SSP_SUCCESS == result), result);

    HW_RSPI_Power_On_Off(channel, RSPI_POWER_ON);

    /* Disable all interrupts in ICU. */
    HW_RSPI_IRQ_InterruptEnable (channel, RSPI_ALL_INTR, false);

    /* set the default register configuration condition */
    rspi_default_config_set(channel);

    if(SPI_MODE_MASTER == p_cfg->operating_mode)
    {
        /* Set the base bit rate. Modifies the SPBR register setting with requested baud rate.*/
        if (0 == rspi_baud_set(channel, p_cfg->bitrate))
        {
            // Failed to set the baud rate
            /* All RSPI channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
             * the base channel 0 lock yields the channel's lock type. */
            R_BSP_HardwareUnlock((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_RSPI0 + channel));
            return SSP_ERR_ASSERTION; /* Could not calculate settings for the requested baud rate. */
        }
    }

    /* Set the common configuration based on the user's basic configuration for simple SPI mode */
    rspi_common_config_set(channel, p_cfg);

    /* Do the extended configuration if it's needed based on the user's extended configuration */
    if (NULL != p_cfg->p_extend)
    {
        rspi_extended_config_set(channel, p_cfg);
    }

    /* Peripheral Initialized */
    g_rspi_controls[channel].channel_opened = true;
    g_rspi_controls[channel].p_callback     = p_cfg->p_callback;
    g_rspi_controls[channel].p_context      = p_cfg->p_context;
    g_rspi_controls[channel].channel        = channel;

    /* Update info to the control block */
    p_ctrl->channel        = channel;
    p_ctrl->channel_opened = g_rspi_controls[channel].channel_opened;
    p_ctrl->p_transfer_rx  = p_cfg->p_transfer_rx;
    p_ctrl->p_transfer_tx  = p_cfg->p_transfer_tx;
    g_rspi_tcb[channel].p_context         = p_ctrl;

    return SSP_SUCCESS;
}/* End of function R_RSPI_Open(). */

/*************************************************************************************************************//**
 * @brief   This function receives data from a SPI device.
 *
 * Implements spi_api_t::read
 *          The function performs the following tasks:<BR>
 *          Performs parameter checking and processes error conditions.<BR>
 *          Disable Interrupts.<BR>
 *          Disable the SPI bus.<BR>
 *          Setup data bit width per user request.<BR>
 *          Enable the SPI bus.<BR>
 *          Enable interrupts.<BR>
 *          Start data transmission with dummy data via transmit buffer empty interrupt.<BR>
 *          Copy data from source buffer to the SPI data register for transmission.<BR>
 *          Receive data from receive buffer full interrupt occurs and copy data to the buffer of destination.<BR>
 *          Complete data reception via receive buffer full interrupt and transmitting dummy data.<BR>
 * @retval  SSP_SUCCESS               Read operation successfully completed.
 * @retval  SSP_ERR_INVALID_ARGUMENT  Channel number invalid.
 * @retval  SSP_ERR_INVALID_POINTER   A required pointer argument is NULL.
 * @retval  SSP_ERR_HW_LOCKED         The lock could not be acquired. The channel is busy.
 * @retval  SSP_ERR_CH_NOT_OPEN       The channel has not been opened. Open channel first.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
ssp_err_t  R_RSPI_Read (spi_ctrl_t          * const p_ctrl,
                        void const                  * p_dest,
                        uint32_t const              length,
                        spi_bit_width_t const       bit_width)
{
    ssp_err_t result;

#if RSPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_dest);
    /* Check the data length, should not be 0 */
    SSP_ASSERT(0 != length);
    /* Check channel number */
    if (RSPI_NUM_CHANNELS < p_ctrl->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
    /* Check compatibility in transfer mode*/
    if ((NULL != p_ctrl->p_transfer_tx) || (NULL != p_ctrl->p_transfer_rx))
    {
        /* Since RSPI transmit receive data registers are of 32-bit,
         * RSPI supports only 32-bit data transfer in DTC transfer mode.
         * Note: S124 RSPI data registers are 16-bit only)*/
#if (BSP_CFG_MCU_PART_SERIES == 1)
        if(SPI_BIT_WIDTH_16_BITS != bit_width)
#else
        if(SPI_BIT_WIDTH_32_BITS != bit_width)
#endif
        {
            return SSP_ERR_UNSUPPORTED;
        }
    }
#if (BSP_CFG_MCU_PART_SERIES == 1)
    /*Check bit width*/
    if(SPI_BIT_WIDTH_32_BITS == bit_width)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
#endif
#endif /* if RSPI_CFG_PARAM_CHECKING_ENABLE */

    result = rspi_write_read_common(p_ctrl, NULL, p_dest, length, bit_width, RSPI_DO_RX);

    return result;
}/* End of function R_RSPI_Read(). */

/*************************************************************************************************************//**
 * @brief   This function transmits data to a SPI  device.
 *
 * Implements spi_api_t::write
 *          The function performs the following tasks:<BR>
 *          Performs parameter checking and processes error conditions.<BR>
 *          Disable Interrupts.<BR>
 *          Disable the SPI bus.<BR>
 *          Setup data bit width per user request.<BR>
 *          Enable the SPI bus.<BR>
 *          Enable interrupts.<BR>
 *          Start data transmission with dummy data via transmit buffer empty interrupt.<BR>
 *          Copy data from source buffer to the SPI data register for transmission.<BR>
 *          Receive data from receive buffer full interrupt occurs and do nothing with the received data.<BR>
 *          Complete data transmission via receive buffer full interrupt.<BR>
 * @retval  SSP_SUCCESS               Write operation successfully completed.
 * @retval  SSP_ERR_INVALID_ARGUMENT  Channel number invalid.
 * @retval  SSP_ERR_INVALID_POINTER   A required pointer argument is NULL.
 * @retval  SSP_ERR_HW_LOCKED         The lock could not be acquired. The channel is busy.
 * @retval  SSP_ERR_CH_NOT_OPEN       The channel has not been opened. Open the channel first.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
ssp_err_t   R_RSPI_Write (spi_ctrl_t          * const p_ctrl,
                          void const                  * p_src,
                          uint32_t const              length,
                          spi_bit_width_t const       bit_width)
{
    ssp_err_t result;

#if RSPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_src);
    /* Check the data length, should not be 0 */
    SSP_ASSERT(0 != length);
    /* Check channel number */
    if (RSPI_NUM_CHANNELS < p_ctrl->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
    /* Check compatibility in transfer mode*/
    if ((NULL != p_ctrl->p_transfer_tx) || (NULL != p_ctrl->p_transfer_rx))
    {
        /* Since RSPI transmit receive data registers are of 32-bit,
         * RSPI supports only 32-bit data transfer in DTC transfer mode.
         * Note: S124 RSPI data registers are 16-bit only)*/
#if (BSP_CFG_MCU_PART_SERIES == 1)
        if(SPI_BIT_WIDTH_16_BITS != bit_width)
#else
        if(SPI_BIT_WIDTH_32_BITS != bit_width)
#endif
        {
            return SSP_ERR_UNSUPPORTED;
        }
    }
#if (BSP_CFG_MCU_PART_SERIES == 1)
    /*Check bit width*/
    if(SPI_BIT_WIDTH_32_BITS == bit_width)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
#endif
#endif /* if RSPI_CFG_PARAM_CHECKING_ENABLE */

    result = rspi_write_read_common(p_ctrl, p_src, NULL, length, bit_width, RSPI_DO_TX);

    return result;
}

/* End of function R_RSPI_Write(). */

/*************************************************************************************************************//**
 * @brief   This function simultaneously transmits data to a SPI device while receiving data from a SPI device
 *          (full duplex).
 *
 * Implements spi_api_t::writeread
 *          The function performs the following tasks:<BR>
 *          Performs parameter checking and processes error conditions.<BR>
 *          Disable Interrupts.<BR>
 *          Disable the SPI bus.<BR>
 *          Setup data bit width per user request.<BR>
 *          Enable the SPI bus.<BR>
 *          Enable interrupts.<BR>
 *          Start data transmission using transmit buffer empty interrupt.<BR>
 *          Copy data from source buffer to the SPI data register for transmission.<BR>
 *          Receive data from receive buffer full interrupt occurs and copy data to the buffer of destination.<BR>
 *          Complete data transmission and reception via receive buffer full interrupt.<BR>
 * @retval  SSP_SUCCESS               Write operation successfully completed.
 * @retval  SSP_ERR_INVALID_ARGUMENT  Channel number invalid.
 * @retval  SSP_ERR_INVALID_POINTER   A required pointer argument is NULL.
 * @retval  SSP_ERR_HW_LOCKED         The lock could not be acquired. The channel is busy.
 * @retval  SSP_ERR_CH_NOT_OPEN       The channel has not been opened. Open the channel first.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
ssp_err_t  R_RSPI_WriteRead (spi_ctrl_t                  * const p_ctrl,
                             void const                  * p_src,
                             void const                  * p_dest,
                             uint32_t const              length,
                             spi_bit_width_t const       bit_width)
{
    ssp_err_t result;

#if RSPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_src);
    SSP_ASSERT(NULL != p_dest);
    /* Check the data length, should not be 0 */
    SSP_ASSERT(0 != length);
    /* Check channel number */
    if (RSPI_NUM_CHANNELS < p_ctrl->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
    /* Check compatibility in transfer mode*/
    if ((NULL != p_ctrl->p_transfer_tx) || (NULL != p_ctrl->p_transfer_rx))
    {
        /* Since RSPI transmit receive data registers are of 32-bit,
         * RSPI supports only 32-bit data transfer in DTC transfer mode.
         * Note: S124 RSPI data registers are 16-bit only)*/
#if (BSP_CFG_MCU_PART_SERIES == 1)
        if(SPI_BIT_WIDTH_16_BITS != bit_width)
#else
        if(SPI_BIT_WIDTH_32_BITS != bit_width)
#endif
        {
            return SSP_ERR_UNSUPPORTED;
        }
    }
#if (BSP_CFG_MCU_PART_SERIES == 1)
    /*Check bit width*/
    if(SPI_BIT_WIDTH_32_BITS == bit_width)
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
#endif
#endif /* if RSPI_CFG_PARAM_CHECKING_ENABLE */

    result = rspi_write_read_common(p_ctrl, p_src, p_dest, length, bit_width, RSPI_DO_TX_RX);

    return result;
}/* End of function R_RSPI_WriteRead(). */


/***************************************************************************************************************
 * @brief   This function initiates write or read process. Common routine used by RSPI API write or read functions.
 *
 * @retval  SSP_SUCCESS               Operation successfully completed.
 * @retval  SSP_ERR_HW_LOCKED         The lock could not be acquired. The channel is busy.
 * @retval  SSP_ERR_CH_NOT_OPEN       The channel has not been opened. Perform R_RSPI_Open() first
 * @retval  SSP_ERR_INVALID_ARGUMENT  An element of the p_ctrl structure contains an invalid value.
 * @note  This function is reentrant.
 ***************************************************************************************************************/
static ssp_err_t  rspi_write_read_common (spi_ctrl_t                 * const p_ctrl,
                                          void const                         * p_src,
                                          void const                         * p_dest,
                                          uint32_t const                     length,
                                          spi_bit_width_t const              bit_width,
                                          rspi_transfer_operation_t          tx_rx_mode)
{
    uint8_t   channel = p_ctrl->channel;
    ssp_err_t result;
    uint16_t   hw_bit_width;

    if (!g_rspi_controls[channel].channel_opened)
    {
        return SSP_ERR_NOT_OPEN;
    }

    /* Attempt to acquire lock for this transfer operation. Prevents re-entrance conflict. */
    if (SSP_SUCCESS != R_BSP_SoftwareLock(&g_rspi_tcb[channel].resource_lock_tx_rx))
    {
        return SSP_ERR_HW_LOCKED;
    }

    /* Disable all interrupts in ICU. */
    HW_RSPI_IRQ_InterruptEnable (channel, RSPI_ALL_INTR, false);

    g_rspi_tcb[channel].xfr_length         = length;
    g_rspi_tcb[channel].tx_count           = 0;
    g_rspi_tcb[channel].rx_count           = 0;
    g_rspi_tcb[channel].bytes_per_transfer = bit_width;
    g_rspi_tcb[channel].psrc               = (void *) p_src;
    g_rspi_tcb[channel].pdest              = (void *) p_dest;
    g_rspi_tcb[channel].transfer_mode      = tx_rx_mode;

    result                                 = SSP_SUCCESS;

    if (tx_rx_mode & RSPI_DO_TX)
    {
        g_rspi_tcb[channel].do_tx = true;
    }
    else
    {
        g_rspi_tcb[channel].do_tx = false;
    }

    g_rspi_tcb[channel].do_rx_now = false;  /* Initialize receive state flag. */

    if (bit_width == SPI_BIT_WIDTH_8_BITS)
    {
        hw_bit_width = RSPI_SPCMD_SPB_8BIT;
    }
    else if (bit_width == SPI_BIT_WIDTH_16_BITS)
    {
        hw_bit_width = RSPI_SPCMD_SPB_16BIT;
    }
    else  /* Set default to 32 bits access */
    {
        hw_bit_width = RSPI_SPCMD_SPB_32BIT;
    }

    /* Wait for channel to be idle before making changes to registers. */
    while( HW_RSPI_IdleCheck (channel))
    {

    }

    /* Update the SPCMD0 command register with the setting of bit width for this transfer. */
    HW_RSPI_DataBitLength (channel, hw_bit_width);

    /* If slave mode, force CPHA bit in command register to 1 to properly support 'burst' operation. */
    if (0 == HW_RSPI_MasterModeCheck(channel))
    {
    	HW_RSPI_ClockPhaseEven (channel, true);
    }

    /* Clear error sources: the SPSR.MODF, OVRF, and PERF flags. */
    while (HW_RSPI_ErrorsClear(channel))
    {
    	HW_RSPI_RxBufferFull_Underrun_Set (channel);
    }

    /* Reset the write transfer with source and destination pointers */
    if ((NULL != p_ctrl->p_transfer_tx) &&
       ((RSPI_DO_TX == tx_rx_mode) || (RSPI_DO_TX_RX == tx_rx_mode)))
    {
#if (BSP_CFG_MCU_PART_SERIES == 1)
        volatile uint16_t * p_dest_reg = (uint16_t *) HW_RSPI_WriteReadAddrGet(p_ctrl->channel);
#else
        volatile uint32_t * p_dest_reg = (uint32_t *) HW_RSPI_WriteReadAddrGet(p_ctrl->channel);
#endif
        result = p_ctrl->p_transfer_tx->p_api->reset(p_ctrl->p_transfer_tx->p_ctrl, p_src, (void *)p_dest_reg, (uint16_t) length);
        RSPI_ERROR_RETURN((SSP_SUCCESS == result), result);
    }

    /* Reset the read transfer with source and destination pointers */
    if ((NULL != p_ctrl->p_transfer_rx) &&
       ((RSPI_DO_RX == tx_rx_mode) || (RSPI_DO_TX_RX == tx_rx_mode)))
    {
#if (BSP_CFG_MCU_PART_SERIES == 1)
    	volatile uint16_t const * p_src_reg = (uint16_t const *) HW_RSPI_WriteReadAddrGet(p_ctrl->channel);
#else
    	volatile uint32_t const * p_src_reg = (uint32_t const *) HW_RSPI_WriteReadAddrGet(p_ctrl->channel);
#endif
        result = p_ctrl->p_transfer_rx->p_api->reset(p_ctrl->p_transfer_rx->p_ctrl, (void const *)p_src_reg, (void *)p_dest, (uint16_t) length);
        RSPI_ERROR_RETURN((SSP_SUCCESS == result), result);

        /* Enable Tx to generate clock for receiving data */
        if((RSPI_DO_RX == tx_rx_mode) && (NULL != p_ctrl->p_transfer_tx))
        {
        	result = p_ctrl->p_transfer_tx->p_api->reset(p_ctrl->p_transfer_tx->p_ctrl, NULL, NULL, (uint16_t) length);
        	RSPI_ERROR_RETURN((SSP_SUCCESS == result), result);
        }
    }

    HW_RSPI_IdleInterruptDisable (channel);                     /* Disable idle interrupt. */
    HW_RSPI_IrqStatusClear(channel, RSPI_ALL_INTR);             /* Clear all IRQ interrupt status */
    HW_RSPI_IRQ_InterruptEnable (channel, RSPI_ALL_INTR, true); /* Enable all interrupts in ICU. */

    /* Enable transmit buffer empty interrupt, Receive buffer full interrupt,
     * and enable RSPI simultaneously. This will generate an SPTI interrupt,
     * and data transfer will proceed in the ISRs. */
    HW_RSPI_InterruptEnable (channel);

    return result;
}/* End of function R_RSPI_WriteRead(). */

/*************************************************************************************************************//**
 * @brief   This function manages the closing of a channel by the following task.
 *
 * Implements spi_api_t::close
 *          Disables SPI operations by disabling the SPI bus.<BR>
 *          Power off the channel.<BR>
 *          Disables all the associated interrupts.<BR>
 *          Update channel status.<BR>
 * @retval  SSP_SUCCESS              Channel successfully closed.
 * @retval  SSP_ERR_INVALID_POINTER  A required pointer argument is NULL.
 * @retval  SSP_ERR_CH_NOT_OPEN      The channel has not been opened. Open the channel first.
 * @note  This function is reentrant.
 ****************************************************************************************************************/
ssp_err_t  R_RSPI_Close (spi_ctrl_t * const p_ctrl)
{
    uint8_t channel;

#if RSPI_CFG_PARAM_CHECKING_ENABLE
    /* Perform parameter checking */
    SSP_ASSERT(NULL != p_ctrl);
    /* Check channel number */
    if (RSPI_NUM_CHANNELS < p_ctrl->channel)
    {
        /* Invalid channel. */
        return SSP_ERR_INVALID_CHANNEL;
    }
#endif /* if RSPI_CFG_PARAM_CHECKING_ENABLE */

    channel = p_ctrl->channel;

    /* Check to see if the channel is currently initialized. */
    if (false == g_rspi_controls[channel].channel_opened)
    {
        /* This channel is not open so need not be closed. */
        return SSP_ERR_NOT_OPEN;
    }

    /* SPE and SPTIE should be cleared simultaneously. */
    HW_RSPI_TxIntr_SPI_Disable (channel);
    HW_RSPI_Power_On_Off(channel, RSPI_POWER_OFF);  /* Turn off power */
    HW_RSPI_IRQ_InterruptEnable (channel, RSPI_ALL_INTR, false); /* Disable all system interrupts. */
    g_rspi_controls[channel].channel_opened = false;

    /* Close transfer block */
    if (NULL != p_ctrl->p_transfer_rx)
    {
        p_ctrl->p_transfer_rx->p_api->close(p_ctrl->p_transfer_rx->p_ctrl);
    }

    if (NULL != p_ctrl->p_transfer_tx)
    {
        p_ctrl->p_transfer_tx->p_api->close(p_ctrl->p_transfer_tx->p_ctrl);
    }

    /* Release lock for this channel. */
    /* All RSPI channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    R_BSP_HardwareUnlock((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_RSPI0 + channel));

    return SSP_SUCCESS;
}/* End of function R_RSPI_Close(). */

/***************************************************************************************************************
 * @brief   This function determines the RSPI channel SPBR register setting for the requested baud rate.
 *
 *          Returns the actual bit rate that the setting will achieve which may differ from requested.
 *          If the requested bit rate cannot be exactly achieved, the next lower bit rate setting will be applied.
 *          If successful, applies the calculated setting to the SPBR register.
 * @param[in]  Channel     Channel number.
 *             bps_target  The requested baud rate.
 * @param[out]
 * @retval     0           Error conditions.
 * @retval     bps_calc    The actual BPS rate achieved
 * @note Target baud must be >= PCLK/4 to get anything out.
 *       The BRDV[1:0} bits are set to 0 to get the higher bit rate.
 ***************************************************************************************************************/
static uint32_t rspi_baud_set (uint8_t channel, uint32_t bps_target)
{
    uint8_t  spbr_result = 0;
    uint32_t bps_calc    = 0;
    int32_t  n = 0;
    uint32_t clock_mhz = 0;

    /* read the clock frequency */
#if (BSP_CFG_MCU_PART_SERIES == 1)
    g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKB, &clock_mhz);
#else
    g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKA, &clock_mhz);
#endif

    /* Get the register settings for requested baud rate. */
    if ((clock_mhz / bps_target) < 2)
    {
        return 0;   /* baud_bps_target too high for the PCLK. */
    }

    /*
     * From Hardware manual: Bit rate = f / (2(n + 1)(2^N))
     * where:
     *      f = PCLK, n = SPBR setting, N = BRDV bits
     * Solving for n:
     *      n = (((f/(2^N))/2) / bps) - 1
     *
     */

    /* Only calculate for BRDV value of 0 (div/1) to get SPBR setting for the board PCLK.
     * BRDV setting will be done during write/read operations.
     * With N = 0;
     * The formula for BPS is simplified as the following:
     *
     * Bit rate (BPS)  = f / (2(n+1).
     *
     * n = f / (2 * BPS) - 1.
     *
     * */
    n = (int32_t)((int32_t)clock_mhz / (2 * (int32_t) bps_target)) - 1; /* Solve for SPBR setting. */

    if ((n >= 0) && (n <= 255))                         /* Must be <= SPBR register max value where N = 0.
                                                         Must not be negative */
    {
        /* Now plug n back into the formula for BPS and check it. */
        bps_calc = (uint32_t) (clock_mhz / (2 * ((uint32_t)n + 1)));

        if (bps_calc > bps_target)
        {
            n += 1;
            if (n > 0xff)
            {
                return 0; /* Result out of range for the PCLK. */
            }
        }

        spbr_result = (uint8_t)n;

        /* Apply the SPBR register value. */
        HW_RSPI_BitRateSet(channel, spbr_result);
    }
    else
    {
        bps_calc = 0;  /* Result is out of range for the PCLK. */
    }

    return bps_calc;    /* Return the actual BPS rate achieved. */
}/* End of function rspi_baud_set(). */

/*****************************************************************************************************************//**
 * @brief   This function gets the version information of the underlying driver.
 *
 * Implements spi_api_t::versionget
 * @retval  void
 * @note  This function is reentrant.
 ********************************************************************************************************************/
ssp_err_t R_RSPI_VersionGet (ssp_version_t * p_version)
{
#ifdef RSPI_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_version != NULL);
#endif

    p_version->version_id = module_version.version_id;

    return SSP_SUCCESS;
}/* End of function R_RSPI_VersionGet() */

/*****************************************************************************************************************//**
 * @} (end addtogroup SPI)
 ********************************************************************************************************************/

/***************************************************************************************************************
 * @brief   This function is the common ISR handler for SPTI and SPRI interrupts.
 *
 * @param[in]  channel  Channel number to use.
 * @retval  void
 ***************************************************************************************************************/
static void rspi_tx_rx_common (uint8_t channel, rspi_int_t which_irq)
{
    void          * psrc    = g_rspi_tcb[channel].psrc;
    void          * pdest   = g_rspi_tcb[channel].pdest;
    uint16_t      tx_count  = g_rspi_tcb[channel].tx_count;
    uint16_t      rx_count  = g_rspi_tcb[channel].rx_count;
    uint8_t       data_size = g_rspi_tcb[channel].bytes_per_transfer;
    uint32_t      rx_data   = g_rxdata[channel];
    spi_callback_args_t rspi_cb_data;
    static volatile uint32_t timeout = 0xfff;
    spi_ctrl_t  * p_ctrl = (spi_ctrl_t *) g_rspi_tcb[channel].p_context;

    if ((NULL == p_ctrl->p_transfer_rx) && (NULL == p_ctrl->p_transfer_tx))
    {
        /* Service the transmit interrupt or receive interrupt */
        if (tx_count < g_rspi_tcb[channel].xfr_length)   /* Don't write transmit buffer more than length. */
        {
            if (g_rspi_tcb[channel].do_tx)
            {
				/* Transmit the data. TX data register accessed in long words. */
				if (RSPI_BYTE_DATA == data_size)
				{
					HW_RSPI_Write (channel, ((uint8_t *) psrc)[tx_count]);
				}
				else if (RSPI_WORD_DATA == data_size)
				{
					HW_RSPI_Write (channel, ((uint16_t *) psrc)[tx_count]);
				}
#if (BSP_CFG_MCU_PART_SERIES != 1)
				else /* Must be long data */
				{
					HW_RSPI_Write (channel, ((uint32_t *) psrc)[tx_count]);
				}
#endif
			}
			else /* Must be RX only mode, so transmit dummy data for clocking.*/
			{
				/* TX data register accessed in long words. */
				HW_RSPI_Write (channel, RSPI_DUMMY_TXDATA);
			}

			g_rspi_tcb[channel].tx_count++;
		}

		/* Store the received data in user buffer.
		 * Receive data not valid until after first transmission is complete. */
		if (g_rspi_tcb[channel].do_rx_now)
		{
			if (RSPI_BYTE_DATA == data_size)
			{
				((uint8_t *) pdest)[rx_count - 1] = (uint8_t) rx_data;
			}
			else if (RSPI_WORD_DATA == data_size)
			{
				((uint16_t *) pdest)[rx_count - 1] = (uint16_t) rx_data;
			}
			else  /* Must be long data */
			{
				((uint32_t *) pdest)[rx_count - 1] = rx_data;
			}
		}

		/* Check for last data.  */
		if (rx_count == g_rspi_tcb[channel].xfr_length)
		{
			/* Check Idle state before finish */
			while(HW_RSPI_IdleCheck(channel) && timeout)
			{
			   timeout--;
			}

			/* Last data was transferred. */
			HW_RSPI_RxIntrDisable (channel);      /* Disable SPRI interrupt. */
			HW_RSPI_RSPIDisable (channel);        /* Disable RSPI. */
			HW_RSPI_RxBufferFullClear (channel);  /* Clear RSPI receiver buffer full flag. */

			/* Transfer complete. Call the user callback function passing pointer to the result structure. */
			if ((NULL != g_rspi_controls[channel].p_callback))
			{
				rspi_cb_data.channel            = channel;
				rspi_cb_data.event              = SPI_EVENT_TRANSFER_COMPLETE;
				rspi_cb_data.p_context            = g_rspi_controls[channel].p_context;
				g_rspi_controls[channel].p_callback((spi_callback_args_t *) &(rspi_cb_data));

				/* Disable all interrupts */
				HW_RSPI_InterruptDisable (channel);
				/* Disable RX interrupt in the ICU */
				HW_RSPI_IRQ_InterruptEnable (channel, RSPI_RX_INTR, false);
				/* Disable TX interrupt in the ICU */
				HW_RSPI_IRQ_InterruptEnable (channel, RSPI_TX_INTR, false);
			}
			/* Transfer is done, release the lock for this operation. */
			R_BSP_SoftwareUnlock(&g_rspi_tcb[channel].resource_lock_tx_rx);
		}
    }
    else
    {
        if(((which_irq == RSPI_RX_INTR) && ((g_rspi_tcb[channel].transfer_mode == RSPI_DO_TX_RX) || (g_rspi_tcb[channel].transfer_mode == RSPI_DO_RX)))
         || ((rx_count == g_rspi_tcb[channel].xfr_length) && (g_rspi_tcb[channel].transfer_mode == RSPI_DO_TX)))
    	{
			/* Last data was transferred. */
			HW_RSPI_RxIntrDisable (channel);      /* Disable SPRI interrupt. */
			HW_RSPI_RSPIDisable (channel);        /* Disable RSPI. */
			HW_RSPI_RxBufferFullClear (channel);  /* Clear RSPI receiver buffer full flag. */

			/* Transfer complete. Call the user callback function passing pointer to the result structure. */
			if ((NULL != g_rspi_controls[channel].p_callback))
			{
				rspi_cb_data.channel            = channel;
				rspi_cb_data.event              = SPI_EVENT_TRANSFER_COMPLETE;
				rspi_cb_data.p_context            = g_rspi_controls[channel].p_context;
				g_rspi_controls[channel].p_callback((spi_callback_args_t *) &(rspi_cb_data));

				/* Disable all interrupts */
				HW_RSPI_InterruptDisable (channel);
				/* Disable RX interrupt in the ICU */
				HW_RSPI_IRQ_InterruptEnable (channel, RSPI_RX_INTR, false);
				/* Disable TX interrupt in the ICU */
				HW_RSPI_IRQ_InterruptEnable (channel, RSPI_TX_INTR, false);
			}
			/* Transfer is done, release the lock for this operation. */
			R_BSP_SoftwareUnlock(&g_rspi_tcb[channel].resource_lock_tx_rx);
    	}
    }
} /* End rspi_transmit_common() */

/***************************************************************************************************************
 * @brief   This function is the ISR function for RSPI receive buffer full (SPRI) interrupts for channel 0.
 *          Each ISR calls a common function but passes its channel number.
 * @retval  void
 ***************************************************************************************************************/
void spi0_rxi_isr (void)
{
    SF_CONTEXT_SAVE
    g_rxdata[0] = HW_RSPI_Read (0); /* Need to read RX data register as soon as possible. */
    g_rspi_tcb[0].rx_count++;
    rspi_tx_rx_common(0, RSPI_RX_INTR);

    /* Clear RX interrupt status in ICU. */
    HW_RSPI_IrqStatusClear (0, RSPI_RX_INTR);
    SF_CONTEXT_RESTORE
}

/* End spi0_rxi_isr */

/***************************************************************************************************************
 * @brief   This function is the ISR function for RSPI receive buffer full (SPRI) interrupts for channel 1.
 *          Each ISR calls a common function but passes its channel number.
 * @retval  void
 ***************************************************************************************************************/
void spi1_rxi_isr (void)
{
    SF_CONTEXT_SAVE
    g_rxdata[1] = HW_RSPI_Read (1); /* Need to read RX data register as soon as possible. */
    g_rspi_tcb[1].rx_count++;
    rspi_tx_rx_common(1, RSPI_RX_INTR);
    /* Clear RX interrupt status in ICU. */
    HW_RSPI_IrqStatusClear (1, RSPI_RX_INTR);
    SF_CONTEXT_RESTORE
}

/* End spi1_rxi_isr */

/***************************************************************************************************************
 * @brief   This is the ISR function for RSPI RSPI SPTI transmit buffer empty (SPTI) interrupts for channel 0.
 *          Each ISR calls a common function but passes its channel number.
 * @retval  void
 ***************************************************************************************************************/
void spi0_txi_isr (void)
{
    SF_CONTEXT_SAVE
    g_rxdata[0] = HW_RSPI_Read (0); /* Need to read RX data register as soon as possible. */
    /* If master mode then disable further SPTI interrupts on first transmit.
     * If slave mode then we do two transmits to fill the double buffer, then disable SPTI interrupts.
     * The receive interrupt will handle any remaining data. */
    if ((HW_RSPI_MasterModeCheck(0)) || (g_rspi_tcb[0].tx_count > 0))
    {
        HW_RSPI_TxIntrDisable(0);  /* Disable SPTI interrupt. */
    }

	rspi_tx_rx_common(0, RSPI_TX_INTR);  /* Process the data in the common handler. */

	if (g_rspi_tcb[0].transfer_mode & RSPI_DO_RX)
	{
		/* Count was incremented in the call to rspi_tx_rx_common. */
		if ((HW_RSPI_MasterModeCheck(0)) || (g_rspi_tcb[0].tx_count > 1))
		{
			g_rspi_tcb[0].do_rx_now = true; /* Enables saving of receive data on next receive interrupt. */
		}
	}

    /* Clear TX interrupt status in ICU. */
    HW_RSPI_IrqStatusClear (0, RSPI_TX_INTR);
    SF_CONTEXT_RESTORE
} /* End spi0_txi_isr */

/***************************************************************************************************************
 * @brief   This is the ISR function for RSPI RSPI SPTI transmit buffer empty (SPTI) interrupts for channel 1.
 *          Each ISR calls a common function but passes its channel number.
 * @retval  void
 ***************************************************************************************************************/
void spi1_txi_isr (void)
{
    SF_CONTEXT_SAVE
    g_rxdata[1] = HW_RSPI_Read(1); /* Need to read RX data register as soon as possible. */

    if ((HW_RSPI_MasterModeCheck(1)) || (g_rspi_tcb[1].tx_count > 0))
    {
        HW_RSPI_TxIntrDisable(1);  /* Disable SPTI interrupt. */
    }

    rspi_tx_rx_common(1, RSPI_TX_INTR);  /* Process the data in the common handler. */

    if (g_rspi_tcb[1].transfer_mode & RSPI_DO_RX)
    {
        /* Count was incremented in the call to rspi_tx_rx_common. */
        if ((HW_RSPI_MasterModeCheck(1)) || (g_rspi_tcb[1].tx_count > 1))
        {
            g_rspi_tcb[1].do_rx_now = true; /* Enables saving of receive data on next receive interrupt. */
        }
    }
   /* Clear TX interrupt status in ICU. */
   HW_RSPI_IrqStatusClear (1, RSPI_TX_INTR);
   SF_CONTEXT_RESTORE
}/* End of spi1_txi_isr */

/***************************************************************************************************************
 * @brief   This function is common ISR handler for  RSPI SPEI-error interrupts.
 *
 * @param[in]  channel  channel number to use.
 * @retval  void
 ***************************************************************************************************************/
static void rspi_spei_isr_common (uint8_t channel)
{
    uint8_t       status_flags = HW_RSPI_Status(channel);
    spi_callback_args_t rspi_cb_data;

    /* Identify and clear error condition. */
    if (status_flags & RSPI_SPSR_OVRF) /* Overrun error occurred. */
    {
        rspi_cb_data.event                    = SPI_EVENT_ERR_OVERRUN;
        /* Clear error source: Overrun flag. */
        HW_RSPI_OverRunErrorClear (channel);
    }
    else if (status_flags & RSPI_SPSR_MODF)
    {
    	/** If Mode Flag is set and Underflow flag is also set, then its an underrun error*/
    	if (status_flags & RSPI_SPSR_UDRF)
    	{
    		rspi_cb_data.event                = SPI_EVENT_ERR_MODE_UNDERRUN;
    		/* Clear error source: Underflow error flag. */
    		HW_RSPI_UnderflowErrorClear (channel);
    	}
    	/** If Mode Flag is set and Underflow flag is not set, then its a mode fault*/
    	else
    	{
    		rspi_cb_data.event                = SPI_EVENT_ERR_MODE_FAULT;
    	}

        /* Clear error source: Mode fault error flag. */
        HW_RSPI_ModeFaultErrorClear (channel);
    }
    else if (status_flags & RSPI_SPSR_PERF)
    {
        rspi_cb_data.event                    = SPI_EVENT_ERR_PARITY;
        /* Clear error source: parity error flag. */
        HW_RSPI_ParityErrorrClear (channel);
    }

    /* Disable the RSPI channel (terminates the transfer operation). */
    HW_RSPI_RxIntrDisable (channel);  /* Disable SPRI interrupt. */
    HW_RSPI_RSPIDisable (channel);    /* Disable RSPI function. */

    /* Call the user callback function passing pointer to the result structure. */
    if (NULL != g_rspi_controls[channel].p_callback)
    {
        rspi_cb_data.channel = channel;
        g_rspi_controls[channel].p_callback((spi_callback_args_t *) &(rspi_cb_data));
    }

    /* Disable all interrupts */
    HW_RSPI_InterruptDisable (channel);
    /* Disable ERR interrupt in the ICU */
    HW_RSPI_IRQ_InterruptEnable (channel, RSPI_ERR_INTR, false);

    /* error condition occurs, release the software lock for this operation. */
    R_BSP_SoftwareUnlock(&g_rspi_tcb[channel].resource_lock_tx_rx);

    /* Release hardware lock for this channel. */
    /* All RSPI channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    R_BSP_HardwareUnlock((bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_RSPI0 + channel));
} /* End rspi_spei_isr_common() */

/***************************************************************************************************************
 * @brief   This function is the ISR function for RSPI error (SPEI) interrupts for channel 0.
 *          Each ISR calls a common function but passes its channel number.
 * @retval  void
 ***************************************************************************************************************/
void spi0_eri_isr (void)
{
    SF_CONTEXT_SAVE
    rspi_spei_isr_common (0);
    /* Clear ERR interrupt status in the ICU */
    HW_RSPI_IrqStatusClear(0, RSPI_ERR_INTR);
    SF_CONTEXT_RESTORE
} /* End of spi0_eri_isr */

/***************************************************************************************************************
 * @brief   This function is the ISR function for RSPI error (SPEI) interrupts for channel 1.
 *          Each ISR calls a common function but passes its channel number.
 * @retval  void
 ***************************************************************************************************************/
void spi1_eri_isr (void)
{
    SF_CONTEXT_SAVE
    rspi_spei_isr_common (1);
    /* Clear ERR interrupt status in the ICU */
    HW_RSPI_IrqStatusClear(1, RSPI_ERR_INTR);
   SF_CONTEXT_RESTORE
}/* End of spi1_eri_isr */

/***************************************************************************************************************
 * @brief   This function sets the common configuration for a SPI channel.
 * @param[in]  channel  Channel number to use.
 * @param[in]  p_cfg    Pointer to SPI configuration structure.
 * @retval  void
 ***************************************************************************************************************/
static void rspi_common_config_set (uint8_t channel, spi_cfg_t  const * const p_cfg)
{
    /* Process the user configuration to update the local configuration image */
    /* Set SPCR-MSTR -- set control register-master/slave mode */
    if (SPI_MODE_MASTER == p_cfg->operating_mode)
    {
        /* Set SPI operating mode to master or slave. True-master; false-slave */
    	HW_RSPI_OperatingModeMaster (channel, true);
    }
    else
    {
        HW_RSPI_OperatingModeMaster (channel, false);
    }

    /* Set SPCMD0-CPHA bit -- set RSPCK phase */
    if (SPI_CLK_PHASE_EDGE_EVEN == p_cfg->clk_phase)
    {
        /* Set clock phase. True-even, false-odd */
    	HW_RSPI_ClockPhaseEven (channel, true);
    }
    else
    {
        HW_RSPI_ClockPhaseEven (channel, false);
    }

    /* Set SPCMD0-CPOL bit -- set RSPCK polarity */
    if (SPI_CLK_POLARITY_HIGH == p_cfg->clk_polarity)
    {
        /* Set clock polarity. True-high at idle, false-low at idle */
    	HW_RSPI_ClockPolarityHigh (channel, true);
    }
    else
    {
        HW_RSPI_ClockPolarityHigh (channel, false);
    }

    /* Set SPCR-MODFEN bit -- set control register mode fault error detection enable */
    if (SPI_MODE_FAULT_ERROR_ENABLE == p_cfg->mode_fault)
    {
        /* Set mode fault detection. True-on; false-off */
    	HW_RSPI_ModeFaultDetectionOn (channel, true);
    }
    else
    {
        HW_RSPI_ModeFaultDetectionOn (channel, false);
    }

    /* Set SPCMD0-LSBF bit -- set command register0 LSB/MSB first mode */
    if (SPI_BIT_ORDER_LSB_FIRST == p_cfg->bit_order)
    {
        /* Set Bit order to MSB or LSB. True-LSB; false-MSB */
    	HW_RSPI_BitOrderLSB (channel, true);
    }
    else
    {
        HW_RSPI_BitOrderLSB (channel, false);
    }
} /* End of rspi_common_config_set() */

/***************************************************************************************************************
 * @brief   This function sets the extended configuration for a SPI channel.
 * @param[in]  channel  Channel number to use.
 * @param[in]  p_cfg    Pointer to SPI configuration structure.
 * @retval  void
 ***************************************************************************************************************/
static void rspi_extended_config_set (uint8_t channel, spi_cfg_t  const * const p_cfg)
{
    /* Process the user extended configuration to update the local configuration image */
    spi_on_rspi_cfg_t * p_rspi_cfg = (spi_on_rspi_cfg_t *) p_cfg->p_extend;

    /* Set SPCR-SPMS bit -- set RSPI SPI or Clock synchronous mode */
    if (RSPI_OPERATION_CLK_SYN == p_rspi_cfg->rspi_clksyn)
    {
    	HW_RSPI_OperationClkSyn (channel, true);
    }
    else
    {
        HW_RSPI_OperationClkSyn (channel, false);
    }

    /* Set SPCR-TXMD bit -- set full duplex synchronous communication mode or transmit only mode */
    if (RSPI_COMMUNICATION_TRANSMIT_ONLY == p_rspi_cfg->rspi_comm)
    {
    	HW_RSPI_CommunicationTransmitOnly (channel, true);
    }
    else
    {
        HW_RSPI_CommunicationTransmitOnly (channel, false);
    }


    /* Sets the slave select polarity level */
    if (RSPI_SSLP_HIGH == p_rspi_cfg->ssl_polarity.rspi_ssl0)
    {
    	HW_RSPI_SlaveSelectPolarity (channel, RSPI_SSL_SELECT_SSL0, true);
    }
    else
    {
        HW_RSPI_SlaveSelectPolarity (channel, RSPI_SSL_SELECT_SSL0, false);
    }


    if (RSPI_SSLP_HIGH == p_rspi_cfg->ssl_polarity.rspi_ssl1)
    {
    	HW_RSPI_SlaveSelectPolarity (channel, RSPI_SSL_SELECT_SSL1, true);
    }
    else
    {
        HW_RSPI_SlaveSelectPolarity (channel, RSPI_SSL_SELECT_SSL1, false);
    }

    if (RSPI_SSLP_HIGH == p_rspi_cfg->ssl_polarity.rspi_ssl2)
    {
    	HW_RSPI_SlaveSelectPolarity (channel, RSPI_SSL_SELECT_SSL2, true);
    }
    else
    {
        HW_RSPI_SlaveSelectPolarity (channel, RSPI_SSL_SELECT_SSL2, false);
    }

    if (RSPI_SSLP_HIGH == p_rspi_cfg->ssl_polarity.rspi_ssl3)
    {
    	HW_RSPI_SlaveSelectPolarity (channel, RSPI_SSL_SELECT_SSL3, true);
    }
    else
    {
        HW_RSPI_SlaveSelectPolarity (channel, RSPI_SSL_SELECT_SSL3, false);
    }

    /* Set SPPCR-SPLP bit -- set loopback mode with inverted data */
    if (RSPI_LOOPBACK1_INVERTED_DATA == p_rspi_cfg->loopback.rspi_loopback1)
    {
    	HW_RSPI_loopback1 (channel, true);
    }
    else
    {
        HW_RSPI_loopback1 (channel, false);
    }

    /* Set SPPCR-SPLP2 bit -- set loopback2 mode with not inverted data */
    if (RSPI_LOOPBACK2_NOT_INVERTED_DATA == p_rspi_cfg->loopback.rspi_loopback2)
    {
    	HW_RSPI_loopback2 (channel, true);
    }
    else
    {
        HW_RSPI_loopback2 (channel, false);
    }

    /* Set SPPCR-MOIFV bit -- set mosi idle fixed value */
    if (RSPI_MOSI_IDLE_FIXED_VAL_HIGH == p_rspi_cfg->mosi_idle.rspi_mosi_idle_fixed_val)
    {
    	HW_RSPI_MOSIIdleLevelHigh (channel, true);
    }
    else
    {
        HW_RSPI_MOSIIdleLevelHigh (channel, false);
    }

    /* Set SPPCR-MOIFE bit -- set mosi idle value fixing enable */
    if (RSPI_MOSI_IDLE_VAL_FIXING_ENABLE == p_rspi_cfg->mosi_idle.rspi_mosi_idle_val_fixing)
    {
        HW_RSPI_MOSIIdleEnable (channel, true);
    }
    else
    {
        HW_RSPI_MOSIIdleEnable (channel, false);
    }

    /* Set SPCR2-SPPE bit -- enable/disable parity */
    if (RSPI_PARITY_STATE_ENABLE == p_rspi_cfg->parity.rspi_parity)
    {
    	HW_RSPI_ParityEnable (channel, true);
    }
    else
    {
        HW_RSPI_ParityEnable (channel, false);
    }

    /* Set SPCR2-SPOE bit -- select even/odd parity */
    if (RSPI_PARITY_MODE_ODD == p_rspi_cfg->parity.rspi_parity_mode)
    {
    	HW_RSPI_ParityOdd (channel, true);
    }
    else
    {
        HW_RSPI_ParityOdd (channel, false);
    }

    /* Set SPCMD0-SSLA bits -- select SSL signal assertion setting */
    HW_RSPI_SlaveSelect (channel, p_rspi_cfg->ssl_select);

#if (BSP_CFG_MCU_PART_SERIES != 1)
    /* Set SPCMD0-SSLKP bits -- set SSL signal level keeping */
    if (RSPI_SSL_LEVEL_KEEP == p_rspi_cfg->ssl_level_keep)
    {
    	HW_RSPI_SlaveSelectLevelKeep (channel, true);
    }
    else
    {
        HW_RSPI_SlaveSelectLevelKeep (channel, false);
    }
#endif

#if (BSP_CFG_MCU_PART_SERIES == 1)
    /* SPCMD0-SPB -- set data length to 16 bits as the default */
    HW_RSPI_DataBitLength (channel, RSPI_SPCMD_SPB_16BIT);
#else
    /* SPCMD0-SPB -- set data length to 32 bits as the default */
    HW_RSPI_DataBitLength (channel, RSPI_SPCMD_SPB_32BIT);
#endif

    /* Set SPCMD0-SCKDEN bit & SPCKD register -- set RSPI RSPCK delay enable and clock delay register */
    if (RSPI_CLOCK_DELAY_STATE_ENABLE == p_rspi_cfg->clock_delay.rspi_clock_delay_state)
    {
    	HW_RSPI_ClockDelay (channel, true, p_rspi_cfg->clock_delay.rspi_clock_delay_count);
    }

    /* Set SPCMD0-SLNDEN bit & SSLND register -- RSPI SSL negation delay and slave select negation delay register */
    if (RSPI_SSL_NEGATION_DELAY_ENABLE == p_rspi_cfg->ssl_neg_delay.rspi_ssl_neg_delay_state)
    {
        HW_RSPI_SlaveSelectNegationDelay (channel, true, p_rspi_cfg->ssl_neg_delay.rspi_ssl_neg_delay_count);
    }

    /* Set SPCMD0-SPNDEN bit & SPND register -- set RSPI next access delay enable and next-access delay register */
    if (RSPI_NEXT_ACCESS_DELAY_STATE_ENABLE == p_rspi_cfg->access_delay.rspi_next_access_delay_state)
    {
    	HW_RSPI_NextAccessDelay (channel, true, p_rspi_cfg->access_delay.rspi_next_access_delay_count);
    }
} /* End of rspi_extended_config_set */

/***************************************************************************************************************
 * @brief   This function sets the necessary default configuration for a SPI channel.
 * @param[in]  channel  Channel number to use.
 * @param[in]  p_cfg  Pointer to SPI configuration structure.
 * @retval  void
 ***************************************************************************************************************/
static void rspi_default_config_set (uint8_t channel)
{
    /* Clear SPCMD0 command register and the Data Control register */
	HW_RSPI_CommandClear (channel);
	HW_RSPI_DataControlClear (channel);

#if (BSP_CFG_MCU_PART_SERIES != 1)
	/* Default Setting - Set RSPI data control register (SPDCR).*/
    /* Force to long word data access as default. */
	HW_RSPI_DefaultDataBitLength (channel);
#endif

	/* Set default data frames to use 1 frame for transmission */
	HW_RSPI_DefaultDataFrame (channel);

    /* Default Setting - Set maximum clock rate (BRDV=0), user does not have the option to
     * configure this parameter from the configuration file */
	HW_RSPI_DefaultBRDV (channel);

    /*  Default Setting - set SPSCR (sequence control register) to use 1 sequence.
     *  We don't support multiple sequences at this time. */
	HW_RSPI_DefaultSequence (channel);
} /* End of rspi_default_config_set */


/*******************************************************************************************************************//**
 * @brief Configures RSPI related transfer drivers (if enabled).
 * @param[in]     p_cfg   Pointer to RSPI specific configuration structure
 * @retval        SSP_SUCCESS
 **********************************************************************************************************************/
static ssp_err_t    rspi_transfer_open     (spi_cfg_t const * const p_cfg)
{
	ssp_err_t result = SSP_SUCCESS;

    if (NULL != p_cfg->p_transfer_rx)
    {
        /** Set default transfer info and open receive transfer module, if enabled. */
#if (SCI_SPI_CFG_PARAM_CHECKING_ENABLE)
        SSP_ASSERT(NULL != p_cfg->p_transfer_rx->p_api);
        SSP_ASSERT(NULL != p_cfg->p_transfer_rx->p_ctrl);
        SSP_ASSERT(NULL != p_cfg->p_transfer_rx->p_cfg);
        SSP_ASSERT(NULL != p_cfg->p_transfer_rx->p_cfg->p_info);

        IRQn_Type irq = r_rspi_select_irqn(p_cfg->channel, RSPI_RX_INTR);
        SSP_ASSERT(BSP_MAX_NUM_IRQn != irq);
#endif
        transfer_info_t * p_info = p_cfg->p_transfer_rx->p_cfg->p_info;
        p_info->mode = TRANSFER_MODE_NORMAL;
        p_info->dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
        p_info->src_addr_mode = TRANSFER_ADDR_MODE_FIXED;
        p_info->irq = TRANSFER_IRQ_END;
#if (BSP_CFG_MCU_PART_SERIES == 1)
        p_info->size = TRANSFER_SIZE_2_BYTE;
#else
        p_info->size = TRANSFER_SIZE_4_BYTE;
#endif

        transfer_cfg_t cfg = *(p_cfg->p_transfer_rx->p_cfg);
        cfg.activation_source = r_rspi_rxi_event_lookup(p_cfg->channel);
        cfg.auto_enable = false;
        cfg.p_callback  = NULL;
        result = p_cfg->p_transfer_rx->p_api->open(p_cfg->p_transfer_rx->p_ctrl, &cfg);
        RSPI_ERROR_RETURN((SSP_SUCCESS == result), result);
    }

    if (NULL != p_cfg->p_transfer_tx)
    {
    	/** Set default transfer info and open transmit transfer module, if enabled. */
#if (SCI_SPI_CFG_PARAM_CHECKING_ENABLE)
        SSP_ASSERT(NULL != p_cfg->p_transfer_tx->p_api);
        SSP_ASSERT(NULL != p_cfg->p_transfer_tx->p_ctrl);
        SSP_ASSERT(NULL != p_cfg->p_transfer_tx->p_cfg);
        SSP_ASSERT(NULL != p_cfg->p_transfer_tx->p_cfg->p_info);

        IRQn_Type irq = r_rspi_select_irqn(p_cfg->channel, RSPI_TX_INTR);
        SSP_ASSERT(BSP_MAX_NUM_IRQn != irq);
#endif
        transfer_info_t * p_info = p_cfg->p_transfer_tx->p_cfg->p_info;
        p_info->mode = TRANSFER_MODE_NORMAL;
        p_info->dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
        p_info->src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
        p_info->irq = TRANSFER_IRQ_END;
#if (BSP_CFG_MCU_PART_SERIES == 1)
        p_info->size = TRANSFER_SIZE_2_BYTE;
#else
        p_info->size = TRANSFER_SIZE_4_BYTE;
#endif

        transfer_cfg_t cfg = *(p_cfg->p_transfer_tx->p_cfg);
        cfg.activation_source = r_rspi_txi_event_lookup(p_cfg->channel);
        cfg.auto_enable = false;
        cfg.p_callback  = NULL;
        result = p_cfg->p_transfer_tx->p_api->open(p_cfg->p_transfer_tx->p_ctrl, &cfg);
    }

    return result;
}

/* End of file R_RSPI */
