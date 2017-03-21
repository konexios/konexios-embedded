/***********************************************************************************************************************
 * Copyright [2016] RELOC s.r.l. - All rights strictly reserved
 * This Software is provided for evaluation purposes; any other use — including reproduction, modification, use for
 * a commercial product, distribution, or republication — without the prior written permission of the Copyright holder
 * is strictly prohibited. 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name        : sf_si7013.c
 * Description      : Si7013 Temperature and Humidity Sensor Framework Layer API Driver.
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_si7013.h"
#include "sf_si7013_cfg.h"
#include "sf_si7013_api.h"
#include "sf_si7013_private_api.h"
#include "temperature_sensor_api.h"
#include "humidity_sensor_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
/** Macro for error logger. */
#ifndef SF_SI7013_ERROR_RETURN
#define SF_SI7013_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_gpt_version)
#endif

/** Macro used to convert milliseconds to tick number */
#define CONV_MS_TO_TICK(a)          ((a)*THREADX_TICK_RATE/1000)

/** Macro used to check bits state in a byte or word */
#define BIT_IS_SET(data, i)         (((data) & (1 << (i))) ? 1 : 0)

/** Macro used to select the maximum between 2 values */
#define MAX(a,b)                    (((a) >= (b)) ? (a) : (b))

#define SI7013_INSTANCE_OPEN    (0x45648652UL)
#define SI7013_INSTANCE_CLOSED  (0x00000000UL)
#define TEMP_TMP_SIZE           (3)
#define HUMD_TMP_SIZE           TEMP_TMP_SIZE
#define SI7013_SN_A_SIZE        (8)
#define SI7013_SN_B_SIZE        (6)


static const uint8_t SI7013_CMD_TRIG_TEMP_MEAS_NOHOLD[] = { 0xF3 };
static const uint8_t SI7013_CMD_TRIG_HUMD_MEAS_NOHOLD[] = { 0xF5 };
static const uint8_t SI7013_CMD_SOFT_RESET[]            = { 0xFE };
static const uint8_t SI7013_CMD_READ_ID1[]              = { 0xFA , 0x0F };
static const uint8_t SI7013_CMD_READ_ID2[]              = { 0xFC , 0xC9 };
static const uint8_t SI7013_CMD_READ_FW_VER[]           = { 0x84 , 0xB8 };
#ifdef UNUSED_CMDS
static const uint8_t SI7013_CMD_TRIG_TEMP_MEAS_HOLD[]   = { 0xE3 };
static const uint8_t SI7013_CMD_TRIG_HUMD_MEAS_HOLD[]   = { 0xE5 };
static const uint8_t SI7013_CMD_WRITE_USER_REG[]        = { 0xE6 , 0x3A };
static const uint8_t SI7013_CMD_READ_USER_REG[]         = { 0xE7 };
static const uint8_t SI7013_CMD_WRITE_HEATER_CTRL[]     = { 0x51 , 0x00 };
static const uint8_t SI7013_CMD_READ_HEATER_CTRL[]      = { 0x11 };
static const uint8_t SI7013_CMD_TRIG_EXT_INPUT_MEAS[]   = { 0xEE };
static const uint8_t SI7013_CMD_TRIG_READ_TEMP[]        = { 0xE0 };
static const uint8_t SI7013_CMD_WR_CFG_EXT_INPUT[]      = { 0x50 };
static const uint8_t SI7013_CMD_RD_CFG_EXT_INPUT[]      = { 0x10 };
static const uint8_t SI7013_CMD_WR_CORR_EXT_INPUT[]     = { 0xC5 };
static const uint8_t SI7013_CMD_RD_CORR_EXT_INPUT[]     = { 0x84 };
#endif


#define TEMPERATURE_CONVERSION_TIME_MS          (11)
#define HUMIDITY_CONVERSION_TIME_MS             (12+TEMPERATURE_CONVERSION_TIME_MS)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Private function prototypes
 *********************************************************************************************************************/
