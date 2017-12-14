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
SDK_IMPL = $(PWD)/acnsdkc

ifeq ($(HARDFP),1)
	FLOAT_ABI = hard
else
	FLOAT_ABI = softfp
endif
CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOAT_ABI)
COMMON_FLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer \
#-Werror #-Wpedantic -Wno-variadic-macros
CC_FLAGS = -std=gnu99 $(COMMON_FLAGS) $(CPU) -Os -include mbed_config.h -MP
CPPC_FLAGS = -std=gnu++98 -fno-rtti -Wvla $(COMMON_FLAGS) $(CPU) -Os -include mbed_config.h -MP
ASM_FLAGS = -x assembler-with-cpp -Os $(COMMON_FLAGS) $(CPU) -Os

CC_SYMBOLS += -D__MBED__=1
CC_SYMBOLS += -DDEVICE_I2CSLAVE=1
CC_SYMBOLS += -DTARGET_LIKE_MBED
CC_SYMBOLS += -DDEVICE_PORTOUT=1
CC_SYMBOLS += -DUSBHOST_OTHER
CC_SYMBOLS += -DTARGET_NUCLEO_F401RE
CC_SYMBOLS += -DTARGET_RTOS_M4_M7
CC_SYMBOLS += -DDEVICE_RTC=1
CC_SYMBOLS += -DTOOLCHAIN_object
CC_SYMBOLS += -DDEVICE_SERIAL_ASYNCH=1
CC_SYMBOLS += -DTARGET_STM32F4
CC_SYMBOLS += -D__CMSIS_RTOS
CC_SYMBOLS += -D__FPU_PRESENT=1
CC_SYMBOLS += -DTOOLCHAIN_GCC
CC_SYMBOLS += -DDEVICE_I2C_ASYNCH=1
CC_SYMBOLS += -DTARGET_CORTEX_M
CC_SYMBOLS += -DTARGET_LIKE_CORTEX_M4
CC_SYMBOLS += -DMBED_BUILD_TIMESTAMP=1506679781.51
CC_SYMBOLS += -DTARGET_M4
CC_SYMBOLS += -DTARGET_UVISOR_UNSUPPORTED
CC_SYMBOLS += -DDEVICE_SPI_ASYNCH=1
CC_SYMBOLS += -DDEVICE_SERIAL=1
CC_SYMBOLS += -DDEVICE_INTERRUPTIN=1
CC_SYMBOLS += -DDEVICE_I2C=1
CC_SYMBOLS += -DTRANSACTION_QUEUE_SIZE_SPI=2
CC_SYMBOLS += -D__CORTEX_M4
CC_SYMBOLS += -DDEVICE_STDIO_MESSAGES=1
CC_SYMBOLS += -DTARGET_STM32F401RE
CC_SYMBOLS += -DTARGET_FF_MORPHO
CC_SYMBOLS += -DTARGET_FAMILY_STM32
CC_SYMBOLS += -DTARGET_FF_ARDUINO
CC_SYMBOLS += -DDEVICE_PORTIN=1
CC_SYMBOLS += -DTARGET_RELEASE
CC_SYMBOLS += -DTARGET_STM
CC_SYMBOLS += -DDEVICE_SERIAL_FC=1
CC_SYMBOLS += -DDEVICE_PORTINOUT=1
CC_SYMBOLS += -D__MBED_CMSIS_RTOS_CM
CC_SYMBOLS += -DDEVICE_SLEEP=1
CC_SYMBOLS += -DTOOLCHAIN_GCC_ARM
CC_SYMBOLS += -DDEVICE_SPI=1
CC_SYMBOLS += -DUSB_STM_HAL
CC_SYMBOLS += -DDEVICE_ERROR_RED=1
CC_SYMBOLS += -DDEVICE_SPISLAVE=1
CC_SYMBOLS += -DDEVICE_ANALOGIN=1
CC_SYMBOLS += -DDEVICE_PWMOUT=1
CC_SYMBOLS += -DARM_MATH_CM4
CC_SYMBOLS += -DTARGET_STM32F401xE
CC_SYMBOLS += -DDEBUG
CC_SYMBOLS += -DARCH_SSL

