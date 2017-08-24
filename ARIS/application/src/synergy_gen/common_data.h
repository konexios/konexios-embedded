/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_i2c_api.h"
#include "sf_i2c.h"
#include "sf_i2c_api.h"
#include "r_cgc_api.h"
#include "r_spi_api.h"
#include "sf_spi.h"
#include "sf_spi_api.h"
#include "r_crypto_api.h"
extern sf_i2c_bus_t g_sf_i2c_bus0;
extern i2c_api_master_t const g_i2c_master_on_riic;
extern sf_spi_bus_t g_sf_spi_bus;
extern const spi_api_t g_spi_on_rspi;
extern const crypto_instance_t g_sce;
void g_common_init(void);
#endif /* COMMON_DATA_H_ */
