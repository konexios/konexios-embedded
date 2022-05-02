#include "konexios/storage.h"
#include <stdio.h>

int restore_gateway_info(konexios_gateway_t *gateway) {
  static char hid[100];
  FILE *fp;
  fp = fopen("gateway.cfg", "r");
  if ( fp ) {
    printf("already registred\r\n");
    int ret = fscanf(fp, "%s", hid);
    if ( ret == 1 ) {
        property_t t = p_stack(hid);
      property_copy(&gateway->hid, t);
      fclose(fp);
      return 0;
    }
  }
  return -1;
}

void save_gateway_info(const konexios_gateway_t *gateway) {
  printf("new registration\r\n");
  FILE *fp;
  fp = fopen("gateway.cfg", "w");
  fprintf(fp, "%s\n", P_VALUE(gateway->hid) );
  fclose(fp);
}

int restore_device_info(konexios_device_t *device) {
  static char dev_hid[100];
  FILE *fp;
  fp = fopen("device.cfg", "r");
  if ( fp ) {
    printf("already dev registred\r\n");
    int ret = fscanf(fp, "%s\n", dev_hid);
    if ( ret != 1 ) {
      fclose(fp);
      return -1;
    }
    property_copy(&device->hid, p_stack(dev_hid));
    fclose(fp);
    return 0;
  }
  return -1;
}

void save_device_info(konexios_device_t *device) {
  FILE *fp;
  fp = fopen("device.cfg", "w");
  fprintf(fp, "%s\n", P_VALUE(device->hid));
  fclose(fp);
}
