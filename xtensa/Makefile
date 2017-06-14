# Copyright (c) 2015 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
# $ATH_LICENSE_TARGET_MK$
#
default: all

#.EXPORT_ALL_VARIABLES:

APP_NAME = iot_arrow

IMAGEDIR := $(SRC_IOE)/image
DEMO_OUT = $(IMAGEDIR)/${APP_NAME}.out
DEMO_LIB = $(LIBDIR)/lib${APP_NAME}.a

C_SDK_PATH = $(PWD)/../acn-sdk-c
C_SDK_MAKE_PATH = $(C_SDK_PATH)/platforms/xtensa

-include $(C_SDK_MAKE_PATH)/Makefile.xtensa.def

INCLUDES += \
	-I$(C_SDK_PATH) \
	-I$(C_SDK_PATH)/include \
	-I$(C_SDK_MAKE_PATH) \
	-I$(C_SDK_MAKE_PATH)/../default


CFLAGS = $(COPTS) $(DEFINES) $(INCLUDES) 
# -Wl,--wrap,console_setup -Wl,--wrap,_time

CC = xt-xcc
AR = xt-ar
RM = rm -f

CSRCS := app_start.c main.c swat_wmiconfig_ota.c
CSRCS += $(wildcard arrow_*.c)
CSRCS += $(shell find $(PWD)/acnsdkc -name "*.c")

OBJS := $(CSRCS:%.c=%.o)
DEPS := $(CSRCS:%.c=%.d)
LIBS := \
	$(LIBDIR)/libhost_dset_api.a \
    	$(LIBDIR)/libhttppatches.a \
    	$(LIBDIR)/libsntppatches.a \
    	$(LIBDIR)/libdnspatches.a \
    	$(LIBDIR)/libotapatches.a \
    	$(LIBDIR)/libstrrclpatches.a \
    	$(LIBDIR)/libipv6patches.a \
    	$(LIBDIR)/libsslpatches.a \
        $(LIBDIR)/libcryptopatches.a \
        $(LIBDIR)/libcrypto_mgmt.a \
        $(LIBDIR)/libcrypto_aes.a \
        $(LIBDIR)/libcrypto_chacha20_poly1305.a \
        $(LIBDIR)/libcrypto_des.a \
        $(LIBDIR)/libcrypto_srp.a \
        $(LIBDIR)/libcrypto_hmacsha384.a \
        $(LIBDIR)/libcrypto_hmacsha256.a \
        $(LIBDIR)/libcrypto_hmacsha1.a \
        $(LIBDIR)/libwhalpatches.a \
        $(LIBDIR)/lib_ezxml.a \
        $(LIBDIR)/libssdp.a 

LDFLAGS = -nostdlib -Wl,-EL -Wl,--gc-sections \
	-L$(IMAGEDIR)  \
	-L. \
	-L$(LIBDIR) \
	-Tsdk_shell.ld \
	-u app_start \
	-Wl,-static \
	-Wl,--start-group \
	-lgcc -ladc -luartserp -lconsole -lramcust -lserport -lacnsdkc -lqcomcrypto -ljson \
	-l${APP_NAME} \
	-lpatches_iot \
	-lpatches \
	$(LIBS)	\
	-Wl,--end-group \
	$(IMAGEDIR)/rom.addrs.ld \
	-Wl,-Map,$(APP_NAME).map


ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),mkdbg)
ifdef DEPS
sinclude $(DEPS)
endif
endif
endif

$(DEMO_LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	$(AR) ru $@ $^

libacnsdkc.a:
	cd $(C_SDK_PATH) && make -f $(C_SDK_MAKE_PATH)/Makefile.xtensa

$(DEMO_OUT): libacnsdkc.a $(LIBS) $(LDS)
	@mkdir -p $(IMAGEDIR)
	$(CC) $(LDFLAGS) -o $@ 
	
firmware:
	cp $(PWD)/config/index.html $(SRC_IOE)/demo/sdk_flash/
	cd $(SRC_IOE)/tool && ./qonstruct.sh --qons ./tmp/

all: libacnsdkc.a $(OBJS) $(DEMO_LIB) $(DEMO_OUT) firmware

clean:
	$(foreach d, $(SUBDIRS), make -C $(d) clean;)
	cd $(C_SDK_PATH) && make -C $(C_SDK_PATH) -f $(C_SDK_MAKE_PATH)/Makefile.xtensa clean
	$(RM) $(OBJS)
	$(RM) $(DEMO_LIB)
	$(RM) $(DEMO_OUT)


mkdbg:
	@echo pwd=`pwd`
	@echo TARGET = $(TARGET)
	@echo AR6002_REV = $(AR6002_REV)
	@echo ROMDIR = $(ROMDIR)
	@echo DEMO_OUT = $(DEMO_OUT)
	@echo DEMO_LIB = $(DEMO_LIB)
	@echo SUBDIRS = $(SUBDIRS)
	@echo CSRCS = $(CSRCS)
	@echo OBJS = $(OBJS)
	@echo DEPS = $(DEPS)
	@echo LIBS = $(LIBS)
	@echo CC = $(CC) at `which $(CC)`
	@echo CFLAGS = $(CFLAGS)
	@echo LDFLAGS = $(LDFLAGS)
