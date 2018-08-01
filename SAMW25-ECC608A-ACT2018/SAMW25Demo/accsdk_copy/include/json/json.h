/*
  Copyright (C) 2011 Joseph A. Adams (joeyadams3.14159@gmail.com)
  All rights reserved.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef CCAN_JSON_H
#define CCAN_JSON_H

#include <sys/mem.h>
#include <data/property.h>

#define is_space(c) ((c) == '\t' || (c) == '\n' || (c) == '\r' || (c) == ' ')
#define is_digit(c) ((c) >= '0' && (c) <= '9')

# define json_key(x)  P_VALUE((x)->key)
# define json_number(x) (x)->number_
# define json_remove_from(obj, x) json_remove_from_parent(x)

typedef enum {
    JSON_NULL,
	JSON_BOOL,
	JSON_STRING,
	JSON_NUMBER,
	JSON_ARRAY,
    JSON_OBJECT
} JsonTag;

typedef struct JsonNode JsonNode;

struct __attribute_packed__ JsonNode
{
	/* only if parent is an object or array (NULL otherwise) */
	JsonNode *parent;
	JsonNode *prev, *next;
	
	/* only if parent is an object (NULL otherwise) */
    property_t key; /* Must be valid UTF-8. */
	
	JsonTag tag;
  union {
		/* JSON_BOOL */
		bool bool_;
		
		/* JSON_STRING */
		char *string_; /* Must be valid UTF-8. */
		
		/* JSON_NUMBER */
		double number_;
		
		/* JSON_ARRAY */
		/* JSON_OBJECT */
		struct {
			JsonNode *head, *tail;
		} children;
  };
};

/* String buffer */
typedef struct {
    char *cur;
    char *end;
    char *start;
} SB;
int sb_init(SB *sb);
int sb_grow(SB *sb, int need);
int sb_need(SB *sb, int need);
int sb_put(SB *sb, const char *bytes, int count);

#define sb_putc(sb, c) do {         \
        if ((sb)->cur >= (sb)->end) \
            sb_grow(sb, 1);         \
        *(sb)->cur++ = (c);         \
    } while (0)

int sb_is_valid(SB *sb);
int sb_puts(SB *sb, const char *str);
char *sb_finish(SB *sb);
void sb_clear(SB *sb);
void sb_free(SB *sb);
int sb_size(SB *sb);

/*** Encoding, decoding, and validation ***/
typedef int(*_json_parse_fn)(void *, char);

typedef struct _json_parse_machine_ {
    struct _json_parse_machine_ *p;
    uint32_t complete;
    _json_parse_fn process_byte;
    SB buffer;
    property_t key;
    JsonNode *root;
    arrow_linked_list_head_node;
} json_parse_machine_t;

int json_parse_machine_init(json_parse_machine_t *jpm);
int json_parse_machine_process(json_parse_machine_t *jpm, char byte);
int json_parse_machine_fin(json_parse_machine_t *jpm);

int json_decode_init(json_parse_machine_t *sm);
int json_decode_part(json_parse_machine_t *sm, const char *json, size_t size);
JsonNode *json_decode_finish(json_parse_machine_t *sm);

JsonNode   *json_decode         (const char *json);

size_t json_size(JsonNode *obj);

enum json_encode_states {
    jem_encode_state_init,
    jem_encode_state_key,
    jem_encode_state_delim,
    jem_encode_state_value
};

typedef struct _json_encode_machine_ {
    uint16_t state;
    uint16_t start;
    uint16_t complete;
    size_t len;
    SB buffer;
    JsonNode *ptr;
    arrow_linked_list_head_node;
} json_encode_machine_t;

int json_encode_machine_init(json_encode_machine_t *jem);
int json_encode_machine_process(json_encode_machine_t *jem, char* s, int len);
int json_encode_machine_fin(json_encode_machine_t *jem);

int         json_encode_init    (json_encode_machine_t *jem, JsonNode *node);
int         json_encode_part    (json_encode_machine_t *jem, char *s, int len);
int         json_encode_fin     (json_encode_machine_t *jem);

char       *json_encode         (const JsonNode *node);
char       *json_encode_string  (const char *str);
property_t  json_encode_property(const JsonNode *node);
char       *json_stringify      (const JsonNode *node, const char *space);
void        json_delete         (JsonNode *node);
char       *json_strdup         (const char *str);
property_t  json_strdup_property(const char *str);
void        json_delete_string  (char *json_str);

int         fill_string_from_json(JsonNode *_node, property_t name, property_t *p);

bool        json_validate       (const char *json);

/*** Lookup and traversal ***/

JsonNode   *json_find_element   (JsonNode *array, int index);
JsonNode   *json_find_member    (JsonNode *object, property_t key);

JsonNode   *json_first_child    (const JsonNode *node);

#define json_next(i) ( (i)->next )

#define json_foreach(i, object_or_array)            \
	for ((i) = json_first_child(object_or_array);   \
		 (i) != NULL;                               \
		 (i) = (i)->next)

#define json_foreach_start(i, object_or_array, start_child)            \
    for ((i) = (start_child);   \
         (i) != NULL;                               \
         (i) = (i)->next)

/*** Construction and manipulation ***/

JsonNode *json_mknull(void);
JsonNode *json_mkbool(bool b);
JsonNode *json_mkstring(const char *s);
JsonNode *json_mknumber(double n);
JsonNode *json_mkarray(void);
JsonNode *json_mkobject(void);

void json_append_element(JsonNode *array, JsonNode *element);
void json_prepend_element(JsonNode *array, JsonNode *element);
void json_append_member(JsonNode *object, const property_t key, JsonNode *value);
void json_prepend_member(JsonNode *object, const property_t key, JsonNode *value);

void json_remove_from_parent(JsonNode *node);

int json_static_memory_max_sector(void);

/*** Debugging ***/

/*
 * Look for structure and encoding problems in a JsonNode or its descendents.
 *
 * If a problem is detected, return false, writing a description of the problem
 * to errmsg (unless errmsg is NULL).
 */
bool json_check(const JsonNode *node, char errmsg[256]);

#endif
