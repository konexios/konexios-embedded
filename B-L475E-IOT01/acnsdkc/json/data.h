#if !defined(_IOT_DATA_H_)
#define _IOT_DATA_H_

typedef struct {
  float    temperature;
  float    humidity;
  float    pressure;
  int16_t  acc[3];
  float    gyr[3];
  int16_t  mag[3];
  uint16_t proximity;
} sensors_data_t;

#endif
