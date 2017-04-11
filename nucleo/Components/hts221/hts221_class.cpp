/**
 ******************************************************************************
 * @file    hts221_class.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-April-2015
 * @brief   Implementation file for the HTS221 driver class
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "hts221_class.h"
#include "hts221.h"

/* Methods -------------------------------------------------------------------*/
/* betzw - based on:
           X-CUBE-MEMS1/trunk/Drivers/BSP/Components/hts221/hts221.c: revision #410,
           X-CUBE-MEMS1/trunk: revision #416
*/

/**
 * @brief  HTS221 Calibration procedure
 * @retval HUM_TEMP_OK in case of success, an error code otherwise
 */
HUM_TEMP_StatusTypeDef HTS221::HTS221_Calibration(void)
{
  /* Temperature Calibration */
  /* Temperature in degree for calibration ( "/8" to obtain float) */
  uint16_t T0_degC_x8_L, T0_degC_x8_H, T1_degC_x8_L, T1_degC_x8_H;
  uint8_t H0_rh_x2, H1_rh_x2;
  uint8_t tempReg[2] = {0, 0};
  
  if(HTS221_IO_Read(tempReg, HTS221_T0_degC_X8_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  T0_degC_x8_L = (uint16_t)tempReg[0];
  
  if(HTS221_IO_Read(tempReg, HTS221_T1_T0_MSB_X8_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  T0_degC_x8_H = (uint16_t) (tempReg[0] & 0x03);
  T0_degC = ((float)((T0_degC_x8_H << 8) | (T0_degC_x8_L))) / 8;
  
  if(HTS221_IO_Read(tempReg, HTS221_T1_degC_X8_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  T1_degC_x8_L = (uint16_t)tempReg[0];
  
  if(HTS221_IO_Read(tempReg, HTS221_T1_T0_MSB_X8_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  T1_degC_x8_H = (uint16_t) (tempReg[0] & 0x0C);
  T1_degC_x8_H = T1_degC_x8_H >> 2;
  T1_degC = ((float)((T1_degC_x8_H << 8) | (T1_degC_x8_L))) / 8;
  
  if(HTS221_IO_Read(tempReg, (HTS221_T0_OUT_L_ADDR | HTS221_I2C_MULTIPLEBYTE_CMD), 2) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  T0_out = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);
  
  if(HTS221_IO_Read(tempReg, (HTS221_T1_OUT_L_ADDR | HTS221_I2C_MULTIPLEBYTE_CMD), 2) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  T1_out = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);
  
  /* Humidity Calibration */
  /* Humidity in degree for calibration ( "/2" to obtain float) */
  
  if(HTS221_IO_Read(&H0_rh_x2, HTS221_H0_RH_X2_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  if(HTS221_IO_Read(&H1_rh_x2, HTS221_H1_RH_X2_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  if(HTS221_IO_Read(&tempReg[0], (HTS221_H0_T0_OUT_L_ADDR | HTS221_I2C_MULTIPLEBYTE_CMD),
                    2) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  H0_T0_out = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);
  
  if(HTS221_IO_Read(&tempReg[0], (HTS221_H1_T0_OUT_L_ADDR  | HTS221_I2C_MULTIPLEBYTE_CMD),
                    2) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  H1_T0_out = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);
  
  H0_rh = ((float)H0_rh_x2) / 2;
  H1_rh = ((float)H1_rh_x2) / 2;
  
  return HUM_TEMP_OK;
}


/**
 * @brief  Set HTS221 Initialization
 * @param  HTS221_Init the configuration setting for the HTS221
 * @retval HUM_TEMP_OK in case of success, an error code otherwise
 */
HUM_TEMP_StatusTypeDef HTS221::HTS221_Init(HUM_TEMP_InitTypeDef *HTS221_Init)
{
  uint8_t tmp = 0x00;
  
  /* Configure the low level interface ---------------------------------------*/
  if(HTS221_IO_Init() != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  if(HTS221_Power_On() != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  if(HTS221_Calibration() != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  if(HTS221_IO_Read(&tmp, HTS221_CTRL_REG1_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  /* Output Data Rate selection */
  tmp &= ~(HTS221_ODR_MASK);
  tmp |= HTS221_Init->OutputDataRate;
  
  if(HTS221_IO_Write(&tmp, HTS221_CTRL_REG1_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  HTS221_IO_ITConfig();
  
  return HUM_TEMP_OK;
}

/**
 * @brief  Read ID address of HTS221
 * @param  ht_id the pointer where the ID of the device is stored
 * @retval HUM_TEMP_OK in case of success, an error code otherwise
 */
HUM_TEMP_StatusTypeDef HTS221::HTS221_ReadID(uint8_t *ht_id)
{
  if(!ht_id)
  {
    return HUM_TEMP_ERROR;
  }
  
  return HTS221_IO_Read(ht_id, HTS221_WHO_AM_I_ADDR, 1);
}

/**
 * @brief  Reboot memory content of HTS221
 * @retval HUM_TEMP_OK in case of success, an error code otherwise
 */
HUM_TEMP_StatusTypeDef HTS221::HTS221_RebootCmd(void)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG2 register */
  if(HTS221_IO_Read(&tmpreg, HTS221_CTRL_REG2_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  /* Enable or Disable the reboot memory */
  tmpreg |= HTS221_BOOT_REBOOTMEMORY;
  
  /* Write value to MEMS CTRL_REG2 regsister */
  if(HTS221_IO_Write(&tmpreg, HTS221_CTRL_REG2_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  return HUM_TEMP_OK;
}


/**
 * @brief  Read HTS221 output register, and calculate the humidity
 * @param  pfData the pointer to data output
 * @retval HUM_TEMP_OK in case of success, an error code otherwise
 */
HUM_TEMP_StatusTypeDef HTS221::HTS221_GetHumidity(float* pfData)
{
  int16_t H_T_out, humidity_t;
  uint8_t tempReg[2] = {0, 0};
  uint8_t tmp = 0x00;
  float H_rh;
  
  if(HTS221_IO_Read(&tmp, HTS221_CTRL_REG1_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  /* Output Data Rate selection */
  tmp &= (HTS221_ODR_MASK);
  
  if(tmp == 0x00)
  {
    if(HTS221_IO_Read(&tmp, HTS221_CTRL_REG2_ADDR, 1) != HUM_TEMP_OK)
    {
      return HUM_TEMP_ERROR;
    }
    
    /* Serial Interface Mode selection */
    tmp &= ~(HTS221_ONE_SHOT_MASK);
    tmp |= HTS221_ONE_SHOT_START;
    
    if(HTS221_IO_Write(&tmp, HTS221_CTRL_REG2_ADDR, 1) != HUM_TEMP_OK)
    {
      return HUM_TEMP_ERROR;
    }
    
    do
    {
    
      if(HTS221_IO_Read(&tmp, HTS221_STATUS_REG_ADDR, 1) != HUM_TEMP_OK)
      {
        return HUM_TEMP_ERROR;
      }
      
    }
    while(!(tmp & 0x02));
  }
  
  
  if(HTS221_IO_Read(&tempReg[0], (HTS221_HUMIDITY_OUT_L_ADDR | HTS221_I2C_MULTIPLEBYTE_CMD),
                    2) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  H_T_out = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);
  
  H_rh = ( float )(((( H_T_out - H0_T0_out ) * ( H1_rh - H0_rh )) / ( H1_T0_out - H0_T0_out )) + H0_rh );
  
  // Truncate to specific number of decimal digits
  humidity_t = (uint16_t)(H_rh * pow(10.0f, HUM_DECIMAL_DIGITS));
  *pfData = ((float)humidity_t) / pow(10.0f, HUM_DECIMAL_DIGITS);
  
  // Prevent data going below 0% and above 100% due to linear interpolation
  if ( *pfData <   0.0f ) *pfData =   0.0f;
  if ( *pfData > 100.0f ) *pfData = 100.0f;
  
  return HUM_TEMP_OK;
}

/**
 * @brief  Read HTS221 output register, and calculate the temperature
 * @param  pfData the pointer to data output
 * @retval HUM_TEMP_OK in case of success, an error code otherwise
 */
HUM_TEMP_StatusTypeDef HTS221::HTS221_GetTemperature(float* pfData)
{
  int16_t T_out, temperature_t;
  uint8_t tempReg[2] = {0, 0};
  uint8_t tmp = 0x00;
  float T_degC;
  
  if(HTS221_IO_Read(&tmp, HTS221_CTRL_REG1_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  /* Output Data Rate selection */
  tmp &= (HTS221_ODR_MASK);
  
  if(tmp == 0x00)
  {
    if(HTS221_IO_Read(&tmp, HTS221_CTRL_REG2_ADDR, 1) != HUM_TEMP_OK)
    {
      return HUM_TEMP_ERROR;
    }
    
    /* Serial Interface Mode selection */
    tmp &= ~(HTS221_ONE_SHOT_MASK);
    tmp |= HTS221_ONE_SHOT_START;
    
    if(HTS221_IO_Write(&tmp, HTS221_CTRL_REG2_ADDR, 1) != HUM_TEMP_OK)
    {
      return HUM_TEMP_ERROR;
    }
    
    do
    {
    
      if(HTS221_IO_Read(&tmp, HTS221_STATUS_REG_ADDR, 1) != HUM_TEMP_OK)
      {
        return HUM_TEMP_ERROR;
      }
      
    }
    while(!(tmp & 0x01));
  }
  
  if(HTS221_IO_Read(&tempReg[0], (HTS221_TEMP_OUT_L_ADDR | HTS221_I2C_MULTIPLEBYTE_CMD),
                    2) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  T_out = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);
  
  T_degC = ((float)(T_out - T0_out)) / (T1_out - T0_out) * (T1_degC - T0_degC) + T0_degC;
  
  temperature_t = (int16_t)(T_degC * pow(10.0f, TEMP_DECIMAL_DIGITS));
  
  *pfData = ((float)temperature_t) / pow(10.0f, TEMP_DECIMAL_DIGITS);
  
  return HUM_TEMP_OK;
}


/**
 * @brief  Exit the shutdown mode for HTS221
 * @retval HUM_TEMP_OK in case of success, an error code otherwise
 */
HUM_TEMP_StatusTypeDef HTS221::HTS221_Power_On(void)
{
  uint8_t tmpReg;
  
  /* Read the register content */
  if(HTS221_IO_Read(&tmpReg, HTS221_CTRL_REG1_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  /* Set the power down bit */
  tmpReg |= HTS221_MODE_ACTIVE;
  
  /* Write register */
  if(HTS221_IO_Write(&tmpReg, HTS221_CTRL_REG1_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  return HUM_TEMP_OK;
}

/**
 * @brief  Enter the shutdown mode for HTS221
 * @retval HUM_TEMP_OK in case of success, an error code otherwise
 */
HUM_TEMP_StatusTypeDef HTS221::HTS221_Power_OFF(void)
{
  uint8_t tmpReg;
  
  /* Read the register content */
  if(HTS221_IO_Read(&tmpReg, HTS221_CTRL_REG1_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  /* Reset the power down bit */
  tmpReg &= ~(HTS221_MODE_ACTIVE);
  
  /* Write register */
  if(HTS221_IO_Write(&tmpReg, HTS221_CTRL_REG1_ADDR, 1) != HUM_TEMP_OK)
  {
    return HUM_TEMP_ERROR;
  }
  
  return HUM_TEMP_OK;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
