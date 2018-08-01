#ifndef __CRYPTO_H__
#define __CRYPTO_H__

void crypto_init(uint8_t addr);

uint8_t * crypto_get_serial();

void crypto_sha256(char *shasum, char *buf, int size);

void crypto_hmac256(char *hmacdig, const char *key, int key_size, const char *buf, int buf_size);

void crypto_set_address(uint8_t current_addr, uint8_t new_addr);

void crypto_dump_config();

void crypto_test();

#endif

// EOF
