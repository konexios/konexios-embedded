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
 * File Name    : ssp_common_api.h
 * Description  : Common definitions used by all AMS layers.  Includes error codes and version structure.
 **********************************************************************************************************************/

#ifndef SSP_COMMON_API_H
#define SSP_COMMON_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <assert.h>

/* Includes board and MCU related header files. */
#include "bsp_api.h"

/* Includes SSP version macros. */
#include "ssp_version.h"

/*******************************************************************************************************************//**
 * @ingroup Common_Error_Codes
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** This macro is used to suppress compiler messages about a parameter not being used in a function. The nice thing
 * about using this implementation is that it does not take any extra RAM or ROM. */
#define SSP_PARAMETER_NOT_USED(p) (void) ((p))

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Common error codes */
typedef enum e_ssp_err
{
    SSP_SUCCESS                     = 0,

    SSP_ERR_ASSERTION               = 1,        ///< A critical assertion has failed
    SSP_ERR_INVALID_POINTER         = 2,        ///< Pointer points to invalid memory location
    SSP_ERR_INVALID_ARGUMENT        = 3,        ///< Invalid input parameter
    SSP_ERR_INVALID_CHANNEL         = 4,        ///< Selected channel does not exist
    SSP_ERR_INVALID_MODE            = 5,        ///< Unsupported or incorrect mode
    SSP_ERR_UNSUPPORTED             = 6,        ///< Selected mode not supported by this API
    SSP_ERR_NOT_OPEN                = 7,        ///< Requested channel is not configured or API not open
    SSP_ERR_IN_USE                  = 8,        ///< Channel/peripheral is running/busy
    SSP_ERR_OUT_OF_MEMORY           = 9,        ///< Allocate more memory in the driver's cfg.h
    SSP_ERR_HW_LOCKED               = 10,       ///< Hardware is locked
    SSP_ERR_IRQ_BSP_DISABLED        = 11,       ///< IRQ not enabled in BSP
    SSP_ERR_OVERFLOW                = 12,       ///< Hardware overflow
    SSP_ERR_UNDERFLOW               = 13,       ///< Hardware underflow
    SSP_ERR_ALREADY_OPEN            = 14,       ///< Requested channel is already open in a different configuration
    SSP_ERR_APPROXIMATION           = 15,       ///< Could not set value to exact result
    SSP_ERR_CLAMPED                 = 16,       ///< Value had to be limited for some reason
    SSP_ERR_INVALID_RATE            = 17,       ///< Selected rate could not be met
    SSP_ERR_ABORTED                 = 18,       ///< An operation was aborted
    SSP_ERR_NOT_ENABLED             = 19,       ///< Requested operation is not enabled
    SSP_ERR_TIMEOUT                 = 20,       ///< Timeout error
    SSP_ERR_INVALID_BLOCKS          = 21,       ///< Invalid number of blocks supplied
    SSP_ERR_INVALID_ADDRESS         = 22,       ///< Invalid address supplied
    SSP_ERR_INVALID_SIZE            = 23,       ///< Invalid size/length supplied for operation
    SSP_ERR_WRITE_FAILED            = 24,       ///< Write operation failed
    SSP_ERR_ERASE_FAILED            = 25,       ///< Erase operation failed
    SSP_ERR_INVALID_CALL            = 26,       ///< Invalid function call is made
    SSP_ERR_INVALID_HW_CONDITION    = 27,       ///< Detected hardware is in invalid condition

    /** Start of RTOS only error codes */
    SSP_ERR_INTERNAL                = 100,      ///< Internal error
    SSP_ERR_WAIT_ABORTED            = 101,      ///< Wait

    /** Start of UART specific */
    SSP_ERR_FRAMING                 = 200,      ///< Framing error occurs
    SSP_ERR_BREAK_DETECT            = 201,      ///< Break signal detects
    SSP_ERR_PARITY                  = 202,      ///< Parity error occurs
    SSP_ERR_RXBUF_OVERFLOW          = 203,      ///< Receive queue overflow
    SSP_ERR_QUEUE_UNAVAILABLE       = 204,      ///< Can't open s/w queue
    SSP_ERR_INSUFFICIENT_SPACE      = 205,      ///< Not enough space in transmission circular buffer
    SSP_ERR_INSUFFICIENT_DATA       = 206,      ///< Not enough data in receive circular buffer

    /** Start of SPI specific */
    SSP_ERR_TRANSFER_ABORTED        = 300,      ///< The data transfer was aborted.
    SSP_ERR_MODE_FAULT              = 301,      ///< Mode fault error.
    SSP_ERR_READ_OVERFLOW           = 302,      ///< Read overflow.
    SSP_ERR_SPI_PARITY              = 303,      ///< Parity error.
    SSP_ERR_OVERRUN                 = 304,      ///< Overrun error.

    /** Start of CGC Specific */
    SSP_ERR_CLOCK_INACTIVE          = 400,      ///< Inactive clock specified as system clock.
    SSP_ERR_CLOCK_ACTIVE            = 401,      ///< Active clock source cannot be modified without stopping first.
    SSP_ERR_STABILIZED              = 402,      ///< Clock has stabilized after its been turned on/off
    SSP_ERR_NOT_STABILIZED          = 403,      ///< Clock has not stabilized after its been turned on/off
    SSP_ERR_MAIN_OSC_INACTIVE       = 404,      ///< PLL initialization attempted when main osc is turned off
    SSP_ERR_OSC_STOP_DET_ENABLED    = 405,      ///< Illegal attempt to stop LOCO when Oscillation stop is enabled
    SSP_ERR_OSC_STOP_DETECTED       = 406,      ///< The Oscillation stop detection status flag is set
    SSP_ERR_OSC_STOP_CLOCK_ACTIVE   = 407,      ///< Attempt to clear Oscillation Stop Detect Status with PLL/MAIN_OSC active
    SSP_ERR_CLKOUT_EXCEEDED         = 408,      ///< Output on target output clock pin exceeds maximum supported limit
    SSP_ERR_USB_MODULE_ENABLED      = 409,      ///< USB clock configure request with USB Module enabled
    SSP_ERR_HARDWARE_TIMEOUT        = 410,      ///< A register read or write timed out

    /** Start of FLASH Specific */
    SSP_ERR_PE_FAILURE              = 500,      ///< Unable to enter Programming mode.
    SSP_ERR_CMD_LOCKED              = 501,      ///< Peripheral in command locked state
    SSP_ERR_FCLK                    = 502,      ///< FCLK must be >= 4 MHz

    /** Start of CAC Specific */
    SSP_ERR_INVALID_CAC_REF_CLOCK   = 600,      ///< Measured clock rate < reference clock rate

    /** Start of GLCD Specific */
    SSP_ERR_CLOCK_GENERATION        = 1000,     ///< Clock cannot be specified as system clock
    SSP_ERR_INVALID_TIMING_SETTING  = 1001,     ///< Invalid timing parameter
    SSP_ERR_INVALID_LAYER_SETTING   = 1002,     ///< Invalid layer parameter
    SSP_ERR_INVALID_ALIGNMENT       = 1003,     ///< Invalid memory alignment found
    SSP_ERR_INVALID_GAMMA_SETTING   = 1004,     ///< Invalid gamma correction parameter
    SSP_ERR_INVALID_LAYER_FORMAT    = 1005,     ///< Invalid color format in layer
    SSP_ERR_INVALID_UPDATE_TIMING   = 1006,     ///< Invalid timing for register update
    SSP_ERR_INVALID_CLUT_ACCESS     = 1007,     ///< Invalid access to CLUT entry
    SSP_ERR_INVALID_FADE_SETTING    = 1008,     ///< Invalid fade-in/fade-out setting

    /** Start of JPEG Specific */
    SSP_ERR_JPEG_ERR                                    = 1100,       ///< JPEG error
    SSP_ERR_JPEG_SOI_NOT_DETECTED                       = 1101,       ///< SOI not detected until EOI detected.
    SSP_ERR_JPEG_SOF1_TO_SOFF_DETECTED                  = 1102,       ///< SOF1 to SOFF detected.
    SSP_ERR_JPEG_UNSUPPORTED_PIXEL_FORMAT               = 1103,       ///< Unprovided pixel format detected.
    SSP_ERR_JPEG_SOF_ACCURACY_ERROR                     = 1104,       ///< SOF accuracy error: other than 8 detected.
    SSP_ERR_JPEG_DQT_ACCURACY_ERROR                     = 1105,       ///< DQT accuracy error: other than 0 detected.
    SSP_ERR_JPEG_COMPONENT_ERROR1                       = 1106,       ///< Component error1: the number of SOF0 header components detected is other than 1,3,or 4.
    SSP_ERR_JPEG_COMPONENT_ERROR2                       = 1107,       ///< Component error2: the number of components differs between SOF0 header and SOS.
    SSP_ERR_JPEG_SOF0_DQT_DHT_NOT_DETECTED              = 1108,       ///< SOF0, DQT, and DHT not detected when SOS detected.
    SSP_ERR_JPEG_SOS_NOT_DETECTED                       = 1109,       ///< SOS not detected: SOS not detected until EOI detected.
    SSP_ERR_JPEG_EOI_NOT_DETECTED                       = 1110,       ///< EOI not detected (default)
    SSP_ERR_JPEG_RESTART_INTERVAL_DATA_NUMBER_ERROR     = 1111,       ///< Restart interval data number error detected.
    SSP_ERR_JPEG_IMAGE_SIZE_ERROR                       = 1112,       ///< Image size error detected.
    SSP_ERR_JPEG_LAST_MCU_DATA_NUMBER_ERROR             = 1113,       ///< Last MCU data number error detected.
    SSP_ERR_JPEG_BLOCK_DATA_NUMBER_ERROR                = 1114,       ///< Block data number error detected.
    SSP_ERR_JPEG_BUFFERSIZE_NOT_ENOUGH                  = 1115,       ///< User provided buffer size not enough
    SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE                 = 1116,       ///< JPEG Image size is not aligned with MCU

	/** Start of touch panel framework specific */
	SSP_ERR_CALIBRATE_FAILED        = 1200,     ///< Calibration failed

    /** Start of Message framework specific */
    SSP_ERR_NO_MORE_BUFFER          = 2000,     ///< No more buffer found in the memory block pool
	SSP_ERR_ILLEGAL_BUFFER_ADDRESS  = 2001,     ///< Buffer address is out of block memory pool
	SSP_ERR_INVALID_WORKBUFFER_SIZE = 2002,     ///< Work buffer size is invalid
	SSP_ERR_INVALID_MSG_BUFFER_SIZE = 2003,     ///< Message buffer size is invalid
	SSP_ERR_TOO_MANY_BUFFERS        = 2004,     ///< Number of buffer is too many
	SSP_ERR_NO_SUBSCRIBER_FOUND     = 2005,     ///< No message subscriber found
	SSP_ERR_MESSAGE_QUEUE_EMPTY     = 2006,     ///< No message found in the message queue
	SSP_ERR_MESSAGE_QUEUE_FULL      = 2007,     ///< No room for new message in the message queue
	SSP_ERR_ILLEGAL_SUBSCRIBER_LISTS= 2008,     ///< Message subscriber lists is illegal
	SSP_ERR_BUFFER_RELEASED         = 2009,     ///< Buffer has been released

    /** Start of 2DG Driver specific */
    SSP_ERR_D2D_ERROR_INIT          = 3000,     ///< Dave/2d has an error in the initialization
    SSP_ERR_D2D_ERROR_DEINIT        = 3001,     ///< Dave/2d has an error in the initialization
    SSP_ERR_D2D_ERROR_RENDERING     = 3002,     ///< Dave/2d has an error in the rendering
    SSP_ERR_D2D_ERROR_SIZE          = 3003,     ///< Dave/2d has an error in the rendering

    /** Start of BYTEQ library specific */
    SSP_ERR_QUEUE_FULL             = 10000,     ///< Queue is full, cannot queue another data
    SSP_ERR_QUEUE_EMPTY            = 10001,     ///< Queue is empty, no data to dequeue

    /** Sensor count overflowed when performing CTSU scan. @note User must clear the CTSUSCOVF bit manually. */
	SSP_ERR_CTSU_SC_OVERFLOW = 0x8010,
	/** Reference count overflowed when performing CTSU scan. @note User must clear the CTSURCOVF bit manually. */
	SSP_ERR_CTSU_RC_OVERFLOW = 0x8020,
	/** Abnormal TSCAP voltage. @note User must clear the CTSUICOMP bit manually. */
	SSP_ERR_CTSU_ICOMP = 0x8040,
	/** Auto tuning algorithm failed. */
	SSP_ERR_CTSU_OFFSET_ADJUSTMENT_FAILED = 0x8080,
	/** Safety check failed **/
	SSP_ERR_CTSU_SAFETY_CHECK_FAILED = 0x8100,
	
    /** Start of SDMMC specific */
    SSP_ERR_CARD_INIT_FAILED     = 40000,       ///< SD card or eMMC device failed to initialize.
    SSP_ERR_CARD_NOT_INSERTED    = 40001,       ///< SD card not installed.
    SSP_ERR_SDHI_FAILED          = 40002,       ///< SD peripheral failed to respond properly.
    SSP_ERR_READ_FAILED          = 40003,       ///< Data read failed.
    SSP_ERR_CARD_NOT_READY       = 40004,       ///< SD card was removed.
    SSP_ERR_CARD_WRITE_PROTECTED = 40005,       ///< Media is write protected.
    SSP_ERR_TRANSFER_BUSY        = 40006,       ///< Transfer in progress.

    /** Start of FX_IO specific */
    SSP_ERR_MEDIA_FORMAT_FAILED  = 50000,       ///< Media format failed.
	SSP_ERR_MEDIA_OPEN_FAILED    = 50001,       ///< Media open failed.

	/** Start of CAN specific */
    SSP_ERR_CAN_DATA_UNAVAILABLE   = 60000,        ///< No data available.
    SSP_ERR_CAN_MODE_SWITCH_FAILED = 60001,        ///< Switching operation modes failed.
    SSP_ERR_CAN_INIT_FAILED        = 60002,        ///< Hardware initialization failed.
    SSP_ERR_CAN_TRANSMIT_NOT_READY = 60003,        ///< Transmit in progress.
    SSP_ERR_CAN_RECEIVE_MAILBOX    = 60004,        ///< Mailbox is setup as a receive mailbox.
    SSP_ERR_CAN_TRANSMIT_MAILBOX   = 60005,        ///< Mailbox is setup as a transmit mailbox.
    SSP_ERR_CAN_MESSAGE_LOST       = 60006,        ///< Receive message has been overwritten or overrun.

	/** Start of <XXX> specific */
} ssp_err_t;

/** ioctl commands. */
typedef enum e_ssp_command
{
    SSP_COMMAND_GET_SECTOR_COUNT     =1,     ///< Get media sector count.
    SSP_COMMAND_GET_SECTOR_SIZE      =2,     ///< Get sector size.
    SSP_COMMAND_GET_BLOCK_SIZE       =3,     ///< Get erase block size.
    SSP_COMMAND_CTRL_ERASE_SECTOR    =4,     ///< Erase sectors.
    SSP_COMMAND_GET_WRITE_PROTECTED  =5,     ///< Get Write Protection status.
} ssp_command_t;


/** Common version structure */
typedef union st_ssp_version
{
    /** Version id */
    uint32_t version_id;
    /** Code version parameters */
    struct
    {
        uint8_t code_version_minor;    ///< Code minor version
        uint8_t code_version_major;    ///< Code major version
        uint8_t api_version_minor;     ///< API minor version
        uint8_t api_version_major;     ///< API major version
    };
} ssp_version_t;

/** @} (end ingroup Common_Error_Codes) */

/***********************************************************************************************************************
 * Function prototypes
 **********************************************************************************************************************/

#endif /* SSP_COMMON_API_H */
