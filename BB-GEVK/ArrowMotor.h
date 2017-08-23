/*
 * ArrowMotor.h
 *
 *  Created on: 3 мая 2017 г.
 *      Author: ddemidov
 */

#ifndef ARROWMOTOR_H_
#define ARROWMOTOR_H_

#if !defined(USE_STEP_MOTOR)
typedef int __dummy_type;
#else

#include "mbed.h"
#include "AMIS30543D_Stpr.h"

class Arrow_Motor {
	AMIS30543D stepper[2];
public:
	Arrow_Motor();
	virtual ~Arrow_Motor();
	int enable(MOTOR_T motor);
	int setMode(MOTOR_T motor, STEPPER_MODE_T mode);
	int rotate(MOTOR_T motor, int angle);
	int direction(MOTOR_T motor, ROTATION_T rot);
	int steps(MOTOR_T motor, int step);
	void disable(MOTOR_T motor);
};

#endif

#endif /* ARROWMOTOR_H_ */
