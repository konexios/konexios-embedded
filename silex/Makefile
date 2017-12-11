default: all

APP_NAME = iot_arrow

# DBG=-g3
CC_NAME = xcc
GCC_BIN =
GCC_PREFIX = xt
WOLFSSL = yes
CJSON   = yes
PLATFORM = silex
SDK_PATH = $(abspath ../acn-sdk-c/)
SDK_IMPL = $(PWD)/acnsdkc

XTENSA_LIBDIR := $(LIBDIR)
IMAGEDIR := $(SRC_IOE)/image
DEMO_OUT := $(IMAGEDIR)/${APP_NAME}.out
DEMO_LIB := $(LIBDIR)/lib${APP_NAME}.a

LIBDIR = build

COMMON_FLAGS := $(DBG) \
  -Wall \
  -Wextra \
  -Wpointer-arith \
  -Wundef \
  -Werror \
  -Wl,-EL \
  -fno-inline-functions \
  -nostdlib \
  -mlongcalls \
  -ffunction-sections
  
OPT := -Os

CC_FLAGS = $(COMMON_FLAGS)
CC_SYMBOLS := \
 -DAR6K \
 -DAR6002 \
 -TARG:unaligned_loads=1 \
 -DIOT_BUILD_FLAG \
 -DAR6002_REV7 \
 -include $(INTERNALDIR)/include/fwconfig_AR6006.h \
 -Wno-return-type \
 -DATHOS \
 -DATH_TARGET \
 -DHTTP_ENABLED \
 -DNO_SOFTWARE_UPDATE \
 -DARCH_MEM -DARCH_TYPE
 
ifeq ($(WOLFSSL), yes) # {
CC_SYMBOLS += -DNO_QCOM_SSL
endif # }

# -DNO_SOFTWARE_UPDATE \
# -DHTTP_SOCK_KEEP_OPEN

ifeq ($(AR6002_REV7_VER), 5) # {
CC_SYMBOLS += -DNUM_OF_VDEVS=2 \
  -DAR6002_REV75
endif # }

ifeq ($(AR6002_REV7_VER), 6) # {
CC_SYMBOLS += -DNUM_OF_VDEVS=2 \
  -DAR6002_REV76
endif # }

ifeq ($(FPGA_FLAG),1) # {
CC_SYMBOLS += -DFPGA
endif # }

SDK_INCLUDES += -I$(FW)/include -I$(FW)/include/qcom

INCLUDES += $(SDK_INCLUDES) \
 -I$(FW)/include/AR6002/hw/include \
 -I$(PWD) \
 -I$(FW)/include/qcom \
 -I$(FW)/include \
 -I$(FW)/include/AR6002/ \
 -I$(FW)/include/AR6K/ \
 -I$(THREADXDIR)/include/ \
 -I$(INTERNALDIR)/include \
 -I$(INTERNALDIR)/include/os \
 -I$(INTERNALDIR)/include/whal \
 -I$(FW)/demo/ezxml

-include $(SDK_PATH)/Makefile.sdk


CFLAGS = $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES)
# -Wl,--wrap,console_setup -Wl,--wrap,_time

CSRCS := $(PWD)/app_start.c \
	$(PWD)/main.c \
	$(PWD)/swat_wmiconfig_ota.c \
	$(PWD)/uart2.c at.c
CSRCS += $(wildcard $(PWD)/arrow_*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/**/*.c)

OBJS := $(patsubst $(PWD)%,${LIBDIR}%,$(CSRCS:%.c=%.o))
DEPS := $(patsubst $(PWD)%,${LIBDIR}%,$(CSRCS:%.c=%.d))
LIBS := \
	-lm \
	-lgcc \
	-ladc \
	-luartserp \
	-lramcust \
	-lserport \
	-lpatches_iot \
	-lpatches \
	-lhost_dset_api \
	-lhttppatches \
	-lsntppatches \
	-ldnspatches \
	-lotapatches \
	-lcustpatches \
	-lstrrclpatches \
	-lipv6patches \
	-lsslpatches \
	-lcryptopatches \
	-lcrypto_mgmt \
	-lcrypto_aes \
	-lcrypto_chacha20_poly1305 \
	-lcrypto_des \
	-lcrypto_srp \
	-lcrypto_hmacsha384 \
	-lcrypto_hmacsha256 \
	-lcrypto_hmacsha1 \
	-lwhalpatches \
	-lssdp \
	-lstackghost \
	-lqcomcrypto
# -lconsole
# -l_ezxml

LDFLAGS = -nostdlib -lgcc -Wl,-EL -Wl,--gc-sections \
	-L$(IMAGEDIR) -L$(LIBDIR) -L$(XTENSA_LIBDIR) \
	-Tarrow.ld \
	-u app_start \
	-Wl,-static \
	-Wl,--start-group \
	-l${APP_NAME} \
	-lacnsdkc \
	$(LIBS) \
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

$(LIBDIR)/%.o : $(PWD)/%.c
	echo $@
	$(call make_dir_for_file,$@) $(QUIET)
	$(CC) $(OPT) $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@

$(DEMO_LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	$(AR) ru $@ $^

$(DEMO_OUT): $(SDK_TARGET)
	@mkdir -p $(IMAGEDIR) $(QUIET)
	$(CC) $(LDFLAGS) -o $@ 
	
firmware:
	cp $(PWD)/config/index.html $(SRC_IOE)/demo/sdk_flash/
	cd $(SRC_IOE)/tool && ./qonstruct.sh --qons ./tmp/
	cp $(SRC_IOE)/bin/ota_image_AR401X_REV6_IOT_MP1_hostless_unidev_singleband_iot_arrow.bin ota_${VERSION}.bin
	cp $(SRC_IOE)/bin/raw_flashimage_AR401X_REV6_IOT_MP1_hostless_unidev_singleband_iot_arrow.bin raw_${VERSION}.bin

all: app_version check_settings $(SDK_TARGET) $(OBJS) $(DEMO_LIB) $(DEMO_OUT) firmware app_settings

clean: sdk_clean
	$(foreach d, $(SUBDIRS), make -C $(d) clean;)
	$(RM) $(OBJS)
	$(RM) $(DEMO_LIB)
	$(RM) $(DEMO_OUT)
	$(RM) -r $(LIBDIR)


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
