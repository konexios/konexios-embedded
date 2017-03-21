/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_system.h"
#include "base.h"
#include "targaddrs.h"

extern void user_main(void);

void app_start(void) {
        QCOM_SYS_START_PARA_t sysStartPara;

        sysStartPara.isMccEnabled = 0;
        sysStartPara.numOfVdev = 1;

#if defined(FPGA)
    #if defined(ENABLED_MCC_MODE)
    sysStartPara.isMccEnabled = ENABLED_MCC_MODE;
    #endif

    #if defined(NUM_OF_VDEVS)
    sysStartPara.numOfVdev = NUM_OF_VDEVS;
    #endif
#else
    /*
    * For asic version, the MCC and device number are set in sdk_proxy.
    */
    if (HOST_INTEREST->hi_option_flag2 & HI_OPTION_MCC_ENABLE) {
        sysStartPara.isMccEnabled = 1;
    }

    sysStartPara.numOfVdev = ((HOST_INTEREST->hi_option_flag >> HI_OPTION_NUM_DEV_SHIFT) & HI_OPTION_NUM_DEV_MASK);
#endif

    qcom_sys_start(user_main, &sysStartPara);
}
