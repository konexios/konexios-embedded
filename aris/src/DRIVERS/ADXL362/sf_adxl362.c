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
 * File Name        : sf_adxl362.c
 * Description      : ADXL362 3-Axis MEMS Accelerometer Framework Layer API Driver.
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_adxl362.h"
#include "sf_adxl362_cfg.h"
#include "sf_adxl362_private_api.h"
#include "r_spi_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
/** Macro for error logger. */
#ifndef SF_ADXL362_ERROR_RETURN
#define SF_ADXL362_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_gpt_version)
#endif


#define ADXL362_INSTANCE_OPEN    (0x45648652UL)
#define ADXL362_INSTANCE_CLOSED  (0x00000000UL)

#define TMP_READ_BUF_LEN            3
#define TMP_WRITE_BUF_LEN           3
#define TMP_ACC_BUF_LEN             8

#define ADXL362_CMD_READ_REG        0x0B
#define ADXL362_CMD_WRITE_REG       0x0A
#define ADXL362_CMD_READ_FIFO       0x0D

#define ADXL362_REG_DEVID_AD        0x00
#define ADXL362_REG_DEVID_MST       0x01
#define ADXL362_REG_PARTID          0x02
#define ADXL362_REG_REVID           0x03
#define ADXL362_REG_XDATA           0x08
#define ADXL362_REG_YDATA           0x09
#define ADXL362_REG_ZDATA           0x0A
#define ADXL362_REG_STATUS          0x0B
#define ADXL362_REG_FIFO_ENT_L      0x0C
#define ADXL362_REG_FIFO_ENT_H      0x0D
#define ADXL362_REG_XDATA_L         0x0E
#define ADXL362_REG_XDATA_H         0x0F
#define ADXL362_REG_YDATA_L         0x10
#define ADXL362_REG_YDATA_H         0x11
#define ADXL362_REG_ZDATA_L         0x12
#define ADXL362_REG_ZDATA_H         0x13
#define ADXL362_REG_TEMP_L          0x14
#define ADXL362_REG_TEMP_H          0x15
#define ADXL362_REG_SOFT_RESET      0x1F
#define ADXL362_REG_THRESH_ACT_L    0x20
#define ADXL362_REG_THRESH_ACT_H    0x21
#define ADXL362_REG_TIME_ACT        0x22
#define ADXL362_REG_THRESH_INACT_L  0x23
#define ADXL362_REG_THRESH_INACT_H  0x24
#define ADXL362_REG_TIME_INACT_L    0x25
#define ADXL362_REG_TIME_INACT_H    0x26
#define ADXL362_REG_ACT_INACT_CTL   0x27
#define ADXL362_REG_FIFO_CONTROL    0x28
#define ADXL362_REG_FIFO_SAMPLES    0x29
#define ADXL362_REG_INTMAP1         0x2A
#define ADXL362_REG_INTMAP2         0x2B
#define ADXL362_REG_FILTER_CTL      0x2C
#define ADXL362_REG_POWER_CTL       0x2D
#define ADXL362_REG_SELF_TEST       0x2E

#define ADXL362_REG_DEVID_AD_DEFVALUE       0xAD
#define ADXL362_REG_DEVID_MST_DEFVALUE      0x1D
#define ADXL362_REG_PARTID_DEFVALUE         0xF2
#define ADXL362_RESET_KEYCODE               0x52

#define ADXL362_2G_RANGE_SENSITIVITY        1000    /* [LSB/g] */
#define ADXL362_4G_RANGE_SENSITIVITY        500     /* [LSB/g] */
#define ADXL362_8G_RANGE_SENSITIVITY        250     /* [LSB/g] */

/* STATUS Register bits     */
#define ADXL362_STATUS_REG_USR_ERR_BIT      0x80
#define ADXL362_STATUS_REG_AWAKE_BIT        0x40
#define ADXL362_STATUS_REG_INACT_BIT        0x20
#define ADXL362_STATUS_REG_ACT_BIT          0x10
#define ADXL362_STATUS_REG_FIFO_OVR_BIT     0x08
#define ADXL362_STATUS_REG_FIFO_WATM_BIT    0x04
#define ADXL362_STATUS_REG_FIFO_RDY_BIT     0x02
#define ADXL362_STATUS_REG_DATA_RDY_BIT     0x01

#define ADXL362_RESET_TIME_MS       (11)

