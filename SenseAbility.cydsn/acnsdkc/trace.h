/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef PLATFORMS_SENSEABILITY_TRACE_H_
#define PLATFORMS_SENSEABILITY_TRACE_H_

    
#define DBG_LINE_SIZE 150
    
void dbg_line(const char *fmt, ...);

#define DBG(...) dbg_line(__VA_ARGS__)


#endif