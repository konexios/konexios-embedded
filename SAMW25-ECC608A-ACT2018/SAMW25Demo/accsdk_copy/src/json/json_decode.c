/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <json/json.h>
#include <data/property_json.h>

#define is_array_context(mach) ((mach)->p->root->tag == JSON_ARRAY)

static int jpm_key_init(json_parse_machine_t *jpm, char byte);
static int jpm_key_body(json_parse_machine_t *jpm, char byte);
static int jpm_value_init(json_parse_machine_t *jpm, char byte);
static int jpm_value_end(json_parse_machine_t *jpm, char byte);
static int jpm_string_body(json_parse_machine_t *jpm, char byte);

#define jpm_bool_state_char(b, c, sym, n) \
static int jpm_bool_##b##_##c(json_parse_machine_t *jpm, char byte) { \
    if ( byte == sym ) { \
        jpm->process_byte = (_json_parse_fn) jpm_bool_##b##_##n; \
        return 0; \
    } \
    return -1; \
}

static int jpm_bool_false_e(json_parse_machine_t *jpm, char byte) {
    if ( byte == 'e' ) {
        jpm->root = json_mkbool(false);
        if ( !jpm->root ) return -1;
        jpm->process_byte = (_json_parse_fn) jpm_value_end;
        return 0;
    }
    return -1;
}

jpm_bool_state_char(false, s, 's', e)
jpm_bool_state_char(false, l, 'l', s)
jpm_bool_state_char(false, a, 'a', l)

static int jpm_bool_true_e(json_parse_machine_t *jpm, char byte) {
    if ( byte == 'e' ) {
        jpm->root = json_mkbool(true);
        if ( !jpm->root ) return -1;
        jpm->process_byte = (_json_parse_fn) jpm_value_end;
        return 0;
    }
    return -1;
}

jpm_bool_state_char(true, u, 'u', e)
jpm_bool_state_char(true, r, 'r', u)


static int jpm_string_escape(json_parse_machine_t *jpm, char byte) {
    switch ( byte ) {
    case 'n': {
        sb_putc(&jpm->buffer, '\n');
    } break;
    case 't': {
        sb_putc(&jpm->buffer, '\t');
    } break;
    default:
        sb_putc(&jpm->buffer, byte);
    }
    if ( !sb_is_valid(&jpm->buffer) ) return -1;
    jpm->process_byte = (_json_parse_fn) jpm_string_body;
    return 0;
}

static int jpm_string_body(json_parse_machine_t *jpm, char byte) {
    switch ( byte ) {
    case '\\': {
        jpm->process_byte = (_json_parse_fn) jpm_string_escape;
    } break;
    case '"': {
        if ( sb_size(&jpm->buffer) <= 0 ) return -1;
        char *str = sb_finish(&jpm->buffer);
        jpm->root = json_mkstring(str);
        if ( !jpm->root ) return -1;
        jpm->process_byte = (_json_parse_fn) jpm_value_end;
        sb_free(&jpm->buffer);
    } break;
    default:
        sb_putc(&jpm->buffer, byte);
        if ( !sb_is_valid(&jpm->buffer) ) return -1;
        break;
    }
    return 0;
}

static int jpm_number_body(json_parse_machine_t *jpm, char byte) {
    if ( is_digit(byte) || byte == '.' ) {
        sb_putc(&jpm->buffer, byte);
        if ( !sb_is_valid(&jpm->buffer) ) return -1;
    } else {
        if ( sb_size(&jpm->buffer) <= 0 ) return -1;
        char *str = sb_finish(&jpm->buffer);
        double d = strtod(str, NULL);
        jpm->root = json_mknumber(d);
        if ( !jpm->root ) return -1;
        jpm->process_byte = (_json_parse_fn) jpm_value_end;
        sb_free(&jpm->buffer);
        return jpm->process_byte(jpm, byte);
    }
    return 0;
}

static int jpm_value_init(json_parse_machine_t *jpm, char byte) {
    if ( is_space(byte) ) return 0;
    switch( byte ) {
    case '{' : {
        json_parse_machine_t *nvalue = alloc_type(json_parse_machine_t);
        json_parse_machine_init(nvalue);
        arrow_linked_list_add_node_last(jpm, json_parse_machine_t, nvalue);
        nvalue->process_byte = (_json_parse_fn) jpm_key_init;
        jpm->root = json_mkobject();
        if ( !jpm->root ) return -1;
        nvalue->p = jpm;
        jpm->process_byte = (_json_parse_fn) jpm_value_end;
    } break;
    case '"': {
        if ( sb_init(&jpm->buffer) < 0 )
            return -1;
        jpm->process_byte = (_json_parse_fn) jpm_string_body;
    } break;
    case 'f': {
        jpm->process_byte = (_json_parse_fn) jpm_bool_false_a;
    } break;
    case 't': {
        jpm->process_byte = (_json_parse_fn) jpm_bool_true_r;
    } break;
    case '[': {
        json_parse_machine_t *nvalue = alloc_type(json_parse_machine_t);
        json_parse_machine_init(nvalue);
        arrow_linked_list_add_node_last(jpm, json_parse_machine_t, nvalue);
        nvalue->process_byte = (_json_parse_fn) jpm_value_init;
        jpm->root = json_mkarray();
        if ( !jpm->root ) return -1;
        nvalue->p = jpm;
        jpm->process_byte = (_json_parse_fn) jpm_value_end;
    } break;
    default:
        if ( sb_init(&jpm->buffer) < 0 )
            return -1;
        jpm->process_byte = (_json_parse_fn) jpm_number_body;
        return jpm->process_byte(jpm, byte);
    break;
    }
    return 0;
}

