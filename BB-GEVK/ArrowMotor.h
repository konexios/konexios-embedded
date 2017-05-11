/*
 * ArrowMotor.h
 *
 *  Created on: 3 мая 2017 г.
 *      Author: ddemidov
 */

#ifndef ARROWMOTOR_H_
#define ARROWMOTOR_H_

#include "mbed.h"
#include "Shields.h"

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

#endif /* ARROWMOTOR_H_ */
