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
 * File Name        : sf_adxrs290.c
 * Description      : ADXRS290 Dual-Axis MEMS Gyroscope Framework Layer API Driver.
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_adxrs290.h"
#include "sf_adxrs290_cfg.h"
#include "sf_adxrs290_private_api.h"
#include "r_spi_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
/** Macro for error logger. */
#ifndef SF_ADXRS290_ERROR_RETURN
#define SF_ADXRS290_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_gpt_version)
#endif


#define ADXRS290_INSTANCE_OPEN    (0x45648652UL)
#define ADXRS290_INSTANCE_CLOSED  (0x00000000UL)

#define TMP_READ_BUF_LEN            2
#define TMP_WRITE_BUF_LEN           2
#define TMP_ACC_BUF_LEN             5

#define ADXRS290_READ_FLAG          0x80
#define ADXRS290_WRITE_FLAG         0x00

#define ADXRS290_REG_DEVID_AD       0x00
#define ADXRS290_REG_DEVID_MST      0x01
#define ADXRS290_REG_PARTID         0x02
#define ADXRS290_REG_REVID          0x03
#define ADXRS290_REG_SN0            0x04
#define ADXRS290_REG_SN1            0x05
#define ADXRS290_REG_SN2            0x06
#define ADXRS290_REG_SN3            0x07
#define ADXRS290_REG_DATAX0         0x08
#define ADXRS290_REG_DATAX1         0x09
#define ADXRS290_REG_DATAY0         0x0A
#define ADXRS290_REG_DATAY1         0x0B
#define ADXRS290_REG_TEMP0          0x0C
#define ADXRS290_REG_TEMP1          0x0D
#define ADXRS290_REG_POWER_CTL      0x10
#define ADXRS290_REG_FILTER         0x11
#define ADXRS290_REG_DATA_READY     0x12

#define ADXRS290_REG_DEVID_AD_DEFVALUE       0xAD
#define ADXRS290_REG_DEVID_MST_DEFVALUE      0x1D
#define ADXRS290_REG_PARTID_DEFVALUE         0x92

#define ADXRS290_SENSITIVITY        200     /* [LSB/°/s] */

#define ADXRS290_BOOT_TIME_MS       (100)
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
    .api_version_major  = SF_ADXRS290_API_VERSION_MAJOR,
    .api_version_minor  = SF_ADXRS290_API_VERSION_MINOR,
    .code_version_major = SF_ADXRS290_CODE_VERSION_MAJOR,
    .code_version_minor = SF_ADXRS290_CODE_VERSION_MINOR
};

/** Name of module used by error logger macro */
static const char g_module_name[] = "sf_adxrs290";

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
const sf_adxrs290_api_t g_sf_adxrs290_api =
{
    .open               = SF_ADXRS290_Open,
    .readAngularRates   = SF_ADXRS290_ReadAngularRates,
    .reset              = SF_ADXRS290_Reset,
    .close              = SF_ADXRS290_Close,
    .versionGet         = SF_ADXRS290_VersionGet,
};

/**********************************************************************************************************************
 * Functions
 *********************************************************************************************************************/
static ssp_err_t SF_ADXRS290_readRegister (  sf_adxrs290_ctrl_t * const p_ctrl,
                                            const uint8_t reg_addr,
                                            uint8_t * reg_value );

static ssp_err_t SF_ADXRS290_writeRegister ( sf_adxrs290_ctrl_t * const p_ctrl,
                                            const uint8_t reg_addr,
                                            uint8_t reg_value );

static ssp_err_t SF_ADXRS290_initDevice (sf_adxrs290_ctrl_t * const p_ctrl);
/******************************************************************************************************************//**
 * @addtogroup SF_ADXRS290
 * @{
 *********************************************************************************************************************/
 
