/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_MISC_H_
#define _SWAT_WMICONFIG_MISC_H_

void swat_wmiconfig_help();

void swat_wmiconfig_version(void);

void swat_wmiconfig_reset();

void swat_wmiconfig_prom_start(A_UINT32 seconds);

void swat_wmiconfig_prom_stop();

void swat_wmiconfig_promiscuous_test(A_BOOL promiscuous);

void swat_wmiconfig_set_current_devid(A_UINT32 devid);

void swat_wmiconfig_maint_timeout_set(A_UINT32 *timeout,A_UINT32 param_size);

#endif
