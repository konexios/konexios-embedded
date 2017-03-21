/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow_temperature.h"

#define TMP106_OK       1
#define TMP106_ERR      0

int temperature_init() {
  int ret =qcom_i2cm_init(I2CM_PIN_PAIR_2,I2C_FREQ_200K,0);
  if(ret<0){
    A_PRINTF("I2C FAIL\r\n");
    return 0;
  }
  A_PRINTF("init temperature sensor\r\n");
  return 0;
}

int tmp106_reg_write(unsigned short val) {
  int ret;

  ret = qcom_i2cm_write(I2CM_PIN_PAIR_2, 0x48, 0,0, (UCHAR*)&val, 2);

  if (ret < 0)
  {
    QCOM_DEBUG_PRINTF("Call tmp106_reg_write failed!\n");
    return TMP106_ERR;
  }

  return TMP106_OK;
}

int tmp106_reg_read(unsigned short * val) {
  int ret;
  unsigned char data[2];

  data[0] = data[1] = 0;

  ret = qcom_i2cm_read(I2CM_PIN_PAIR_2, 0x48, 0, 0, data, 2);
  if (ret < 0)
  {
    QCOM_DEBUG_PRINTF("Call tmp106_reg_read failed!\n");
    return TMP106_ERR;
  }

  *(unsigned short * )val = (data[0]<< 8) | data[1];

  return TMP106_OK;
}