#define ADXL362_READ_TIMEOUT_CYCLES  1000
/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Private function prototypes
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
static const ssp_version_t g_version =
{
    .api_version_major  = SF_ADXL362_API_VERSION_MAJOR,
    .api_version_minor  = SF_ADXL362_API_VERSION_MINOR,
    .code_version_major = SF_ADXL362_CODE_VERSION_MAJOR,
    .code_version_minor = SF_ADXL362_CODE_VERSION_MINOR
};

/** Name of module used by error logger macro */
static const char g_module_name[] = "sf_adxl362";

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
const sf_adxl362_api_t g_sf_adxl362_api =
{
    .open               = SF_ADXL362_Open,
    .readAccelerations  = SF_ADXL362_ReadAccelerations,
    .reset              = SF_ADXL362_Reset,
    .close              = SF_ADXL362_Close,
    .versionGet         = SF_ADXL362_VersionGet,
};

/**********************************************************************************************************************
 * Functions
 *********************************************************************************************************************/
static ssp_err_t SF_ADXL362_readRegister (  sf_adxl362_ctrl_t * const p_ctrl,
                                            const uint8_t reg_addr,
                                            uint8_t * reg_value );

static ssp_err_t SF_ADXL362_writeRegister ( sf_adxl362_ctrl_t * const p_ctrl,
                                            const uint8_t reg_addr,
                                            uint8_t reg_value );

static ssp_err_t SF_ADXL362_initDevice (sf_adxl362_ctrl_t * const p_ctrl);
/******************************************************************************************************************//**
 * @addtogroup SF_ADXL362
 * @{
 *********************************************************************************************************************/
 
