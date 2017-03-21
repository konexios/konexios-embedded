/**
 ******************************************************************************
 * @file    x_nucleo_iks01a1.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    08-October-2014
 * @brief   Implementation file for the X_NUCLEO_IKS01A1 singleton class
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#include "mbed.h"
#include "x_nucleo_iks01a1.h"

/* Static variables ----------------------------------------------------------*/
X_NUCLEO_IKS01A1* X_NUCLEO_IKS01A1::_instance = NULL;


/* Methods -------------------------------------------------------------------*/
/**
 * @brief  Constructor
 */
X_NUCLEO_IKS01A1::X_NUCLEO_IKS01A1(DevI2C *ext_i2c, PinName ff_irq_pin) : dev_i2c(ext_i2c),
	ht_sensor(new HTS221(*dev_i2c)),
	magnetometer(new LIS3MDL(*dev_i2c)),
	pt_sensor(new LPS25H(*dev_i2c)),
	gyro_lsm6ds0(new LSM6DS0(*dev_i2c))
{ 
	if(ff_irq_pin == NC) {
		gyro_lsm6ds3 = NULL;
	} else {
		gyro_lsm6ds3 = new LSM6DS3(*dev_i2c, ff_irq_pin);
	}
}

/**
 * @brief     Get singleton instance
 * @return    a pointer to the initialized singleton instance of class X_NUCLEO_IKS01A1.
 *            A return value of NULL indicates an out of memory situation.
 * @param[in] ext_i2c (optional) pointer to an instance of DevI2C to be used
 *            for communication on the expansion board. 
 *            Defaults to NULL.
 *            Taken into account only on the very first call of one of the 'Instance' functions.
 *            If not provided a new DevI2C will be created with standard
 *            configuration parameters.
 *            The used DevI2C object gets saved in instance variable dev_i2c.
 * @param[in] ff_irq_pin (optional) PinName of the pin associated to asynchronous 
 *            (i.e. interrupt based) free fall detection in case a LSM6DS3 3D 
 *            Acceleromenter and 3D Gyroscope is mounted on top of the DIL 24-pin socket.
 *            Defaults to IKS01A1_PIN_FF.
 *            Taken into account only on the very first call of one of the 'Instance' functions.
 *            A value of 'NC' will avoid instantiation of the LSM6DS3 even if present.
 */
X_NUCLEO_IKS01A1* X_NUCLEO_IKS01A1::Instance(DevI2C *ext_i2c, PinName ff_irq_pin) {
	if(_instance == NULL) {
		if(ext_i2c == NULL)
			ext_i2c = new DevI2C(IKS01A1_PIN_I2C_SDA, IKS01A1_PIN_I2C_SCL);

		if(ext_i2c != NULL)
			_instance = new X_NUCLEO_IKS01A1(ext_i2c, ff_irq_pin);
	
		if(_instance != NULL) {
			bool ret = _instance->Init();
			if(!ret) {
				error("Failed to init X_NUCLEO_IKS01A1 expansion board!\n");
			}
		}
	}

	return _instance;
}

/**
 * @brief     Get singleton instance
 * @return    a pointer to the initialized singleton instance of class X_NUCLEO_IKS01A1.
 *            A return value of NULL indicates an out of memory situation.
 * @param[in] sda I2C data line pin.
 *            Taken into account only on the very first call of one of the 'Instance' functions.
 *            A new DevI2C will be created based on parameters 'sda' and 'scl'.
 *            The used DevI2C object gets saved in instance variable dev_i2c.
 * @param[in] scl I2C clock line pin.
 *            Taken into account only on the very first call of one of the 'Instance' functions.
 *            A new DevI2C will be created based on parameters 'sda' and 'scl'.
 *            The used DevI2C object gets saved in instance variable dev_i2c.
 * @param[in] ff_irq_pin (optional) PinName of the pin associated to asynchronous 
 *            (i.e. interrupt based) free fall detection in case a LSM6DS3 3D 
 *            Acceleromenter and 3D Gyroscope is mounted on top of the DIL 24-pin socket.
 *            Defaults to NC.
 *            Taken into account only on the very first call of one of the 'Instance' functions.
 *            A value of 'NC' will avoid instantiation of the LSM6DS3 even if present.
 */
X_NUCLEO_IKS01A1* X_NUCLEO_IKS01A1::Instance(PinName sda, PinName scl, PinName ff_irq_pin) {
	if(_instance == NULL) {
		DevI2C *ext_i2c = new DevI2C(sda, scl);

		if(ext_i2c != NULL)
			_instance = new X_NUCLEO_IKS01A1(ext_i2c, ff_irq_pin);
	
		if(_instance != NULL) {
			bool ret = _instance->Init();
			if(!ret) {
				error("Failed to init X_NUCLEO_IKS01A1 expansion board!\n");
			}
		}
	}

	return _instance;
}

/**
 * @brief  Initialize the singleton's HT sensor
 * @retval true if initialization successful, 
 * @retval false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_HTS221(void) {
	uint8_t ht_id = 0;
	HUM_TEMP_InitTypeDef InitStructure;

	/* Check presence */
	if((ht_sensor->ReadID(&ht_id) != HUM_TEMP_OK) ||
	   (ht_id != I_AM_HTS221))
		{
			delete ht_sensor;
			ht_sensor = NULL;
			return true;
		}
	
	/* Configure sensor */
	InitStructure.OutputDataRate = HTS221_ODR_12_5Hz;

	if(ht_sensor->Init(&InitStructure) != HUM_TEMP_OK)
		{
			return false;
		}
      
	return true;
}

