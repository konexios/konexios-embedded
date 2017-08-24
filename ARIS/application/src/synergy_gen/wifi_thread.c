/* generated thread source file - do not edit */
#include "wifi_thread.h"

TX_THREAD wifi_thread;
void wifi_thread_create(void);
static void wifi_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t wifi_thread_stack[28672] BSP_PLACE_IN_SECTION(".stack.wifi_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
static qspi_ctrl_t g_flash_ctrl;
const qspi_cfg_t g_flash_cfg =
{ .p_extend = NULL, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
const qspi_instance_t g_flash =
{ .p_ctrl = &g_flash_ctrl, .p_cfg = &g_flash_cfg, .p_api = &g_qspi_on_qspi, };
#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_wifi_irq
#if defined(__ICCARM__)
#define wifiIRQIsr_WEAK_ATTRIBUTE
#pragma weak wifiIRQIsr                            = wifiIRQIsr_internal
#elif defined(__GNUC__)
#define wifiIRQIsr_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("wifiIRQIsr_internal")))
#endif
void wifiIRQIsr(external_irq_callback_args_t * p_args) wifiIRQIsr_WEAK_ATTRIBUTE;
#endif
static external_irq_ctrl_t g_wifi_irq_ctrl;
static const external_irq_cfg_t g_wifi_irq_cfg =
{ .channel = 9, .trigger = EXTERNAL_IRQ_TRIG_FALLING, .filter_enable = false, .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = false, .p_callback = wifiIRQIsr, .p_context = &g_wifi_irq, .p_extend = NULL };
/* Instance structure to use this module. */
const external_irq_instance_t g_wifi_irq =
{ .p_ctrl = &g_wifi_irq_ctrl, .p_cfg = &g_wifi_irq_cfg, .p_api = &g_external_irq_on_icu };

#if EXTERNAL_IRQ_ON_ICU_CALLBACK_USED_g_wifi_irq
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void wifiIRQIsr(external_irq_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void wifiIRQIsr_internal(external_irq_callback_args_t * p_args);
void wifiIRQIsr_internal(external_irq_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if spi_on_rspi_CALLBACK_USED_g_spi
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(spi_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
spi_ctrl_t g_spi_ctrl;

/** RSPI extended configuration for RSPI HAL driver */
const spi_on_rspi_cfg_t g_spi_ext_cfg_t =
{ .rspi_clksyn = RSPI_OPERATION_SPI, .rspi_comm = RSPI_COMMUNICATION_FULL_DUPLEX, .ssl_polarity.rspi_ssl0 =
          RSPI_SSLP_LOW,
  .ssl_polarity.rspi_ssl1 = RSPI_SSLP_LOW, .ssl_polarity.rspi_ssl2 = RSPI_SSLP_LOW, .ssl_polarity.rspi_ssl3 =
          RSPI_SSLP_LOW,
  .loopback.rspi_loopback1 = RSPI_LOOPBACK1_NORMAL_DATA, .loopback.rspi_loopback2 = RSPI_LOOPBACK2_NORMAL_DATA,
  .mosi_idle.rspi_mosi_idle_fixed_val = RSPI_MOSI_IDLE_FIXED_VAL_LOW, .mosi_idle.rspi_mosi_idle_val_fixing =
          RSPI_MOSI_IDLE_VAL_FIXING_DISABLE,
  .parity.rspi_parity = RSPI_PARITY_STATE_DISABLE, .parity.rspi_parity_mode = RSPI_PARITY_MODE_ODD, .ssl_select =
          RSPI_SSL_SELECT_SSL0,
  .ssl_level_keep = RSPI_SSL_LEVEL_KEEP, .clock_delay.rspi_clock_delay_count = RSPI_CLOCK_DELAY_COUNT_1,
  .clock_delay.rspi_clock_delay_state = RSPI_CLOCK_DELAY_STATE_DISABLE, .ssl_neg_delay.rspi_ssl_neg_delay_count =
          RSPI_SSL_NEGATION_DELAY_1,
  .ssl_neg_delay.rspi_ssl_neg_delay_state = RSPI_SSL_NEGATION_DELAY_DISABLE,
  .access_delay.rspi_next_access_delay_count = RSPI_NEXT_ACCESS_DELAY_COUNT_1,
  .access_delay.rspi_next_access_delay_state = RSPI_NEXT_ACCESS_DELAY_STATE_DISABLE, };

const spi_cfg_t g_spi_cfg =
{ .channel = 0, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST, .bitrate = 1000000,
#define SYNERGY_NOT_DEFINED (1)            
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
  .p_transfer_tx = &SYNERGY_NOT_DEFINED,
#endif
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
  .p_transfer_rx = &SYNERGY_NOT_DEFINED,
#endif
#undef SYNERGY_NOT_DEFINED
  .p_callback = NULL,
  .p_context = (void *) &g_spi, .p_extend = (void *) &g_spi_ext_cfg_t, };
/* Instance structure to use this module. */
const spi_instance_t g_spi =
{ .p_ctrl = &g_spi_ctrl, .p_cfg = &g_spi_cfg, .p_api = &g_spi_on_rspi };

#if spi_on_rspi_CALLBACK_USED_g_spi
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(spi_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(spi_callback_args_t * p_args);
void NULL_internal(spi_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
sf_spi_ctrl_t g_sf_spi_device_ctrl;
const sf_spi_cfg_t g_sf_spi_device_cfg =
{ .p_bus = (sf_spi_bus_t *) &g_sf_spi_bus, .chip_select = IOPORT_PORT_05_PIN_07, .chip_select_level_active =
          IOPORT_LEVEL_LOW,
  .p_lower_lvl_cfg = &g_spi_cfg, };
/* Instance structure to use this module. */
const sf_spi_instance_t g_sf_spi_device =
{ .p_ctrl = &g_sf_spi_device_ctrl, .p_cfg = &g_sf_spi_device_cfg, .p_api = &g_sf_spi_on_sf_spi };
TX_MUTEX wifi_mutex;
void wifi_thread_create(void)
{
    /* Initialize each kernel object. */
    tx_mutex_create (&wifi_mutex, (CHAR *) "Wifi Mutex", TX_NO_INHERIT);

    tx_thread_create (&wifi_thread, (CHAR *) "WiFi Thread", wifi_thread_func, (ULONG) NULL, &wifi_thread_stack, 28672,
                      7, 7, 1, TX_AUTO_START);
}

static void wifi_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    wifi_thread_entry ();
}