static uint8_t SF_SI7013_computeCRC (   uint8_t* data,
                                        uint32_t data_len);

static ssp_err_t SF_SI7013_T_Open(temperature_sensor_ctrl_t * const p_ctrl, temperature_sensor_cfg_t const * const p_cfg);
static ssp_err_t SF_SI7013_T_ReadTemperature (temperature_sensor_ctrl_t * const p_ctrl, float * temperature);
static ssp_err_t SF_SI7013_T_Reset(temperature_sensor_ctrl_t * const p_ctrl);
static ssp_err_t SF_SI7013_T_Close(temperature_sensor_ctrl_t * const p_ctrl);
static ssp_err_t SF_SI7013_T_VersionGet(ssp_version_t * const p_version);
static ssp_err_t SF_SI7013_T_ReadInfo(temperature_sensor_ctrl_t * const p_ctrl, temperature_sensor_info_t* info);

static ssp_err_t SF_SI7013_H_Open(humidity_sensor_ctrl_t * const p_ctrl, humidity_sensor_cfg_t const * const p_cfg);
static ssp_err_t SF_SI7013_H_ReadHumidity (humidity_sensor_ctrl_t * const p_ctrl, float * humidity);
static ssp_err_t SF_SI7013_H_Reset(humidity_sensor_ctrl_t * const p_ctrl);
static ssp_err_t SF_SI7013_H_Close(humidity_sensor_ctrl_t * const p_ctrl);
static ssp_err_t SF_SI7013_H_VersionGet(ssp_version_t * const p_version);
static ssp_err_t SF_SI7013_H_ReadInfo(humidity_sensor_ctrl_t * const p_ctrl, humidity_sensor_info_t* info);


/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
static const ssp_version_t g_T_version =
{
    .api_version_major  = TEMPERATURE_SENSOR_API_VERSION_MAJOR,
    .api_version_minor  = TEMPERATURE_SENSOR_API_VERSION_MINOR,
    .code_version_major = SF_SI7013_CODE_VERSION_MAJOR,
    .code_version_minor = SF_SI7013_CODE_VERSION_MINOR
};

static const ssp_version_t g_H_version =
{
    .api_version_major  = HUMIDITY_SENSOR_API_VERSION_MAJOR,
    .api_version_minor  = HUMIDITY_SENSOR_API_VERSION_MINOR,
    .code_version_major = SF_SI7013_CODE_VERSION_MAJOR,
    .code_version_minor = SF_SI7013_CODE_VERSION_MINOR
};

/** Name of module used by error logger macro */
static const char g_module_name[] = "sf_si7013";

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
const temperature_sensor_api_t g_sf_si7013_temperature_api =
{
    .open               = SF_SI7013_T_Open,
    .readTemperature    = SF_SI7013_T_ReadTemperature,
    .reset              = SF_SI7013_T_Reset,
    .close              = SF_SI7013_T_Close,
    .versionGet         = SF_SI7013_T_VersionGet,
    .readInfo           = SF_SI7013_T_ReadInfo,
};

const humidity_sensor_api_t g_sf_si7013_humidity_api =
{
    .open               = SF_SI7013_H_Open,
    .readHumidity       = SF_SI7013_H_ReadHumidity,
    .reset              = SF_SI7013_H_Reset,
    .close              = SF_SI7013_H_Close,
    .versionGet         = SF_SI7013_H_VersionGet,
    .readInfo           = SF_SI7013_H_ReadInfo,
};

/**********************************************************************************************************************
 * Functions
 *********************************************************************************************************************/

/******************************************************************************************************************//**
 * @addtogroup SF_SI7013
 * @{
 *********************************************************************************************************************/
 
