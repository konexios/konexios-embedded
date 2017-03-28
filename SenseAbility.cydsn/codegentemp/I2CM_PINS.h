/*******************************************************************************
* File Name: I2CM_PINS.h
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_I2CM_H)
#define CY_SCB_PINS_I2CM_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define I2CM_REMOVE_RX_WAKE_SDA_MOSI_PIN  (1u)
#define I2CM_REMOVE_RX_SDA_MOSI_PIN      (1u)
#define I2CM_REMOVE_TX_SCL_MISO_PIN      (1u)
#define I2CM_REMOVE_CTS_SCLK_PIN      (1u)
#define I2CM_REMOVE_RTS_SS0_PIN      (1u)
#define I2CM_REMOVE_SS1_PIN                 (1u)
#define I2CM_REMOVE_SS2_PIN                 (1u)
#define I2CM_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define I2CM_REMOVE_I2C_PINS                (0u)
#define I2CM_REMOVE_SPI_MASTER_PINS         (1u)
#define I2CM_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define I2CM_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define I2CM_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define I2CM_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define I2CM_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define I2CM_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define I2CM_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define I2CM_REMOVE_SPI_SLAVE_PINS          (1u)
#define I2CM_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define I2CM_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define I2CM_REMOVE_UART_TX_PIN             (1u)
#define I2CM_REMOVE_UART_RX_TX_PIN          (1u)
#define I2CM_REMOVE_UART_RX_PIN             (1u)
#define I2CM_REMOVE_UART_RX_WAKE_PIN        (1u)
#define I2CM_REMOVE_UART_RTS_PIN            (1u)
#define I2CM_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define I2CM_RX_WAKE_SDA_MOSI_PIN (0u == I2CM_REMOVE_RX_WAKE_SDA_MOSI_PIN)
#define I2CM_RX_SDA_MOSI_PIN     (0u == I2CM_REMOVE_RX_SDA_MOSI_PIN)
#define I2CM_TX_SCL_MISO_PIN     (0u == I2CM_REMOVE_TX_SCL_MISO_PIN)
#define I2CM_CTS_SCLK_PIN     (0u == I2CM_REMOVE_CTS_SCLK_PIN)
#define I2CM_RTS_SS0_PIN     (0u == I2CM_REMOVE_RTS_SS0_PIN)
#define I2CM_SS1_PIN                (0u == I2CM_REMOVE_SS1_PIN)
#define I2CM_SS2_PIN                (0u == I2CM_REMOVE_SS2_PIN)
#define I2CM_SS3_PIN                (0u == I2CM_REMOVE_SS3_PIN)

/* Mode defined pins */
#define I2CM_I2C_PINS               (0u == I2CM_REMOVE_I2C_PINS)
#define I2CM_SPI_MASTER_PINS        (0u == I2CM_REMOVE_SPI_MASTER_PINS)
#define I2CM_SPI_MASTER_SCLK_PIN    (0u == I2CM_REMOVE_SPI_MASTER_SCLK_PIN)
#define I2CM_SPI_MASTER_MOSI_PIN    (0u == I2CM_REMOVE_SPI_MASTER_MOSI_PIN)
#define I2CM_SPI_MASTER_MISO_PIN    (0u == I2CM_REMOVE_SPI_MASTER_MISO_PIN)
#define I2CM_SPI_MASTER_SS0_PIN     (0u == I2CM_REMOVE_SPI_MASTER_SS0_PIN)
#define I2CM_SPI_MASTER_SS1_PIN     (0u == I2CM_REMOVE_SPI_MASTER_SS1_PIN)
#define I2CM_SPI_MASTER_SS2_PIN     (0u == I2CM_REMOVE_SPI_MASTER_SS2_PIN)
#define I2CM_SPI_MASTER_SS3_PIN     (0u == I2CM_REMOVE_SPI_MASTER_SS3_PIN)
#define I2CM_SPI_SLAVE_PINS         (0u == I2CM_REMOVE_SPI_SLAVE_PINS)
#define I2CM_SPI_SLAVE_MOSI_PIN     (0u == I2CM_REMOVE_SPI_SLAVE_MOSI_PIN)
#define I2CM_SPI_SLAVE_MISO_PIN     (0u == I2CM_REMOVE_SPI_SLAVE_MISO_PIN)
#define I2CM_UART_TX_PIN            (0u == I2CM_REMOVE_UART_TX_PIN)
#define I2CM_UART_RX_TX_PIN         (0u == I2CM_REMOVE_UART_RX_TX_PIN)
#define I2CM_UART_RX_PIN            (0u == I2CM_REMOVE_UART_RX_PIN)
#define I2CM_UART_RX_WAKE_PIN       (0u == I2CM_REMOVE_UART_RX_WAKE_PIN)
#define I2CM_UART_RTS_PIN           (0u == I2CM_REMOVE_UART_RTS_PIN)
#define I2CM_UART_CTS_PIN           (0u == I2CM_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (I2CM_RX_WAKE_SDA_MOSI_PIN)
    #include "I2CM_uart_rx_wake_i2c_sda_spi_mosi.h"
