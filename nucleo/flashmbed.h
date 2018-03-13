#ifndef MBED_CONFIG_H_
#define MBED_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <config.h>
#include <stddef.h>

int write_flash(char *flash, size_t size);
char *flash_start();

int geti();
void menu();

#ifdef __cplusplus
}
#endif

#endif
