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
/***********************************************************************************************************************
* File Name    : r_rspi.h
* Description  : Function Declarations and Data Definitions for the RSPI module.
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup SPI SPI
 *
 * @brief Driver for the Serial Peripheral Interface (SPI).
 *
 *  This module supports SPI serial communication for the SPI module.
 *  The SPI Interface is defined in r_spi_api.h
 *
 * @{
***********************************************************************************************************************/
#ifndef R_RSPI_H
#define R_RSPI_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Used for configuring the RSPI code */
#include "bsp_api.h"
#include "r_rspi_cfg.h"
#include "r_spi_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of Interface. */
#define RSPI_CODE_VERSION_MAJOR       (1U)
#define RSPI_CODE_VERSION_MINOR       (3U)

#if defined(BSP_MCU_GROUP_NPP2)
    #define RSPI_NUM_CHANNELS         (02)
#elif defined(BSP_MCU_GROUP_NPP7)
    #define RSPI_NUM_CHANNELS         (01)
#else
    #define RSPI_NUM_CHANNELS         (02)
#endif

/*************************************************************************************************
 * Type defines for the RSPI interface API
 *************************************************************************************************/
/** Frame data length */
typedef enum e_rspi_spcmd_bit_length
{
    RSPI_SPCMD_BIT_LENGTH_8  = (0x7),    /**< 0100 to 0111 = 8 bits data length */
    RSPI_SPCMD_BIT_LENGTH_16 = (0xF),    /**< 1111 = 16 bits data length */
    RSPI_SPCMD_BIT_LENGTH_32 = (0x3)     /**< 0011 = 32 bits data length */
} rspi_spcmd_bit_length_t;

/** SPCR (RSPI Control register) -- SPMS (RSPI mode) select */
typedef enum e_rspi_operation
{
    RSPI_OPERATION_SPI,               /**< SPI operation (4-wire method) */
    RSPI_OPERATION_CLK_SYN            /**< Clock Synchronous operation (3-wire method) */
}rspi_operation_t;

/** SPCR (RSPI Control register) -- TXMD (communication operating mode) select */
typedef enum e_rspi_communication
{
    RSPI_COMMUNICATION_FULL_DUPLEX,   /**< Full-Duplex synchronous serial communication */
    RSPI_COMMUNICATION_TRANSMIT_ONLY  /**< Transit only serial communication */
}rspi_communication_t;

/** Definition for SSLP (RSPI Slave Select Polarity register) select */
typedef enum e_rspi_sslp
{
    RSPI_SSLP_LOW,                    /**< SSLP signal polarity active low */
    RSPI_SSLP_HIGH                    /**< SSLP signal polarity active high */
}rspi_sslp_t;

/** SSLP (RSPI Slave Select Polarity register) -- SSLnP select */
typedef struct st_rspi_ssl_polarity
{
    rspi_sslp_t  rspi_ssl2;
    rspi_sslp_t  rspi_ssl3;
    rspi_sslp_t  rspi_ssl0;
    rspi_sslp_t  rspi_ssl1;
}rspi_ssl_polarity_t;

/** SPPCR (RSPI Pin Control Register) -- Loopback1 select */
typedef enum e_rspi_loopback1
{
    RSPI_LOOPBACK1_NORMAL_DATA,      /**< Loopback1 normal mode */
    RSPI_LOOPBACK1_INVERTED_DATA     /**< Loopback1 with inverted data */
}rspi_loopback1_t;

/** SPPCR (RSPI Pin Control Register) -- Loopback2 select */
typedef enum e_rspi_loopback2
{
    RSPI_LOOPBACK2_NORMAL_DATA,         /**< Loopback2 normal mode */
    RSPI_LOOPBACK2_NOT_INVERTED_DATA    /**< Loopback2 with not inverted data */
}rspi_loopback2_t;

/** SPPCR (RSPI Pin Control Register) -- Loopback select */
typedef struct st_spi_loopback
{
    rspi_loopback1_t  rspi_loopback1;
    rspi_loopback2_t  rspi_loopback2;
}rspi_loopback_t;

