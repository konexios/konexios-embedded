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
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

void console_init();

bool console_engineering_get();
void console_engineering_set(bool en);

void console_input(char * line, int len);

#endif

// EOF
