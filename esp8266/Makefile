default: all

APP_NAME = arrow

# DBG=-g3
LIBDIR		= build
SDKLIB		= $(LIBDIR)/sdklib
GCC_BIN		=
GCC_PREFIX	= xtensa-lx106-elf
CJSON	= yes
WOLFSSL	= yes
PLATFORM = esp8266

SDK_PATH = $(abspath ../acn-sdk-c/)
SDK_IMPL = $(abspath $(PWD)/acnsdkc)
ESP_OPEN_RTOS_PATH = $(abspath ../../esp-open-rtos/)

OPT = -Os

COMMON_FLAGS := $(DBG) \
  -Wall \
  -Wl,-EL \
  -nostdlib  \
  -mlongcalls \
  -mtext-section-literals \
  -ffunction-sections \
  -fdata-sections
# -Wextra -Wundef -Werror

CC_FLAGS = $(COMMON_FLAGS) \
	-std=gnu99
	
XX_FLAGS = $(COMMON_FLAGS) \
	-std=c++0x -fno-exceptions -fno-rtti

CC_SYMBOLS := \
 -DGITSHORTREV="7cd1afc" \
 -DDEBUG \
 -D_USE_LONG_TIME_T \
 -DNO_SOFTWARE_UPDATE \
 -DNO_RELEASE_UPDATE \
 -D__AMICA__
# -DHTTP_SOCK_KEEP_OPEN


