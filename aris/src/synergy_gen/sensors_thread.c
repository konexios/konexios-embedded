/* generated thread source file - do not edit */
#include "sensors_thread.h"

TX_THREAD sensors_thread;
void sensors_thread_create(void);
static void sensors_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t sensors_thread_stack[2048] BSP_PLACE_IN_SECTION(".stack.sensors_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#if spi_on_rspi_CALLBACK_USED_g_spi0
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(spi_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
spi_ctrl_t g_spi0_ctrl;

/** RSPI extended configuration for RSPI HAL driver */
const spi_on_rspi_cfg_t g_spi0_ext_cfg_t =
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

const spi_cfg_t g_spi0_cfg =
{ .channel = 0, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_EVEN, .clk_polarity =
          SPI_CLK_POLARITY_HIGH,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST, .bitrate = 500000,
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
  .p_context = (void *) &g_spi0, .p_extend = (void *) &g_spi0_ext_cfg_t, };
/* Instance structure to use this module. */
const spi_instance_t g_spi0 =
{ .p_ctrl = &g_spi0_ctrl, .p_cfg = &g_spi0_cfg, .p_api = &g_spi_on_rspi };

#if spi_on_rspi_CALLBACK_USED_g_spi0
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
sf_spi_ctrl_t g_sf_spi_gyro_ctrl;
const sf_spi_cfg_t g_sf_spi_gyro_cfg =
{ .p_bus = (sf_spi_bus_t *) &g_sf_spi_bus, .chip_select = IOPORT_PORT_04_PIN_14, .chip_select_level_active =
          IOPORT_LEVEL_LOW,
  .p_lower_lvl_cfg = &g_spi0_cfg, };
/* Instance structure to use this module. */
const sf_spi_instance_t g_sf_spi_gyro =
{ .p_ctrl = &g_sf_spi_gyro_ctrl, .p_cfg = &g_sf_spi_gyro_cfg, .p_api = &g_sf_spi_on_sf_spi };
#if spi_on_rspi_CALLBACK_USED_g_spi_acc
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(spi_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
spi_ctrl_t g_spi_acc_ctrl;

/** RSPI extended configuration for RSPI HAL driver */
const spi_on_rspi_cfg_t g_spi_acc_ext_cfg_t =
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

const spi_cfg_t g_spi_acc_cfg =
{ .channel = 0, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST, .bitrate = 500000,
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
  .p_context = (void *) &g_spi_acc, .p_extend = (void *) &g_spi_acc_ext_cfg_t, };
/* Instance structure to use this module. */
const spi_instance_t g_spi_acc =
{ .p_ctrl = &g_spi_acc_ctrl, .p_cfg = &g_spi_acc_cfg, .p_api = &g_spi_on_rspi };

#if spi_on_rspi_CALLBACK_USED_g_spi_acc
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
sf_spi_ctrl_t g_sf_spi_acc_ctrl;
const sf_spi_cfg_t g_sf_spi_acc_cfg =
{ .p_bus = (sf_spi_bus_t *) &g_sf_spi_bus, .chip_select = IOPORT_PORT_04_PIN_13, .chip_select_level_active =
          IOPORT_LEVEL_LOW,
  .p_lower_lvl_cfg = &g_spi_acc_cfg, };
/* Instance structure to use this module. */
const sf_spi_instance_t g_sf_spi_acc =
{ .p_ctrl = &g_sf_spi_acc_ctrl, .p_cfg = &g_sf_spi_acc_cfg, .p_api = &g_sf_spi_on_sf_spi };
#if I2C_CALLBACK_USED_g_i2c_tempHum
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(i2c_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
i2c_ctrl_t g_i2c_tempHum_ctrl;
const i2c_cfg_t g_i2c_tempHum_cfg =
{ .channel = 1, .rate = I2C_RATE_STANDARD, .slave = 0x40, .addr_mode = I2C_ADDR_MODE_7BIT, .p_callback = NULL,
  .p_context = (void *) &g_i2c_tempHum, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_tempHum =
{ .p_ctrl = &g_i2c_tempHum_ctrl, .p_cfg = &g_i2c_tempHum_cfg, .p_api = &g_i2c_master_on_riic };

#if I2C_CALLBACK_USED_g_i2c_tempHum
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(i2c_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(i2c_callback_args_t * p_args);
void NULL_internal(i2c_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
sf_i2c_ctrl_t g_sf_i2c_device_tempHum_ctrl;
const sf_i2c_cfg_t g_sf_i2c_device_tempHum_cfg =
{ .p_bus = (sf_i2c_bus_t *) &g_sf_i2c_bus0, .p_lower_lvl_cfg = &g_i2c_tempHum_cfg, };
/* Instance structure to use this module. */
const sf_i2c_instance_t g_sf_i2c_device_tempHum =
{ .p_ctrl = &g_sf_i2c_device_tempHum_ctrl, .p_cfg = &g_sf_i2c_device_tempHum_cfg, .p_api = &g_sf_i2c_on_sf_i2c };
void sensors_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&sensors_thread, (CHAR *) "Sensors Thread", sensors_thread_func, (ULONG) NULL,
                      &sensors_thread_stack, 2048, 5, 5, 1, TX_AUTO_START);
}

static void sensors_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    sensors_thread_entry ();
}