#endif /* (I2CM_RX_SDA_MOSI) */

#if (I2CM_RX_SDA_MOSI_PIN)
    #include "I2CM_uart_rx_i2c_sda_spi_mosi.h"
#endif /* (I2CM_RX_SDA_MOSI) */

#if (I2CM_TX_SCL_MISO_PIN)
    #include "I2CM_uart_tx_i2c_scl_spi_miso.h"
#endif /* (I2CM_TX_SCL_MISO) */

#if (I2CM_CTS_SCLK_PIN)
    #include "I2CM_uart_cts_spi_sclk.h"
#endif /* (I2CM_CTS_SCLK) */

#if (I2CM_RTS_SS0_PIN)
    #include "I2CM_uart_rts_spi_ss0.h"
#endif /* (I2CM_RTS_SS0_PIN) */

#if (I2CM_SS1_PIN)
    #include "I2CM_spi_ss1.h"
#endif /* (I2CM_SS1_PIN) */

#if (I2CM_SS2_PIN)
    #include "I2CM_spi_ss2.h"
#endif /* (I2CM_SS2_PIN) */

#if (I2CM_SS3_PIN)
    #include "I2CM_spi_ss3.h"
#endif /* (I2CM_SS3_PIN) */

#if (I2CM_I2C_PINS)
    #include "I2CM_scl.h"
    #include "I2CM_sda.h"
#endif /* (I2CM_I2C_PINS) */

#if (I2CM_SPI_MASTER_PINS)
#if (I2CM_SPI_MASTER_SCLK_PIN)
    #include "I2CM_sclk_m.h"
#endif /* (I2CM_SPI_MASTER_SCLK_PIN) */

#if (I2CM_SPI_MASTER_MOSI_PIN)
    #include "I2CM_mosi_m.h"
#endif /* (I2CM_SPI_MASTER_MOSI_PIN) */

#if (I2CM_SPI_MASTER_MISO_PIN)
    #include "I2CM_miso_m.h"
#endif /*(I2CM_SPI_MASTER_MISO_PIN) */
#endif /* (I2CM_SPI_MASTER_PINS) */

#if (I2CM_SPI_SLAVE_PINS)
    #include "I2CM_sclk_s.h"
    #include "I2CM_ss_s.h"

#if (I2CM_SPI_SLAVE_MOSI_PIN)
    #include "I2CM_mosi_s.h"
#endif /* (I2CM_SPI_SLAVE_MOSI_PIN) */

#if (I2CM_SPI_SLAVE_MISO_PIN)
    #include "I2CM_miso_s.h"
#endif /*(I2CM_SPI_SLAVE_MISO_PIN) */
#endif /* (I2CM_SPI_SLAVE_PINS) */

#if (I2CM_SPI_MASTER_SS0_PIN)
    #include "I2CM_ss0_m.h"
#endif /* (I2CM_SPI_MASTER_SS0_PIN) */

#if (I2CM_SPI_MASTER_SS1_PIN)
    #include "I2CM_ss1_m.h"
#endif /* (I2CM_SPI_MASTER_SS1_PIN) */

#if (I2CM_SPI_MASTER_SS2_PIN)
    #include "I2CM_ss2_m.h"
#endif /* (I2CM_SPI_MASTER_SS2_PIN) */

#if (I2CM_SPI_MASTER_SS3_PIN)
    #include "I2CM_ss3_m.h"
