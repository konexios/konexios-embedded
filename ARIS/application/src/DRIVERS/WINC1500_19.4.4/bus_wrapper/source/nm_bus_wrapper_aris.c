/**
 *
 * \file
 *
 * \brief This module contains NMC1000 bus wrapper APIs implementation.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "sf_spi_api.h"
#include "reloc_macro.h"
#include "reloc_assert.h"

#define NM_BUS_MAX_TRX_SZ       256

tstrNmBusCapabilities egstrNmBusCapabilities =
{
    .u16MaxTrxSz = NM_BUS_MAX_TRX_SZ
};

#ifdef CONF_WINC_USE_SPI
static sf_spi_instance_t const * spi_device;
static bool spi_configured = false;

void nm_bus_config (const sf_spi_instance_t * wifi_spi_device)
{
    ASSERT (wifi_spi_device != NULL);
    spi_configured = true;
    spi_device = wifi_spi_device;
}
#endif

#ifdef CONF_WINC_USE_I2C
#err "i2c not configured with Renesas Synergy yet."
struct i2c_master_module i2c_master_instance;
#define SLAVE_ADDRESS 0x60

/** Number of times to try to send packet if failed. */
#define I2C_TIMEOUT 100

static sint8 nm_i2c_write(uint8 *b, uint16 sz)
{
    sint8 result = M2M_SUCCESS;
    uint16_t timeout = 0;

    struct i2c_master_packet packet = {
        .address     = SLAVE_ADDRESS,
        .data_length = sz,
        .data        = b,
    };

    /* Write buffer to slave until success. */
    while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
        /* Increment timeout counter and check if timed out. */
        if (timeout++ == I2C_TIMEOUT) {
            break;
        }
    }

    return result;
}

static sint8 nm_i2c_read(uint8 *rb, uint16 sz)
{
    uint16_t timeout = 0;
    sint8 result = M2M_SUCCESS;
    struct i2c_master_packet packet = {
        .address     = SLAVE_ADDRESS,
        .data_length = sz,
        .data        = rb,
    };

    /* Write buffer to slave until success. */
    while (i2c_master_read_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
        /* Increment timeout counter and check if timed out. */
        if (timeout++ == I2C_TIMEOUT) {
            break;
        }
    }

    return result;
}

static sint8 nm_i2c_write_special(uint8 *wb1, uint16 sz1, uint8 *wb2, uint16 sz2)
{
    static uint8 tmp[NM_BUS_MAX_TRX_SZ];
    m2m_memcpy(tmp, wb1, sz1);
    m2m_memcpy(&tmp[sz1], wb2, sz2);
    return nm_i2c_write(tmp, sz1+sz2);
}
#endif

#ifdef CONF_WINC_USE_SPI
#define MAX_BUF_SIZE            NM_BUS_MAX_TRX_SZ
static const uint8 allZero[MAX_BUF_SIZE] = { [0 ... (MAX_BUF_SIZE-1)] = 0x00 }; 

static sint8 spi_rw(uint8* pu8Mosi, uint8* pu8Miso, uint16 u16Sz)
{
ssp_err_t err;
    
    ASSERT (spi_configured == true);

    if ((pu8Mosi != NULL) && (pu8Miso != NULL))
    {
        err = spi_device->p_api->writeRead (spi_device->p_ctrl,
                                            pu8Mosi,
                                            pu8Miso,
                                            u16Sz,
                                            SPI_BIT_WIDTH_8_BITS,
                                            TX_WAIT_FOREVER);
    }
    else if (pu8Mosi != NULL)
    {
        err = spi_device->p_api->write (    spi_device->p_ctrl,
                                            pu8Mosi,
                                            u16Sz,
                                            SPI_BIT_WIDTH_8_BITS,
                                            TX_WAIT_FOREVER);
    }
    else if (pu8Miso != NULL)
    {
        ASSERT (u16Sz <= MAX_BUF_SIZE);
        err = spi_device->p_api->writeRead (spi_device->p_ctrl,
                                            (void*) allZero,
                                            pu8Miso,
                                            u16Sz,
                                            SPI_BIT_WIDTH_8_BITS,
                                            TX_WAIT_FOREVER);
    }
    else
    {
        err = SSP_SUCCESS;
    }
    
    ASSERT (err == SSP_SUCCESS);
    
return M2M_SUCCESS;
}
#endif

