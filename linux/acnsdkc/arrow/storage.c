/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/storage.h"
#include <stdio.h>

int restore_gateway_info(arrow_gateway_t *gateway) {
  static char hid[100];
  FILE *fp;
  fp = fopen("gateway.cfg", "r");
  if ( fp ) {
    printf("already registred\r\n");
    int ret = fscanf(fp, "%s", hid);
    if ( ret == 1 ) {
      arrow_gateway_add_hid(gateway, hid);
      fclose(fp);
      return 0;
    }
  }
  return -1;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
  printf("new registration\r\n");
  FILE *fp;
  fp = fopen("gateway.cfg", "w");
  fprintf(fp, "%s\n", gateway->hid);
  fclose(fp);
}

int restore_device_info(arrow_device_t *device) {
  static char dev_hid[100];
#if defined(__IBM__)
  static char dev_eid[100];
#endif
  FILE *fp;
  fp = fopen("device.cfg", "r");
  if ( fp ) {
    printf("already dev registred\r\n");
    int ret = fscanf(fp, "%s\n", dev_hid);
    if ( ret != 1 ) {
      fclose(fp);
      return -1;
    }
    arrow_device_set_hid(device, dev_hid);
#if defined(__IBM__)
    ret = fscanf(fp, "%s", dev_eid);
    if ( ret != 1 ) {
      fclose(fp);
      return 0;
    }
    arrow_device_set_eid(device, dev_eid);
#endif
    fclose(fp);
    return 0;
  }
  return -1;
}

void save_device_info(arrow_device_t *device) {
  FILE *fp;
  fp = fopen("device.cfg", "w");
  fprintf(fp, "%s\n", device->hid);
#if defined(__IBM__)
  fprintf(fp, "%s\n", device->eid);
#endif
  fclose(fp);
}
