/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#ifndef __TEST_H__
#define __TEST_H__

void test_i2cbus();

void test_info();

void test_set_host(char * newhost);

void test_set_port(uint16_t port);

void test_toggle_print();

int test_network();

void test_closenetwork();

void test_signature();

#endif

// EOF
