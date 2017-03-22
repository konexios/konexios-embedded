#define MODULE_NAME "Sensors"

#include "sensors_thread.h"
#include "DATA_QUEUE_OWN.h"
#include "DATA_QUEUE_USE.h"
#include "sensors_data.h"
#include "sf_adxrs290_api.h"
#include "sf_adxrs290.h"
#include "sf_adxl362_api.h"
#include "sf_adxl362.h"
#include "sf_si7013_api.h"
#include "sf_si7013.h"
#include "reloc_assert.h"
#include "reloc_macro.h"
#include "THREADS_SYNC/THREADS_SYNC_USE.h"
#include "threads_sync_events.h"
#include "config.h"
#include "TRACE_USE.h"

static sf_adxl362_ctrl_t adxl362_ctrl;
static const sf_adxl362_cfg_t adxl362_cfg = { .device = &g_sf_spi_acc };
static const sf_adxl362_instance_t adxl362 = { .p_ctrl = &adxl362_ctrl, .p_cfg = &adxl362_cfg, .p_api = &g_sf_adxl362_api };

static sf_adxrs290_ctrl_t adxrs290_ctrl;
static const sf_adxrs290_cfg_t adxrs290_cfg = { .device = &g_sf_spi_gyro };
static const sf_adxrs290_instance_t adxrs290 = { .p_ctrl = &adxrs290_ctrl, .p_cfg = &adxrs290_cfg, .p_api = &g_sf_adxrs290_api };


static sf_si7013_ctrl_t si7013_ctrl;
static const sf_si7013_cfg_t si7013_cfg = { .device = &g_sf_i2c_device_tempHum };

static temperature_sensor_ctrl_t temp_sens_ctrl;
static const temperature_sensor_cfg_t temp_sens_cfg = { .p_extend_cfg = &si7013_cfg, .p_extend_ctrl = &si7013_ctrl };
static const temperature_sensor_instance_t temp_sens = { .p_ctrl = &temp_sens_ctrl, .p_cfg = &temp_sens_cfg, .p_api = &g_sf_si7013_temperature_api };

static humidity_sensor_ctrl_t hum_sens_ctrl;
static const humidity_sensor_cfg_t hum_sens_cfg = { .p_extend_cfg = &si7013_cfg, .p_extend_ctrl = &si7013_ctrl };
static const humidity_sensor_instance_t hum_sens = { .p_ctrl = &hum_sens_ctrl, .p_cfg = &hum_sens_cfg, .p_api = &g_sf_si7013_humidity_api };

extern int __ap_mode;

ioport_port_pin_t led2 = IOPORT_PORT_06_PIN_00;

void sensors_thread_entry(void);

/* Sensors Thread entry function */
void sensors_thread_entry(void)
{
    sensor_data_t new_data;
    uint32_t i = 0;
    DATA_QUEUE_build();
    /* TODO: add your own code here */
    ASSERT( adxrs290.p_api->open( adxrs290.p_ctrl, adxrs290.p_cfg ) == SSP_SUCCESS);
    ASSERT( adxl362.p_api->open( adxl362.p_ctrl, adxl362.p_cfg ) == SSP_SUCCESS);
    ASSERT( temp_sens.p_api->open( temp_sens.p_ctrl, temp_sens.p_cfg ) == SSP_SUCCESS );
    ASSERT( hum_sens.p_api->open( hum_sens.p_ctrl, hum_sens.p_cfg ) == SSP_SUCCESS );

    ASSERT( THREADS_SYNC_setEvent( THREAD_SENSORS_RDY_EV ) == true );
    ASSERT( THREADS_SYNC_wait( THREADS_ALL_RDY_EV, TX_WAIT_FOREVER ) == true );

    ioport_level_t lvl;
    while (!__ap_mode)
    {
        if ( hum_sens.p_api->readHumidity( hum_sens.p_ctrl, &new_data.hygro.humidity ) == SSP_SUCCESS ) {
            if ( temp_sens.p_api->readTemperature(  temp_sens.p_ctrl, &new_data.hygro.temperature ) == SSP_SUCCESS ) {
                new_data.hygro.validity_flag = true;
            }
        } else {
            new_data.hygro.validity_flag = false;
        }
        if ( adxl362.p_api->readAccelerations( adxl362.p_ctrl, &new_data.acc.x_rates, &new_data.acc.y_rates, &new_data.acc.z_rates ) == SSP_SUCCESS) {
            new_data.acc.validity_flag = true;
        } else {
            new_data.acc.validity_flag = false;
        }
        if ( adxrs290.p_api->readAngularRates( adxrs290.p_ctrl, &new_data.gyro.x_rates, &new_data.gyro.y_rates ) == SSP_SUCCESS) {
            new_data.gyro.validity_flag = true;
        } else {
            new_data.gyro.validity_flag = false;
        }

        new_data.index = i++;

//        DBG("write data %d", new_data.index);
        DATA_QUEUE_write(  &new_data,  TX_WAIT_FOREVER );

        g_ioport.p_api->pinRead(led2, &lvl);
        if (lvl==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led2 ,IOPORT_LEVEL_HIGH);
        else g_ioport.p_api->pinWrite(led2 ,IOPORT_LEVEL_LOW);
        tx_thread_sleep ( CONV_MS_TO_TICK(TELEMETRY_DELAY) );
    }
}
