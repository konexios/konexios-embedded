#ifndef _ARROW_TEMPERATURE_H_
#define _ARROW_TEMPERATURE_H_

#include <qcom_common.h>
#include <qcom_i2c_master.h>

int temperature_init();
int tmp106_reg_write(unsigned short val);
int tmp106_reg_read(unsigned short * val);

#endif





