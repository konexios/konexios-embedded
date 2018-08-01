/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ARROW_UTF8_H_
#define ARROW_UTF8_H_

char *copy_till(const char *src, const char *end_pattern, char *dst);
char *copy_till_to_int(const char *src, const char *end_pattern, int *i);
char *copy_till_hex_to_int(const char *src, const char *end_pattern, int *i);
int utf8check(const char *s);
void fix_urldecode(char *query);
void urlencode(char *dst, char *src, int len);
void hex_encode(char *dst, const char *src, int size);
void hex_decode(char *dst, const char *src, int size);

#endif
