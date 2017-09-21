default: all

APP_NAME = arrow
APP_OUT = ${APP_NAME}.elf

GCC_BIN = 
PROJECT = arrow
LIBDIR = .build
WOLFSSL = yes
CJSON = yes
GCC_BIN =
GCC_PREFIX = arm-none-eabi
PLATFORM = nucleo
SDK_PATH = $(abspath ../acn-sdk-c/)

ifeq ($(HARDFP),1)
	FLOAT_ABI = hard
else
	FLOAT_ABI = softfp
endif
CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOAT_ABI)
COMMON_FLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Werror -Wpedantic -Wno-variadic-macros
CC_FLAGS = $(COMMON_FLAGS) $(CPU) -Os -std=gnu11 -include mbed_config.h -MP
CPPC_FLAGS = $(COMMON_FLAGS) $(CPU) -Os -std=gnu++98 -fno-rtti -Wvla -include mbed_config.h -MP
ASM_FLAGS = -x assembler-with-cpp -c $(COMMON_FLAGS) $(CPU) -Os

CC_SYMBOLS = -D__MBED__=1 \
			 -DOPENSSL_EXTRA \
			 -DDEVICE_I2CSLAVE=1 \
			 -DTARGET_LIKE_MBED \
			 -DTARGET_NUCLEO_F401RE \
			 -DTARGET_RTOS_M4_M7 \
			 -DDEVICE_RTC=1 \
			 -DTOOLCHAIN_object \
			 -DTARGET_STM32F4 \
			 -D__CMSIS_RTOS \
			 -DTOOLCHAIN_GCC \
			 -DTARGET_CORTEX_M \
			 -DTARGET_LIKE_CORTEX_M4 \
			 -DTARGET_M4 \
			 -DTARGET_UVISOR_UNSUPPORTED \
			 -DDEVICE_PWMOUT=1 \
			 -DDEVICE_INTERRUPTIN=1 \
			 -DDEVICE_I2C=1 \
			 -DDEVICE_PORTOUT=1 \
			 -D__CORTEX_M4 \
			 -DDEVICE_STDIO_MESSAGES=1 \
			 -DTARGET_STM32F401RE \
			 -DTARGET_FF_MORPHO \
			 -D__FPU_PRESENT=1 \
			 -DTARGET_FF_ARDUINO \
			 -DDEVICE_PORTIN=1 \
			 -DTARGET_RELEASE \
			 -DTARGET_STM \
			 -DDEVICE_SERIAL_FC=1 \
			 -DDEVICE_PORTINOUT=1 \
			 -DMBED_BUILD_TIMESTAMP=1471326725.12 \
			 -D__MBED_CMSIS_RTOS_CM \
			 -DDEVICE_SLEEP=1 \
			 -DTOOLCHAIN_GCC_ARM \
			 -DDEVICE_SPI=1 \
			 -DDEVICE_ERROR_RED=1 \
			 -DDEVICE_SPISLAVE=1 \
			 -DDEVICE_ANALOGIN=1 \
			 -DDEVICE_SERIAL=1 \
			 -DARM_MATH_CM4 \
			 -DDEBUG

ifdef DEV_ENV
CC_SYMBOLS += -DDEV_ENV
endif

LD_FLAGS =-Wl,--gc-sections -Wl,--wrap,main $(CPU)
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

SDK_INCLUDES += -I$(PWD)/

-include $(SDK_PATH)/Makefile.sdk

CPPSRCS  = $(wildcard $(PWD)/*.cpp)
CPPSRCS += $(wildcard $(PWD)/DnsQuery/*.cpp)
CPPSRCS += $(shell find $(PWD)/X_NUCLEO_IDW01M1 -name "*.cpp")
CPPSRCS += $(shell find $(PWD)/Components -name "*.cpp")
CPPSRCS += $(shell find $(PWD)/NetworkSocketAPI -name "*.cpp")
CSRCS += $(shell find $(PWD)/X_NUCLEO_IDW01M1 -name "*.c")
# CSRCS += $(wildcard $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/ -name "*.c")
CSRCS += $(wildcard $(PWD)/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/arrow/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/bsd/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/time/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/json/*.c)

SSRCS += $(wildcard $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/*.S)
CSRCS += $(wildcard $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/*.c)

SOBJS := $(SSRCS:%.S=%.o)
COBJS := $(CSRCS:%.c=%.o)
CPPOBJS := $(CPPSRCS:%.cpp=%.o)
CPPOBJS += $(COBJS)
CPPOBJS += $(SOBJS)
CPPDEPS := $(CPPSRCS:%.cpp=%.d)

SYS_OBJECTS = $(wildcard $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/*.o)

INCLUDES += $(SDK_INCLUDES) \
            -I$(PWD) \
            -I$(PWD)/mbed \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4 \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4/TARGET_NUCLEO_F401RE \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM

LIBRARY_PATHS = -L$(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM -L$(PWD)/.build
LIBRARIES = -lmbed -lacnsdkc
LINKER_SCRIPT = $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/STM32F401XE.ld

# WIFI
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1/Spwf/utils
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1/Spwf_API \
            -I$(PWD)/X_NUCLEO_IDW01M1/Spwf/inc
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1/Spwf_API/utils

# NetworkSocketAPI
INCLUDES += -I$(PWD)/NetworkSocketAPI
 
# # IKS
INCLUDES += -I$(PWD)/Components \
            -I$(PWD)/Components/Common \
            -I$(PWD)/Components/Interfaces \
            -I$(PWD)/Components/hts221 \
            -I$(PWD)/X_NUCLEO_COMMON/DevI2C/ \
            -I$(PWD)/X_NUCLEO_COMMON/DevSPI \
            -I$(PWD)/X_NUCLEO_COMMON/DbgMCU \
            -I$(PWD)/TARGET_NUCLEO_F401RE

# DNS
INCLUDES += -I$(PWD)/DnsQuery

ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),mkdbg)
ifdef DEPS
sinclude $(DEPS)
endif
endif
endif

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@
.cpp.o:
	$(CPP) $(CPPC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@

libacnsdkc.a: $(SDK_TARGET)
	@echo BUILD: $(PWD)

$(APP_OUT): $(CPPOBJS) $(SYS_OBJECTS) $(LINKER_SCRIPT) libacnsdkc.a bootloader
	  @echo "build $(APP_OUT)"
	  $(LD) $(LD_FLAGS) -T$(filter %.ld, $^) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group
	
clean: sdk_clean
	cd bootloader && make clean
	$(RM) bootloader.bin
	$(RMDIR) $(LIBDIR)
	$(RM) $(CPPOBJS)
	
$(APP_OUT).bin: $(APP_OUT)
	$(OBJCOPY) -O binary $< $@

$(APP_OUT).lst: $(APP_OUT)
	@$(OBJDUMP) -Sdh $< > $@

lst: $(APP_OUT).lst

size: $(APP_OUT)
	$(SIZE) $(APP_OUT)

bootloader: bootloader.bin

bootloader.bin:
	cd bootloader && make clean && make && cp bootloader.bin ..
	
flash: $(APP_OUT)
	st-flash write bootloader.bin 0x8000000
	st-flash write $(APP_OUT).bin 0x8008000

all: libacnsdkc.a $(CPPOBJS) $(APP_OUT) $(APP_OUT).bin