#endif /* (I2CM_SPI_MASTER_SS3_PIN) */

#if (I2CM_UART_TX_PIN)
    #include "I2CM_tx.h"
#endif /* (I2CM_UART_TX_PIN) */

#if (I2CM_UART_RX_TX_PIN)
    #include "I2CM_rx_tx.h"
#endif /* (I2CM_UART_RX_TX_PIN) */

#if (I2CM_UART_RX_PIN)
    #include "I2CM_rx.h"
#endif /* (I2CM_UART_RX_PIN) */

#if (I2CM_UART_RX_WAKE_PIN)
    #include "I2CM_rx_wake.h"
#endif /* (I2CM_UART_RX_WAKE_PIN) */

#if (I2CM_UART_RTS_PIN)
    #include "I2CM_rts.h"
#endif /* (I2CM_UART_RTS_PIN) */

#if (I2CM_UART_CTS_PIN)
    #include "I2CM_cts.h"
#endif /* (I2CM_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (I2CM_RX_WAKE_SDA_MOSI_PIN)
    #define I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG \
                            (*(reg32 *) I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM)
    #define I2CM_RX_WAKE_SDA_MOSI_HSIOM_PTR \
                            ( (reg32 *) I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM)
    #define I2CM_RX_WAKE_SDA_MOSI_HSIOM_MASK \
                            (I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_MASK)
    #define I2CM_RX_WAKE_SDA_MOSI_HSIOM_POS \
                            (I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_SHIFT)

    #define I2CM_RX_WAKE_SDA_MOSI_INTCFG_REG \
                            (*(reg32 *) I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__INTCFG)
    #define I2CM_RX_WAKE_SDA_MOSI_INTCFG_PTR \
                            ( (reg32 *) I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__INTCFG)
    #define I2CM_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS  (I2CM_uart_rx_wake_i2c_sda_spi_mosi__SHIFT)
    #define I2CM_RX_WAKE_SDA_MOSI_INTCFG_TYPE_MASK \
                            ((uint32) I2CM_INTCFG_TYPE_MASK << \
                                      I2CM_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS)
#endif /* (I2CM_RX_WAKE_SDA_MOSI_PIN) */

#if (I2CM_RX_SDA_MOSI_PIN)
    #define I2CM_RX_SDA_MOSI_HSIOM_REG   (*(reg32 *) I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM)
    #define I2CM_RX_SDA_MOSI_HSIOM_PTR   ( (reg32 *) I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM)
    #define I2CM_RX_SDA_MOSI_HSIOM_MASK  (I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM_MASK)
    #define I2CM_RX_SDA_MOSI_HSIOM_POS   (I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM_SHIFT)
#endif /* (I2CM_RX_SDA_MOSI_PIN) */

#if (I2CM_TX_SCL_MISO_PIN)
    #define I2CM_TX_SCL_MISO_HSIOM_REG   (*(reg32 *) I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM)
    #define I2CM_TX_SCL_MISO_HSIOM_PTR   ( (reg32 *) I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM)
    #define I2CM_TX_SCL_MISO_HSIOM_MASK  (I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM_MASK)
    #define I2CM_TX_SCL_MISO_HSIOM_POS   (I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM_SHIFT)
#endif /* (I2CM_TX_SCL_MISO_PIN) */

#if (I2CM_CTS_SCLK_PIN)
    #define I2CM_CTS_SCLK_HSIOM_REG   (*(reg32 *) I2CM_uart_cts_spi_sclk__0__HSIOM)
    #define I2CM_CTS_SCLK_HSIOM_PTR   ( (reg32 *) I2CM_uart_cts_spi_sclk__0__HSIOM)
    #define I2CM_CTS_SCLK_HSIOM_MASK  (I2CM_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define I2CM_CTS_SCLK_HSIOM_POS   (I2CM_uart_cts_spi_sclk__0__HSIOM_SHIFT)
#endif /* (I2CM_CTS_SCLK_PIN) */

#if (I2CM_RTS_SS0_PIN)
    #define I2CM_RTS_SS0_HSIOM_REG   (*(reg32 *) I2CM_uart_rts_spi_ss0__0__HSIOM)
    #define I2CM_RTS_SS0_HSIOM_PTR   ( (reg32 *) I2CM_uart_rts_spi_ss0__0__HSIOM)
    #define I2CM_RTS_SS0_HSIOM_MASK  (I2CM_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define I2CM_RTS_SS0_HSIOM_POS   (I2CM_uart_rts_spi_ss0__0__HSIOM_SHIFT)