/*
*   @fn     nm_bus_init
*   @brief  Initialize the bus wrapper
*   @return M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*/
sint8 nm_bus_init(void *pvinit)
{
    UNUSED_PARAMETER (pvinit);
    sint8 result = M2M_SUCCESS;

#ifdef CONF_WINC_USE_I2C
    /* Initialize config structure and software module. */
    struct i2c_master_config config_i2c_master;
    i2c_master_get_config_defaults(&config_i2c_master);

    /* Change buffer timeout to something longer. */
    config_i2c_master.buffer_timeout = 1000;

    /* Initialize and enable device with config. */
    i2c_master_init(&i2c_master_instance, SERCOM2, &config_i2c_master);

    i2c_master_enable(&i2c_master_instance);

#elif defined CONF_WINC_USE_SPI

ssp_err_t err;
    ASSERT (spi_configured == true);
    
    /** Open SPI Framework Device. */
    err = spi_device->p_api->open ( spi_device->p_ctrl,
                                    spi_device->p_cfg );
    ASSERT (err == SSP_SUCCESS);
    
    nm_bsp_reset();
    nm_bsp_sleep(1);
#endif
    return result;
}

/*
*   @fn     nm_bus_ioctl
*   @brief  send/receive from the bus
*   @param[IN]  u8Cmd
*                   IOCTL command for the operation
*   @param[IN]  pvParameter
*                   Arbitrary parameter depenging on IOCTL
*   @return M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*   @note   For SPI only, it's important to be able to send/receive at the same time
*/
sint8 nm_bus_ioctl(uint8 u8Cmd, void* pvParameter)
{
sint8 s8Ret = 0;
    switch(u8Cmd)
    {
#ifdef CONF_WINC_USE_I2C
        case NM_BUS_IOCTL_R: {
            tstrNmI2cDefault *pstrParam = (tstrNmI2cDefault *)pvParameter;
            s8Ret = nm_i2c_read(pstrParam->pu8Buf, pstrParam->u16Sz);
        }
        break;
        case NM_BUS_IOCTL_W: {
            tstrNmI2cDefault *pstrParam = (tstrNmI2cDefault *)pvParameter;
            s8Ret = nm_i2c_write(pstrParam->pu8Buf, pstrParam->u16Sz);
        }
        break;
        case NM_BUS_IOCTL_W_SPECIAL: {
            tstrNmI2cSpecial *pstrParam = (tstrNmI2cSpecial *)pvParameter;
            s8Ret = nm_i2c_write_special(pstrParam->pu8Buf1, pstrParam->u16Sz1, pstrParam->pu8Buf2, pstrParam->u16Sz2);
        }
        break;
#elif defined CONF_WINC_USE_SPI
        case NM_BUS_IOCTL_RW: {
            tstrNmSpiRw *pstrParam = (tstrNmSpiRw *)pvParameter;
            s8Ret = spi_rw(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
        }
        break;
#endif
        default:
            s8Ret = -1;
            M2M_ERR("invalid ioclt cmd\n");
            break;
    }

    return s8Ret;
}

/*
*   @fn     nm_bus_deinit
*   @brief  De-initialize the bus wrapper
*/
sint8 nm_bus_deinit(void)
{
sint8 result = M2M_SUCCESS;

#ifdef CONF_WINC_USE_I2C
    i2c_master_disable(&i2c_master_instance);
#endif /* CONF_WINC_USE_I2C */
#ifdef CONF_WINC_USE_SPI
ssp_err_t err;
    
    ASSERT (spi_configured == true);
    /** Open SPI Framework Device. */
    err = spi_device->p_api->close (spi_device->p_ctrl);
    ASSERT (err == SSP_SUCCESS);
#endif /* CONF_WINC_USE_SPI */
    return result;
}

/*
*   @fn         nm_bus_reinit
*   @brief      re-initialize the bus wrapper
*   @param [in] void *config
*                   re-init configuration data
*   @return     M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*   @author     Dina El Sissy
*   @date       19 Sept 2012
*   @version    1.0
*/
sint8 nm_bus_reinit(void* config)
{
    UNUSED_PARAMETER (config);
    return M2M_SUCCESS;
}

