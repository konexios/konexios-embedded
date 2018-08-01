/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow/api/json/parse.h>
#include <arrow/utf8.h>

void who_when_init(who_when_t *ww) {
    memset(&ww->date, 0x0, sizeof(struct tm));
    property_init(&ww->by);
}

void who_when_free(who_when_t *ww) {
    property_free(&ww->by);
}

void who_when_move(who_when_t *dst, who_when_t *src) {
    property_move(&dst->by, &src->by);
    memcpy(&dst->date, &src->date, sizeof(struct tm));
}

JsonNode *parse_size_data(JsonNode *_main, page_size_t *ps) {
    if ( ps ) memset(ps, 0x0, sizeof(page_size_t));
    JsonNode *_size = json_find_member(_main, p_const("size"));
    if ( !_size && _size->tag != JSON_NUMBER ) return NULL;
    int __size = (int)_size->number_;
    if ( __size ) {
        if ( ps ) {
            ps->size = __size;
            JsonNode *_page = json_find_member(_main, p_const("page"));
            if ( !_page && _page->tag != JSON_NUMBER ) return NULL;
            ps->page = _page->number_;
            JsonNode *_totalSize = json_find_member(_main, p_const("totalSize"));
            if ( !_totalSize && _totalSize->tag != JSON_NUMBER ) return NULL;
            ps->totalSize = _totalSize->number_;
            JsonNode *_totalPages = json_find_member(_main, p_const("totalPages"));
            if ( !_totalPages && _totalPages->tag != JSON_NUMBER ) return NULL;
            ps->totalPages = _totalPages->number_;
        }
        JsonNode *_data = json_find_member(_main, p_const("data"));
        if ( !_data ) return NULL;
        return _data;
    }
    return NULL;
}


int who_when_parse(JsonNode *tmp, who_when_t *ww, const char *date, const char *person) {
    // FIXME parse timestamp
    // FIXME property
    JsonNode *t = json_find_member(tmp, p_stack(date));
    if ( t && t->tag == JSON_STRING )
        strptime(t->string_, "%Y-%m-%dT%H:%M:%S", &ww->date);
    t = json_find_member(tmp, p_stack(person));
    if ( t && t->tag == JSON_STRING )
        property_copy( &ww->by, p_stack(t->string_));
    return 0;
}

// pasre this for ex "2018-05-21T13:40:32.173Z"
int timestamp_parse(timestamp_t *t, const char *s) {
    // parse year
    int tmp;
    char *p = copy_till_to_int(s, "-", &tmp);
    if ( !p ) return -1;
    t->year = tmp;
    // mon
    p = copy_till_to_int(p, "-", &tmp);
    if ( !p ) return -1;
    t->mon = tmp;
    // day
    p = copy_till_to_int(p, "T", &tmp);
    if ( !p ) return -1;
    t->day = tmp;
    // hour
    p = copy_till_to_int(p, ":", &tmp);
    if ( !p ) return -1;
    t->hour = tmp;
    // min
    p = copy_till_to_int(p, ":", &tmp);
    if ( !p ) return -1;
    t->min = tmp;
    // sec
    p = copy_till_to_int(p, ".", &tmp);
    if ( !p ) return -1;
    t->sec = tmp;
    // msec
    p = copy_till_to_int(p, "Z", &tmp);
    if ( !p ) return -1;
    t->msec = tmp;
    return 0;
}