/*******************************************************************************************************************//**
 * @brief Implements:   temperature_sensor_api_t::open,
 *                      humidity_sensor_api_t::open.
 *
 * @retval SSP_SUCCESS          Lower level drivers opened successfully.
 * @retval SSP_ERR_ASSERTION    A pointer parameter was NULL, or a lower level driver reported this error.
 * @return                      See Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_SI7013_Open (  sf_si7013_ctrl_t    * const p_ctrl,
                            sf_si7013_cfg_t const * const p_cfg)
{
ssp_err_t err;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_cfg->device);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    /** Asserts for info buffers size check */
    SSP_ASSERT(TEMPERATURE_SENSOR_INFO_SIZE >= sizeof(sf_si7013_info_t));
    SSP_ASSERT(HUMIDITY_SENSOR_INFO_SIZE >= sizeof(sf_si7013_info_t));
    
    /** Store user configurations in control block. */
    p_ctrl->device = p_cfg->device;
    
    /** Open I2C Framework Device. */
    err = p_ctrl->device->p_api->open(  p_ctrl->device->p_ctrl,
                                        p_ctrl->device->p_cfg );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Implements temperature_sensor_api_t::readTemperature.
 *
 * @retval SSP_SUCCESS          Temperature read successfully.
 * @retval SSP_ERR_ASSERTION    A pointer parameter was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_SI7013_Open.
 * @retval SSP_ERR_ABORTED      CRC check failed.
 * @return                      See Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_SI7013_ReadTemperature (   sf_si7013_ctrl_t * const p_ctrl,
                                        float * temperature)
{
ssp_err_t err;
uint8_t tmp[TEMP_TMP_SIZE];
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != temperature);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    err = p_ctrl->device->p_api->write( p_ctrl->device->p_ctrl,
                                        (uint8_t * const)&SI7013_CMD_TRIG_TEMP_MEAS_NOHOLD[0],
                                        sizeof(SI7013_CMD_TRIG_TEMP_MEAS_NOHOLD),
                                        true,
                                        TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    tx_thread_sleep( CONV_MS_TO_TICK(TEMPERATURE_CONVERSION_TIME_MS)+1 );
    
    err = p_ctrl->device->p_api->read( p_ctrl->device->p_ctrl,
                                       &tmp[0],
                                       TEMP_TMP_SIZE,
                                       false,
                                       TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    if (tmp[2] == SF_SI7013_computeCRC( &tmp[0], (TEMP_TMP_SIZE-1) ))
    {
        /* See Si7013 Datasheet for temperature conversion detail */
        *temperature = (float)( ( ( ((float)( (tmp[0] << 8) + tmp[1] )) * 175.72 )/ 65536 ) - 46.85);
        err = SSP_SUCCESS;
    }
    else
    {
        err = SSP_ERR_ABORTED;
    }
    return err;
}

/*******************************************************************************************************************//**
 * @brief Implements humidity_sensor_api_t::readHumidity.
 *
 * @retval SSP_SUCCESS          Relative humidity read successfully.
 * @retval SSP_ERR_ASSERTION    A pointer parameter was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_SI7013_Open.
 * @retval SSP_ERR_ABORTED      CRC check failed.
 * @return                      See Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_SI7013_ReadHumidity (  sf_si7013_ctrl_t * const p_ctrl,
                                    float * humidity)
{
ssp_err_t err;
uint8_t tmp[HUMD_TMP_SIZE];
    
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != humidity);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    err = p_ctrl->device->p_api->write( p_ctrl->device->p_ctrl,
                                        (uint8_t * const) &SI7013_CMD_TRIG_HUMD_MEAS_NOHOLD[0],
                                        sizeof(SI7013_CMD_TRIG_HUMD_MEAS_NOHOLD),
                                        true,
                                        TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    tx_thread_sleep( CONV_MS_TO_TICK(HUMIDITY_CONVERSION_TIME_MS)+1 );
    
    err = p_ctrl->device->p_api->read( p_ctrl->device->p_ctrl,
                                       &tmp[0],
                                       HUMD_TMP_SIZE,
                                       false,
                                       TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    

    if (tmp[2] == SF_SI7013_computeCRC( &tmp[0], (HUMD_TMP_SIZE-1) ))
    {
        /* See Si7013 Datasheet for humidity conversion detail */
        *humidity = (float)( ( ( ((float)( tmp[0]*256 + tmp[1] )) * 125 )/ 65536 ) - 6);
        err = SSP_SUCCESS;
    }
    else
    {
        err = SSP_ERR_ABORTED;
    }
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Implements:   temperature_sensor_api_t::reset,
 *                      humidity_sensor_api_t::reset.
 *
 * @retval SSP_SUCCESS          SI7013 was reset successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_SI7013_Open.
 * @return                      See Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_SI7013_Reset (sf_si7013_ctrl_t * const p_ctrl)
{
ssp_err_t err;
    
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    err = p_ctrl->device->p_api->write( p_ctrl->device->p_ctrl,
                                        (uint8_t * const) &SI7013_CMD_SOFT_RESET[0],
                                        sizeof(SI7013_CMD_SOFT_RESET),
                                        false,
                                        TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Implements:   temperature_sensor_api_t::close,
 *                      humidity_sensor_api_t::close.
 *
 * @retval SSP_SUCCESS          Si7013 instance successfully closed.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_SI7013_Open.
 * @return                      See Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_SI7013_Close ( sf_si7013_ctrl_t * const p_ctrl)
{
ssp_err_t err;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    /** Close I2C Framework Device. */
    err = p_ctrl->device->p_api->close(p_ctrl->device->p_ctrl);
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Implements:   temperature_sensor_api_t::versionGet,
 *                      humidity_sensor_api_t::versionGet.
 *
 * @retval SSP_SUCCESS          Version returned successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_version was NULL.
 * @return                      See Common_Error_Codes.
  *********************************************************************************************************************/
ssp_err_t SF_SI7013_VersionGet (ssp_version_t * const p_version,
                                bool from_temp_int)
{
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    if (from_temp_int == true)
    {
        *p_version = g_T_version;
    }
    else
    {
        *p_version = g_H_version;
    }
    
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Implements:   temperature_sensor_api_t::readDevInfo,
 *                      humidity_sensor_api_t::readDevInfo.
 *
 * @retval SSP_SUCCESS          Si7013 serial numbers and firmware version read successfully.
 * @retval SSP_ERR_ASSERTION    A pointer parameter was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_SI7013_Open.
 * @retval SSP_ERR_ABORTED      CRC check failed.
 * @return                      See Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_SI7013_ReadInfo (  sf_si7013_ctrl_t * const p_ctrl,
                                sf_si7013_info_t* info)
{
ssp_err_t err;
uint8_t tmp[MAX(SI7013_SN_A_SIZE, SI7013_SN_B_SIZE)];
uint8_t serial_byte[sizeof(uint32_t)];
sf_si7013_info_t* si7013_info = (sf_si7013_info_t*) info;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(SI7013_SN_A_SIZE  >= 8);
    SSP_ASSERT(SI7013_SN_B_SIZE  >= 6);
    SSP_ASSERT(NULL != p_ctrl);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    /* Read Firmware Version */
    err = p_ctrl->device->p_api->write( p_ctrl->device->p_ctrl,
                                        (uint8_t * const)&SI7013_CMD_READ_FW_VER[0],
                                        sizeof(SI7013_CMD_READ_FW_VER),
                                        true,
                                        TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    err = p_ctrl->device->p_api->read( p_ctrl->device->p_ctrl,
                                       &tmp[0],
                                       1,
                                       false,
                                       TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    si7013_info->fw_ver = tmp[0];
    
    
    /* Read Serial Number Part A */
    err = p_ctrl->device->p_api->write( p_ctrl->device->p_ctrl,
                                        (uint8_t * const)&SI7013_CMD_READ_ID1[0],
                                        sizeof(SI7013_CMD_READ_ID1),
                                        true,
                                        TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    err = p_ctrl->device->p_api->read( p_ctrl->device->p_ctrl,
                                       &tmp[0],
                                       SI7013_SN_A_SIZE,
                                       false,
                                       TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    serial_byte[0] = tmp[0];
    serial_byte[1] = tmp[2];
    serial_byte[2] = tmp[4];
    serial_byte[3] = tmp[6];
    
    if( (SF_SI7013_computeCRC (&serial_byte[0], 1) == tmp[1]) &&
        (SF_SI7013_computeCRC (&serial_byte[0], 2) == tmp[3]) &&
        (SF_SI7013_computeCRC (&serial_byte[0], 3) == tmp[5]) &&
        (SF_SI7013_computeCRC (&serial_byte[0], 4) == tmp[7]) )
    {
        si7013_info->sn_a = (uint32_t) ((tmp[0] << 24) | (tmp[2] << 16) | (tmp[4] << 8) | (tmp[6] << 0));
    }
    else
    {
        SF_SI7013_ERROR_RETURN(0, SSP_ERR_ABORTED);
    }
    
    /* Read Serial Number Part B */
    err = p_ctrl->device->p_api->write( p_ctrl->device->p_ctrl,
                                        (uint8_t * const)&SI7013_CMD_READ_ID2[0],
                                        sizeof(SI7013_CMD_READ_ID2),
                                        true,
                                        TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    err = p_ctrl->device->p_api->read( p_ctrl->device->p_ctrl,
                                       &tmp[0],
                                       SI7013_SN_B_SIZE,
                                       false,
                                       TX_WAIT_FOREVER );
    SF_SI7013_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    serial_byte[0] = tmp[0];
    serial_byte[1] = tmp[1];
    serial_byte[2] = tmp[3];
    serial_byte[3] = tmp[4];
    
    if( (SF_SI7013_computeCRC (&serial_byte[0], 2) == tmp[2]) &&
        (SF_SI7013_computeCRC (&serial_byte[0], 4) == tmp[5]) )
    {
        si7013_info->sn_b = (uint32_t) ((tmp[0] << 24) | (tmp[1] << 16) | (tmp[3] << 8) | (tmp[4] << 0));
    }
    else
    {
        SF_SI7013_ERROR_RETURN(0, SSP_ERR_ABORTED);
    }
    
    return SSP_SUCCESS;
}



/* Polynomial used = x^8 + x^5 + x^4 + 1                */
/* Initialization = 0x00                                */
/*      See page 22 of the Si7013 Datasheet             */
static uint8_t SF_SI7013_computeCRC (   uint8_t* data,
                                        uint32_t data_len)
{
uint8_t result = 0;
bool crc_bit[8];
uint32_t n;
uint8_t i;
bool invert;
    
    /* Initialization byte = 0x00 */
    for (i = 0; i < 8; i++)
    {
        crc_bit[i] = 0;
    }
    
    for (n = 0; n < data_len; n++)
    {
        for (i=8; i > 0; i--)       /* MSB first! */
        {
            invert = (BIT_IS_SET(data[n], (i-1))) ^ crc_bit[7];
            
            crc_bit[7] = crc_bit[6];
            crc_bit[6] = crc_bit[5];
            crc_bit[5] = crc_bit[4] ^ invert;
            crc_bit[4] = crc_bit[3] ^ invert;
            crc_bit[3] = crc_bit[2];
            crc_bit[2] = crc_bit[1];
            crc_bit[1] = crc_bit[0];
            crc_bit[0] = invert;
        }
    }
    
    /* Compose the CRC byte */
    for (i = 0; i < 8; i++)
    {
        result = result | ((uint8_t)(crc_bit[i] << i));
    }
    
    return result;
}


/**********************************************************************************************************************
 * Interface Specific API ---- TEMPERATURE API
 *********************************************************************************************************************/
static ssp_err_t SF_SI7013_T_Open(  temperature_sensor_ctrl_t * const p_ctrl_temp,
                                    temperature_sensor_cfg_t const * const p_cfg_temp)
{
ssp_err_t err = SSP_SUCCESS;
sf_si7013_ctrl_t * p_ctrl;
sf_si7013_cfg_t * p_cfg;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_temp);
    SSP_ASSERT(NULL != p_cfg_temp);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    /** Store user configurations in control block. */
    p_ctrl_temp->p_extend_ctrl = p_cfg_temp->p_extend_ctrl;
    
    /** Cast to implementation specific structures */
    p_ctrl = (sf_si7013_ctrl_t*) p_cfg_temp->p_extend_ctrl;
    p_cfg = (sf_si7013_cfg_t*) p_cfg_temp->p_extend_cfg;
    
    p_ctrl->temp_open = SI7013_INSTANCE_OPEN;
    if (p_ctrl->hum_open != SI7013_INSTANCE_OPEN)
    {
        /* Open if the other interface was not already open only */
        err = SF_SI7013_Open ( p_ctrl, p_cfg);
    }
    return err;
}

static ssp_err_t SF_SI7013_T_ReadTemperature (  temperature_sensor_ctrl_t * const p_ctrl_temp,
                                                float * temperature)
{
sf_si7013_ctrl_t * p_ctrl;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_temp);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    p_ctrl = (sf_si7013_ctrl_t * const) p_ctrl_temp->p_extend_ctrl;
    SF_SI7013_ERROR_RETURN(SI7013_INSTANCE_OPEN == p_ctrl->temp_open, SSP_ERR_NOT_OPEN);
    return SF_SI7013_ReadTemperature (p_ctrl, temperature);
}

static ssp_err_t SF_SI7013_T_Reset(temperature_sensor_ctrl_t * const p_ctrl_temp)
{
sf_si7013_ctrl_t * p_ctrl;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_temp);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    p_ctrl = (sf_si7013_ctrl_t * const) p_ctrl_temp->p_extend_ctrl;
    SF_SI7013_ERROR_RETURN(SI7013_INSTANCE_OPEN == p_ctrl->temp_open, SSP_ERR_NOT_OPEN);
    return SF_SI7013_Reset(p_ctrl);
}

static ssp_err_t SF_SI7013_T_Close(temperature_sensor_ctrl_t * const p_ctrl_temp)
{
ssp_err_t err;
sf_si7013_ctrl_t * p_ctrl;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_temp);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    p_ctrl = (sf_si7013_ctrl_t * const) p_ctrl_temp->p_extend_ctrl;
    SF_SI7013_ERROR_RETURN(SI7013_INSTANCE_OPEN == p_ctrl->temp_open, SSP_ERR_NOT_OPEN);
    p_ctrl->temp_open = SI7013_INSTANCE_CLOSED;
    if (p_ctrl->hum_open != SI7013_INSTANCE_OPEN)
    {
        /* Close if both interfaces have been closed only */
        err = SF_SI7013_Close (p_ctrl);
    }
    else
    {
        err = SSP_SUCCESS;
    }
    return err;
}

static ssp_err_t SF_SI7013_T_VersionGet(ssp_version_t * const p_version)
{
    return SF_SI7013_VersionGet (p_version, true);
}

static ssp_err_t SF_SI7013_T_ReadInfo(  temperature_sensor_ctrl_t* const p_ctrl_temp,
                                        temperature_sensor_info_t* info)
{
sf_si7013_ctrl_t * p_ctrl;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_temp);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    p_ctrl = (sf_si7013_ctrl_t * const) p_ctrl_temp->p_extend_ctrl;
    SF_SI7013_ERROR_RETURN(SI7013_INSTANCE_OPEN == p_ctrl->temp_open, SSP_ERR_NOT_OPEN);
    return SF_SI7013_ReadInfo(p_ctrl, (sf_si7013_info_t*) info);
}


/**********************************************************************************************************************
 * Interface Specific API ---- HUMIDITY API
 *********************************************************************************************************************/
static ssp_err_t SF_SI7013_H_Open(  humidity_sensor_ctrl_t * const p_ctrl_hum,
                                    humidity_sensor_cfg_t const * const p_cfg_hum)
{
ssp_err_t err = SSP_SUCCESS;
sf_si7013_ctrl_t * p_ctrl;
sf_si7013_cfg_t * p_cfg;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_hum);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    
    /** Store user configurations in control block. */
    p_ctrl_hum->p_extend_ctrl = p_cfg_hum->p_extend_ctrl;
    
    /** Cast to implementation specific structures */
    p_ctrl = (sf_si7013_ctrl_t*) p_cfg_hum->p_extend_ctrl;
    p_cfg = (sf_si7013_cfg_t*) p_cfg_hum->p_extend_cfg;
    
    p_ctrl->hum_open = SI7013_INSTANCE_OPEN;
    if (p_ctrl->temp_open != SI7013_INSTANCE_OPEN)
    {
        /* Open if the other interface was not already open only */
        err = SF_SI7013_Open ( p_ctrl, p_cfg);
    }
    return err;
}

static ssp_err_t SF_SI7013_H_ReadHumidity(  humidity_sensor_ctrl_t * const p_ctrl_hum,
                                            float * humidity)
{
sf_si7013_ctrl_t * p_ctrl;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_hum);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    p_ctrl = (sf_si7013_ctrl_t * const) p_ctrl_hum->p_extend_ctrl;
    SF_SI7013_ERROR_RETURN(SI7013_INSTANCE_OPEN == p_ctrl->hum_open, SSP_ERR_NOT_OPEN);
    return SF_SI7013_ReadHumidity (p_ctrl, humidity);
}

static ssp_err_t SF_SI7013_H_Reset(humidity_sensor_ctrl_t * const p_ctrl_hum)
{
sf_si7013_ctrl_t * p_ctrl;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_hum);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    p_ctrl = (sf_si7013_ctrl_t * const) p_ctrl_hum->p_extend_ctrl;
    SF_SI7013_ERROR_RETURN(SI7013_INSTANCE_OPEN == p_ctrl->hum_open, SSP_ERR_NOT_OPEN);
    return SF_SI7013_Reset(p_ctrl);
}

static ssp_err_t SF_SI7013_H_Close(humidity_sensor_ctrl_t * const p_ctrl_hum)
{
ssp_err_t err;
sf_si7013_ctrl_t * p_ctrl;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_hum);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    p_ctrl = (sf_si7013_ctrl_t * const) p_ctrl_hum->p_extend_ctrl;
    SF_SI7013_ERROR_RETURN(SI7013_INSTANCE_OPEN == p_ctrl->hum_open, SSP_ERR_NOT_OPEN);
    p_ctrl->hum_open = SI7013_INSTANCE_CLOSED;
    if (p_ctrl->temp_open != SI7013_INSTANCE_OPEN)
    {
        /* Close if both interfaces have been closed only */
        err = SF_SI7013_Close (p_ctrl);
    }
    else
    {
        err = SSP_SUCCESS;
    }
    return err;
}

static ssp_err_t SF_SI7013_H_VersionGet(ssp_version_t * const p_version)
{
    return SF_SI7013_VersionGet(p_version, false);
}

static ssp_err_t SF_SI7013_H_ReadInfo(  humidity_sensor_ctrl_t * const p_ctrl_hum,
                                        humidity_sensor_info_t* info)
{
sf_si7013_ctrl_t * p_ctrl;
#if SF_SI7013_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl_hum);
#endif /* if SF_SI7013_CFG_PARAM_CHECKING_ENABLE */
    p_ctrl = (sf_si7013_ctrl_t * const) p_ctrl_hum->p_extend_ctrl;
    SF_SI7013_ERROR_RETURN(SI7013_INSTANCE_OPEN == p_ctrl->hum_open, SSP_ERR_NOT_OPEN);
    return SF_SI7013_ReadInfo(p_ctrl, (sf_si7013_info_t*) info);
}
/******************************************************************************************************************//**
 * @}
 *********************************************************************************************************************/