/** SPPCR (RSPI Pin Control Register) -- MOIFV select */
typedef enum e_rspi_mosi_idle_fixed_val
{
    RSPI_MOSI_IDLE_FIXED_VAL_LOW,     /**< MOSIn level low during MOSI idling */
    RSPI_MOSI_IDLE_FIXED_VAL_HIGH     /**< MOSIn level high during MOSI idling */
}rspi_mosi_idle_fixed_val_t;

/** SPPCR (RSPI Pin Control Register) -- MOIFE (MOSI idle value fixing) select */
typedef enum e_rspi_mosi_idle_val_fixing
{
    RSPI_MOSI_IDLE_VAL_FIXING_ENABLE,    /**< MOSI output value=final data from previous transfer */
    RSPI_MOSI_IDLE_VAL_FIXING_DISABLE    /**< MOSI output value=value set in MOIFV bit */
}rspi_mosi_idle_val_fixing_t;

/** SPPCR (RSPI Pin Control Register) -- MOIFV (mosi idle value) select */
typedef struct st_spi_mosi_idle
{
	rspi_mosi_idle_fixed_val_t  rspi_mosi_idle_fixed_val;
	rspi_mosi_idle_val_fixing_t rspi_mosi_idle_val_fixing;
}rspi_mosi_idle_t;

/** SPCR2 (RSPI Control Register 2) -- Parity Enable select */
typedef enum e_rspi_parity_state
{
    RSPI_PARITY_STATE_DISABLE,    /**< Disable parity */
    RSPI_PARITY_STATE_ENABLE      /**< Enable parity */
}rspi_parity_state_t;

/** SPCR2 (RSPI Control Register 2) -- Parity select */
typedef enum e_rspi_parity_mode
{
    RSPI_PARITY_MODE_ODD,    /**< Select even parity */
    RSPI_PARITY_MODE_EVEN    /**< Select odd parity  */
}rspi_parity_mode_t;

/** SPCR2 (RSPI Control Register 2) -- Parity select */
typedef struct st_rspi_parity
{
	rspi_parity_state_t  rspi_parity;
	rspi_parity_mode_t   rspi_parity_mode;
}rspi_parity_t;

/** SPCMD (RSPI Command) Register -- SSL Signal Assertion select */
typedef enum e_rspi_ssl_select
{
    RSPI_SSL_SELECT_SSL0,          /**< Select SSL0 as slave */
    RSPI_SSL_SELECT_SSL1,          /**< Select SSL1 as slave */
    RSPI_SSL_SELECT_SSL2,          /**< Select SSL2 as slave */
    RSPI_SSL_SELECT_SSL3           /**< Select SSL3 as slave */
}rspi_ssl_select_t;

/** SPCMD (RSPI Command) Register -- SSL Signal Level Keeping select */
typedef enum e_rspi_ssl_level_keep
{
    RSPI_SSL_LEVEL_KEEP_NOT,   /**< Negates all SSL signals upon transfer completion */
    RSPI_SSL_LEVEL_KEEP        /**< Keeps the SSL level upon transfer completion */
}rspi_ssl_level_keep_t;

/** SPCKD (RSPI Clock Delay) Register -- Clock Delay Count select */
typedef enum e_rspi_clock_delay_count
{
    RSPI_CLOCK_DELAY_COUNT_1,           /**< Set RSPCK Clock delay to 1 RSPCK */
    RSPI_CLOCK_DELAY_COUNT_2,           /**< Set RSPCK Clock delay to 2 RSPCK */
    RSPI_CLOCK_DELAY_COUNT_3,           /**< Set RSPCK Clock delay to 3 RSPCK */
    RSPI_CLOCK_DELAY_COUNT_4,           /**< Set RSPCK Clock delay to 4 RSPCK */
    RSPI_CLOCK_DELAY_COUNT_5,           /**< Set RSPCK Clock delay to 5 RSPCK */
    RSPI_CLOCK_DELAY_COUNT_6,           /**< Set RSPCK Clock delay to 6 RSPCK */
    RSPI_CLOCK_DELAY_COUNT_7,           /**< Set RSPCK Clock delay to 7 RSPCK */
    RSPI_CLOCK_DELAY_COUNT_8            /**< Set RSPCK Clock delay to 8 RSPCK */
}rspi_clock_delay_count_t;

