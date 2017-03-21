/* generated thread header file - do not edit */
#ifndef SENSORS_THREAD_H_
#define SENSORS_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
void sensors_thread_entry(void);
#include "r_rspi.h"
#include "r_spi_api.h"
#include "sf_spi.h"
#include "sf_spi_api.h"
#include "r_riic.h"
#include "r_i2c_api.h"
#include "sf_i2c.h"
#include "sf_i2c_api.h"
extern const spi_cfg_t g_spi0_cfg;
/** SPI on RSPI Instance. */
extern const spi_instance_t g_spi0;
#ifdef NULL
#define spi_on_rspi_CALLBACK_USED_g_spi0 (0)
#else
#define spi_on_rspi_CALLBACK_USED_g_spi0 (1)
#endif
#if spi_on_rspi_CALLBACK_USED_g_spi0
void NULL(spi_callback_args_t * p_args);
#endif
/* SF SPI on SF SPI Instance. */
extern const sf_spi_instance_t g_sf_spi_gyro;
extern const spi_cfg_t g_spi_acc_cfg;
/** SPI on RSPI Instance. */
extern const spi_instance_t g_spi_acc;
#ifdef NULL
#define spi_on_rspi_CALLBACK_USED_g_spi_acc (0)
#else
#define spi_on_rspi_CALLBACK_USED_g_spi_acc (1)
#endif
#if spi_on_rspi_CALLBACK_USED_g_spi_acc
void NULL(spi_callback_args_t * p_args);
#endif
/* SF SPI on SF SPI Instance. */
extern const sf_spi_instance_t g_sf_spi_acc;
extern const i2c_cfg_t g_i2c_tempHum_cfg;
/** I2C on RIIC Instance. */
extern const i2c_master_instance_t g_i2c_tempHum;
#ifdef NULL
#define I2C_CALLBACK_USED_g_i2c_tempHum (0)
#else
#define I2C_CALLBACK_USED_g_i2c_tempHum (1)
#endif
#if I2C_CALLBACK_USED_g_i2c_tempHum
void NULL(i2c_callback_args_t * p_args);
#endif
/* SF I2C on SF I2C Instance. */
extern const sf_i2c_instance_t g_sf_i2c_device_tempHum;
#endif /* SENSORS_THREAD_H_ */
