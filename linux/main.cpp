extern "C" {
#include <config.h>
#include <arrow/device.h>
#include <arrow/connection.h>
#include <arrow/mqtt.h>
#include <json/telemetry.h>
#include <ntp/ntp.h>
#include <time/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sensors/sensors.h>
}

#include <iostream>

//extern "C" {
//void *__real_malloc (size_t size);

///* This function wraps the real malloc */
//void * __wrap_malloc (size_t size)
//{
//    void *lptr = __real_malloc(size);
////    printf("Malloc: %lu bytes @%p\n", size, lptr);
//    return lptr;
//}

//void __real_free(void *ptr);
//void __wrap_free(void *ptr) {
////  printf("Free: @%p\n", ptr);
//  __real_free(ptr);
//}

//void *__real_realloc(void *ptr, size_t newsize);
//void *__wrap_realloc(void *ptr, size_t newsize) {
////  printf("Realloc: newsize %d, @%p\n", newsize, ptr);
//  void *lptr = __real_realloc(ptr, newsize);
////  printf("Realloc: newptr, @%p\n", lptr);
//  return lptr;
//}
//}

#define HTTP_API

int main() {
    printf("\r\n--- Starting new run ---\r\n");

//    printf("Get UTC time...\r\n");

    // set time
//    ntp_set_time_cycle();

//    time_t ctTime = time(NULL);
//    printf("Time is set to (UTC): %s\r\n", ctime(&ctTime));

    // cloud
    arrow_gateway_t gateway;
    arrow_gateway_config_t gate_config;
    arrow_device_t device;

#if defined(HTTP_API)
    printf("register gateway via API\r\n");
    while ( arrow_connect_gateway(&gateway) < 0) {
      printf("arrow gateway connect fail...\r\n");
      sleep(5);
    }

//    arrow_prepare_gateway(&gateway);
//    arrow_gateway_add_hid(&gateway, "e8f4e6f957006d0e338ef0513336b75e943d4f5b");

//    arrow_gateway_config_init(&gate_config);
//    gate_config.type = IBM;

//    arrow_gateway_config_add_organizationId(&gate_config, "awxmvb");
//    arrow_gateway_config_add_gatewayType(&gate_config, "gateway-local");
//    arrow_gateway_config_add_gatewayId(&gate_config, "probook-20689dc1b689");
//    arrow_gateway_config_add_authToken(&gate_config, "token");
//    arrow_gateway_config_add_authMethod(&gate_config, "K5(gCm1RndOxinoObz");

    while ( arrow_config(&gateway, &gate_config) < 0 ) {
      printf("arrow gateway config fail...\r\n");
      sleep(5);
    }

    std::cout<<"config: "<<gate_config.type<<std::endl;

    // device registaration
    printf("register device via API\r\n");
//    arrow_prepare_device(&gateway, &device);
//    arrow_device_add_uid(&device, "probook-20689dc1b689-notebook");

    while (arrow_connect_device(&gateway, &device) < 0 ) {
      printf("device connect fail...\r\n");
      sleep(1);
    }

    printf("send telemetry via API\r\n");
#endif
    {
    FILE *fp = fopen("/etc/sensors3.conf", "r");
    int ret = sensors_init( NULL );
    printf("load sensors %d\r\n", ret);

//    sensors_cleanup();


    if (fp) fclose(fp);
    }

    const sensors_chip_name *chip;
    const sensors_feature *feature;
    const sensors_subfeature *sub0;
    const sensors_subfeature *sub1;
    int chip_number = 2;

    chip = sensors_get_detected_chips(NULL, &chip_number);

    char chip_name[512];
    sensors_snprintf_chip_name(chip_name, 512, chip);
    printf("chip %s - %d\r\n", chip_name, chip_number);

    int feature_number = 1;
    feature = sensors_get_features(chip, &feature_number);
    if ( feature->type != SENSORS_FEATURE_TEMP ) {
      printf("check SENSORS_FEATURE_TEMP fail\r\n");
      return -1;
    }

    sub0 = sensors_get_subfeature(chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
    if ( ! sub0->flags & SENSORS_MODE_R ) {
      printf("no R mode - core 0\r\n");
      return -1;
    }

    feature_number = 2;
    feature = sensors_get_features(chip, &feature_number);
    if ( feature->type != SENSORS_FEATURE_TEMP ) return -1;
    sub1 = sensors_get_subfeature(chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
    if ( ! sub1->flags & SENSORS_MODE_R ) {
      printf("no R mode - core 1\r\n");
      return -1;
    }

    probook_data_t data;
    double tmp;
    sensors_get_value(chip, sub0->number, &tmp);
    data.temperature_core0 = tmp;
    sensors_get_value(chip, sub1->number, &tmp);
    data.temperature_core1 = tmp;
//    while ( arrow_send_telemetry(&device, &data) < 0) {
//      printf("arrow: send telemetry fail\r\n");
//    }

    // MQTT
    printf("mqtt connect...\r\n");
    while ( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {
      printf("mqtt connect fail\r\n");
      sleep(1);
    } //every sec try to connect

    int i = 0;
    while (true) {
      sleep(5); // every 5 sec send data via mqtt
      sensors_get_value(chip, sub0->number, &tmp);
      data.temperature_core0 = tmp;
      sensors_get_value(chip, sub1->number, &tmp);
      data.temperature_core1 = tmp;
      printf("mqtt publish [%d]: T(%4.2f)...\r\n", i++, tmp);
      if ( mqtt_publish(&device, &data) < 0 ) {
        printf("mqtt publish failure...");
        mqtt_disconnect();
        while (mqtt_connect(&gateway, &device, &gate_config) < 0) { sleep(1);}
      }
    }

    arrow_device_free(&device);
    arrow_gateway_free(&gateway);


    printf("\r\ndisconnecting....\r\n");
    printf("\r\nTest complete.\r\n");
}