/** SPCMD (RSPI Command) Register -- RSPCK Delay Enable/Disable select -- SCKDEN */
typedef enum e_rspi_clock_delay_state
{
    RSPI_CLOCK_DELAY_STATE_DISABLE,  /**< RSPCK delay=1 RSPCK */
    RSPI_CLOCK_DELAY_STATE_ENABLE    /**< RSPCK delay=SPCKD register setting */
}rspi_clock_delay_state_t;

/** Select RSPI Clock Delay Register (SPCKD) and SPCMD (RSPI Command) Register-Clock Delay state(SCKDEN) */
typedef struct st_rspi_clock_delay
{
	rspi_clock_delay_count_t    rspi_clock_delay_count;
	rspi_clock_delay_state_t    rspi_clock_delay_state;
}rspi_clock_delay_t;

/** SSLND (RSPI Slave Select Negation Delay) Register -- Slave Select Negation Delay Count select */
typedef enum e_rspi_ssl_negation_delay_count
{
    RSPI_SSL_NEGATION_DELAY_1,          /**< Set SSL negation delay to 1 RSPCK */
    RSPI_SSL_NEGATION_DELAY_2,          /**< Set SSL negation delay to 2 RSPCK */
    RSPI_SSL_NEGATION_DELAY_3,          /**< Set SSL negation delay to 3 RSPCK */
    RSPI_SSL_NEGATION_DELAY_4,          /**< Set SSL negation delay to 4 RSPCK */
    RSPI_SSL_NEGATION_DELAY_5,          /**< Set SSL negation delay to 5 RSPCK */
    RSPI_SSL_NEGATION_DELAY_6,          /**< Set SSL negation delay to 6 RSPCK */
    RSPI_SSL_NEGATION_DELAY_7,          /**< Set SSL negation delay to 7 RSPCK */
    RSPI_SSL_NEGATION_DELAY_8           /**< Set SSL negation delay to 8 RSPCK */
}rspi_ssl_negation_delay_count_t;

/** SPCMD (RSPI Command) Register -- SSL Negation Delay select -- SLNDEN */
typedef enum e_rspi_ssl_negation_delay_state
{
    RSPI_SSL_NEGATION_DELAY_DISABLE,    /**< SSL negation delay=1 RSPCK */
    RSPI_SSL_NEGATION_DELAY_ENABLE      /**< SSL negation delay=SSLND register setting */
}rspi_ssl_negation_delay_state_t;

/** Select SSL Negation Delay(SSLND) and SPCMD Register-SSL negation Delay state(SLNDEN) */
typedef struct st_rspi_ssl_neg_delay
{
	rspi_ssl_negation_delay_count_t    rspi_ssl_neg_delay_count;
	rspi_ssl_negation_delay_state_t    rspi_ssl_neg_delay_state;
}rspi_ssl_negation_delay_t;

/** SPND (RSPI Next-Access Delay) Register -- Next Access Delay Count select */
typedef enum e_rspi_next_access_delay_count
{
    RSPI_NEXT_ACCESS_DELAY_COUNT_1,       /**< Set next access delay to 1 RSPCK+2PCLK */
    RSPI_NEXT_ACCESS_DELAY_COUNT_2,       /**< Set next access delay to 2 RSPCK+2PCLK */
    RSPI_NEXT_ACCESS_DELAY_COUNT_3,       /**< Set next access delay to 3 RSPCK+2PCLK */
    RSPI_NEXT_ACCESS_DELAY_COUNT_4,       /**< Set next access delay to 4 RSPCK+2PCLK */
    RSPI_NEXT_ACCESS_DELAY_COUNT_5,       /**< Set next access delay to 5 RSPCK+2PCLK */
    RSPI_NEXT_ACCESS_DELAY_COUNT_6,       /**< Set next access delay to 6 RSPCK+2PCLK */
    RSPI_NEXT_ACCESS_DELAY_COUNT_7,       /**< Set next access delay to 7 RSPCK+2PCLK */
    RSPI_NEXT_ACCESS_DELAY_COUNT_8        /**< Set next access delay to 8 RSPCK+2PCLK */
}rspi_next_access_delay_count_t;

