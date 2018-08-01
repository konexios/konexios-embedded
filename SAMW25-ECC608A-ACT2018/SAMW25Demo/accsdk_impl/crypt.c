/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#include "defs.h"

#include <string.h>
#include "crypto.h"

#include "third-party/sha256.h"
#include "ssl/crypt.h"
#include "arrow/sign.h"

// Types and definitions
// ---------------------------------------------------------------------------
// DEBUG
#define SHA256_CRYPTO_CHIP
//#define DEBUG_CRYPT

#if defined(DEBUG_CRYPT)
#define OUT(...)	printf(__VA_ARGS__)
#if 1
static void pbuf(char * title, uint8_t * ptr,int size)
{
	int x;
	printf("%s: ",title);
	if(ptr) for(x=0;x<size;x++) printf("%02X",ptr[x]);
	printf("\n");
}
#else
#define pbuf(...)
#endif
#else
#define OUT(...)
#define pbuf(...)
#endif

// Variables
// ---------------------------------------------------------------------------
static SHA256_CTX m_module_ctx_sha256;
static SHA256_CTX m_module_ctx_hmac;

#define SHA256_DIGEST_SIZE	32
#define HMAC_BLOCK_SIZE		64
#define HMAC_INNER_PAD		0x36
#define HMAC_OUTTER_PAD		0x5c
static uint8_t m_tmp_shadigest[SHA256_DIGEST_SIZE];
static uint8_t m_keyinner[HMAC_BLOCK_SIZE];
static uint8_t m_keyoutter[HMAC_BLOCK_SIZE];

// Local prototypes
// ---------------------------------------------------------------------------
static void _xor_with_buffer(uint8_t * buf, int len, uint8_t v);
static void hmac_sha256(uint8_t * inbuf, int inbuf_size, uint8_t * key, int key_len, uint8_t *digest);

// Public functions
// ---------------------------------------------------------------------------
void sdk_crypt_init()
{
	sha256lib_init(&m_module_ctx_sha256);
}

extern char test_GBA[];
extern char test_key[];
extern char test_key2[];
void crypt_test()
{
	ecc_data_slots_t slot = 5;
	uint8_t digest[32];
	int x;
	char key[] = "SomeKey";
	char message[] = "Testing";

	printf("-- SDK Crypt file test\n");
		
	//_crypto_test_hmac(slot,digest,(uint8_t*)message,strlen(message),(uint8_t*)key,strlen(key));
	hmac256((char*)digest, key, strlen(key), message, strlen(message));
	printf("Key  : '%s'\n",(char*)key);
	printf("Input: '%s'\n",(char*)message);
	printf("HMAC : ");
	for(x=0;x<32;x++) printf("%02X",digest[x]);
	printf("\n");

	//_crypto_test_hmac(slot,digest,(uint8_t*)message,strlen(message),(uint8_t*)test_key,strlen(test_key));
	hmac256((char*)digest, test_key, strlen(test_key), message, strlen(message));
	printf("Key  : '%s'\n",(char*)test_key);
	printf("Input: '%s'\n",(char*)message);
	printf("HMAC : ");
	for(x=0;x<32;x++) printf("%02X",digest[x]);
	printf("\n");

	//_crypto_test_hmac(slot,digest,(uint8_t*)test_GBA,strlen(test_GBA),(uint8_t*)key,strlen(key));
	hmac256((char*)digest, key, strlen(key), test_GBA, strlen(test_GBA));
	printf("Key  : '%s'\n",(char*)key);
	printf("Input: '%s'\n",(char*)test_GBA);
	printf("HMAC : ");
	for(x=0;x<32;x++) printf("%02X",digest[x]);
	printf("\n");

	//_crypto_test_hmac(slot,digest,(uint8_t*)test_GBA,strlen(test_GBA),(uint8_t*)test_key,strlen(test_key));
	hmac256((char*)digest, test_key, strlen(test_key), test_GBA, strlen(test_GBA));
	printf("Key  : '%s'\n",(char*)test_key);
	printf("Input: '%s'\n",(char*)test_GBA);
	printf("HMAC : ");
	for(x=0;x<32;x++) printf("%02X",digest[x]);
	printf("\n");
	
	//_crypto_test_hmac(slot,digest,(uint8_t*)test_GBA,strlen(test_GBA),(uint8_t*)test_key2,strlen(test_key2));
	hmac256((char*)digest, test_key2, strlen(test_key2), message, strlen(message));
	printf("Key  : '%s'\n",(char*)test_key2);
	printf("Input: '%s'\n",(char*)message);
	printf("HMAC : ");
	for(x=0;x<32;x++) printf("%02X",digest[x]);
	printf("\n");
	
	//_crypto_test_hmac(slot,digest,(uint8_t*)test_GBA,strlen(test_GBA),(uint8_t*)test_key2,strlen(test_key2));
	hmac256((char*)digest, test_key2, strlen(test_key2), test_GBA, strlen(test_GBA));
	printf("Key  : '%s'\n",(char*)test_key2);
	printf("Input: '%s'\n",(char*)test_GBA);
	printf("HMAC : ");
	for(x=0;x<32;x++) printf("%02X",digest[x]);
	printf("\n");
	
}