LD_FLAGS =-Wl,--gc-sections -Wl,--wrap,main $(CPU)
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

SDK_INCLUDES += -I$(PWD)/

-include $(SDK_PATH)/Makefile.sdk

CPPSRCS  = $(wildcard $(PWD)/*.cpp)
CPPSRCS += $(wildcard $(PWD)/X_NUCLEO_IKS01A1/*.cpp)
CPPSRCS += $(shell find $(PWD)/X_NUCLEO_IKS01A1/Components -name "*.cpp")
CPPSRCS += $(shell find $(PWD)/X_NUCLEO_IDW01M1v2/ -name "*.cpp")
#CPPSRCS += $(wildcard $(PWD)/X_NUCLEO_IDW01M1v2/SPWF01SA/ATParser/*.cpp)
CPPSRCS += $(shell find $(PWD)/NetworkSocketAPI -name "*.cpp")
# CSRCS += $(wildcard $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/ -name "*.c")
CSRCS += $(wildcard $(PWD)/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/**/*.c)

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
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4/TARGET_STM32F401xE \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4/TARGET_STM32F401xE/device \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4/device \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4/TARGET_STM32F401xE/TARGET_NUCLEO_F401RE \
            -I$(PWD)/mbed/platform \
            -I$(PWD)/mbed/hal \
            -I$(PWD)/mbed/drivers

LIBRARY_PATHS = -L$(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM -L$(PWD)/.build
LIBRARIES = -lmbed -lacnsdkc
LINKER_SCRIPT = $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/STM32F401XE.ld

# WIFI
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1v2/
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1v2/SPWF01SA
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1v2/SPWF01SA/ATParser
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1v2/SPWF01SA/ATParser/BufferedSerial
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1v2/SPWF01SA/ATParser/BufferedSerial/Buffer

# NetworkSocketAPI
INCLUDES += -I$(PWD)/NetworkSocketAPI
 
# IKS
INCLUDES += -I$(PWD)/X_NUCLEO_IKS01A1/Components \
            -I$(PWD)/X_NUCLEO_IKS01A1/Components/Common \
            -I$(PWD)/X_NUCLEO_IKS01A1/Components/Interfaces \
            -I$(PWD)/X_NUCLEO_IKS01A1/Components/hts221 \
            -I$(PWD)/X_NUCLEO_IKS01A1/X_NUCLEO_COMMON/DevI2C/ \
            -I$(PWD)/X_NUCLEO_IKS01A1/X_NUCLEO_COMMON/DevSPI \
            -I$(PWD)/X_NUCLEO_IKS01A1/X_NUCLEO_COMMON/DbgMCU \
            -I$(PWD)/X_NUCLEO_IKS01A1 \
            -I$(PWD)/X_NUCLEO_IKS01A1/ST_INTERFACES/Sensors \
            -I$(PWD)/X_NUCLEO_IKS01A1/ST_INTERFACES/Common \
            -I$(PWD)/TARGET_NUCLEO_F401RE

# DNS
INCLUDES += -I$(PWD)/NetworkSocketAPI/DnsQuery

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
	
$(APP_OUT).bin: $(APP_OUT) app_settings
	$(OBJCOPY) -O binary $< $@
	cp bootloader/raw.bin ${LIBDIR}/raw_${VERSION}.bin
	cat $(APP_OUT).bin >> ${LIBDIR}/raw_${VERSION}.bin
	cp $(APP_OUT).bin ${LIBDIR}/ota_${VERSION}.bin

$(APP_OUT).lst: $(APP_OUT)
	@$(OBJDUMP) -Sdh $< > $@

lst: $(APP_OUT).lst

size: $(APP_OUT)
	$(SIZE) $(APP_OUT)

version: app_version
	
bootloader: bootloader.bin

bootloader.bin:
	cd bootloader && make clean && make && cp bootloader.bin ..
	
flash: version $(APP_OUT)
	st-flash write ${LIBDIR}/raw_${VERSION}.bin 0x8000000

all: check_settings version libacnsdkc.a $(CPPOBJS) $(APP_OUT) $(APP_OUT).bin

