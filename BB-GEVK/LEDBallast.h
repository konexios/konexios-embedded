/*
 * LEDBallast.h
 *
 *  Created on: 3 мая 2017 г.
 *      Author: ddemidov
 */

#include "mbed.h"
#include "NCV78763_LED.h"

#include <debug.h>

#ifndef LEDBALLAST_H_
#define LEDBALLAST_H_

#define CHANNEL_ONE_EN 0x01
#define CHANNEL_TWO_EN 0x02

class LEDBallast {
	NCV78763 led_ballast;
	uint8_t _channels_en;
public:
	LEDBallast();
	virtual ~LEDBallast();
	int init();
	int setup(LEDCHANNEL_T channel);
	int enable(LEDCHANNEL_T channel);
	int disable(LEDCHANNEL_T channel);
	int setDC(LEDCHANNEL_T channel, int dc);

};

#endif /* LEDBALLAST_H_ */
