/* generated thread header file - do not edit */
#ifndef WIFI_THREAD_H_
#define WIFI_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void wifi_thread_entry(void);
#include "r_qspi.h"
#include "r_qspi_api.h"
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "r_rspi.h"
#include "r_spi_api.h"
#include "sf_spi.h"
#include "sf_spi_api.h"
extern const qspi_instance_t g_flash;
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_wifi_irq;
#ifdef wifiIRQIsr
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_wifi_irq (0)
#else
#define EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_wifi_irq (1)
#endif
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_wifi_irq
void wifiIRQIsr(external_irq_callback_args_t * p_args);
#endif
extern const spi_cfg_t g_spi_cfg;
/** SPI on RSPI Instance. */
extern const spi_instance_t g_spi;
#ifdef NULL
#define spi_on_rspi_CALLBACK_USED_g_spi (0)
#else
#define spi_on_rspi_CALLBACK_USED_g_spi (1)
#endif
#if spi_on_rspi_CALLBACK_USED_g_spi
void NULL(spi_callback_args_t * p_args);
#endif
/* SF SPI on SF SPI Instance. */
extern const sf_spi_instance_t g_sf_spi_device;
extern TX_MUTEX wifi_mutex;
#endif /* WIFI_THREAD_H_ */
