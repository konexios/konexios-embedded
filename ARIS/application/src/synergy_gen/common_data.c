/* generated common source file - do not edit */
#include "common_data.h"
static TX_MUTEX sf_bus_mutex_g_sf_i2c_bus0;
static TX_EVENT_FLAGS_GROUP sf_bus_eventflag_g_sf_i2c_bus0;
static sf_i2c_ctrl_t * sf_curr_ctrl_g_sf_i2c_bus0;
sf_i2c_bus_t g_sf_i2c_bus0 =
{ .p_bus_name = (uint8_t *) "g_sf_i2c_bus0", .channel = 1, .p_lock_mutex = &sf_bus_mutex_g_sf_i2c_bus0,
  .p_sync_eventflag = &sf_bus_eventflag_g_sf_i2c_bus0, .pp_curr_ctrl = (sf_i2c_ctrl_t **) &sf_curr_ctrl_g_sf_i2c_bus0,
  .p_lower_lvl_api = (i2c_api_master_t *) &g_i2c_master_on_riic, .device_count = 0, };
static TX_MUTEX sf_bus_mutex_g_sf_spi_bus;
static TX_EVENT_FLAGS_GROUP sf_bus_eventflag_g_sf_spi_bus;
static sf_spi_ctrl_t * p_sf_curr_ctrl_g_sf_spi_bus;

#define SF_SPI_LOWER_LEVEL_MODULE_rspi

#if  defined(BSP_MCU_GROUP_S124)

#if BSP_CFG_HOCO_FREQUENCY == 0
#define BSP_CFG_HOCO_FREQUENCY_HZ 24000000
#elif BSP_CFG_HOCO_FREQUENCY == 2
#define BSP_CFG_HOCO_FREQUENCY_HZ 32000000
#elif BSP_CFG_HOCO_FREQUENCY == 4
#define BSP_CFG_HOCO_FREQUENCY_HZ 48000000
#elif BSP_CFG_HOCO_FREQUENCY == 5
#define BSP_CFG_HOCO_FREQUENCY_HZ 64000000
#endif

#define PCLKB_FREQ_HZ (BSP_CFG_HOCO_FREQUENCY_HZ / (0x01 << BSP_CFG_PCKB_DIV))

#ifdef SF_SPI_LOWER_LEVEL_MODULE_sci
#define SF_SPI_FREQ_HZ_MAX (PCLKB_FREQ_HZ / 4)
#else
#define SF_SPI_FREQ_HZ_MAX (PCLKB_FREQ_HZ / 2)
#endif

#else 

#define PCLKA_FREQ_HZ (((BSP_CFG_XTAL_HZ/(BSP_CFG_PLL_DIV + 1)) * BSP_CFG_PLL_MUL) / (0x01 << BSP_CFG_PCKA_DIV))

#ifdef SF_SPI_LOWER_LEVEL_MODULE_sci

#define SF_SPI_FREQ_HZ_MAX (PCLKA_FREQ_HZ / 8)
#else

uint32_t pclk_freqhz = (uint32_t)PCLKA_FREQ_HZ;
#define GREATER_THAN_60MHZ (pclk_freqhz > 60000000) ? 1 : 0		

#if GREATER_THAN_60MHZ
#define SF_SPI_FREQ_HZ_MAX (PCLKA_FREQ_HZ / 4)
#else
#define SF_SPI_FREQ_HZ_MAX (PCLKA_FREQ_HZ / 2)
#endif			
#endif
#endif

sf_spi_bus_t g_sf_spi_bus =
{ .p_bus_name = (uint8_t *) "g_sf_spi_bus", .channel = 0, .freq_hz_max = SF_SPI_FREQ_HZ_MAX, .freq_hz_min = 0,
  .p_lock_mutex = &sf_bus_mutex_g_sf_spi_bus, .p_sync_eventflag = &sf_bus_eventflag_g_sf_spi_bus, .pp_curr_ctrl =
          (sf_spi_ctrl_t **) &p_sf_curr_ctrl_g_sf_spi_bus,
  .p_lower_lvl_api = (spi_api_t *) &g_spi_on_rspi, .device_count = 0, };
crypto_ctrl_t g_sce_ctrl;
crypto_cfg_t g_sce_cfg =
{ .p_sce_long_plg_start_callback = NULL, .p_sce_long_plg_end_callback = NULL };
const crypto_instance_t g_sce =
{ .p_ctrl = &g_sce_ctrl, .p_cfg = &g_sce_cfg, .p_api = &g_sce_crypto_api };
void g_common_init(void)
{

}
