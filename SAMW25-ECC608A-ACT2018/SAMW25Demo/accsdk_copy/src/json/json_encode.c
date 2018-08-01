/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "json/json.h"

extern void emit_number(SB *out, double num);
int encoded_strlen(const char *str);

#define is_escape(c) ( (c) == '\"' )

size_t json_size(JsonNode *o) {
    JsonNode *tmp = o;
    size_t ret = 0;
    if ( !IS_EMPTY(o->key) ) {
        ret += property_size(&o->key) + 3;
    }
    switch(tmp->tag) {
    case JSON_NULL:
        // FIXME add handler
        break;
    case JSON_STRING:
        ret += encoded_strlen(tmp->string_);
        break;
    case JSON_NUMBER: {
        SB t;
        sb_init(&t);
        emit_number(&t, tmp->number_);
        ret += t.cur - t.start;
        sb_free(&t);
    } break;
    case JSON_BOOL:
        ret += tmp->bool_ ? 4 : 5;
        break;
    case JSON_ARRAY:
    case JSON_OBJECT:
        json_foreach(tmp, o) {
            ret += json_size(tmp);
            if ( json_next(tmp) ) ret++;
        }
        ret += 2;
        break;
    }
    return ret;
}

static int jem_encode_string(char start_,
                             char end_,
                             const char *s,
                             int offset,
                             char *buf,
                             int len);

static int jem_encode_obj(json_encode_machine_t *jem, char *s, int len);
static int jem_encode_number(json_encode_machine_t *jem, char *s, int len);


int json_encode_machine_init(json_encode_machine_t *jem) {
    jem->state = jem_encode_state_init;
    jem->start = 0;
    jem->complete = 0;
    jem->ptr = NULL;
    sb_clear(&jem->buffer);
    arrow_linked_list_init(jem);
    return 0;
}

#define json_encode_inc(buf, len, ret) { \
    (ret) ++; \
    (len) --; \
    (buf) ++; \
    }

#define json_encode_add(size, buf, len, ret) { \
    (ret) += (size); \
    (len) -= (size); \
    (buf) += (size); \
    }

int encoded_strlen(const char *str) {
    int len = strlen(str);
    int total = 2;
    int i = 0;
    for ( i = 0; i < len; i++ ) {
        if ( is_escape(str[i]) ) total++;
        total++;
    }
    return total;
}

int jem_encode_value_string(json_encode_machine_t *jem, char *s, int len) {
    int value_size = encoded_strlen(jem->ptr->string_);

    int r = jem_encode_string('\"', '\"',
                              jem->ptr->string_,
                              jem->start, s, len);
    if ( r < 0 ) return -1;
    jem->start += r;
    if ( jem->start == value_size ) jem->complete = 1;

    return r;
}

int jem_encode_bool(json_encode_machine_t *jem, char *s, int len) {
    char s_true[]  = "true";
    char s_false[] = "false";
    char *buf_bool = jem->ptr->bool_ ? s_true : s_false;
    int buf_size = (jem->ptr->bool_ ? 4 : 5) - jem->start;
    if ( buf_size < len ) {
        memcpy(s, buf_bool + jem->start, buf_size);
        jem->complete = 1;
        return buf_size;
    } else {
        memcpy(s, buf_bool + jem->start, len);
        jem->start += len;
    }
    return len;
}

int jem_encode_number(json_encode_machine_t *jem, char *s, int len) {
    if ( !sb_size(&jem->buffer) ) {
        sb_init(&jem->buffer);
        emit_number(&jem->buffer, jem->ptr->number_);
    }
    int buf_size = (jem->buffer.cur - jem->buffer.start) - jem->start;
    if ( buf_size < len ) {
        memcpy(s, jem->buffer.start + jem->start, buf_size);
        jem->complete = 1;
        return buf_size;
    } else {
        memcpy(s, jem->buffer.start + jem->start, len);
        jem->start += len;
    }
    return len;
}

int jem_encode_string(char start_,
                      char end_,
                      const char *s,
                      int offset,
                      char *buf,
                      int len) {
    int buf_start = 0;
    if ( !offset && len ) {
        offset++;
        buf[buf_start++] = start_;
        len--;
    }
    if ( offset >= 1 && offset < encoded_strlen(s) - 1 && len ) {
        int skip = 0;
        int index = 0;
        int threash = offset - 1;
        while ( len && offset < encoded_strlen(s) - 1 ) {
            if ( skip < threash ) {
                if ( is_escape(s[index++]) ) {
                    skip++;
                    if ( skip == threash ) {
                        buf[buf_start++] = s[index-1];
                        offset++;
                        if ( ! --len ) break;
                    }
                }
                skip++;
                continue;
            }
            char sym = s[index++];
            if ( is_escape(sym) ) {
                buf[buf_start++] = '\\';
                offset++;
                if ( ! --len ) break;
            }
            buf[buf_start++] = sym;
            offset++;
            len--;
        }
    }
    if ( offset >= encoded_strlen(s) - 1 && len ) {
        buf[buf_start++] = end_;
        len--;
    }
    return buf_start;
}

int jem_encode_key(json_encode_machine_t *jem, char *s, int len) {
    int key_size = encoded_strlen(P_VALUE(jem->ptr->key));

    int r = jem_encode_string('\"', '\"',
                              P_VALUE(jem->ptr->key),
                              jem->start, s, len);
    if ( r < 0 ) return -1;
    if ( jem->start + r == key_size ) {
        jem->complete = 1;
    }

    return r;
}

static void jem_change_state(json_encode_machine_t *jem, int newstate) {
    jem->state = newstate;
    jem->complete = 0;
    jem->start = 0;
}

