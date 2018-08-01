#include "arrow/gateway_payload_sign.h"
#include <arrow/sign.h>
#include <ssl/crypt.h>
#include <sys/mem.h>
#include <debug.h>
#include <arrow/utf8.h>
// FIXME std?
#include <ctype.h>

#include <data/chunk.h>

#define MAX_PARAM_LINE 20

#if defined(STATIC_MQTT_ENV)
// FIXME buffer len?
static char static_canonical_prm[3000];
#else
static int cmpstringp(const void *p1, const void *p2) {
  return strcmp(* (char * const *) p1, * (char * const *) p2);
}
#endif

#if defined(STATIC_MQTT_ENV)
typedef struct _str_t {
  char *start;
  int len;
} str_t;

static int less(str_t *s1, str_t *s2) {
    int _min = (s1->len < s2->len ? s1->len : s2->len);
    int i = 0;
    for ( i = 0; i<_min; i++) {
        if ( s1->start[i] == s2->start[i] ) continue;
        if ( s1->start[i] < s2->start[i] ) {
            return 1;
        } else return -1;
    }
    return 0;
}

static void swap(str_t *s1, str_t *s2) {
    char saved;
    int start_pos = s1->len-1;
    int current_pos = start_pos;
    int i = 0;
    start_pos = 0;
    while (i < s1->len + s2->len ) {
        current_pos = start_pos;
        saved = s1->start[current_pos];
        do {
            int next_pos = 0;
            if ( current_pos < s1->len ) next_pos = s2->len + current_pos;
            else next_pos = current_pos - s1->len;
            char t = s1->start[next_pos];
            s1->start[next_pos] = saved;
            saved = t;
            i++;
            current_pos = next_pos;
        } while ( start_pos != current_pos );
        start_pos++;
    }
    int size1 = s1->len;
    s1->len = s2->len;
    s2->len = size1;
    s2->start = s1->start + s1->len;
}

static int bubble(str_t *s, int len) {
    int do_sort = 1;
    while(do_sort) {
        do_sort = 0;
        int i = 0;
        for ( i = 0; i<len-1; i++ ) {
            if ( less (&s[i], &s[i+1]) == -1 ) {
                swap(&s[i], &s[i+1]);
                do_sort = 1;
            }
        }
    }
    return 0;
}

static char *form_canonical_prm(JsonNode *param) {
  JsonNode *child;
  char *canParam = static_canonical_prm;
  str_t can_list[MAX_PARAM_LINE];
  int total = 0;
  int count = 0;
  json_foreach(child, param) {
      can_list[count].start = canParam + total;
      int i;
      int key_len = strlen(json_key(child));
      for ( i=0; i < key_len; i++ )
          *(can_list[count].start+i) = tolower((int)json_key(child)[i]);
      *(can_list[count].start+i) = '=';
      can_list[count].len = key_len + 1;

      int r = 0;
      switch(child->tag) {
      case JSON_STRING:
          r = snprintf(can_list[count].start+can_list[count].len,
                       sizeof(static_canonical_prm) - total,
                       "%s",
                       child->string_);
          break;
      case JSON_BOOL:
          r = snprintf(can_list[count].start+can_list[count].len,
                       sizeof(static_canonical_prm) - total,
                       "%s",
                       (child->bool_?"true":"false"));
          break;
      default:
          r = snprintf(can_list[count].start+can_list[count].len,
                       16,
                       "%f",
                       child->number_);
      }

      can_list[count].len += r;
      can_list[count].start[can_list[count].len] = '\n';
      can_list[count].len++;
      total += can_list[count].len;
      count++;
  }
  can_list[count-1].start[can_list[count-1].len] = '\0';
  bubble(can_list, count);
  can_list[count-1].start[can_list[count-1].len-1] = '\0';
  return canParam;
}
#else
static char *form_canonical_prm(JsonNode *param) {
  JsonNode *child;
  char *canParam = NULL;
  char *can_list[MAX_PARAM_LINE] = {0};
  int total_len = 0;
  int count = 0;
  json_foreach(child, param) {
    int alloc_len = child->tag==JSON_STRING?strlen(child->string_):50;
    alloc_len += strlen(json_key(child));
    alloc_len += 10;
    can_list[count] = (char*)malloc( alloc_len );
    if ( !can_list[count] ) {
        DBG("GATEWAY SIGN: not enough memory");
        goto can_list_error;
    }
    total_len += alloc_len;
    unsigned int i;
    for ( i=0; i<strlen(json_key(child)); i++ ) *(can_list[count]+i) = tolower((int)json_key(child)[i]);
    *(can_list[count]+i) = '=';
    switch(child->tag) {
      case JSON_STRING: strcpy(can_list[count]+i+1, child->string_);
        break;
      case JSON_BOOL: strcpy(can_list[count]+i+1, (child->bool_?"true\0":"false\0"));
        break;
      default: {
        int r = snprintf(can_list[count]+i+1, 50, "%f", child->number_);
        *(can_list[count]+i+1 + r ) = 0x0;
      }
    }
    count++;
  }
  canParam = (char*)malloc(total_len);
  DBG("GATEWAY SIGN: alloc memory %d", total_len);
  if ( !canParam ) {
      DBG("GATEWAY SIGN: not enough memory %d", total_len);
      goto can_list_error;
  }
  *canParam = 0;
  qsort(can_list, count, sizeof(char *), cmpstringp);
  int i = 0;
  for (i=0; i<count; i++) {
    strcat(canParam, can_list[i]);
    if ( i < count-1 ) strcat(canParam, "\n");
    free(can_list[i]);
  }
  return canParam;
can_list_error:
  for (i=0; i < count; i++) {
    free(can_list[i]);
  }
  return NULL;
}
#endif