#endif /* (I2CM_RTS_SS0_PIN) */

#if (I2CM_SS1_PIN)
    #define I2CM_SS1_HSIOM_REG      (*(reg32 *) I2CM_spi_ss1__0__HSIOM)
    #define I2CM_SS1_HSIOM_PTR      ( (reg32 *) I2CM_spi_ss1__0__HSIOM)
    #define I2CM_SS1_HSIOM_MASK     (I2CM_spi_ss1__0__HSIOM_MASK)
    #define I2CM_SS1_HSIOM_POS      (I2CM_spi_ss1__0__HSIOM_SHIFT)
#endif /* (I2CM_SS1_PIN) */

#if (I2CM_SS2_PIN)
    #define I2CM_SS2_HSIOM_REG     (*(reg32 *) I2CM_spi_ss2__0__HSIOM)
    #define I2CM_SS2_HSIOM_PTR     ( (reg32 *) I2CM_spi_ss2__0__HSIOM)
    #define I2CM_SS2_HSIOM_MASK    (I2CM_spi_ss2__0__HSIOM_MASK)
    #define I2CM_SS2_HSIOM_POS     (I2CM_spi_ss2__0__HSIOM_SHIFT)
#endif /* (I2CM_SS2_PIN) */

#if (I2CM_SS3_PIN)
    #define I2CM_SS3_HSIOM_REG     (*(reg32 *) I2CM_spi_ss3__0__HSIOM)
    #define I2CM_SS3_HSIOM_PTR     ( (reg32 *) I2CM_spi_ss3__0__HSIOM)
    #define I2CM_SS3_HSIOM_MASK    (I2CM_spi_ss3__0__HSIOM_MASK)
    #define I2CM_SS3_HSIOM_POS     (I2CM_spi_ss3__0__HSIOM_SHIFT)
#endif /* (I2CM_SS3_PIN) */

#if (I2CM_I2C_PINS)
    #define I2CM_SCL_HSIOM_REG     (*(reg32 *) I2CM_scl__0__HSIOM)
    #define I2CM_SCL_HSIOM_PTR     ( (reg32 *) I2CM_scl__0__HSIOM)
    #define I2CM_SCL_HSIOM_MASK    (I2CM_scl__0__HSIOM_MASK)
    #define I2CM_SCL_HSIOM_POS     (I2CM_scl__0__HSIOM_SHIFT)

    #define I2CM_SDA_HSIOM_REG     (*(reg32 *) I2CM_sda__0__HSIOM)
    #define I2CM_SDA_HSIOM_PTR     ( (reg32 *) I2CM_sda__0__HSIOM)
    #define I2CM_SDA_HSIOM_MASK    (I2CM_sda__0__HSIOM_MASK)
    #define I2CM_SDA_HSIOM_POS     (I2CM_sda__0__HSIOM_SHIFT)
#endif /* (I2CM_I2C_PINS) */

#if (I2CM_SPI_MASTER_SCLK_PIN)
    #define I2CM_SCLK_M_HSIOM_REG   (*(reg32 *) I2CM_sclk_m__0__HSIOM)
    #define I2CM_SCLK_M_HSIOM_PTR   ( (reg32 *) I2CM_sclk_m__0__HSIOM)
    #define I2CM_SCLK_M_HSIOM_MASK  (I2CM_sclk_m__0__HSIOM_MASK)
    #define I2CM_SCLK_M_HSIOM_POS   (I2CM_sclk_m__0__HSIOM_SHIFT)
#endif /* (I2CM_SPI_MASTER_SCLK_PIN) */

#if (I2CM_SPI_MASTER_SS0_PIN)
    #define I2CM_SS0_M_HSIOM_REG    (*(reg32 *) I2CM_ss0_m__0__HSIOM)
    #define I2CM_SS0_M_HSIOM_PTR    ( (reg32 *) I2CM_ss0_m__0__HSIOM)
    #define I2CM_SS0_M_HSIOM_MASK   (I2CM_ss0_m__0__HSIOM_MASK)
    #define I2CM_SS0_M_HSIOM_POS    (I2CM_ss0_m__0__HSIOM_SHIFT)