int jem_encode_value(json_encode_machine_t *jem, char *s, int len) {
    int total = 0;
    switch(jem->state) {
    case jem_encode_state_init: {
        if ( !IS_EMPTY(jem->ptr->key) ) {
            int r = jem_encode_key(jem, s, len);
            if ( r < 0 )  return -1;
            json_encode_add(r, s, len, total);
            if ( jem->complete ) {
                jem_change_state(jem, jem_encode_state_key);
            }
            else {
                jem->start += r;
                return total;
            }
            if ( !len ) return total;
        } else {
            jem_change_state(jem, jem_encode_state_delim);
        }
    }
    case jem_encode_state_key: {
        if ( jem->state == jem_encode_state_key ) {
            s[0] = ':';
            json_encode_inc(s, len, total);
            jem_change_state(jem, jem_encode_state_delim);
            if ( !len ) return total;
        } else {
            jem_change_state(jem, jem_encode_state_delim);
        }
    }
    case jem_encode_state_delim: {
        switch(jem->ptr->tag) {
        case JSON_STRING: {
            int r = jem_encode_value_string(jem, s, len);
            if ( r < 0 ) return -1;
            json_encode_add(r, s, len, total)
            if ( !jem->complete ) {
                return total;
            }
        } break;
        case JSON_BOOL: {
            int r = jem_encode_bool(jem, s, len);
            if ( r < 0 ) return -1;
            json_encode_add(r, s, len, total)
            if ( !jem->complete ) {
                return total;
            }
        } break;
        case JSON_NUMBER: {
            int r = jem_encode_number(jem, s, len);
            if ( r < 0 ) return -1;
            json_encode_add(r, s, len, total)
            if ( !jem->complete ) {
                return total;
            }
        } break;
        case JSON_ARRAY: {
            switch(jem->start) {
            case 0: {
                s[0] = '[';
                json_encode_inc(s, len, total);
                jem->start++;
                if ( !len ) return total;
            }
            case 1: {
                int r = jem_encode_obj(jem, s, len);
                if ( r < 0 ) return -1;
                json_encode_add(r, s, len, total);
                if ( jem->complete ) {
                    jem->start++;
                    jem->complete = 0;
                } else {
                    return total;
                }
                if ( !len ) return total;
            }
            case 2:{
                s[0] = ']';
                json_encode_inc(s, len, total);
                jem->start++;
                if ( !len ) return total;
            }
            case 3: {
                jem->complete = 1;
            }
            }
        } break;
        case JSON_OBJECT: {
            switch(jem->start) {
            case 0: {
                s[0] = '{';
                json_encode_inc(s, len, total);
                jem->start++;
                if ( !len ) return total;
            }
            case 1: {
                int r = jem_encode_obj(jem, s, len);
                if ( r < 0 ) return -1;
                json_encode_add(r, s, len, total);
                if ( jem->complete ) {
                    jem->start++;
                    jem->complete = 0;
                } else {
                    return total;
                }
                if ( !len ) return total;
            }
            case 2:{
                s[0] = '}';
                json_encode_inc(s, len, total);
                jem->start++;
                if ( !len ) return total;
            }
            case 3: {
                jem->complete = 1;
            }
            }
        } break;
        default:
            return -1;
        }
        jem->state = jem_encode_state_value;
    }
    case jem_encode_state_value: {
        return total;
    }
    default:
        return -1;
    }
    return total;
}

int jem_encode_obj(json_encode_machine_t *jem, char *s, int len) {
    if ( !len ) return 0;
    int ret = 0;
    json_encode_machine_t *next = NULL;
    arrow_linked_list_next_node(next, jem, json_encode_machine_t);
    if ( !next ) {
        next = alloc_type(json_encode_machine_t);
        next->ptr = NULL;
        arrow_linked_list_add_node_last(jem, json_encode_machine_t, next);
    }
    if ( !next->ptr && !next->complete ) {
        json_encode_machine_init(next);
        next->ptr = json_first_child(jem->ptr);
    }
    while( next->ptr ) {

        if ( next->complete ) {
            if ( len ) {
                s[0] = ',';
                json_encode_inc(s, len, ret);
                next->complete = 0;
            } else {
                return ret;
            }
        }
        if ( !len ) return ret;

        int r = jem_encode_value(next, s, len);
        if ( ret < 0 ) return -1;
        json_encode_add(r, s, len, ret);
        if ( next->complete ) {
            JsonNode *p = json_next(next->ptr);
            json_encode_machine_fin(next);
            json_encode_machine_init(next);
            next->ptr = p;
            next->complete = 1;
        }
    }
    if ( next && next->complete ) {
        arrow_linked_list_del_node_last(jem, json_encode_machine_t);
        json_encode_machine_fin(next);
        free(next);
        jem->complete = 1;
    }
    return ret;
}

int json_encode_machine_process(json_encode_machine_t *jem, char* s, int len) {
    return jem_encode_value(jem, s, len);
}

int json_encode_machine_fin(json_encode_machine_t *jem) {
      if ( sb_size(&jem->buffer) ) sb_free(&jem->buffer);
      jem->ptr = NULL;
      json_encode_machine_t *next = NULL;
      arrow_linked_list_next_node(next, jem, json_encode_machine_t);
      if ( next ) {
          json_encode_machine_fin(next);
          free(next);
      }
    return 0;
}

int json_encode_init(json_encode_machine_t *jem, JsonNode *node) {
    json_encode_machine_init(jem);
    jem->ptr = node;
    return 0;
}

int json_encode_part(json_encode_machine_t *jem, char *s, int len) {
    return json_encode_machine_process(jem, s, len);
}

int json_encode_fin(json_encode_machine_t *jem) {
    return json_encode_machine_fin(jem);
}

