#if !defined(_ARROW_GATEWAY_PAYLOAD_SIGN_H_)
#define _ARROW_GATEWAY_PAYLOAD_SIGN_H_

#include <config.h>
#include <json/json.h>

int gateway_payload_sign(char *signature,
                         const char *hid,
                         const char *name,
                         int encrypted,
                         const char *canParString,
                         const char *signatureVersion);

int arrow_event_sign(char *signature,
                  property_t ghid,
                  const char *name,
                  int encrypted,
                  JsonNode *_parameters);

#endif // _ARROW_GATEWAY_PAYLOAD_SIGN_H_