LIBMAIN_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/open_esplibs/libmain/*.c)
LIBMAIN_SRCA	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/open_esplibs/libmain/*.S)
LIBMAIN_OBJS	:= $(LIBMAIN_SRC:%.c=%.o)
LIBMAIN_OBJS	+= $(LIBMAIN_SRCA:%.S=%.o)
LIBMAIN_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,${LIBMAIN_OBJS})
LIBMAIN_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBMAIN_SRC:%.c=%.d))

LIBNET80211_SRC		+= $(wildcard $(ESP_OPEN_RTOS_PATH)/open_esplibs/libnet80211/*.c)
LIBNET80211_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBNET80211_SRC:%.c=%.o))
LIBNET80211_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBNET80211_SRC:%.c=%.d))

LIBPHY_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/open_esplibs/libphy/*.c)
LIBPHY_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBPHY_SRC:%.c=%.o))
LIBPHY_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBPHY_SRC:%.c=%.d))

LIBPP_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/open_esplibs/libpp/*.c)
LIBPP_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBPP_SRC:%.c=%.o))
LIBPP_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBPP_SRC:%.c=%.d))

LIBWPA_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/open_esplibs/libwpa/*.c)
LIBWPA_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBWPA_SRC:%.c=%.o))
LIBWPA_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBWPA_SRC:%.c=%.d))

LIBONEWIRE_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/extras/onewire/*.c)
LIBONEWIRE_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBONEWIRE_SRC:%.c=%.o))
LIBONEWIRE_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBONEWIRE_SRC:%.c=%.d))

LIBDS18B20_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/extras/ds18b20/*.c)
LIBDS18B20_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBDS18B20_SRC:%.c=%.o))
LIBDS18B20_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBDS18B20_SRC:%.c=%.d))

LIBFREERTOS_SRC		+= $(wildcard $(ESP_OPEN_RTOS_PATH)/FreeRTOS/Source/*.c)
LIBFREERTOS_SRC		+= $(wildcard $(ESP_OPEN_RTOS_PATH)/FreeRTOS/Source/portable/esp8266/*.c)
LIBFREERTOS_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBFREERTOS_SRC:%.c=%.o))
LIBFREERTOS_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBFREERTOS_SRC:%.c=%.d))

LIBLWIP_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/lwip/*.c)
LIBLWIP_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/lwip/lwip/src/**/*.c)
LIBLWIP_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/lwip/lwip/src/core/ipv4/*.c)
LIBLWIP_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBLWIP_SRC:%.c=%.o))
LIBLWIP_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBLWIP_SRC:%.c=%.d))

LIBCORE_SRC	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/core/*.c)
LIBCORE_SRCA	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/core/*.S)
LIBCORE_SRCCPP	+= $(wildcard $(ESP_OPEN_RTOS_PATH)/core/*.cpp)
LIBCORE_OBJS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBCORE_SRC:%.c=%.o) $(LIBCORE_SRCA:%.S=%.o) $(LIBCORE_SRCCPP:%.cpp=%.o))
LIBCORE_DEPS	:= $(patsubst $(ESP_OPEN_RTOS_PATH)%,$(abspath ${LIBDIR})%,$(LIBCORE_SRC:%.c=%.d))


RTOS_INCLUDES += \
  -I$(ESP_OPEN_RTOS_PATH)/include \
  -I$(ESP_OPEN_RTOS_PATH)/FreeRTOS/Source/include  \
  -I$(ESP_OPEN_RTOS_PATH)/FreeRTOS/Source/portable/esp8266 \
  -I$(ESP_OPEN_RTOS_PATH)/lwip/lwip/src/include \
  -I$(ESP_OPEN_RTOS_PATH)/lwip/include \
  -I$(ESP_OPEN_RTOS_PATH)/lwip/lwip/src/include/compat/posix \
  -I$(ESP_OPEN_RTOS_PATH)/lwip/lwip/src/include/ipv4 \
  -I$(ESP_OPEN_RTOS_PATH)/lwip/lwip/src/include/ipv4/lwip \
  -I$(ESP_OPEN_RTOS_PATH)/lwip/lwip/src/include/lwip \
  -I$(ESP_OPEN_RTOS_PATH)/core/include \
  -I$(ESP_OPEN_RTOS_PATH)/open_esplibs/include \
  -I$(ESP_OPEN_RTOS_PATH)/libc/xtensa-lx106-elf/include \
  -I$(ESP_OPEN_RTOS_PATH)/extras
  
SDK_INCLUDES += ${RTOS_INCLUDES}

LIBDIR = $(PWD)/build
DEMO_OUT = $(LIBDIR)/${APP_NAME}.out
DEMO_LIB = $(LIBDIR)/lib${APP_NAME}.a

-include $(SDK_PATH)/Makefile.sdk

INCLUDES += $(SDK_INCLUDES) \
  -I$(PWD) \
  -I$(PWD)/include

CFLAGS = $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES)

CSRCS := arrow.c
CSRCS += $(wildcard arrow_*.c)
CSRCS += $(shell find $(PWD)/acnsdkc -name "*.c")
CSRCS += ${RTOS_SOURCE}

OBJS	:= $(CSRCS:%.c=%.o)
OBJ     := $(patsubst ${SDK_PATH}%,${LIBDIR}/acn-sdk-c%,${OBJ})
DEPS	:= $(CSRCS:%.c=%.d)
DEPS    := $(patsubst ${SDK_PATH}%,${LIBDIR}/acn-sdk-c%,${DEPS})

DEPS	+= $(LIBMAIN_DEPS)

LIBS := \
	-l$(APP_NAME)  \
	-lacnsdkc \
	-lonewire \
	-lds18b20 \
	-lfreertos \
	-llwip \
	-lcore \
	-lgcc \
	-lc \
	-lhal \
	-lmain \
	-lnet80211 \
	-lphy \
	-lpp \
	-lwpa

LDFLAGS = -nostdlib -Wl,--no-check-sections \
	-L$(LIBDIR) \
	-L$(LIBDIR)/sdklib \
	-L$(ESP_OPEN_RTOS_PATH)/lib \
	-L$(ESP_OPEN_RTOS_PATH)/libc/xtensa-lx106-elf/lib \
	-T$(ESP_OPEN_RTOS_PATH)/ld/program.ld  \
	-T$(ESP_OPEN_RTOS_PATH)/ld/rom.ld \
	-u call_user_start -u _printf_float -u _scanf_float \
	-Wl,-static \
	-Wl,-Map=$(LIBDIR)/arrow.map \
	-Wl,-gc-sections \
	-Wl,--whole-archive \
	$(LIBDIR)/open_esplibs_libmain.a \
	$(LIBDIR)/open_esplibs_libnet80211.a \
	$(LIBDIR)/open_esplibs_libphy.a \
	$(LIBDIR)/open_esplibs_libpp.a \
	$(LIBDIR)/open_esplibs_libwpa.a \
	-Wl,--no-whole-archive \
	-Wl,--start-group \
	$(LIBS) \
	-Wl,--end-group


ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),mkdbg)
ifdef DEPS
sinclude $(DEPS)
endif
endif
endif


# -- libmain
$(LIBDIR)/%.o : $(ESP_OPEN_RTOS_PATH)/%.c
	$(call dir_make,$@) $(QUIET)
	$(CC) $(OPT) $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@
	
$(LIBDIR)/%.o : $(ESP_OPEN_RTOS_PATH)/%.S
	$(call dir_make,$@) $(QUIET)
	$(CC) $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@
	
$(LIBDIR)/%.o : $(ESP_OPEN_RTOS_PATH)/%.cpp
	$(call dir_make,$@) $(QUIET)
	$(CC) $(XX_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@

$(LIBDIR)/open_esplibs_libmain.a: $(LIBMAIN_OBJS)
	$(AR) cru $@ $^

# -- libnet80211	
$(LIBDIR)/open_esplibs_libnet80211.a : $(LIBNET80211_OBJS)
	$(AR) cru $@ $^
	
# -- libphy	
$(LIBDIR)/open_esplibs_libphy.a : $(LIBPHY_OBJS)
	$(AR) cru $@ $^

# -- libpp
$(LIBDIR)/open_esplibs_libpp.a : $(LIBPP_OBJS)
	$(AR) cru $@ $^

# -- libwpa
$(LIBDIR)/open_esplibs_libwpa.a : $(LIBWPA_OBJS)
	$(AR) cru $@ $^

# -- libonewire	
$(LIBDIR)/libonewire.a : $(LIBONEWIRE_OBJS)
	$(AR) cru $@ $^

# -- libds18b20
	
$(LIBDIR)/libds18b20.a : $(LIBDS18B20_OBJS)
	$(AR) cru $@ $^
	
# -- LIBFREERTOS	
$(LIBDIR)/libfreertos.a : $(LIBFREERTOS_OBJS)
	$(AR) cru $@ $^
	
$(LIBDIR)/liblwip.a : $(LIBLWIP_OBJS)
	$(AR) cru $@ $^
	
$(LIBDIR)/libcore.a : $(LIBCORE_OBJS)
	$(AR) cru $@ $^
	
	
$(DEMO_LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	$(AR) cru $@ $^

$(DEMO_OUT): $(LIBDIR)/open_esplibs_libmain.a \
	$(LIBDIR)/open_esplibs_libnet80211.a \
	$(LIBDIR)/open_esplibs_libphy.a \
	$(LIBDIR)/open_esplibs_libpp.a \
	$(LIBDIR)/open_esplibs_libwpa.a \
	$(LIBDIR)/libonewire.a \
	$(LIBDIR)/libds18b20.a \
	$(LIBDIR)/libfreertos.a \
	$(LIBDIR)/liblwip.a \
	$(LIBDIR)/libcore.a \
	$(LIBDIR)/libgcc.a \
	$(SDKLIB)/libmain.a \
	$(SDKLIB)/libphy.a \
	$(SDKLIB)/libnet80211.a \
	$(SDKLIB)/libpp.a \
	$(SDKLIB)/libwpa.a \
	$(SDK_TARGET)
	@mkdir -p $(IMAGEDIR) $(QUIET)
	$(CC) $(LDFLAGS) -o $@ 
	
firmware: $(DEMO_OUT)
	@echo make firmware
	@mkdir -p firmware
	esptool.py elf2image --version=2 -fs 16m -fm qio -ff 40m $(DEMO_OUT) -o ./firmware/$(APP_NAME).bin
	
flash:
	esptool.py -p /dev/ttyUSB0 --baud 115200 write_flash -fs 16m -fm qio -ff 40m \
	0x0 $(ESP_OPEN_RTOS_PATH)/bootloader/firmware_prebuilt/rboot.bin 0x1000 $(ESP_OPEN_RTOS_PATH)/bootloader/firmware_prebuilt/blank_config.bin 0x2000 ./firmware/$(APP_NAME).bin

all: $(SDK_TARGET) $(OBJS) $(DEMO_LIB) $(DEMO_OUT) firmware

clean: sdk_clean
	$(foreach d, $(SUBDIRS), make -C $(d) clean;)
	$(RM) $(OBJS)
	$(RM) $(DEMO_LIB)
	$(RM) $(DEMO_OUT)
	$(RM) -r $(LIBDIR)
	$(RM) -r firmware
	
# Remove comment lines from libgcc.remove file
$(LIBDIR)/libgcc.remove: $(ESP_OPEN_RTOS_PATH)/lib/libgcc.remove | $(LIBDIR)
	@ grep -v "^#" $< | cat > $@

# Remove unwanted object files listed in libgcc.remove
$(LIBDIR)/libgcc.a: $(ESP_OPEN_RTOS_PATH)/lib/libgcc.a $(LIBDIR)/libgcc.remove | $(LIBDIR)
	@echo "Removing unwanted objects from $<"
	cat $< > $@
	$(AR) d $@ @$(word 2,$^)
	
# Remove comment lines from libc.remove file
$(LIBDIR)/libc.remove: $(ESP_OPEN_RTOS_PATH)/libc/libc.remove | $(LIBDIR)
	grep -v "^#" $< | cat > $@

# Remove unwanted object files listed in libgcc.remove
$(LIBDIR)/libc.a: $(ESP_OPEN_RTOS_PATH)/libc/xtensa-lx106-elf/lib/libc.a $(LIBDIR)/libc.remove | $(LIBDIR)
	@echo "Removing unwanted objects from $<"
	cat $< > $@
	$(AR) d $@ @$(word 2,$^)
	
$(SDKLIB):
	@ mkdir -p $(SDKLIB)

# Remove comment lines from <libname>.remove files
$(SDKLIB)/%.remove: $(ESP_OPEN_RTOS_PATH)/lib/%.remove | $(SDKLIB)
	$(call dir_make,$@) $(QUIET)
	grep -v "^#" $< | cat > $@
	
# Stage 1: remove unwanted object files listed in <libname>.remove alongside each library
$(SDKLIB)/%_stage1.a: $(ESP_OPEN_RTOS_PATH)/lib/%.a $(SDKLIB)/%.remove | $(SDKLIB)
	@echo "SDK processing stage 1: Removing unwanted objects from $<"
	cat $< > $@
	$(AR) d $@ @$(word 2,$^)

# Stage 2: Redefine all SDK symbols as sdk_, weaken all symbols.
$(SDKLIB)/%.a: $(SDKLIB)/%_stage1.a $(ESP_OPEN_RTOS_PATH)/lib/allsymbols.rename
	$(call dir_make,$@) $(QUIET)
	echo "SDK processing stage 2: Renaming symbols in SDK library $< -> $@"
	$(OBJCOPY) --redefine-syms $(word 2,$^) --weaken $< $@
