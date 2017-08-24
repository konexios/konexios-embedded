/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#define MODULE_NAME "Console"

#include "console_thread.h"
#include "TRACE_OWN.h"
#include "TRACE_USE.h"
#include "reloc_macro.h"
#include "reloc_assert.h"
#include "version.h"
#include "../synergy/ssp/src/driver/r_flash_hp/hw/target/hw_flash_hp_private.h"

// copy image from 3M to 16K
#define FLASH_CF_FROM   0x300000
#define FLASH_CF_TO     0x4000
#define IMG_MAX_SIZE    0xF8000

void console_thread_entry(void);

uint8_t flash_buffer[FLASH_MIN_PGM_SIZE_CF];

ioport_port_pin_t led1 = IOPORT_PORT_00_PIN_14;
ioport_port_pin_t led2 = IOPORT_PORT_06_PIN_00;

/* Console Thread entry function */
void console_thread_entry(void) {
    g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_HIGH);
    g_ioport.p_api->pinWrite(led2 ,IOPORT_LEVEL_HIGH);

    g_flash0.p_api->open(g_flash0.p_ctrl, g_flash0.p_cfg);

    g_sf_trace.p_api->open(g_sf_trace.p_ctrl, g_sf_trace.p_cfg);
    TRACE_build( &g_sf_trace );
    tx_thread_sleep(CONV_MS_TO_TICK(500));

    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00 ,IOPORT_LEVEL_HIGH);

    TRACE(BOOT_HEADER_STRING);

    g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);

    uint32_t img_size = *((uint32_t*)FLASH_DF_BLOCK_0);

    char log_buffer[256];

    TRACE("--------- START -------------\r\n");

    if ( img_size > 0 && img_size < IMG_MAX_SIZE ) {
        int r = snprintf(log_buffer, sizeof(log_buffer), "- FLASHING - %d\r\n", img_size);
        log_buffer[r] = 0;
        TRACE(log_buffer);
        uint32_t er_num = (img_size < 0xc000?(6):(7+(img_size-0xc000)/0x8000));
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00 ,IOPORT_LEVEL_LOW);
        g_flash0.p_api->erase(g_flash0.p_ctrl, FLASH_CF_TO, er_num);
        r = snprintf(log_buffer, sizeof(log_buffer), "- ERASE - %d\r\n", img_size);
        log_buffer[r] = 0;
        TRACE(log_buffer);
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00 ,IOPORT_LEVEL_LOW);
        for (uint32_t i = 0; i < img_size / FLASH_MIN_PGM_SIZE_CF +1; i++) {
            uint32_t read_addr = FLASH_CF_FROM + i*FLASH_MIN_PGM_SIZE_CF;
            g_flash0.p_api->read(g_flash0.p_ctrl, flash_buffer, read_addr, FLASH_MIN_PGM_SIZE_CF);
            if ( i < 3 ) {
                r = snprintf(log_buffer, sizeof(log_buffer), "- MEMDUMP - %02x %02x %02x %02x\r\n",
                             flash_buffer[0],
                             flash_buffer[1],
                             flash_buffer[2],
                             flash_buffer[3]);
                log_buffer[r] = 0;
                TRACE(log_buffer);
            }
            if ( i % 2 ) g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
            else g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_HIGH);
            uint32_t write_addr = FLASH_CF_TO + i*FLASH_MIN_PGM_SIZE_CF;
            g_flash0.p_api->write(g_flash0.p_ctrl, (uint32_t)flash_buffer, write_addr, FLASH_MIN_PGM_SIZE_CF);
            tx_thread_sleep ( CONV_MS_TO_TICK(10) );
        }
        flash_buffer[0] = 0xff;
        flash_buffer[1] = 0xff;
        flash_buffer[2] = 0xff;
        flash_buffer[3] = 0xff;
        g_flash0.p_api->write(g_flash0.p_ctrl, (uint32_t)flash_buffer, FLASH_DF_BLOCK_0, 4);
        tx_thread_sleep ( CONV_MS_TO_TICK(1000) );
        g_flash0.p_api->erase(g_flash0.p_ctrl, FLASH_DF_BLOCK_0, 2);
    }
    tx_thread_sleep ( CONV_MS_TO_TICK(1000) );
    g_flash0.p_api->close(g_flash0.p_ctrl);
    TRACE("--------- FINISH -------------\r\n");
    tx_thread_sleep ( CONV_MS_TO_TICK(1000) );

    NVIC_SystemReset();
}