#endif /* (I2CM_SPI_MASTER_SS0_PIN) */

#if (I2CM_SPI_MASTER_SS1_PIN)
    #define I2CM_SS1_M_HSIOM_REG    (*(reg32 *) I2CM_ss1_m__0__HSIOM)
    #define I2CM_SS1_M_HSIOM_PTR    ( (reg32 *) I2CM_ss1_m__0__HSIOM)
    #define I2CM_SS1_M_HSIOM_MASK   (I2CM_ss1_m__0__HSIOM_MASK)
    #define I2CM_SS1_M_HSIOM_POS    (I2CM_ss1_m__0__HSIOM_SHIFT)
#endif /* (I2CM_SPI_MASTER_SS1_PIN) */

#if (I2CM_SPI_MASTER_SS2_PIN)
    #define I2CM_SS2_M_HSIOM_REG    (*(reg32 *) I2CM_ss2_m__0__HSIOM)
    #define I2CM_SS2_M_HSIOM_PTR    ( (reg32 *) I2CM_ss2_m__0__HSIOM)
    #define I2CM_SS2_M_HSIOM_MASK   (I2CM_ss2_m__0__HSIOM_MASK)
    #define I2CM_SS2_M_HSIOM_POS    (I2CM_ss2_m__0__HSIOM_SHIFT)
#endif /* (I2CM_SPI_MASTER_SS2_PIN) */

#if (I2CM_SPI_MASTER_SS3_PIN)
    #define I2CM_SS3_M_HSIOM_REG    (*(reg32 *) I2CM_ss3_m__0__HSIOM)
    #define I2CM_SS3_M_HSIOM_PTR    ( (reg32 *) I2CM_ss3_m__0__HSIOM)
    #define I2CM_SS3_M_HSIOM_MASK   (I2CM_ss3_m__0__HSIOM_MASK)
    #define I2CM_SS3_M_HSIOM_POS    (I2CM_ss3_m__0__HSIOM_SHIFT)
#endif /* (I2CM_SPI_MASTER_SS3_PIN) */

#if (I2CM_UART_TX_PIN)
    #define I2CM_TX_HSIOM_REG   (*(reg32 *) I2CM_tx__0__HSIOM)
    #define I2CM_TX_HSIOM_PTR   ( (reg32 *) I2CM_tx_0__HSIOM)
    #define I2CM_TX_HSIOM_MASK  (I2CM_tx__0__HSIOM_MASK)
    #define I2CM_TX_HSIOM_POS   (I2CM_tx__0__HSIOM_SHIFT)
#endif /* (I2CM_UART_TX_PIN) */

#if (I2CM_UART_RTS_PIN)
    #define I2CM_RTS_HSIOM_REG  (*(reg32 *) I2CM_rts__0__HSIOM)
    #define I2CM_RTS_HSIOM_PTR  ( (reg32 *) I2CM_rts__0__HSIOM)
    #define I2CM_RTS_HSIOM_MASK (I2CM_rts__0__HSIOM_MASK)
    #define I2CM_RTS_HSIOM_POS  (I2CM_rts__0__HSIOM_SHIFT)
#endif /* (I2CM_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* Pins constants */
#define I2CM_HSIOM_DEF_SEL      (0x00u)
#define I2CM_HSIOM_GPIO_SEL     (0x00u)
#define I2CM_HSIOM_UART_SEL     (0x09u)
#define I2CM_HSIOM_I2C_SEL      (0x0Eu)
#define I2CM_HSIOM_SPI_SEL      (0x0Fu)

#define I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX   (0u)
#define I2CM_RX_SDA_MOSI_PIN_INDEX       (0u)
#define I2CM_TX_SCL_MISO_PIN_INDEX       (1u)
#define I2CM_CTS_SCLK_PIN_INDEX       (2u)
#define I2CM_RTS_SS0_PIN_INDEX       (3u)
#define I2CM_SS1_PIN_INDEX                  (4u)
#define I2CM_SS2_PIN_INDEX                  (5u)
#define I2CM_SS3_PIN_INDEX                  (6u)