static int jpm_append_to_parent(json_parse_machine_t *jpm) {
    if ( jpm->p ) {
        if ( is_array_context(jpm) ) {
            json_append_element(jpm->p->root, jpm->root);
        } else {
            json_append_member(jpm->p->root, jpm->key, jpm->root);
        }
        return 0;
    }
    return -1;
}

static int jpm_value_end(json_parse_machine_t *jpm, char byte) {
    if ( is_space(byte) ) return 0;
    switch ( byte ) {
    case '}': {
        jpm->complete = 1;
        if ( jpm_append_to_parent(jpm) < 0 ) return -1;
        jpm->key = p_null();
        sb_clear(&jpm->buffer);
        jpm->root = NULL;
    } break;
    case ']': {
        jpm->complete = 1;
        if ( jpm_append_to_parent(jpm) < 0 )
            return -1;
        else
            jpm->root = NULL;
        jpm->key = p_null();
        sb_clear(&jpm->buffer);
        jpm->process_byte = NULL;
    } break;
    case ',': {
        if ( jpm_append_to_parent(jpm) < 0 ) return -1;
        jpm->key = p_null();
        json_parse_machine_t *prev = jpm->p;
        json_parse_machine_init(jpm);
        sb_clear(&jpm->buffer);
        jpm->p = prev;
        if ( is_array_context(jpm) )
            jpm->process_byte = (_json_parse_fn) jpm_value_init;
        else
            jpm->process_byte = (_json_parse_fn) jpm_key_init;

    } break;
    default:
        return -1;
    }
    return 0;
}

static int jpm_key_init(json_parse_machine_t *jpm, char byte) {
    if ( is_space(byte) ) return 0;
    switch ( byte ) {
    case '"': {
        if ( sb_init(&jpm->buffer) < 0 )
            return -1;
        jpm->process_byte = (_json_parse_fn) jpm_key_body;
    } break;
    default:
        return -1;
    }
    return 0;
}

static int jpm_key_end(json_parse_machine_t *jpm, char byte) {
    if ( is_space(byte) ) return 0;
    switch ( byte ) {
    case ':': {
        jpm->process_byte = (_json_parse_fn) jpm_value_init;
    } break;
    default:
        return -1;
    }
    return 0;
}

static int jpm_key_body(json_parse_machine_t *jpm, char byte) {
    switch ( byte ) {
    case '"': {
        if ( sb_size(&jpm->buffer) <= 0 ) return -1;
        char *str = sb_finish(&jpm->buffer);
        property_t key = p_json(str);
        property_move(&jpm->key, &key);
        property_free(&key);
        sb_clear(&jpm->buffer);
        jpm->process_byte = (_json_parse_fn) jpm_key_end;
    } break;
    default:
        sb_putc(&jpm->buffer, byte);
        if ( !sb_is_valid(&jpm->buffer) ) return -1;
        break;
    }
    return 0;
}


int json_parse_machine_init(json_parse_machine_t *jpm) {
    jpm->complete = 0;
    jpm->process_byte = (_json_parse_fn) jpm_value_init;
    property_init(&jpm->key);
    jpm->p = NULL;
    jpm->root = NULL;
    sb_clear(&jpm->buffer);
    arrow_linked_list_init(jpm);
    return 0;
}

int json_parse_machine_process(json_parse_machine_t *jpm, char byte) {
    json_parse_machine_t *next = NULL;
    arrow_linked_list_next_node(next, jpm, json_parse_machine_t);
    if ( next ) {
        // pass into last machine
        if ( next->complete ) {
            arrow_linked_list_del_node(jpm, json_parse_machine_t, next);
            json_parse_machine_fin(next);
            free(next);
            return json_parse_machine_process(jpm, byte);
        } else {
            return json_parse_machine_process(next, byte);
        }
    } else {
        if ( jpm->process_byte )
            return jpm->process_byte(jpm, byte);
    }
    return -1;
}

int json_parse_machine_fin(json_parse_machine_t *jpm) {
    if ( sb_size(&jpm->buffer) ) sb_free(&jpm->buffer);
    if ( !IS_EMPTY(jpm->key) )property_free(&jpm->key);
    jpm->process_byte = NULL;
    jpm->p = NULL;
    jpm->root = NULL;
    return 0;
}

int json_decode_init(json_parse_machine_t *sm) {
    return json_parse_machine_init(sm);
}

int json_decode_part(json_parse_machine_t *sm, const char *json, size_t size) {
    const char *s = json;
    size_t i = 0;
    for ( i = 0; i < size; i++ ) {
        if ( json_parse_machine_process(sm, s[i]) < 0 ) return -1;
    }
    return size;
}

JsonNode *json_decode_finish(json_parse_machine_t *sm) {
    JsonNode *root = sm->root;
    json_parse_machine_t *next = NULL;
    arrow_linked_list_next_node(next, sm, json_parse_machine_t);
    if ( next ) {
        json_parse_machine_t *tmp = NULL;
        arrow_linked_list_for_each_safe(tmp, next, json_parse_machine_t) {
            json_parse_machine_fin(tmp);
            free(tmp);
        }
    }
    json_parse_machine_fin(sm);
    return root;
}