void crypt_sign_buffer_with_secretkey(char * digest, char * buffer, int buffer_length)
{
	char * signKey;
	#if 1
	// Chip based
	crypto_hmac256((uint8_t*)digest, SECRET_KEY_DATA_SLOT, (uint8_t*)buffer, buffer_length);
	#else
	// Software based
	signKey = get_secret_key();
	hmac256(digest, (char*)signKey, (int)strlen(signKey), buffer, buffer_length );
	#endif
	
	return;
}

void sha256(char *shasum, char *buf, int size)
{
	//printf("Crypto chip: %s\n",(crypto_is_attached()?"FOUND":"FAILED") );
	
	#ifdef SHA256_CRYPTO_CHIP
	OUT("*** ECC608A SHA256\n");
	crypto_sha256((uint8_t*)shasum,(uint8_t*)buf,size);
	#else
	SHA256_CTX ctx;
	OUT("*** Software SHA256\n");
	sha256lib_init(&ctx);
	sha256lib_update(&ctx,(BYTE*)buf,size);
	sha256lib_final(&ctx,(BYTE*)shasum);
	#endif
	
	return;
}
void sha256_init()
{
	sha256lib_init(&m_module_ctx_sha256);
}
void sha256_chunk(const char *buf, int size)
{
	OUT("BUF: '%s'\n",buf);
	sha256lib_update(&m_module_ctx_sha256,(BYTE*)buf,size);
}
void sha256_fin(char *shasum)
{
	sha256lib_final(&m_module_ctx_sha256,(uint8_t*)shasum);
}

void hmac256(char *hmacdig, const char *key, int key_size, const char *buf, int buf_size)
{
	hmac256_init(key, key_size);
	hmac256_chunk(buf,buf_size);
	hmac256_fin(hmacdig);
}

void hmac256_init(const char *key, int key_size)
{
	OUT("++++ New HMAC +++++\n");
	pbuf("- Key",(uint8_t*)key,key_size);
	OUT("KEY: '%s'\n",key);
	
	// Get the keys
	memset(m_keyinner,0,sizeof(m_keyinner));
	memset(m_keyoutter,0,sizeof(m_keyoutter));
	if ( key_size > HMAC_BLOCK_SIZE ) {
		// if key is longer than 64 bytes make it key=sha256(key)
		OUT("Key too long, generating hash\n");
		sha256((char*)m_keyinner, (char*)key, key_size);
		memcpy(m_keyoutter,m_keyinner,sizeof(m_keyoutter));
		key_size = sizeof(m_keyinner);
	}else{
		// Else just zero padded
		memcpy(m_keyinner,key,key_size);
		memcpy(m_keyoutter,key,key_size);
	}
	pbuf("- m_keyinner ",(uint8_t*)m_keyinner,sizeof(m_keyinner));
	pbuf("- m_keyoutter",(uint8_t*)m_keyoutter,sizeof(m_keyoutter));
	
	// XOR keys with ipad and opad values
	_xor_with_buffer(m_keyinner,sizeof(m_keyinner),HMAC_INNER_PAD);
	_xor_with_buffer(m_keyoutter,sizeof(m_keyoutter),HMAC_OUTTER_PAD);
	pbuf("- m_keyinner XORd",(uint8_t*)m_keyinner,sizeof(m_keyinner));
	pbuf("- m_keyoutter XORd",(uint8_t*)m_keyoutter,sizeof(m_keyoutter));
	
	sha256lib_init(&m_module_ctx_hmac);

	// HMAC is key + data to follow
	sha256lib_update(&m_module_ctx_hmac,(BYTE*)m_keyinner,sizeof(m_keyinner));
	
	return;
}
void hmac256_chunk(const char *buf, int buf_size)
{
	OUT("BUF: '%s'\n",buf);
	pbuf("- Inbuf",(uint8_t*)buf,buf_size);
	sha256lib_update(&m_module_ctx_hmac,(BYTE*)buf,buf_size);
}
void hmac256_fin(char *hmacdig)
{
	sha256lib_final(&m_module_ctx_hmac,(uint8_t*)m_tmp_shadigest);
	pbuf("- m_tmp_shadigest ",(uint8_t*)m_tmp_shadigest,sizeof(m_tmp_shadigest));


	// Perform outer SHA256
	sha256lib_init(&m_module_ctx_hmac);
	sha256lib_update(&m_module_ctx_hmac,(BYTE*)m_keyoutter,sizeof(m_keyoutter));
	sha256lib_update(&m_module_ctx_hmac,(BYTE*)m_tmp_shadigest,sizeof(m_tmp_shadigest));
	sha256lib_final(&m_module_ctx_hmac,(uint8_t*)m_tmp_shadigest);
	pbuf("- m_tmp_shadigest ",(uint8_t*)m_tmp_shadigest,sizeof(m_tmp_shadigest));

	if(hmacdig)
		memcpy(hmacdig,m_tmp_shadigest,SHA256_BLOCK_SIZE);
		
	return;
}