#define I2CM_RX_WAKE_SDA_MOSI_PIN_MASK ((uint32) 0x01u << I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX)
#define I2CM_RX_SDA_MOSI_PIN_MASK     ((uint32) 0x01u << I2CM_RX_SDA_MOSI_PIN_INDEX)
#define I2CM_TX_SCL_MISO_PIN_MASK     ((uint32) 0x01u << I2CM_TX_SCL_MISO_PIN_INDEX)
#define I2CM_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << I2CM_CTS_SCLK_PIN_INDEX)
#define I2CM_RTS_SS0_PIN_MASK     ((uint32) 0x01u << I2CM_RTS_SS0_PIN_INDEX)
#define I2CM_SS1_PIN_MASK                ((uint32) 0x01u << I2CM_SS1_PIN_INDEX)
#define I2CM_SS2_PIN_MASK                ((uint32) 0x01u << I2CM_SS2_PIN_INDEX)
#define I2CM_SS3_PIN_MASK                ((uint32) 0x01u << I2CM_SS3_PIN_INDEX)

/* Pin interrupt constants */
#define I2CM_INTCFG_TYPE_MASK           (0x03u)
#define I2CM_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants */
#define I2CM_PIN_DM_ALG_HIZ  (0u)
#define I2CM_PIN_DM_DIG_HIZ  (1u)
#define I2CM_PIN_DM_OD_LO    (4u)
#define I2CM_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define I2CM_DM_MASK    (0x7u)
#define I2CM_DM_SIZE    (3)
#define I2CM_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) I2CM_DM_MASK << (I2CM_DM_SIZE * (pos)))) >> \
                                                              (I2CM_DM_SIZE * (pos)) )

#if (I2CM_TX_SCL_MISO_PIN)
    #define I2CM_CHECK_TX_SCL_MISO_PIN_USED \
                (I2CM_PIN_DM_ALG_HIZ != \
                    I2CM_GET_P4_PIN_DM(I2CM_uart_tx_i2c_scl_spi_miso_PC, \
                                                   I2CM_uart_tx_i2c_scl_spi_miso_SHIFT))
#endif /* (I2CM_TX_SCL_MISO_PIN) */

#if (I2CM_RTS_SS0_PIN)
    #define I2CM_CHECK_RTS_SS0_PIN_USED \
                (I2CM_PIN_DM_ALG_HIZ != \
                    I2CM_GET_P4_PIN_DM(I2CM_uart_rts_spi_ss0_PC, \
                                                   I2CM_uart_rts_spi_ss0_SHIFT))
#endif /* (I2CM_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define I2CM_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define I2CM_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define I2CM_SET_HSIOM_SEL(reg, mask, pos, sel) I2CM_SET_REGISTER_BITS(reg, mask, pos, sel)
#define I2CM_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        I2CM_SET_REGISTER_BITS(reg, mask, pos, intType)
#define I2CM_SET_INP_DIS(reg, mask, val) I2CM_SET_REGISTER_BIT(reg, mask, val)

/* I2CM_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  I2CM_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (I2CM_CY_SCBIP_V0)
#if (I2CM_I2C_PINS)
    #define I2CM_SET_I2C_SCL_DR(val) I2CM_scl_Write(val)

    #define I2CM_SET_I2C_SCL_HSIOM_SEL(sel) \
                          I2CM_SET_HSIOM_SEL(I2CM_SCL_HSIOM_REG,  \
                                                         I2CM_SCL_HSIOM_MASK, \
                                                         I2CM_SCL_HSIOM_POS,  \
                                                         (sel))
    #define I2CM_WAIT_SCL_SET_HIGH  (0u == I2CM_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (I2CM_RX_WAKE_SDA_MOSI_PIN)
    #define I2CM_SET_I2C_SCL_DR(val) \
                            I2CM_uart_rx_wake_i2c_sda_spi_mosi_Write(val)

    #define I2CM_SET_I2C_SCL_HSIOM_SEL(sel) \
                    I2CM_SET_HSIOM_SEL(I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG,  \
                                                   I2CM_RX_WAKE_SDA_MOSI_HSIOM_MASK, \
                                                   I2CM_RX_WAKE_SDA_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define I2CM_WAIT_SCL_SET_HIGH  (0u == I2CM_uart_rx_wake_i2c_sda_spi_mosi_Read())

#elif (I2CM_RX_SDA_MOSI_PIN)
    #define I2CM_SET_I2C_SCL_DR(val) \
                            I2CM_uart_rx_i2c_sda_spi_mosi_Write(val)


    #define I2CM_SET_I2C_SCL_HSIOM_SEL(sel) \
                            I2CM_SET_HSIOM_SEL(I2CM_RX_SDA_MOSI_HSIOM_REG,  \
                                                           I2CM_RX_SDA_MOSI_HSIOM_MASK, \
                                                           I2CM_RX_SDA_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define I2CM_WAIT_SCL_SET_HIGH  (0u == I2CM_uart_rx_i2c_sda_spi_mosi_Read())

#else
    #define I2CM_SET_I2C_SCL_DR(val) \
                                                    do{ /* Does nothing */ }while(0)
    #define I2CM_SET_I2C_SCL_HSIOM_SEL(sel) \
                                                    do{ /* Does nothing */ }while(0)

    #define I2CM_WAIT_SCL_SET_HIGH  (0u)
