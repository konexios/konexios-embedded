/**
 *
 * \file
 *
 * \brief This module contains SAMD21 BSP APIs implementation.
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

#include "bsp/include/nm_bsp_aris.h"
#include "hal_data.h"
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "r_external_irq_api.h"
#include "bsp_wifi.h"
#include "reloc_assert.h"

static volatile tpfNmBspIsr isrFun = NULL;
static bsp_wifi_t wifi_ctrl;
static external_irq_instance_t const * wifi_irq_instance;
static bool module_initialized = false;


/*
 *  @fn     nm_bsp_init
 *  @brief  Initialize BSP
 *  @return 0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_init(const external_irq_instance_t * wifi_irq)
{
    ASSERT (wifi_irq != NULL);
    
    wifi_irq_instance = wifi_irq;
    isrFun = NULL;
    
    /* Open ISR Driver */
    wifi_irq_instance->p_api->open (wifi_irq_instance->p_ctrl, wifi_irq_instance->p_cfg);
    
    /* Get configuration */
    R_BSP_WifiPinOutGet( &wifi_ctrl );
    
    module_initialized = true;
    
    /* Perform chip reset. */
    nm_bsp_reset();
    
    return M2M_SUCCESS;
}

/**
 *  @fn     nm_bsp_deinit
 *  @brief  De-iInitialize BSP
 *  @return 0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_deinit(void)
{
    ASSERT (module_initialized == true);
    module_initialized = false;
    
    /* Close ISR Driver */
    wifi_irq_instance->p_api->disable (wifi_irq_instance->p_ctrl);
    wifi_irq_instance->p_api->close (wifi_irq_instance->p_ctrl);
    
    /* Disable chip */
    g_ioport.p_api->pinWrite (wifi_ctrl.chip_enable, IOPORT_LEVEL_LOW);
    g_ioport.p_api->pinWrite (wifi_ctrl.reset, IOPORT_LEVEL_LOW);
    
    return M2M_SUCCESS;
}

/**
 *  @fn     nm_bsp_reset
 *  @brief  Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
    ASSERT (module_initialized == true);
    g_ioport.p_api->pinWrite (wifi_ctrl.chip_enable, IOPORT_LEVEL_LOW);
    g_ioport.p_api->pinWrite (wifi_ctrl.reset, IOPORT_LEVEL_LOW);
    nm_bsp_sleep(100);
    g_ioport.p_api->pinWrite (wifi_ctrl.chip_enable, IOPORT_LEVEL_HIGH);
    nm_bsp_sleep(100);
    g_ioport.p_api->pinWrite (wifi_ctrl.reset, IOPORT_LEVEL_HIGH);
    nm_bsp_sleep(100);
}

/*
 *  @fn     nm_bsp_sleep
 *  @brief  Sleep in units of mSec
 *  @param[IN]  u32TimeMsec
 *              Time in milliseconds
 */
void nm_bsp_sleep(uint32 u32TimeMsec)
{
ULONG delay;
    delay = ((u32TimeMsec)*TX_TIMER_TICKS_PER_SECOND/1000);
    if( (delay == 0) && (u32TimeMsec != 0) )
    {
        /* Set minimum possible delay if computed delay was null */
        delay = 1;
    }
    tx_thread_sleep (delay);
}

/*
 *  @fn     nm_bsp_register_isr
 *  @brief  Register interrupt service routine
 *  @param[IN]  pfIsr
 *              Pointer to ISR handler
 */
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
    ASSERT (module_initialized == true);
    wifi_irq_instance->p_api->disable (wifi_irq_instance->p_ctrl);
    
    isrFun = pfIsr;
    
    wifi_irq_instance->p_api->enable (wifi_irq_instance->p_ctrl);
}

/*
 *  @fn     nm_bsp_interrupt_ctrl
 *  @brief  Enable/Disable interrupts
 *  @param[IN]  u8Enable
 *              '0' disable interrupts. '1' enable interrupts
 */
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
    ASSERT (module_initialized == true);
    if (u8Enable)
    {
        wifi_irq_instance->p_api->enable (wifi_irq_instance->p_ctrl);
    }
    else
    {
        wifi_irq_instance->p_api->disable (wifi_irq_instance->p_ctrl);
    }
}

void nm_bsp_wifi_chip_isr(void)
{
/* Make a copy of the function before check and execute it... */
tpfNmBspIsr isr_copy = isrFun;
    if (isr_copy != NULL)
    {
        isr_copy();
    }
}