// How to do HMAC: https://en.wikipedia.org/wiki/HMAC
static void _xor_with_buffer(uint8_t * buf, int len, uint8_t v)
{
	int x;
	for(x=0;x<len;x++)
	{
		buf[x] = (buf[x] ^ v);
	}
}
static void hmac_sha256(uint8_t * inbuf, int inbuf_size, uint8_t * key, int key_len, uint8_t *digest)
{
	SHA256_CTX ctx;
	
	// NOTE: Digest must be 32 bytes!!!!!
	
	pbuf("- Inbuf",inbuf,inbuf_size);
	pbuf("- Key",key,key_len);
	OUT("KEY: '%s'\n",key);
	
	// Get the keys
	memset(m_keyinner,0,sizeof(m_keyinner));
	memset(m_keyoutter,0,sizeof(m_keyoutter));
    if ( key_len > HMAC_BLOCK_SIZE ) {
		// if key is longer than 64 bytes reset it to key=sha256(key)
        sha256((char*)m_keyinner, (char*)key, key_len);
		memcpy(m_keyoutter,m_keyinner,sizeof(m_keyoutter));
        key_len = sizeof(m_keyinner);
    }else{
		// Else jsut zero padded
		memcpy(m_keyinner,key,key_len);
		memcpy(m_keyoutter,key,key_len);
	}
	pbuf("- m_keyinner ",m_keyinner,sizeof(m_keyinner));
	pbuf("- m_keyoutter",m_keyoutter,sizeof(m_keyoutter));

    /*
     * the HMAC_SHA256 transform looks like:
     *
     * SHA256(K XOR opad, SHA256(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */

    
    // XOR keys with ipad and opad values
	_xor_with_buffer(m_keyinner,sizeof(m_keyinner),HMAC_INNER_PAD);
	_xor_with_buffer(m_keyoutter,sizeof(m_keyoutter),HMAC_OUTTER_PAD);
	pbuf("- m_keyinner ",m_keyinner,sizeof(m_keyinner));
	pbuf("- m_keyoutter",m_keyoutter,sizeof(m_keyoutter));

    // Perform inner SHA256
	sha256lib_init(&ctx);
	sha256lib_update(&ctx,(BYTE*)m_keyinner,sizeof(m_keyinner));
	sha256lib_update(&ctx,(BYTE*)inbuf,inbuf_size);
	sha256lib_final(&ctx,(uint8_t*)m_tmp_shadigest);
	pbuf("- m_tmp_shadigest ",m_tmp_shadigest,sizeof(m_tmp_shadigest));
   

	// Perform outer SHA256
    sha256lib_init(&ctx);
    sha256lib_update(&ctx,(BYTE*)m_keyoutter,sizeof(m_keyoutter));
    sha256lib_update(&ctx,(BYTE*)m_tmp_shadigest,sizeof(m_tmp_shadigest));
    sha256lib_final(&ctx,(uint8_t*)m_tmp_shadigest);
	pbuf("- m_tmp_shadigest ",m_tmp_shadigest,sizeof(m_tmp_shadigest));
	
	if(digest)
		memcpy(digest,m_tmp_shadigest,SHA256_BLOCK_SIZE);

	return;
}


// EOF