int gateway_payload_sign(char *signature,
                         const char *hid,
                         const char *name,
                         int encrypted,
                         const char *canParString,
                         const char *signatureVersion) {
  int ret = -1;
  // step 1
  sha256_init();
  sha256_chunk(hid, strlen(hid));
  sha256_chunk("\n", 1);
  sha256_chunk(name, strlen(name));
  sha256_chunk("\n", 1);
  if ( encrypted ) {
      sha256_chunk("true\n", 5);
  } else {
      sha256_chunk("false\n", 6);
  }
  sha256_chunk(canParString, strlen(canParString));
  sha256_chunk("\n", 1);

  CREATE_CHUNK(hex_canreq, 66);
  CHECK_CHUNK(hex_canreq, goto hex_tmp_error);

  CREATE_CHUNK(tmp, 34);
  CHECK_CHUNK(tmp, goto tmp_error);

  sha256_fin(tmp);
  hex_encode(hex_canreq, tmp, 32);
  hex_canreq[64] = '\0';
#if defined(DEBUG_GATEWAY_PAYLOAD_SIGN)
  DBG("can: %s", hex_canreq);
#endif

  CREATE_CHUNK(hex_tmp, 66);
  CHECK_CHUNK(hex_tmp, goto hex_tmp_error);

  // step 3

#if defined(DEBUG_GATEWAY_PAYLOAD_SIGN)
  DBG("api: %s", get_api_key());
  DBG("sec: %s", get_secret_key());
#endif
  hmac256(tmp, get_api_key(), (int)strlen(get_api_key()), get_secret_key(), (int)strlen(get_secret_key()));
  hex_encode(hex_tmp, tmp, 32);
  hex_tmp[64] = 0x0;
#if defined(DEBUG_GATEWAY_PAYLOAD_SIGN)
  DBG("hex1: %s", hex_tmp);
#endif
  memset(tmp, 0, 34);
  hmac256(tmp, signatureVersion, (int)strlen(signatureVersion), hex_tmp, (int)strlen(hex_tmp));
  hex_encode(hex_tmp, tmp, 32);
  hex_tmp[64] = 0x0;
#if defined(DEBUG_GATEWAY_PAYLOAD_SIGN)
  DBG("hex2: [%d]%s", strlen(hex_tmp), hex_tmp);
#endif
  hmac256_init(hex_tmp, strlen(hex_tmp));
  hmac256_chunk(hex_canreq, strlen(hex_canreq));
  hmac256_chunk("\n", 1);
  hmac256_chunk(get_api_key(), strlen(get_api_key()));
  hmac256_chunk("\n", 1);
  hmac256_chunk(signatureVersion, strlen(signatureVersion));
  hmac256_fin(tmp);

  hex_encode(signature, tmp, 32);
  signature[64] = 0x0;
#if defined(DEBUG_GATEWAY_PAYLOAD_SIGN)
  DBG("sig: [%d]%s", strlen(signature), signature);
#endif
  ret  = 0;
  FREE_CHUNK(tmp);
tmp_error:
  FREE_CHUNK(hex_tmp);
hex_tmp_error:
  FREE_CHUNK(canonicalRequest);
  FREE_CHUNK(hex_canreq);
  return ret;
}

int arrow_event_sign(char *signature,
                     property_t ghid,
                     const char *name,
                     int encrypted,
                     JsonNode *_parameters) {
    char *can = form_canonical_prm(_parameters);
    if ( !can ) goto sign_error;
    int err = gateway_payload_sign(signature,
                                   P_VALUE(ghid),
                                   name,
                                   encrypted,
                                   can,
                                   "1");
    if ( err < 0 ) goto sign_error;
#if !defined(STATIC_MQTT_ENV)
    free(can);
#endif
    return 0;
sign_error:
    return -1;
}