/******************************************************************************************************************//**
 * @brief Implements sf_adxrs290_api_t::open.
 *
 * @retval SSP_SUCCESS          Lower level drivers opened successfully.
 * @retval SSP_ERR_ASSERTION    A pointer paramer was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_ABORTED      Test read of device identification register failed.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_ADXRS290_Open (sf_adxrs290_ctrl_t    * const p_ctrl,
                            sf_adxrs290_cfg_t const * const p_cfg)
{
ssp_err_t err;
uint8_t tmp_data;
uint32_t hei = 10000000;
#if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_cfg->device);
    SSP_ASSERT(TMP_READ_BUF_LEN == 2);
    SSP_ASSERT(TMP_WRITE_BUF_LEN == 2);
    SSP_ASSERT(TMP_ACC_BUF_LEN == 5);
#endif /* if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE */
    
    /** Store user configurations in control block. */
    p_ctrl->device = p_cfg->device;
    
    /** Open SPI Framework Device. */
    err = p_ctrl->device->p_api->open(  p_ctrl->device->p_ctrl,
                                        p_ctrl->device->p_cfg );
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);

    while (hei--) {;}

    /* Check Device AD ID */
    err = SF_ADXRS290_readRegister (p_ctrl,
                                    ADXRS290_REG_DEVID_AD,
                                    &tmp_data );
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);
    SF_ADXRS290_ERROR_RETURN(ADXRS290_REG_DEVID_AD_DEFVALUE == tmp_data, SSP_ERR_ABORTED);
    
    /* Check Device MEMS ID */
    err = SF_ADXRS290_readRegister ( p_ctrl,
                                    ADXRS290_REG_DEVID_MST,
                                    &tmp_data );
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);
    SF_ADXRS290_ERROR_RETURN(ADXRS290_REG_DEVID_MST_DEFVALUE == tmp_data, SSP_ERR_ABORTED);
    
    /* Check Device PART ID */
    err = SF_ADXRS290_readRegister ( p_ctrl,
                                    ADXRS290_REG_PARTID,
                                    &tmp_data );
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);
    SF_ADXRS290_ERROR_RETURN(ADXRS290_REG_PARTID_DEFVALUE == tmp_data, SSP_ERR_ABORTED);
    
    /* Configure device for measurement operation */
    err = SF_ADXRS290_initDevice( p_ctrl );
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    /** Mark control block open so other tasks know it is valid. */
    p_ctrl->open = ADXRS290_INSTANCE_OPEN;
    
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_adxrs290_api_t::readAngularRates.
 *
 * @retval SSP_SUCCESS          XY-axies angular rates read successfully.
 * @retval SSP_ERR_ASSERTION    A pointer paramer was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_ADXRS290_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_ADXRS290_ReadAngularRates (sf_adxrs290_ctrl_t * const p_ctrl,
                                        float * x_value,
                                        float * y_value )
{
ssp_err_t err;
uint8_t tmp_buf_src[TMP_ACC_BUF_LEN];
uint8_t tmp_buf_dst[TMP_ACC_BUF_LEN];
int16_t tmp_value;
#if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != x_value);
    SSP_ASSERT(NULL != y_value);
    SF_ADXRS290_ERROR_RETURN(ADXRS290_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif /* if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE */
    
    tmp_buf_src[0] = (ADXRS290_READ_FLAG | ADXRS290_REG_DATAX0);
    err = p_ctrl->device->p_api->writeRead (p_ctrl->device->p_ctrl,
                                            tmp_buf_src,
                                            tmp_buf_dst,
                                            TMP_ACC_BUF_LEN,
                                            SPI_BIT_WIDTH_8_BITS,
                                            TX_WAIT_FOREVER);
    if (SSP_SUCCESS == err)
    {
        ((uint8_t*) &tmp_value)[0] = tmp_buf_dst[1];
        ((uint8_t*) &tmp_value)[1] = tmp_buf_dst[2];
        *x_value = ((float) tmp_value)/ ADXRS290_SENSITIVITY;
        
        ((uint8_t*) &tmp_value)[0] = tmp_buf_dst[3];
        ((uint8_t*) &tmp_value)[1] = tmp_buf_dst[4];
        *y_value = ((float) tmp_value)/ ADXRS290_SENSITIVITY;
    }
    
    return err;
}

/******************************************************************************************************************//**
 * @brief Implements sf_dummy_api_t::reset.
 *
 * @retval SSP_SUCCESS          ADXRS290 were reset successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_ADXRS290_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_ADXRS290_Reset (sf_adxrs290_ctrl_t * const p_ctrl)
{
ssp_err_t err;
#if SF_DUMMY_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_ADXRS290_ERROR_RETURN(ADXRS290_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif /* if SF_DUMMY_CFG_PARAM_CHECKING_ENABLE */
    
    /** Stop running measurements */
    err = SF_ADXRS290_writeRegister (   p_ctrl,
                                        ADXRS290_REG_POWER_CTL,
                                        (ADXRS290_MEAS_DISABLE | ADXRS290_MEAS_DISABLE) );
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    /* Configure device for continuous measure */
    err = SF_ADXRS290_initDevice( p_ctrl );
    
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_adxrs290_api_t::close.
 *
 * @retval SSP_SUCCESS          ADXRS290 instance successfully closed and device set in standby.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_ADXRS290_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_ADXRS290_Close (sf_adxrs290_ctrl_t * const p_ctrl)
{
ssp_err_t err;
#if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_ADXRS290_ERROR_RETURN(ADXRS290_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif /* if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE */
    
    /** Stop running measurements */
    err = SF_ADXRS290_writeRegister (   p_ctrl,
                                        ADXRS290_REG_POWER_CTL,
                                        (ADXRS290_MEAS_DISABLE | ADXRS290_MEAS_DISABLE) );
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);
    
    /** Close SPI Framework Device. */
    err = p_ctrl->device->p_api->close(p_ctrl->device->p_ctrl);
    SF_ADXRS290_ERROR_RETURN(SSP_SUCCESS == err, err);

    /** Mark control block open so other tasks know it is NOT valid. */
    p_ctrl->open = ADXRS290_INSTANCE_CLOSED;
    
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements sf_adxrs290_api_t::versionGet.
 *
 * @retval SSP_SUCCESS          Version returned successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_version was NULL.
 * @return                      See @ref Common_Error_Codes.
  *********************************************************************************************************************/
ssp_err_t SF_ADXRS290_VersionGet (ssp_version_t * const p_version)
{
#if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif /* if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE */
    
    *p_version = g_version;
    
    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Implements static SF_ADXRS290_initDevice
 *
 * @retval SSP_SUCCESS          Device initialization successfully.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
  *********************************************************************************************************************/
static ssp_err_t SF_ADXRS290_initDevice (sf_adxrs290_ctrl_t * const p_ctrl)
{
ssp_err_t err;
#if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif /* if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE */
    
    /** Configure Device writing filter and power control registers */
    err = SF_ADXRS290_writeRegister (   p_ctrl,
                                        ADXRS290_REG_FILTER,
                                        ADXRS290_FILTER_SETTINGS );
    if (err == SSP_SUCCESS)
    {
        err = SF_ADXRS290_writeRegister (   p_ctrl,
                                            ADXRS290_REG_POWER_CTL,
                                            ADXRS290_POWER_SETTINGS );
    }
    return err;
}

/******************************************************************************************************************//**
 * @brief Implements static SF_ADXRS290_readRegister
 *
 * @retval SSP_SUCCESS          Read procedure successfully.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
  *********************************************************************************************************************/
static ssp_err_t SF_ADXRS290_readRegister (  sf_adxrs290_ctrl_t * const p_ctrl,
                                            const uint8_t reg_addr,
                                            uint8_t * reg_value )
{
ssp_err_t err;
uint8_t tmp_buf_src[TMP_READ_BUF_LEN];
uint8_t tmp_buf_dst[TMP_READ_BUF_LEN];
#if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != reg_value);
#endif /* if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE */
    
    tmp_buf_src[0] = (ADXRS290_READ_FLAG | reg_addr);
    tmp_buf_src[1] = 0;
    
    err = p_ctrl->device->p_api->writeRead (p_ctrl->device->p_ctrl,
                                            tmp_buf_src,
                                            tmp_buf_dst,
                                            TMP_READ_BUF_LEN,
                                            SPI_BIT_WIDTH_8_BITS,
                                            TX_WAIT_FOREVER);
    if (err == SSP_SUCCESS)
    {
        *reg_value = tmp_buf_dst[1];
    }
    
    return err;
}

/******************************************************************************************************************//**
 * @brief Implements static SF_ADXRS290_writeRegister
 *
 * @retval SSP_SUCCESS          Write procedure successfully.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
  *********************************************************************************************************************/
static ssp_err_t SF_ADXRS290_writeRegister ( sf_adxrs290_ctrl_t * const p_ctrl,
                                            const uint8_t reg_addr,
                                            uint8_t reg_value )
{
ssp_err_t err;
uint8_t tmp_buf[TMP_WRITE_BUF_LEN];
#if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif /* if SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE */
    
    tmp_buf[0] = (ADXRS290_WRITE_FLAG | reg_addr);
    tmp_buf[1] = reg_value;
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
