/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ARROW_DEVICE_TYPE_H_)
#define ARROW_DEVICE_TYPE_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <arrow/device.h>
#include <data/propmap.h>

typedef struct _device_type_telemetry {
  property_map_t *variables;
  char *description;
  char *name;
  char *type;
  arrow_linked_list_head_node;
} device_type_telemetry_t;

typedef struct _device_type {
  char *description;
  int enabled;
  char *name;
  device_type_telemetry_t *telemetries;
} device_type_t;

// initialize the type object
void device_type_init(device_type_t *dev, int enable, const char *name, const char *dec);
// add a telemetry type
device_type_telemetry_t *device_type_add_telemetry(device_type_t *dev, const char *name, const char *type, const char *desc);
// terminate the type object
void device_type_free(device_type_t *dev);

int device_type_add_telemetry_variables(device_type_telemetry_t *tel, const char *key, const char *value);

// list existing device types
int arrow_device_type_list(void);
// create new device type
int arrow_device_type_create(device_type_t *dev_type);
// update existing device type
int arrow_device_type_update(arrow_device_t *dev, device_type_t *dev_type);

#if defined(__cplusplus)
}
#endif

#endif  // ARROW_DEVICE_TYPE_H_
