#include "arrow/node.h"
#include <http/http_routine.h>
#include <json/json.h>
#include <debug.h>

#include <data/chunk.h>

#define URI_LEN sizeof(ARROW_API_NODE_ENDPOINT) + 50

static void _node_list_init(http_request_t *request, void *arg) {
  SSP_PARAMETER_NOT_USED(arg);
  http_request_init(request, GET, ARROW_API_NODE_ENDPOINT);
}

static int _node_list_proc(http_response_t *response, void *arg) {
  SSP_PARAMETER_NOT_USED(arg);
  if ( response->m_httpResponseCode != 200 ) return -1;
  DBG("gateway hid: %s", P_VALUE(response->payload));
  return 0;
}

int arrow_node_list(void) {
  STD_ROUTINE(_node_list_init, NULL,
              _node_list_proc, NULL,
              "Arrow Node list failed...");
}

static property_t arrow_node_serialize(arrow_node_t *node) {
  JsonNode *_main = json_mkobject();
  json_append_member(_main, p_const("description"), json_mkstring(node->description));
  json_append_member(_main, p_const("enabled"), json_mkbool(node->enabled));
  json_append_member(_main, p_const("name"), json_mkstring(node->name));
  json_append_member(_main, p_const("nodeTypeHid"), json_mkstring(node->nodeTypeHid));
  json_append_member(_main, p_const("parentNodeHid"), json_mkstring(node->parentNodeHid));
  property_t payload = json_encode_property(_main);
  json_delete(_main);
  return payload;
}

static void _node_create_init(http_request_t *request, void *arg) {
  arrow_node_t *node = (arrow_node_t *) arg;
  http_request_init(request, POST, ARROW_API_NODE_ENDPOINT);
  http_request_set_payload(request, arrow_node_serialize(node));
}

static int _node_create_proc(http_response_t *response, void *arg) {
  SSP_PARAMETER_NOT_USED(arg);
  if ( response->m_httpResponseCode != 200 ) return -1;
  DBG("ans: %s", P_VALUE(response->payload));
  return 0;
}

int arrow_node_create(arrow_node_t *node) {
  STD_ROUTINE(_node_create_init, node,
              _node_create_proc, NULL,
              "Arrow Node create failed...");
}

static void _node_update_init(http_request_t *request, void *arg) {
  arrow_node_t *node = (arrow_node_t *) arg;
  CREATE_CHUNK(uri, URI_LEN);
  snprintf(uri, URI_LEN, "%s/%s", ARROW_API_NODE_ENDPOINT, node->hid);
  http_request_init(request, PUT, uri);
  FREE_CHUNK(uri);
  http_request_set_payload(request, arrow_node_serialize(node));
}

static int _node_update_proc(http_response_t *response, void *arg) {
  SSP_PARAMETER_NOT_USED(arg);
  if ( response->m_httpResponseCode != 200 ) return -1;
  DBG("ans: %s", P_VALUE(response->payload));
  return 0;
}


int arrow_node_update(arrow_node_t *node) {
  STD_ROUTINE(_node_update_init, node,
              _node_update_proc, NULL,
              "Arrow Node updaet failed...");
}