/** SPCMD (RSPI Command) Register -- Next Access Delay select -- SPNDEN */
typedef enum e_rspi_next_access_delay_state
{
    RSPI_NEXT_ACCESS_DELAY_STATE_DISABLE,  /**< Next access delay=1 RSPCK + 2 PCLK */
    RSPI_NEXT_ACCESS_DELAY_STATE_ENABLE    /**< Next access delay=SPND register setting */
}rspi_next_access_delay_state_t;

/** Select Next Access Delay(SPND) and SPCMD Register-Next Access Delay state(SPNDEN) */
typedef struct st_rspi_access_delay
{
	rspi_next_access_delay_count_t    rspi_next_access_delay_count;
	rspi_next_access_delay_state_t    rspi_next_access_delay_state;
}rspi_access_delay_t;

/* Clock base rate division */
typedef enum e_rspi_spcmd_br_div
{
    RSPI_SPCMD_BR_DIV_1 = (0x0),    /* Select the base bit rate */
    RSPI_SPCMD_BR_DIV_2 = (0x1),    /* Select the base bit rate divided by 2 */
    RSPI_SPCMD_BR_DIV_4 = (0x2),    /* Select the base bit rate divided by 4 */
    RSPI_SPCMD_BR_DIV_8 = (0x3)     /* Select the base bit rate divided by 8 */
} rspi_spcmd_br_div_t;

/** Slave select to be asserted during transfer operation. */
typedef enum e_rspi_spcmd_assert_ssl
{
    RSPI_SPCMD_ASSERT_SSL0 = (0x0),    /* Select SSL0 */
    RSPI_SPCMD_ASSERT_SSL1 = (0x1),    /* Select SSL1 */
    RSPI_SPCMD_ASSERT_SSL2 = (0x2),    /* Select SSL2 */
    RSPI_SPCMD_ASSERT_SSL3 = (0x3)     /* Select SSL3 */
} rspi_spcmd_assert_ssl_t;

/** Extended SPI interface configuration */
typedef struct st_spi_on_rspi_cfg
{
    rspi_operation_t      rspi_clksyn;     /**< Select spi or clock syn mode operation */
    rspi_communication_t  rspi_comm;       /**< Select full-duplex or transmit-only communication */
    rspi_ssl_polarity_t   ssl_polarity;   /**< Select SSLn signal polarity */
    rspi_loopback_t       loopback;       /**< Select loopback1 and loopback2 */
    rspi_mosi_idle_t      mosi_idle;      /**< Select mosi idle fixed value and selection */
    rspi_parity_t         parity;         /**< Select parity and enable/disable parity */
    rspi_ssl_select_t     ssl_select;     /**< Select which slave to use;0-SSL0;1-SSL1;2-SSL2;3-SSL3 */
    rspi_ssl_level_keep_t ssl_level_keep; /**< Select SSL level after transfer completion;0-negate;1-keeps */
    rspi_clock_delay_t    clock_delay;    /**< Select clock delay from 0 to 7*/
    rspi_ssl_negation_delay_t  ssl_neg_delay;  /**< Select Slave elect negation delay from 0 to 7 */
    rspi_access_delay_t   access_delay;   /**< Select next access delay from 0 to 7 */
} spi_on_rspi_cfg_t;

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const spi_api_t g_spi_on_rspi;
/** @endcond */

#endif /* R_RSPI_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup SPI)
***********************************************************************************************************************/
