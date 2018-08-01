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
#ifndef __TERM_H__
#define __TERM_H__

void term_init();

int term_readline(char * line, int max);

int term_promptline(char * msg, char * line, int max);

void term_rx(uint8_t * buf, int size);


#endif

// EOF