/**
 * @brief  Initialize the singleton's magnetometer
 * @retval true if initialization successful, 
 * @retval false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_LIS3MDL(void) {
	uint8_t m_id = 0;
	MAGNETO_InitTypeDef InitStructure;

	/* Check presence */
	if((magnetometer->ReadID(&m_id) != MAGNETO_OK) ||
	   (m_id != I_AM_LIS3MDL_M))
		{
			delete magnetometer;
			magnetometer = NULL;
			return true;
		}
      
	/* Configure sensor */
	InitStructure.M_FullScale = LIS3MDL_M_FS_4;
	InitStructure.M_OperatingMode = LIS3MDL_M_MD_CONTINUOUS;
	InitStructure.M_XYOperativeMode = LIS3MDL_M_OM_HP;
	InitStructure.M_OutputDataRate = LIS3MDL_M_DO_80;

	if(magnetometer->Init(&InitStructure) != MAGNETO_OK)
		{
			return false;
		}
      
	return true;
}

/**
 * @brief  Initialize the singleton's pressure sensor
 * @retval true if initialization successful, 
 * @retval false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_LPS25H(void) {
	uint8_t p_id = 0;
	PRESSURE_InitTypeDef InitStructure;
	
	/* Check presence */
	if((pt_sensor->ReadID(&p_id) != PRESSURE_OK) ||
	   (p_id != I_AM_LPS25H))
		{
			delete pt_sensor;
			pt_sensor = NULL;
			return true;
		}
            
	/* Configure sensor */
	InitStructure.OutputDataRate = LPS25H_ODR_1Hz;
	InitStructure.BlockDataUpdate = LPS25H_BDU_CONT;
	InitStructure.DiffEnable = LPS25H_DIFF_ENABLE;
	InitStructure.SPIMode = LPS25H_SPI_SIM_3W;
	InitStructure.PressureResolution = LPS25H_P_RES_AVG_32;
	InitStructure.TemperatureResolution = LPS25H_T_RES_AVG_16;
        
	if(pt_sensor->Init(&InitStructure) != PRESSURE_OK)
		{
			return false;
		}
            
	return true;
}

/**
 * @brief  Initialize the singleton's LSM6DS0 gyroscope
 * @retval true if initialization successful, 
 * @retval false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_LSM6DS0(void) {
	IMU_6AXES_InitTypeDef InitStructure;
	uint8_t xg_id = 0;

	/* Check presence */
	if((gyro_lsm6ds3 != NULL) || // by default do not instantiate two gyroscopes
	   (gyro_lsm6ds0->ReadID(&xg_id) != IMU_6AXES_OK) ||
	   (xg_id != I_AM_LSM6DS0_XG))
		{
			delete gyro_lsm6ds0;
			gyro_lsm6ds0 = NULL;
			return true;
		}
            
	/* Configure sensor */
	InitStructure.G_FullScale       = 2000.0f; /* 2000DPS */
	InitStructure.G_OutputDataRate  = 119.0f;  /* 119HZ */
	InitStructure.G_X_Axis          = 1;       /* Enable */
	InitStructure.G_Y_Axis          = 1;       /* Enable */
	InitStructure.G_Z_Axis          = 1;       /* Enable */

	InitStructure.X_FullScale       = 2.0f;    /* 2G */
	InitStructure.X_OutputDataRate  = 119.0f;  /* 119HZ */
	InitStructure.X_X_Axis          = 1;       /* Enable */
	InitStructure.X_Y_Axis          = 1;       /* Enable */
	InitStructure.X_Z_Axis          = 1;       /* Enable */
              
	if(gyro_lsm6ds0->Init(&InitStructure) != IMU_6AXES_OK)
		{
			return false; 
		}
            
	return true;
}

/**
 * @brief  Initialize the singleton's LSMDS3 gyroscope
 * @retval true if initialization successful, 
 * @retval false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_LSM6DS3(void) {
	IMU_6AXES_InitTypeDef InitStructure;
	uint8_t xg_id = 0;

	/* Check presence */
	if(gyro_lsm6ds3 == NULL) return true;

	if((gyro_lsm6ds3->ReadID(&xg_id) != IMU_6AXES_OK) ||
	   (xg_id != I_AM_LSM6DS3_XG))
		{
			delete gyro_lsm6ds3;
			gyro_lsm6ds3 = NULL;
			return true;
		}
            
	/* Configure sensor */
	InitStructure.G_FullScale      = 2000.0f; /* 2000DPS */
	InitStructure.G_OutputDataRate = 104.0f;  /* 104HZ */
	InitStructure.G_X_Axis         = 1;       /* Enable */
	InitStructure.G_Y_Axis         = 1;       /* Enable */
	InitStructure.G_Z_Axis         = 1;       /* Enable */
        
	InitStructure.X_FullScale      = 2.0f;    /* 2G */
	InitStructure.X_OutputDataRate = 104.0f;  /* 104HZ */
	InitStructure.X_X_Axis         = 1;       /* Enable */
	InitStructure.X_Y_Axis         = 1;       /* Enable */
	InitStructure.X_Z_Axis         = 1;       /* Enable */
        
	if(gyro_lsm6ds3->Init(&InitStructure) != IMU_6AXES_OK)
		{
			return false; 
		}
            
	return true;
}
