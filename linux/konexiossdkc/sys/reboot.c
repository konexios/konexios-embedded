/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "sys/reboot.h"
#include <stdio.h>
#include <unistd.h>

/*
void reboot(void) {
    char *argv[64] = { "./updater", "update.file" };
    execvp(*argv, argv);
    while(1)
        ;
} */
