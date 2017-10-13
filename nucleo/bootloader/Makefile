default: all

PRJ_PATH = $(PWD)
GCC_BIN =
PROJECT = bootloader
LIBDIR	= .build
GCC_BIN =
GCC_PREFIX = arm-none-eabi
PLATFORM = nucleo

CPP := $(GCC_PREFIX)-g++
CC  := $(GCC_PREFIX)-gcc
LD  := $(GCC_PREFIX)-gcc
OBJCOPY = $(GCC_PREFIX)-objcopy
OBJDUMP = $(GCC_PREFIX)-objdump
SIZE    = $(GCC_PREFIX)-size
RMDIR = rm -rf
MAKEDIR = mkdir -p

FLOAT_ABI = hard
CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOAT_ABI)
COMMON_FLAGS = -c -Wall -Wextra \
               -Wno-unused-parameter \
               -Wno-missing-field-initializers \
               -fmessage-length=0 \
               -fno-exceptions \
               -fno-builtin \
               -ffunction-sections \
               -fdata-sections \
               -funsigned-char \
               -MMD -fno-delete-null-pointer-checks \
               -fomit-frame-pointer \
               $(CPU)

CC_FLAGS = $(COMMON_FLAGS) -Os -std=gnu99 -MP
CPPC_FLAGS = $(COMMON_FLAGS) -Os -std=gnu++98 -fno-rtti -Wvla -MP
ASM_FLAGS = -x assembler-with-cpp $(COMMON_FLAGS) -Os


CC_SYMBOLS = -DTOOLCHAIN_GCC \
       -DTOOLCHAIN_GCC_ARM \
       -DUSE_HAL_DRIVER \
       -DUSE_IOEXPANDER \
       -DSTM32F401xE \
       -DDEBUG


LD_FLAGS = -Wl,--gc-sections $(CPU)
LD_SYS_LIBS = -lm -lc -lgcc -lnosys

SRCS += $(wildcard $(PRJ_PATH)/Src/*.c)
SRCS += $(PWD)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
SRCS += $(PWD)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
SRCS += $(PWD)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
SRCS += $(PWD)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
SRCS += $(PWD)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
SRCS += $(PWD)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
SRCS += $(PWD)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c

ASRCS = $(PRJ_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xe.s

OBJECTS := $(SRCS:%.c=%.o)
OBJECTS += $(ASRCS:%.s=%.o)

INCLUDE_PATHS += -I$(PRJ_PATH)/Inc \
		  -I$(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc \
                 -I$(PWD)/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
                 -I$(PWD)/Drivers/CMSIS/Include \
                 -I$(PWD)/Drivers/STM32L4xx_HAL_Driver/Inc
		

LIBRARY_PATHS = -L$(PRJ_PATH)/$(LIBDIR)
LINKER_SCRIPT = $(PRJ_PATH)/STM32F401XE.ld

ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

.PHONY: all lst size

all: $(PROJECT).bin $(PROJECT).hex size

.asm.o:
	+@$(MAKEDIR) $(dir $@) $(QUIET)
	$(CC) $(CPU) -c $(ASM_FLAGS) -o $@ $<
.s.o:
	+@$(MAKEDIR) $(dir $@) $(QUIET)
	$(CC) $(CPU) -c $(ASM_FLAGS) -o $@ $<
.S.o:
	+@$(MAKEDIR) $(dir $@) $(QUIET)
	$(CC) $(CPU) -c $(ASM_FLAGS) -o $@ $<

.c.o:
	+@$(MAKEDIR) $(dir $@) $(QUIET)
	echo $(CC)  $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDE_PATHS) -o $@ $<
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	+@$(MAKEDIR) $(dir $@) $(QUIET)
	$(CPP) $(CPPC_FLAGS) $(CC_SYMBOLS) $(INCLUDE_PATHS) -o $@ $<


$(PROJECT).elf: $(SDK_TARGET) $(OBJECTS) $(SYS_OBJECTS) $(LINKER_SCRIPT)
	$(LD) $(LD_FLAGS) -T$(filter %.ld, $^) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group


$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

$(PROJECT).lst: $(PROJECT).elf
	@$(OBJDUMP) -Sdh $< > $@

lst: $(PROJECT).lst

size: $(PROJECT).elf
	$(SIZE) $(PROJECT).elf
	@cp $(PROJECT).bin raw.bin
	$(eval size = $(shell ls -la $(PROJECT).bin | awk '{print $$5}'))
	@echo $(PROJECT).bin size: ${size}
	$(eval pad = $(shell echo $$(( 0x4000 - $(size) )) ))
	@echo Padding: ${pad}
	@dd if=/dev/zero ibs=1 count=${pad} 2>/dev/null | tr "\000" "\377" >> raw.bin
	@echo write to raw.bin
	
flash: $(PROJECT).bin
	st-flash write $(PROJECT).bin 0x08000000


DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)

clean: 
	$(RM) $(OBJECTS) $(QUIET)
	$(RM) $(PROJECT)* $(QUIET)
	$(RM) $(DEPS) $(QUIET)
	$(RMDIR) $(LIBDIR) $(QUIET)
	
-include $(DEPS)

