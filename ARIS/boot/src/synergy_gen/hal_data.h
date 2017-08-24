/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_flash_hp.h"
#include "r_flash_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
/* Flash on Flash HP Instance */
extern const flash_instance_t g_flash0;
#ifdef NULL
#define FLASH_CALLBACK_USED (0)
#else
#define FLASH_CALLBACK_USED (1)
#endif
#if FLASH_CALLBACK_USED
void NULL(flash_callback_args_t * p_args);
#endif
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
/** ELC Instance */
extern const elc_instance_t g_elc;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
void hal_entry(void);
void g_hal_init(void);
#endif /* HAL_DATA_H_ */
