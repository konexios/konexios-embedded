/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <qcom_common.h>
#include <qcom_system.h>
#include <qcom_uart.h>
#include "select_api.h"


int uart2_init();
char get_char(int *status, int timeout_s);
int uart2_write(const char *msg, int size);
