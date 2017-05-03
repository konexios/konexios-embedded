/*
 * LEDBallast.cpp
 *
 *  Created on: 3 мая 2017 г.
 *      Author: ddemidov
 */

#include "LEDBallast.h"

LEDBallast::LEDBallast() {}

LEDBallast::~LEDBallast() {
	this->disable(CHANNEL_ONE);
	this->disable(CHANNEL_TWO);
}

int LEDBallast::init() {
	_channels_en = 0;
    int ret = led_ballast.init();
    if(ret != LED_RET_SUCCESS){
        DBG("LED Driver : initialization failed, error = %d !!!", ret);
        return (-1);
    } else {
        DBG("LED Driver : initialization success ...");
    }
    //set led channels voltage parameters
    ret = led_ballast.setLEDBoosterVoltage (45, 6);
    if(ret != LED_RET_SUCCESS){
        DBG("LED Driver : booster voltage settings failed, error = %d !!!", ret);
        return (-1);
    } else {
        DBG("LED Driver : booster voltage settings success ...");
    }
    //set internal dimming produced by LED Driver to be 1000Hz
    ret = led_ballast.setInternalDimmingFreq(PWM_INT_1000);
    if(ret != LED_RET_SUCCESS){
        DBG("LED Driver : internal dimming frequency setting to 1000Hz failed, error = %d !!!", ret);
        return (-1);
    } else {
        DBG("LED Driver : internal dimming frequency setting to 1000Hz success ...");
    }
    return 0;
}
int LEDBallast::setup(LEDCHANNEL_T channel) {
    //set peak current in channel two as 252mA
    int ret = led_ballast.channelSetPeakCurrent(channel, 252);
    if(ret != LED_RET_SUCCESS){
        DBG("LED Driver : channel 2 peak current setting to 252mA failed, error = %d!!!", ret);
        return (-1);
    } else {
        DBG("LED Driver : channel 2 peak current setting to 252mA success ...");
    }
    //set average current in channel two as 140mA
    ret = led_ballast.channelSetAvgCurrent(channel, 140);
    if(ret != LED_RET_SUCCESS){
        DBG("LED Driver : channel 2 average current setting to 140mA failed, error = %d !!!", ret);
        return (-1);
    } else {
        DBG("LED Driver : channel 2 average current setting to 140mA success ...");
    }
    //enable internal dimming on channel two at internal pwm frequency of 500Hz
    ret = led_ballast.enableInternalDimming(channel);
    if(ret != LED_RET_SUCCESS){
        DBG("LED Driver : channel 2 enabling of internal dimming failed, error = %d !!!", ret);
        return (-1);
    } else {
        DBG("LED Driver : channel 2 enabling of internal dimming success ...");
    }
    return 0;
}

int LEDBallast::enable(LEDCHANNEL_T channel) {
	int ret = led_ballast.buckRegulatorEnable(channel);
	if(ret != LED_RET_SUCCESS){
		DBG("LED Driver : channel 2 buck regulator enabling failed, error = %d !!!", ret);
		return (-1);
	} else {
		DBG("LED Driver : channel 2 buck regulator enabling success ...");
	}
	//enable the booster to make the channels operate
	if ( !_channels_en ) {
		ret = led_ballast.boosterEnable();
		if(ret != LED_RET_SUCCESS){
			DBG("LED Driver : booster enabling failed, error = %d !!!", ret);
			return (-1);
		} else {
			DBG("LED Driver : booster enabling success ...");
		}
	}
	_channels_en |= (channel == CHANNEL_ONE?CHANNEL_ONE_EN:CHANNEL_TWO_EN);
    return 0;
}

int LEDBallast::disable(LEDCHANNEL_T channel) {
	int ret = led_ballast.buckRegulatorDisable(channel);
	if(ret != LED_RET_SUCCESS){
		DBG("LED Driver : channel 2 buck regulator disabling failed, error = %d !!!", ret);
		return (-1);
	} else {
		DBG("LED Driver : channel 2 buck regulator disabling success ...");
	}
	_channels_en &= ~(channel == CHANNEL_ONE?CHANNEL_ONE_EN:CHANNEL_TWO_EN);
	//disable the booster to make the channels operate
	if ( !_channels_en ) {
		ret = led_ballast.boosterDisable();
		if(ret != LED_RET_SUCCESS){
			DBG("LED Driver : booster disabling failed, error = %d !!!", ret);
			return (-1);
		} else {
			DBG("LED Driver : booster disabling success ...");
		}
	}
	return 0;
}

int LEDBallast::setDC(LEDCHANNEL_T channel, int dc) {
	int ret = led_ballast.setIPWMDutyCycle(channel, dc);
	if(ret != LED_RET_SUCCESS){
		DBG("LED DRIVER : CHANNEL 2, dimming setting to : %d, failed, error = %d!!!", dc, ret);
		return 0;
	}
	DBG("LED DRIVER : CHANNEL 2, dimming setting to : %d, success !!!", dc);
	return -1;
}
