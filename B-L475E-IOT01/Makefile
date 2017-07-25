default: all
VPATH = .. 

PRJ_PATH = $(PWD)
GCC_BIN =
PROJECT = arrow
LIBDIR	= .build
WOLFSSL = yes
CJSON		= yes
GCC_BIN =
GCC_PREFIX = arm-none-eabi
PLATFORM = l475
SDK_PATH = $(abspath ../acn-sdk-c/)

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
       -DSTM32L475xx \
       -DUSE_HAL_DRIVER \
       -DUSE_STM32L475E_IOT01 \
       -DUSE_IOEXPANDER \
       -D__stm32l475iot__ \
       -DDEBUG
# -Werror

ifdef DEV_ENV
CC_SYMBOLS += -DDEV_ENV
endif

LD_FLAGS = -Wl,--gc-sections $(CPU)
LD_SYS_LIBS = -lm -lc -lgcc -lnosys

SRCS += $(PRJ_PATH)/Src/main.c
SRCS += $(PRJ_PATH)/Src/freertos.c
SRCS += $(PRJ_PATH)/Src/stm32l4xx_it.c
SRCS += $(PRJ_PATH)/Src/system_stm32l4xx.c
SRCS += $(PRJ_PATH)/Src/stm32l4xx_hal_msp.c
SRCS += $(PRJ_PATH)/Src/sensors_data.c
SRCS += $(PRJ_PATH)/Src/flash_l4.c
SRCS += $(wildcard $(PWD)/Src/vl*.c)
SRCS += $(wildcard $(PWD)/Src/usb*.c)
SRCS += $(wildcard $(PWD)/WiFi/Src/*.c)
SRCS += $(wildcard $(PWD)/Drivers/BSP/Components/es_wifi/*.c)
SRCS += $(wildcard $(PWD)/Drivers/BSP/Components/vl53l0x/*.c)
SRCS += $(wildcard $(PWD)/Drivers/BSP/Components/hts221/*.c)
SRCS += $(wildcard $(PWD)/Drivers/BSP/Components/lps22hb/*.c)
SRCS += $(wildcard $(PWD)/Drivers/BSP/Components/lsm6dsl/*.c)
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_comp.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_qspi.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_usart.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usb.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc_ex.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c
SRCS += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c
SRCS += $(wildcard $(PWD)/Drivers/BSP/B-L475E-IOT01/*.c)
SRCS += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
SRCS += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
SRCS += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
SRCS += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/list.c
SRCS += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/queue.c
SRCS += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
SRCS += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/timers.c
SRCS += $(wildcard $(PWD)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c)
SRCS += $(wildcard $(PWD)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/*.c)
SRCS += $(wildcard $(PWD)/Drivers/BSP/Components/lis3mdl/*.c)

SRCS += $(wildcard $(PWD)/acnsdkc/*.c)
SRCS += $(wildcard $(PWD)/acnsdkc/time/*.c)
SRCS += $(wildcard $(PWD)/acnsdkc/bsd/*.c)
SRCS += $(wildcard $(PWD)/acnsdkc/arrow/*.c)
SRCS += $(wildcard $(PWD)/acnsdkc/json/*.c)

ASRCS = Src/startup_stm32l475xx.s

OBJECTS := $(SRCS:%.c=%.o)
OBJECTS += $(ASRCS:%.s=%.o)

-include $(SDK_PATH)/Makefile.sdk

INCLUDE_PATHS = $(SDK_INCLUDES)
		
INCLUDE_PATHS += -I$(PRJ_PATH)/Inc \
                 -I$(PRJ_PATH)/WiFi/Inc \
                 -I$(PWD)/Drivers/CMSIS/Device/ST/STM32L4xx/Include \
                 -I$(PWD)/Drivers/CMSIS/Include \
                 -I$(PWD)/Drivers/STM32L4xx_HAL_Driver/Inc \
                 -I$(PWD)/Drivers/BSP/B-L475E-IOT01 \
                 -I$(PWD)/Drivers/BSP/Components/Common \
                 -I$(PWD)/Drivers/BSP/Components/es_wifi \
                 -I$(PWD)/Drivers/BSP/Components/vl53l0x \
                 -I$(PWD)/Drivers/BSP/Components/lis3mdl \
                 -I$(PWD)/Middlewares/Third_Party/FreeRTOS/Source/include \
                 -I$(PWD)/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
                 -I$(PWD)/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS \
                 -I$(PWD)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc \
                 -I$(PWD)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
		

LIBRARY_PATHS = -L$(PRJ_PATH)/$(LIBDIR)
LINKER_SCRIPT = $(PRJ_PATH)/STM32L475VG_FLASH.ld
LIBRARIES += -lacnsdkc 
LD_FLAGS += -Wl,--wrap,malloc,--wrap,realloc,--wrap,calloc,--wrap,free,--wrap,strdup,--wrap,strndup
# -Wl,--wrap,strdup
# -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r

ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

CC_FLAGS += -DUSE_LPS22HB_TEMP

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
	
flash: $(PROJECT).bin
	st-flash write $(PROJECT).bin 0x08000000


DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)

clean: sdk_clean
	$(RM) $(OBJECTS) $(QUIET)
	$(RM) $(PROJECT)* $(QUIET)
	$(RM) $(DEPS) $(QUIET)
	$(RMDIR) $(LIBDIR) $(QUIET)
	
-include $(DEPS)