#endif /* (I2CM_I2C_PINS) */

/* SCB I2C: sda signal */
#if (I2CM_I2C_PINS)
    #define I2CM_WAIT_SDA_SET_HIGH  (0u == I2CM_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (I2CM_TX_SCL_MISO_PIN)
    #define I2CM_WAIT_SDA_SET_HIGH  (0u == I2CM_uart_tx_i2c_scl_spi_miso_Read())
#else
    #define I2CM_WAIT_SDA_SET_HIGH  (0u)
#endif /* (I2CM_MOSI_SCL_RX_PIN) */
#endif /* (I2CM_CY_SCBIP_V0) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define I2CM_REMOVE_MOSI_SCL_RX_WAKE_PIN    I2CM_REMOVE_RX_WAKE_SDA_MOSI_PIN
#define I2CM_REMOVE_MOSI_SCL_RX_PIN         I2CM_REMOVE_RX_SDA_MOSI_PIN
#define I2CM_REMOVE_MISO_SDA_TX_PIN         I2CM_REMOVE_TX_SCL_MISO_PIN
#ifndef I2CM_REMOVE_SCLK_PIN
#define I2CM_REMOVE_SCLK_PIN                I2CM_REMOVE_CTS_SCLK_PIN
#endif /* I2CM_REMOVE_SCLK_PIN */
#ifndef I2CM_REMOVE_SS0_PIN
#define I2CM_REMOVE_SS0_PIN                 I2CM_REMOVE_RTS_SS0_PIN
#endif /* I2CM_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define I2CM_MOSI_SCL_RX_WAKE_PIN   I2CM_RX_WAKE_SDA_MOSI_PIN
#define I2CM_MOSI_SCL_RX_PIN        I2CM_RX_SDA_MOSI_PIN
#define I2CM_MISO_SDA_TX_PIN        I2CM_TX_SCL_MISO_PIN
#ifndef I2CM_SCLK_PIN
#define I2CM_SCLK_PIN               I2CM_CTS_SCLK_PIN
#endif /* I2CM_SCLK_PIN */
#ifndef I2CM_SS0_PIN
#define I2CM_SS0_PIN                I2CM_RTS_SS0_PIN
#endif /* I2CM_SS0_PIN */

#if (I2CM_MOSI_SCL_RX_WAKE_PIN)
    #define I2CM_MOSI_SCL_RX_WAKE_HSIOM_REG     I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define I2CM_MOSI_SCL_RX_WAKE_HSIOM_PTR     I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define I2CM_MOSI_SCL_RX_WAKE_HSIOM_MASK    I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define I2CM_MOSI_SCL_RX_WAKE_HSIOM_POS     I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG

    #define I2CM_MOSI_SCL_RX_WAKE_INTCFG_REG    I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define I2CM_MOSI_SCL_RX_WAKE_INTCFG_PTR    I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG

    #define I2CM_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define I2CM_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
#endif /* (I2CM_RX_WAKE_SDA_MOSI_PIN) */

