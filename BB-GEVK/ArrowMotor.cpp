/*
 * ArrowMotor.cpp
 *
 *  Created on: 3 мая 2017 г.
 *      Author: ddemidov
 */

#include "ArrowMotor.h"
#include <debug.h>

#if !defined(USE_STEP_MOTOR)
typedef int __dummy_type;
#else

Arrow_Motor::Arrow_Motor() : stepper{MOTOR1, MOTOR2} { }

int Arrow_Motor::enable(MOTOR_T motor) {
	int ret = stepper[motor].enable();
	if(ret != STPR_RET_SUCCESS){
		DBG("motor driver 2 : enabling the stepper motor 2 failed, error : %d !!!", ret);
		return -1;
	} else {
		DBG("motor driver 2 : enabling the stepper motor 2 success ...");
	}
	ret = stepper[motor].checkStprErrorOut();
	if(ret == true){
		DBG("ERR OUT set low, check status registers for exact reason of error !!!");
	} else {
		DBG("ERR OUT set HIGH, No errors ...");
	}
	//enable motor2 peak coil current as 715mA.
	ret = stepper[motor].setMcPeakCurr(9);
	if(ret != STPR_RET_SUCCESS){
		DBG("motor driver 2 : setting the peak current of motor 2 coil to 715mA failed, error : %d !!!", ret);
		return -1;
	} else {
		DBG("motor driver 2 : setting the peak current of motor 2 coil to 715mA success ...");
	}
	return setMode(motor, MICRO_1_4);
}

Arrow_Motor::~Arrow_Motor() {
	this->disable(MOTOR1);
	this->disable(MOTOR2);
}

void Arrow_Motor::disable(MOTOR_T motor) {
	stepper[motor].disable();
}

int Arrow_Motor::setMode(MOTOR_T motor, STEPPER_MODE_T mode) {
	int ret = stepper[motor].setStepperMode(mode);
	if(ret != STPR_RET_SUCCESS){
		DBG("motor driver 2 : setting stepper mode to micro step 1/4 failed, error : %d !!!", ret);
		return (-1);
	} else {
		DBG("motor driver 2 : setting stepper mode to micro step 1/4 success ...");
	}
	return 0;
}
int Arrow_Motor::direction(MOTOR_T motor, ROTATION_T rot) {
	int ret = stepper[motor].setRotationDirection(rot);
	if(ret != STPR_RET_SUCCESS) {
		DBG("motor driver 2 : setting motor direction to %d failed, error : %d !!!", rot, ret);
		return (-1);
	} else {
		DBG("motor driver 2 : setting motor direction to %d success ...", rot);
	}
	return 0;
}

int Arrow_Motor::rotate(MOTOR_T motor, int angle) {
	int ret = stepper[motor].rotateAngle(angle);
	if(ret != STPR_RET_SUCCESS){
		DBG("motor driver 2 : rotating motor %d degrees failed, error : %d !!!", angle, ret);
		return (-1);
	} else {
		DBG("motor driver 2 : rotating motor %d degrees success ...", angle);
	}
	return 0;
}

int Arrow_Motor::steps(MOTOR_T motor, int step) {
	stepper[motor].rotateSteps(step);
	DBG("motor driver 2 : motor rotated %d steps ...", step);
	return 0;
}

#endif
