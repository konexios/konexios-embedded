/*
 * DLBSmotor.cpp
 *
 *  Created on: 5 мая 2017 г.
 *      Author: ddemidov
 */

#include "DLBSmotor.h"

LV8907_BLDC DLBSmotor::bldc;

//Interrupt handler for BLDC
void DLBSmotor::bldcHndlr() {
	uint8_t regVal = 0x00;
	DBG("BLDC Interrupt generated");
	bldc.readDiag0(&regVal);
	DBG("MRDIAG0 = %x", regVal);
	regVal = 0x00;
	bldc.readDiag1(&regVal);
	DBG("MRDIAG1 = %x", regVal);
	regVal = 0x00;
	//Clear the latchable interrupts
	bldc.motorOff();
	//Wait so that the toggling is detected
	wait_ms(20);
	//Turn ON the motor again
	bldc.motorOn();
}

DLBSmotor::DLBSmotor() {
	// TODO Auto-generated constructor stub

}

DLBSmotor::~DLBSmotor() {
	// TODO Auto-generated destructor stub
}

int DLBSmotor::init() {
	if(bldc.init(&DLBSmotor::bldcHndlr) != BLDC_SUCCESS) {
		DBG("BLDC Shield init failed");
		return -1;
	}
	//Enable reading of motor ERPM
	bldc.enableRpmRead();
	return 0;
}

int DLBSmotor::start(int RPM) {
	//Demo the Counter clockwise rotation of BLDC
	bldc.setRotateDir(COUNTER_CLOCKWISE);
	bldc.setRPM(RPM);
	bldc.motorOn();
	int motorRpm = bldc.readRPM();
	DBG("ERPM = %d", motorRpm);
	return 0;
}

int DLBSmotor::stop() {
	bldc.motorOff();
	return 0;
}
