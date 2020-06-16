#include <konexios_config.h>
#include <json/telemetry.h>
#include <iostream>
#if defined(__probook_4540s__)
#include <sensors/sensors.h>
#include <json/probook.h>
#else
#include <stdlib.h>
#include <json/pm.h>
#endif


int get_telemetry_data(void *d) {
#if defined(__probook_4540s__)
  probook_data_t *data = (probook_data_t *)d;
    // init the sensors (libsensors4-devel)
    int ret = sensors_init( NULL );
    if ( ret ) {
      std::cerr<<"sensors init failed"<<std::endl;
      return 1;
    }

    const sensors_chip_name *chip;
    const sensors_feature *feature;
    const sensors_subfeature *sub0;
    const sensors_subfeature *sub1;
    int chip_number = 2;

    
    // now find the temperature sensors on my laptop!
    chip = sensors_get_detected_chips(NULL, &chip_number);

    char chip_name[512];
    sensors_snprintf_chip_name(chip_name, 512, chip);
    std::cout<<"chip " << chip_name << " - " << chip_number << std::endl;

    int feature_number = 1;
    feature = sensors_get_features(chip, &feature_number);
    if ( feature->type != SENSORS_FEATURE_TEMP ) {
      std::cerr << "check SENSORS_FEATURE_TEMP fail" << std::endl;
      return -1;
    }

    sub0 = sensors_get_subfeature(chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
    if ( ! sub0->flags & SENSORS_MODE_R ) {
      std::cerr << "no R mode - core 0"<<std::endl;
      return -1;
    }

    feature_number = 2;
    feature = sensors_get_features(chip, &feature_number);
    if ( feature->type != SENSORS_FEATURE_TEMP ) return -1;
    sub1 = sensors_get_subfeature(chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
    if ( ! sub1->flags & SENSORS_MODE_R ) {
      std::cerr << "no R mode - core 1"<< std::endl;
      return -1;
    }
    
    // sub0 and sub1 - is the termerature sensors
    // we can get a temperature value by sensors_get_value()
    double tmp;
    sensors_get_value(chip, sub0->number, &tmp);
    data->temperature_core0 = tmp;
    sensors_get_value(chip, sub1->number, &tmp);
    data->temperature_core1 = tmp;
    std::cout<<"mqtt publish: T("<<data->temperature_core0
            <<", "<<data->temperature_core1<<")..."<<std::endl;
#else
  pm_data_t *data = (pm_data_t *)d;
  data->pm_2_5 = rand()%5 + 25;
  data->pm_10 = rand()%5 + 30;
  std::cout<<"mqtt publish: T("<<data->pm_2_5
          <<", "<<data->pm_10<<")..."<<std::endl;
#endif
  return 0;
}
