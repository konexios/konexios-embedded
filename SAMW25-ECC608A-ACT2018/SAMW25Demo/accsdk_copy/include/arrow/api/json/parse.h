/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_API_JSON_PARSE_H_)
#define ACN_SDK_C_API_JSON_PARSE_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <data/property.h>
#include <json/json.h>
#include <time/time.h>

// FIXME move to file
typedef struct _page_size_ {
    int size;
    int page;
    int totalSize;
    int totalPages;
} page_size_t;
JsonNode *parse_size_data(JsonNode *_main, page_size_t *ps);

typedef struct _who_when_ {
    struct tm date;
    property_t by;
} who_when_t;
void who_when_init(who_when_t *ww);
void who_when_free(who_when_t *ww);
void who_when_move(who_when_t *dst, who_when_t *src);
int who_when_parse(JsonNode *tmp, who_when_t *ww, const char *date, const char *person);

int timestamp_parse(timestamp_t *t, const char *s);

// FIXME useless?
#define json_fill_property(tmp, gx, x) do { \
    JsonNode *t = json_find_member(tmp, p_const(xstr(x))); \
    if ( t && t->tag == JSON_STRING ) \
        property_copy( &(gx)->x, p_stack(t->string_)); \
  } while(0)

#if defined(__cplusplus)
}
#endif

#endif // ACN_SDK_C_API_JSON_PARSE_H_
