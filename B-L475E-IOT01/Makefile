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

# common sources

ASRCS = Src/startup_stm32l475xx.s

C_SRC += $(PRJ_PATH)/Src/system_stm32l4xx.c
C_SRC += $(PRJ_PATH)/Src/stm32l4xx_hal_msp.c
C_SRC += $(PRJ_PATH)/Src/flash_l4.c

BSP_SRC += $(wildcard $(PWD)/Drivers/BSP/B-L475E-IOT01/*.c)

HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_comp.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_qspi.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_usart.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usb.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc_ex.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c
HAL_SRC += $(PWD)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c

USB_SRC += $(wildcard $(PWD)/Src/usb*.c)
USB_SRC += $(wildcard $(PWD)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c)
USB_SRC += $(wildcard $(PWD)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/*.c)

# app sources

SENSOR_SRC += $(wildcard $(PWD)/Drivers/BSP/Components/vl53l0x/*.c)
SENSOR_SRC += $(wildcard $(PWD)/Drivers/BSP/Components/hts221/*.c)
SENSOR_SRC += $(wildcard $(PWD)/Drivers/BSP/Components/lps22hb/*.c)
SENSOR_SRC += $(wildcard $(PWD)/Drivers/BSP/Components/lsm6dsl/*.c)
SENSOR_SRC += $(wildcard $(PWD)/Drivers/BSP/Components/lis3mdl/*.c)

FREERTOS_SRC += $(PRJ_PATH)/Src/freertos.c
FREERTOS_SRC += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
FREERTOS_SRC += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
FREERTOS_SRC += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
FREERTOS_SRC += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/list.c
FREERTOS_SRC += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/queue.c
FREERTOS_SRC += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
FREERTOS_SRC += $(PWD)/Middlewares/Third_Party/FreeRTOS/Source/timers.c

WIFI_SRC += $(wildcard $(PWD)/WiFi/Src/*.c)
WIFI_SRC += $(wildcard $(PWD)/Drivers/BSP/Components/es_wifi/*.c)

APP_SRC += $(PRJ_PATH)/Src/main.c
APP_SRC += $(PRJ_PATH)/Src/sensors_data.c
APP_SRC += $(wildcard $(PWD)/Src/vl*.c)
APP_SRC += $(PRJ_PATH)/Src/stm32l4xx_it.c

ACN_SDK_SRC += $(wildcard $(PWD)/acnsdkc/*.c)
ACN_SDK_SRC += $(wildcard $(PWD)/acnsdkc/time/*.c)
ACN_SDK_SRC += $(wildcard $(PWD)/acnsdkc/bsd/*.c)
ACN_SDK_SRC += $(wildcard $(PWD)/acnsdkc/arrow/*.c)
ACN_SDK_SRC += $(wildcard $(PWD)/acnsdkc/json/*.c)

# bootloader sources

BOOT_SRC += $(PRJ_PATH)/bootloader/main.c
BOOT_SRC += $(PRJ_PATH)/bootloader/stm32l4xx_it.c
BOOT_SRC += $(PRJ_PATH)/bootloader/debug.c

### common objects

COMMON_OBJ += $(ASRCS:%.s=%.o)
COMMON_OBJ += $(C_SRC:%.c=%.o)
COMMON_OBJ += $(BSP_SRC:%.c=%.o)
COMMON_OBJ += $(HAL_SRC:%.c=%.o)

### application objects

APP_OBJECTS := $(COMMON_OBJ)
APP_OBJECTS += $(USB_SRC:%.c=%.o)
APP_OBJECTS += $(FREERTOS_SRC:%.c=%.o)
APP_OBJECTS += $(SENSOR_SRC:%.c=%.o)
APP_OBJECTS += $(WIFI_SRC:%.c=%.o)
APP_OBJECTS += $(ACN_SDK_SRC:%.c=%.o)
APP_OBJECTS += $(APP_SRC:%.c=%.o)

### bootloader objects

BOOT_OBJECTS := $(COMMON_OBJ)
BOOT_OBJECTS += $(BOOT_SRC:%.c=%.o)


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
LINKER_SCRIPT_APP  = $(PRJ_PATH)/Src/STM32L475VG_FLASH.ld
LINKER_SCRIPT_BOOT = $(PRJ_PATH)/bootloader/STM32L475VG_BOOT.ld
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

all: $(PROJECT).bin bootloader.bin size

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


$(PROJECT).elf: $(SDK_TARGET) $(APP_OBJECTS) $(LINKER_SCRIPT_APP)
	$(LD) $(LD_FLAGS) -T$(filter %.ld, $^) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group


$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

$(PROJECT).lst: $(PROJECT).elf
	@$(OBJDUMP) -Sdh $< > $@

lst: $(PROJECT).lst

size: $(PROJECT).elf bootloader.elf
	$(SIZE) bootloader.elf
	$(SIZE) $(PROJECT).elf
	
bootloader.elf: $(SDK_TARGET) $(BOOT_OBJECTS) $(LINKER_SCRIPT_BOOT)
	$(LD) $(LD_FLAGS) -T$(filter %.ld, $^) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group


bootloader.bin: bootloader.elf
	$(OBJCOPY) -O binary $< $@
	
flash: $(PROJECT).bin
	st-flash write $(PROJECT).bin 0x08080000
	st-flash write bootloader.bin 0x08000000


DEPS = $(APP_OBJECTS:.o=.d)

clean: sdk_clean
	$(RM) $(APP_OBJECTS) $(QUIET)
	$(RM) $(PROJECT)* $(QUIET)
	$(RM) $(DEPS) $(QUIET)
	$(RMDIR) $(LIBDIR) $(QUIET)
	
-include $(DEPS)

