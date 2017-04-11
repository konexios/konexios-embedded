/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "steval_stlcx01v1.h"
//#define DEBUG
#include "debug.h"

#define MAX_TIMES 1000

void SensorTile::spi_write_burst(uint8_t addr, uint8_t *ptr, uint8_t len) {
  uint8_t i;
  spi_master.lock();
#ifdef DEBUG
  int value =
#endif
  spi_master.write(addr|0x80);
  DBG("write register = 0x%X\r\n", value);
  for (i=0; i<len; i++){
    spi_master.write(ptr[i]);
  }
  spi_master.unlock();
}

void SensorTile::spi_read_burst(uint8_t addr, uint8_t *ptr, uint8_t len) {
  uint8_t i;
  spi_master.lock();
#ifdef DEBUG
  int value =
#endif
  spi_master.write(addr&0x7f);
  DBG("read register = 0x%X\r\n", value);
  wait_us(100);
  for (i=0; i<len; i++){
    ptr[i] = spi_master.write(0x00);
    //      printf("ptr register = 0x%X\r\n", ptr[i]);
  }
  spi_master.unlock();
}

SensorTile::SensorTile() :
  spi_master(PA_7, PA_6, PA_5, PB_5),
  _reset(PA_1),
  sync_times(MAX_TIMES) {
  spi_master.format(8, 0);
  spi_master.frequency();
  reset();
}

void SensorTile::test() {
  int value = spi_master.write(0x6F);
  printf("start register = 0x%X\r\n", value);
  int whoami = spi_master.write(0x00);
  printf("WHOAMI register = 0x%X\r\n", whoami);
}

bool SensorTile::sync() {
  int whoami = 0, i = 0;
  while(whoami!=0x88) {
    spi_master.write(0x6c);
    wait_us(100);
    whoami = spi_master.write(0x00);
    if (whoami!=0x88) {
      if ( i++ > sync_times ) return false;
      wait_ms(10);
    }
  }
  for (i=0; i<10; i++) {
    whoami = spi_master.write(0x00);
  }
  return true;
}

SensorAxes_t SensorTile::getAcceleration() {
  SensorAxes_t acc;
  uint8_t *data  = reinterpret_cast<uint8_t*>(&acc);
  spi_read_burst(0x60, data, sizeof(acc));
  DBG("acc: {%d, %d, %d} \r\n", acc.AXIS_X, acc.AXIS_Y, acc.AXIS_Z);
//  for (int i=0; i<sizeof(acc); i++){
//    printf("acc register[%d] = 0x%X\r\n", i, data[i]);
//  }
  return acc;
}

SensorAxes_t SensorTile::getAngularVelocity() {
  SensorAxes_t av;
  uint8_t *data  = reinterpret_cast<uint8_t*>(&av);
  spi_read_burst(0x61, data, sizeof(av));
  DBG("av: {%d, %d, %d} \r\n", av.AXIS_X, av.AXIS_Y, av.AXIS_Z);
//  for (int i=0; i<sizeof(av); i++){
//    printf("av register[%d] = 0x%X\r\n", i, data[i]);
//  }
  return av;
}

SensorAxes_t SensorTile::getMagneticField() {
  SensorAxes_t mf;
  uint8_t *data  = reinterpret_cast<uint8_t*>(&mf);
  spi_read_burst(0x62, data, sizeof(mf));
  DBG("mf: {%d, %d, %d} \r\n", mf.AXIS_X, mf.AXIS_Y, mf.AXIS_Z);
  return mf;
}

float SensorTile::getPressure() {
  float pressure;
  uint8_t *data  = reinterpret_cast<uint8_t*>(&pressure);
  spi_read_burst(0x63, data, sizeof(pressure));
  DBG("press: {%f} \r\n", pressure);
  return pressure;
}

float SensorTile::getTemperature() {
  float temperature;
  uint8_t *data  = reinterpret_cast<uint8_t*>(&temperature);
  spi_read_burst(0x64, data, sizeof(temperature));
  DBG("temp: {%f} \r\n", temperature);
  return temperature;
}

float SensorTile::getHumidity() {
  float humidity;
  uint8_t *data  = reinterpret_cast<uint8_t*>(&humidity);
  spi_read_burst(0x65, data, sizeof(humidity));
  DBG("hum: {%f} \r\n", humidity);
  return humidity;
}

bool SensorTile::hasTemperature() {
  uint8_t noT;
  spi_read_burst(0x6a, &noT, 1);
  return noT ? false : true;
}

bool SensorTile::hasHumidity() {
  uint8_t noH;
  spi_read_burst(0x6b, &noH, 1);
  return noH ? false : true;
}

void SensorTile::reset() {
  _reset = 0;
  wait_us(200);
  _reset = 1;
}

#undef DEBUG