#if (I2CM_MOSI_SCL_RX_PIN)
    #define I2CM_MOSI_SCL_RX_HSIOM_REG      I2CM_RX_SDA_MOSI_HSIOM_REG
    #define I2CM_MOSI_SCL_RX_HSIOM_PTR      I2CM_RX_SDA_MOSI_HSIOM_PTR
    #define I2CM_MOSI_SCL_RX_HSIOM_MASK     I2CM_RX_SDA_MOSI_HSIOM_MASK
    #define I2CM_MOSI_SCL_RX_HSIOM_POS      I2CM_RX_SDA_MOSI_HSIOM_POS
#endif /* (I2CM_MOSI_SCL_RX_PIN) */

#if (I2CM_MISO_SDA_TX_PIN)
    #define I2CM_MISO_SDA_TX_HSIOM_REG      I2CM_TX_SCL_MISO_HSIOM_REG
    #define I2CM_MISO_SDA_TX_HSIOM_PTR      I2CM_TX_SCL_MISO_HSIOM_REG
    #define I2CM_MISO_SDA_TX_HSIOM_MASK     I2CM_TX_SCL_MISO_HSIOM_REG
    #define I2CM_MISO_SDA_TX_HSIOM_POS      I2CM_TX_SCL_MISO_HSIOM_REG
#endif /* (I2CM_MISO_SDA_TX_PIN_PIN) */

#if (I2CM_SCLK_PIN)
    #ifndef I2CM_SCLK_HSIOM_REG
    #define I2CM_SCLK_HSIOM_REG     I2CM_CTS_SCLK_HSIOM_REG
    #define I2CM_SCLK_HSIOM_PTR     I2CM_CTS_SCLK_HSIOM_PTR
    #define I2CM_SCLK_HSIOM_MASK    I2CM_CTS_SCLK_HSIOM_MASK
    #define I2CM_SCLK_HSIOM_POS     I2CM_CTS_SCLK_HSIOM_POS
    #endif /* I2CM_SCLK_HSIOM_REG */
#endif /* (I2CM_SCLK_PIN) */

#if (I2CM_SS0_PIN)
    #ifndef I2CM_SS0_HSIOM_REG
    #define I2CM_SS0_HSIOM_REG      I2CM_RTS_SS0_HSIOM_REG
    #define I2CM_SS0_HSIOM_PTR      I2CM_RTS_SS0_HSIOM_PTR
    #define I2CM_SS0_HSIOM_MASK     I2CM_RTS_SS0_HSIOM_MASK
    #define I2CM_SS0_HSIOM_POS      I2CM_RTS_SS0_HSIOM_POS
    #endif /* I2CM_SS0_HSIOM_REG */
#endif /* (I2CM_SS0_PIN) */

#define I2CM_MOSI_SCL_RX_WAKE_PIN_INDEX I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX
#define I2CM_MOSI_SCL_RX_PIN_INDEX      I2CM_RX_SDA_MOSI_PIN_INDEX
#define I2CM_MISO_SDA_TX_PIN_INDEX      I2CM_TX_SCL_MISO_PIN_INDEX
#ifndef I2CM_SCLK_PIN_INDEX
#define I2CM_SCLK_PIN_INDEX             I2CM_CTS_SCLK_PIN_INDEX
#endif /* I2CM_SCLK_PIN_INDEX */
#ifndef I2CM_SS0_PIN_INDEX
#define I2CM_SS0_PIN_INDEX              I2CM_RTS_SS0_PIN_INDEX
#endif /* I2CM_SS0_PIN_INDEX */

#define I2CM_MOSI_SCL_RX_WAKE_PIN_MASK I2CM_RX_WAKE_SDA_MOSI_PIN_MASK
#define I2CM_MOSI_SCL_RX_PIN_MASK      I2CM_RX_SDA_MOSI_PIN_MASK
#define I2CM_MISO_SDA_TX_PIN_MASK      I2CM_TX_SCL_MISO_PIN_MASK
#ifndef I2CM_SCLK_PIN_MASK
#define I2CM_SCLK_PIN_MASK             I2CM_CTS_SCLK_PIN_MASK
#endif /* I2CM_SCLK_PIN_MASK */
#ifndef I2CM_SS0_PIN_MASK
#define I2CM_SS0_PIN_MASK              I2CM_RTS_SS0_PIN_MASK
#endif /* I2CM_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_I2CM_H) */


/* [] END OF FILE */
