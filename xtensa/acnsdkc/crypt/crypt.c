/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "crypt/crypt.h"

#include <qcom_common.h>
#include <qcom_crypto.h>
#include <arrow/mem.h>
#include <debug.h>

void ref_attr_init(qcom_crypto_attrib_t* attr, A_UINT32 id,
        void* buffer, uint32_t length) {
    attr->attrib_id = id;
    attr->u.ref.len = length;
    attr->u.ref.buf = buffer;
}

void sha256(char *shasum, char *buf, int size) {
    qcom_crypto_op_hdl_t opHdl;
    uint32_t hashLen;
    if (qcom_crypto_op_alloc(QCOM_CRYPTO_ALG_SHA256, QCOM_CRYPTO_MODE_DIGEST, 0, &opHdl) != A_CRYPTO_OK) {
      DBG("Failed to alloc op");
    }
    if (qcom_crypto_op_digest_dofinal(opHdl, buf, size, shasum, &hashLen) != A_CRYPTO_OK) {
      DBG("Failed to digest dofinal");
    }
    qcom_crypto_op_free(opHdl);
}

#define SHA256_DIGEST_LENGTH 32
static void hmac_sha256(
    const char *text,
    int text_len,
    const char *key,
    int key_len,
    char *digest) {
    unsigned char k_ipad[65];
    unsigned char k_opad[65];
    unsigned char tk[SHA256_DIGEST_LENGTH];
    unsigned char tk2[SHA256_DIGEST_LENGTH];
    unsigned char *bufferIn;
    unsigned char bufferOut[20 * (64 + SHA256_DIGEST_LENGTH)];
    int           i;

    bufferIn = (unsigned char *)malloc(64 + text_len);
    if ( key_len > 64 ) {
        sha256( (char*)tk, (char*)key, key_len );
        key = (char*)tk;
        key_len = SHA256_DIGEST_LENGTH;
    }
    memset( k_ipad, 0, sizeof k_ipad );
    memset( k_opad, 0, sizeof k_opad );
    memcpy( k_ipad, key, key_len );
    memcpy( k_opad, key, key_len );
    for ( i = 0; i < 64; i++ ) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    memset( bufferIn, 0x00, 64 + text_len );
    memcpy( bufferIn, k_ipad, 64 );
    memcpy( bufferIn + 64, text, text_len );
    sha256( (char*)tk2, (char*)bufferIn, 64 + text_len );
    free(bufferIn);
    memset( bufferOut, 0x00,  64 + SHA256_DIGEST_LENGTH );
    memcpy( bufferOut, k_opad, 64 );
    memcpy( bufferOut + 64, tk2, SHA256_DIGEST_LENGTH );
    sha256( digest, (char*)bufferOut, 64 + SHA256_DIGEST_LENGTH );
}

void hmac256(char *hmacdig, const char *key, int key_size, const char *buf, int buf_size) {
//  DBG("key %s | %s", key, buf);
  if ( key_size < QCOM_CRYPTO_HMAC_SHA256_MIN_KEY_BYTES ) {
    hmac_sha256(buf, buf_size, key, key_size, hmacdig);
    return;
  }
  qcom_crypto_obj_hdl_t mac_objHdl;
  qcom_crypto_op_hdl_t compute_opHdl;
  qcom_crypto_attrib_t attr;
  uint32_t macLen;
  A_CRYPTO_STATUS status;

  status = qcom_crypto_transient_obj_alloc(
             QCOM_CRYPTO_OBJ_TYPE_HMAC_SHA256,
             QCOM_CRYPTO_HMAC_SHA256_MAX_KEY_BITS,
             &mac_objHdl);
  if ( status != A_CRYPTO_OK) {
    DBG("Failed to alloc mac %d", status);
  }

  status = qcom_crypto_op_alloc(
                             QCOM_CRYPTO_ALG_HMAC_SHA256,
                             QCOM_CRYPTO_MODE_MAC,
                             QCOM_CRYPTO_HMAC_SHA256_MAX_KEY_BITS,
                             &compute_opHdl);
  if ( status != A_CRYPTO_OK) {
    DBG("Failed to alloc op %d", status);
  }
  ref_attr_init(&attr,
      QCOM_CRYPTO_ATTR_SECRET_VALUE, (void*)key, key_size);
  status = qcom_crypto_transient_obj_populate(mac_objHdl, &attr, 1);
  if ( status != A_CRYPTO_OK) {
    DBG("Failed to populate mac public obj %d", status);
  }
  status = qcom_crypto_op_key_set(compute_opHdl, mac_objHdl);
  if ( status != A_CRYPTO_OK) {
    DBG("Failed to set key %d", status);
  }
  status = qcom_crypto_op_mac_init(compute_opHdl, NULL, 0);
  if ( status != A_CRYPTO_OK) {
    DBG("Failed to mac init %d", status);
  }
  status = qcom_crypto_op_mac_compute_final(compute_opHdl, (void*)buf, buf_size, hmacdig, &macLen);
  if ( status != A_CRYPTO_OK) {
    DBG("Failed to mac compute final %d", status);
  }
  qcom_crypto_transient_obj_free(mac_objHdl);
  qcom_crypto_op_free(compute_opHdl);
}
