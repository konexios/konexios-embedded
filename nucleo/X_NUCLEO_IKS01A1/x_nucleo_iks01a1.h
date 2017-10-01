/**
 ******************************************************************************
 * @file    x_nucleo_iks01a1.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    13-April-2015
 * @brief   Header file for class X_NUCLEO_IKS01A1 representing a X-NUCLEO-IKS01A1
 *          expansion board
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

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __X_NUCLEO_IKS01A1_H
#define __X_NUCLEO_IKS01A1_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_iks01a1_targets.h"
#include "hts221/hts221_class.h"
#include "lis3mdl/lis3mdl_class.h"
#include "lps25h/lps25h_class.h"
#include "lsm6ds0/lsm6ds0_class.h"
#include "lsm6ds3/lsm6ds3_class.h"
#include "DevI2C.h"

/* Macros -------------------------------------------------------------------*/
#define CALL_METH(obj, meth, param, ret) ((obj == NULL) ?		\
					  ((*(param) = (ret)), 0) :	\
					  ((obj)->meth(param))		\
					  )

/* Classes -------------------------------------------------------------------*/
/** Class X_NUCLEO_IKS01A1 is intended to represent the MEMS Inertial & Environmental 
 *  Nucleo Expansion Board with the same name.
 *
 *  The expansion board is featuring basically four IPs:\n
 *  -# a HTS221 Relative Humidity and Temperature Sensor\n
 *  -# a LIS3MDL 3-Axis Magnetometer\n
 *  -# a LPS25H MEMS Pressure Sensor (and Temperature Sensor)\n
 *  -# and a LSM6DS0 3D Acceleromenter and 3D Gyroscope\n
 *
 * The expansion board features also a DIL 24-pin socket which makes it possible
 * to add further MEMS adapters and other sensors (e.g. UV index). 
 *
 * It is intentionally implemented as a singleton because only one
 * X_NUCLEO_IKS01A1 at a time might be deployed in a HW component stack.\n
 * In order to get the singleton instance you have to call class method `Instance()`, 
 * e.g.:
 * @code
 * // Inertial & Environmental expansion board singleton instance
 * static X_NUCLEO_IKS01A1 *<TODO>_expansion_board = X_NUCLEO_IKS01A1::Instance();
 * @endcode
 */
class X_NUCLEO_IKS01A1
{
 protected:
	X_NUCLEO_IKS01A1(DevI2C *ext_i2c, PinName ff_irq_pin);

	~X_NUCLEO_IKS01A1(void) {
	    /* should never be called */
	    error("Trial to delete X_NUCLEO_IKS01A1 singleton!\n");
	}

	/**
	 * @brief  Initialize the singleton's sensors to default settings
	 * @retval true if initialization successful, 
	 * @retval false otherwise
	 */
	bool Init(void) {
		return (Init_HTS221() &&
			Init_LIS3MDL() &&
			Init_LPS25H() &&
			Init_Gyro());
	}
	
	/**
	 * @brief  Initialize the singleton's gyroscope
	 * @retval true if initialization successful, 
	 * @retval false otherwise
	 * @note   only one sensor among LSM6DS3 & LSM6DS0 will be instantiated
	 *         with a preference on LSM6DS3 when available
	 */
	bool Init_Gyro(void) {
		// Note: order is important!
		return (Init_LSM6DS3() &&
			Init_LSM6DS0());
	}

	bool Init_HTS221(void);
	bool Init_LIS3MDL(void);
	bool Init_LPS25H(void);
	bool Init_LSM6DS0(void);
	bool Init_LSM6DS3(void);

 public:
	static X_NUCLEO_IKS01A1* Instance(DevI2C *ext_i2c = NULL, 
					  PinName ff_irq_pin = IKS01A1_PIN_FF);
	static X_NUCLEO_IKS01A1* Instance(PinName sda, PinName scl, PinName ff_irq_pin = NC);

	DevI2C  *dev_i2c;

	HTS221  *ht_sensor;
	LIS3MDL *magnetometer;
	LPS25H  *pt_sensor;

	GyroSensor *GetGyroscope(void) {
		return ((gyro_lsm6ds3 == NULL) ? 
			(GyroSensor*)gyro_lsm6ds0 : (GyroSensor*)gyro_lsm6ds3);
	}
	MotionSensor *GetAccelerometer(void) {
		return ((gyro_lsm6ds3 == NULL) ? 
			(MotionSensor*)gyro_lsm6ds0 : (MotionSensor*)gyro_lsm6ds3);
	}
	LSM6DS0 *gyro_lsm6ds0;
	LSM6DS3 *gyro_lsm6ds3;

 private:
	static X_NUCLEO_IKS01A1 *_instance;
};

#endif /* __X_NUCLEO_IKS01A1_H */