/******************************************************************************************************************//**
 * @brief Implements sf_adxl362_api_t::open.
 *
 * @retval SSP_SUCCESS          Lower level drivers opened successfully.
 * @retval SSP_ERR_ASSERTION    A pointer paramer was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_ABORTED      Test read of device identification register failed.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_ADXL362_Open ( sf_adxl362_ctrl_t    * const p_ctrl,
                            sf_adxl362_cfg_t const * const p_cfg)
{
ssp_err_t err;
uint8_t tmp_data;
#if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_cfg->device);
    SSP_ASSERT(TMP_READ_BUF_LEN == 3);
    SSP_ASSERT(TMP_WRITE_BUF_LEN == 3);
    SSP_ASSERT(TMP_ACC_BUF_LEN == 8);
#endif /* if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE */
    
    /** Store user configurations in control block. */
    p_ctrl->device = p_cfg->device;
    
    /** Open SPI Framework Device. */
    err = p_ctrl->device->p_api->open(  p_ctrl->device->p_ctrl,
                                        p_ctrl->device->p_cfg );
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    /** Reset device in order to reset register values */
    err = SF_ADXL362_writeRegister (p_ctrl,
                                    ADXL362_REG_SOFT_RESET,
                                    ADXL362_RESET_KEYCODE );
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    /** A latency of approximately 0.5 ms is required after soft reset */
    tx_thread_sleep((THREADX_TICK_RATE/1000)+1);
    
    /* Check Device AD ID */
    err = SF_ADXL362_readRegister ( p_ctrl,
                                    ADXL362_REG_DEVID_AD,
                                    &tmp_data );
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    SF_ADXL362_ERROR_RETURN(ADXL362_REG_DEVID_AD_DEFVALUE == tmp_data, SSP_ERR_ABORTED);
    
    /* Check Device MEMS ID */
    err = SF_ADXL362_readRegister ( p_ctrl,
                                    ADXL362_REG_DEVID_MST,
                                    &tmp_data );
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    SF_ADXL362_ERROR_RETURN(ADXL362_REG_DEVID_MST_DEFVALUE == tmp_data, SSP_ERR_ABORTED);
    
    /* Check Device PART ID */
    err = SF_ADXL362_readRegister ( p_ctrl,
                                    ADXL362_REG_PARTID,
                                    &tmp_data );
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    SF_ADXL362_ERROR_RETURN(ADXL362_REG_PARTID_DEFVALUE == tmp_data, SSP_ERR_ABORTED);
    
    /* Configure device for continuous measure */
    err = SF_ADXL362_initDevice( p_ctrl );
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    switch( (ADXL362_FILTER_SETTINGS & ADXL362_RANGE_MASK))
    {
        case ADXL362_2G_RANGE   : p_ctrl->scale_factor = ADXL362_2G_RANGE_SENSITIVITY;  break;
        case ADXL362_4G_RANGE   : p_ctrl->scale_factor = ADXL362_4G_RANGE_SENSITIVITY;  break;
        case ADXL362_8G_RANGE   : p_ctrl->scale_factor = ADXL362_8G_RANGE_SENSITIVITY;  break;
        default                 : p_ctrl->scale_factor = 0; SSP_ASSERT(0);              break;
    }


    /** Mark control block open so other tasks know it is valid. */
    p_ctrl->open = ADXL362_INSTANCE_OPEN;
    
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_adxl362_api_t::readTemperature.
 *
 * @retval SSP_SUCCESS          XYZ-axies accelerations read successfully.
 * @retval SSP_ERR_ASSERTION    A pointer paramer was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_ADXL362_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_ADXL362_ReadAccelerations (sf_adxl362_ctrl_t * const p_ctrl,
                                        float * x_value,
                                        float * y_value,
                                        float * z_value)
{
ssp_err_t err;
uint8_t tmp_buf_src[TMP_ACC_BUF_LEN];
uint8_t tmp_buf_dst[TMP_ACC_BUF_LEN];
int16_t tmp_value;
uint16_t i = 0;
#if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != x_value);
    SSP_ASSERT(NULL != y_value);
    SSP_ASSERT(NULL != z_value);
    SF_ADXL362_ERROR_RETURN(ADXL362_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif /* if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE */
    
    /* Check DATA READY bit */
    do
    {
        err = SF_ADXL362_readRegister ( p_ctrl,
                                        ADXL362_REG_STATUS,
                                        &tmp_buf_dst[0] );
        SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
        if( i >= ADXL362_READ_TIMEOUT_CYCLES )
        {
            err = SSP_ERR_TIMEOUT;
            break;
        }
        else
        {
            i++;
        }
    }
    while (!(tmp_buf_dst[0] & ADXL362_STATUS_REG_DATA_RDY_BIT));


    if (err == SSP_SUCCESS)
    {
        /* Read data */
        tmp_buf_src[0] = ADXL362_CMD_READ_REG;
        tmp_buf_src[1] = ADXL362_REG_XDATA_L;
        for (i=2 ; i<TMP_ACC_BUF_LEN ; i++ )
        {
            tmp_buf_src[i] = 0;
        }
        err = p_ctrl->device->p_api->writeRead (p_ctrl->device->p_ctrl,
                                                tmp_buf_src,
                                                tmp_buf_dst,
                                                TMP_ACC_BUF_LEN,
                                                SPI_BIT_WIDTH_8_BITS,
                                                TX_WAIT_FOREVER);
        if (SSP_SUCCESS == err)
        {
            ((uint8_t*) &tmp_value)[0] = tmp_buf_dst[2];
            ((uint8_t*) &tmp_value)[1] = tmp_buf_dst[3];
            *x_value = ((float) tmp_value) / p_ctrl->scale_factor;
            
            ((uint8_t*) &tmp_value)[0] = tmp_buf_dst[4];
            ((uint8_t*) &tmp_value)[1] = tmp_buf_dst[5];
            *y_value = ((float) tmp_value) / p_ctrl->scale_factor;
            
            ((uint8_t*) &tmp_value)[0] = tmp_buf_dst[6];
            ((uint8_t*) &tmp_value)[1] = tmp_buf_dst[7];
            *z_value = ((float) tmp_value) / p_ctrl->scale_factor;
        }
    }
    return err;
}

/******************************************************************************************************************//**
 * @brief Implements sf_dummy_api_t::reset.
 *
 * @retval SSP_SUCCESS          ADXL362 were reset successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_ADXL362_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_ADXL362_Reset (sf_adxl362_ctrl_t * const p_ctrl)
{
ssp_err_t err;
    
#if SF_DUMMY_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_ADXL362_ERROR_RETURN(ADXL362_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif /* if SF_DUMMY_CFG_PARAM_CHECKING_ENABLE */
    
    /** Reset device in order to stop running measurements and reset register values */
    err = SF_ADXL362_writeRegister (p_ctrl,
                                    ADXL362_REG_SOFT_RESET,
                                    ADXL362_RESET_KEYCODE );
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    tx_thread_sleep((ADXL362_RESET_TIME_MS*THREADX_TICK_RATE/1000)+1);
    
    /* Configure device for continuous measure */
    err = SF_ADXL362_initDevice( p_ctrl );
    
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_adxl362_api_t::close.
 *
 * @retval SSP_SUCCESS          ADXL362 instance successfully closed and device resetted.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_ADXL362_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_ADXL362_Close (sf_adxl362_ctrl_t * const p_ctrl)
{
ssp_err_t err;
#if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_ADXL362_ERROR_RETURN(ADXL362_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif /* if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE */
    
    
    /** Change device mode to standby */
    err = SF_ADXL362_writeRegister (p_ctrl,
                                    ADXL362_REG_POWER_CTL,
                                    ADXL362_MEAS_MODE_STANDBY );
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    /** Close SPI Framework Device. */
    err = p_ctrl->device->p_api->close(p_ctrl->device->p_ctrl);
    SF_ADXL362_ERROR_RETURN(SSP_SUCCESS == err, err);

    /** Mark control block open so other tasks know it is NOT valid. */
    p_ctrl->open = ADXL362_INSTANCE_CLOSED;
    
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_adxl362_api_t::versionGet.
 *
 * @retval SSP_SUCCESS          Version returned successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_version was NULL.
 * @return                      See @ref Common_Error_Codes.
  *********************************************************************************************************************/
ssp_err_t SF_ADXL362_VersionGet (ssp_version_t * const p_version)
{
#if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif /* if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE */
    
    *p_version = g_version;
    
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements static SF_ADXL362_initDevice
 *
 * @retval SSP_SUCCESS          Device initialization successfully.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
  *********************************************************************************************************************/
static ssp_err_t SF_ADXL362_initDevice (sf_adxl362_ctrl_t * const p_ctrl)
{
ssp_err_t err;
#if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif /* if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE */
    
    /** Configure Device writing filter and power control registers */
    err = SF_ADXL362_writeRegister (p_ctrl,
                                    ADXL362_REG_FILTER_CTL,
                                    ADXL362_FILTER_SETTINGS );
    if (err == SSP_SUCCESS)
    {
        err = SF_ADXL362_writeRegister (p_ctrl,
                                        ADXL362_REG_POWER_CTL,
                                        ADXL362_POWER_SETTING );
    }
    return err;
}

/******************************************************************************************************************//**
 * @brief Implements static SF_ADXL362_readRegister
 *
 * @retval SSP_SUCCESS          Read procedure successfully.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
  *********************************************************************************************************************/
static ssp_err_t SF_ADXL362_readRegister (  sf_adxl362_ctrl_t * const p_ctrl,
                                            const uint8_t reg_addr,
                                            uint8_t * reg_value )
{
ssp_err_t err;
uint8_t tmp_buf_src[TMP_READ_BUF_LEN];
uint8_t tmp_buf_dst[TMP_READ_BUF_LEN];
#if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != reg_value);
#endif /* if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE */
    
    tmp_buf_src[0] = ADXL362_CMD_READ_REG;
    tmp_buf_src[1] = reg_addr;
    tmp_buf_src[2] = 0x00;
    err = p_ctrl->device->p_api->writeRead (p_ctrl->device->p_ctrl,
                                            tmp_buf_src,
                                            tmp_buf_dst,
                                            TMP_READ_BUF_LEN,
                                            SPI_BIT_WIDTH_8_BITS,
                                            TX_WAIT_FOREVER);
    if (err == SSP_SUCCESS)
    {
        *reg_value = tmp_buf_dst[2];
    }
    
    return err;
}

/******************************************************************************************************************//**
 * @brief Implements static SF_ADXL362_writeRegister
 *
 * @retval SSP_SUCCESS          Write procedure successfully.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
  *********************************************************************************************************************/
static ssp_err_t SF_ADXL362_writeRegister ( sf_adxl362_ctrl_t * const p_ctrl,
                                            const uint8_t reg_addr,
                                            uint8_t reg_value )
{
ssp_err_t err;
uint8_t tmp_buf[TMP_WRITE_BUF_LEN];
#if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif /* if SF_ADXL362_CFG_PARAM_CHECKING_ENABLE */
    
    tmp_buf[0] = ADXL362_CMD_WRITE_REG;
    tmp_buf[1] = reg_addr;
    tmp_buf[2] = reg_value;
    err = p_ctrl->device->p_api->write (p_ctrl->device->p_ctrl,
                                        tmp_buf,
                                        TMP_READ_BUF_LEN,
                                        SPI_BIT_WIDTH_8_BITS,
                                        TX_WAIT_FOREVER);
    return err;
}
/******************************************************************************************************************//**
 * @}
 *********************************************************************************************************************/
