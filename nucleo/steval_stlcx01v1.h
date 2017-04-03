#ifndef __STEVAL_STLCX01V1_H
#define __STEVAL_STLCX01V1_H

#include "mbed.h"

typedef struct {
  int32_t AXIS_X;
  int32_t AXIS_Y;
  int32_t AXIS_Z;
} SensorAxes_t;

class SensorTile {
  SPI spi_master;
  DigitalOut _reset;
  int sync_times;
  void spi_write_burst(uint8_t addr, uint8_t *ptr, uint8_t len);
  void spi_read_burst(uint8_t addr, uint8_t *ptr, uint8_t len);

public:
  SensorTile();
  void test();
  bool sync();
  SensorAxes_t getAcceleration();
  SensorAxes_t getAngularVelocity();
  SensorAxes_t getMagneticField();
  float getPressure();
  float getTemperature();
  float getHumidity();
  bool hasTemperature();
  bool hasHumidity();
  void reset();
};


#endif /* __STEVAL_STLCX01V1_H */
