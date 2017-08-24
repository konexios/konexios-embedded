default: all

APP_NAME = startup
APP_OUT = ${APP_NAME}

GCC_BIN = 
PROJECT = boot
GCC_BIN =
GCC_PREFIX = arm-none-eabi
PLATFORM = ARIS

CPP 	:= $(GCC_PREFIX)-g++
CC  	:= $(GCC_PREFIX)-gcc
LD  	:= $(GCC_PREFIX)-gcc
OBJCOPY := $(GCC_PREFIX)-objcopy
OBJDUMP := $(GCC_PREFIX)-objdump
SIZE    := $(GCC_PREFIX)-size

CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16
COMMON_FLAGS = -Wall -Wextra -Wunused -Wuninitialized -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -nostdlib
CC_FLAGS = $(COMMON_FLAGS) $(CPU) -Os -std=c99 -MP -MMD
CPPC_FLAGS = $(COMMON_FLAGS) $(CPU) -Os -std=gnu++98 -fno-rtti -Wvla -MP
ASM_FLAGS = -x assembler-with-cpp -c $(COMMON_FLAGS) $(CPU) -Os

CC_SYMBOLS = -D_ARIS_ \
	     -DDEBUG

LD_FLAGS = $(CPU) $(COMMON_FLAGS)

CSRCS += $(wildcard $(PWD)/*.c)
# SSRCS += $(wildcard $(PWD)/*.S)
# SOBJS := $(SSRCS:%.S=%.o)

COBJS := $(CSRCS:%.c=%.o)

INCLUDES += \
            -I$(PWD) \
            -I$(PWD)/../synergy_cfg/ssp_cfg/bsp \
            -I$(PWD)/../synergy_cfg/ssp_cfg/driver \
            -I$(PWD)/../synergy/ssp/inc \
            -I$(PWD)/../synergy/ssp/inc/bsp \
            -I$(PWD)/../synergy/ssp/inc/bsp/cmsis/Include \
            -I$(PWD)/../synergy/ssp/inc/driver/api \
	    -I$(PWD)/../synergy/ssp/inc/driver/instances \
	    -I$(PWD)/../src \
	    -I$(PWD)/../src/synergy_gen \
	    -I$(PWD)/../synergy_cfg/ssp_cfg/framework \
	    -I$(PWD)/../synergy_cfg/ssp_cfg/framework/el

LIBRARY_PATHS  = -L$(PWD)/../synergy/ssp/src/bsp/cmsis/DSP_Lib/cm0plus_gcc
LIBRARY_PATHS += -L$(PWD)/../synergy/ssp/src/bsp/cmsis/DSP_Lib/cm0plus_iar
LIBRARY_PATHS += -L$(PWD)/../synergy/ssp/src/bsp/cmsis/DSP_Lib/cm4_gcc
LINKER_SCRIPT = $(PWD)/../script/startup.ld 

ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@
.cpp.o:
	$(CPP) $(CPPC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@

$(APP_OUT).elf: $(COBJS) $(LINKER_SCRIPT)
	  @echo "build $(APP_OUT)"
	  $(LD) $(LD_FLAGS) -T$(filter %.ld, $^) -Xlinker --gc-sections $(LIBRARY_PATHS) --specs=rdimon.specs -o $@ -Wl,--start-group $(filter %.o, $^) -Wl,--end-group
	
clean: 
	$(RM) $(COBJS) $(APP_OUT).bin  $(APP_OUT).elf  $(APP_OUT).srec $(APP_OUT).d
	
$(APP_OUT).bin: $(APP_OUT).elf
	$(OBJCOPY) -O binary $< $@

$(APP_OUT).srec: $(APP_OUT).elf
	@$(OBJCOPY) -O srec $< $@

size: $(APP_OUT)
	$(SIZE) $(APP_OUT)
	
all: $(CPPOBJS) $(APP_OUT).elf $(APP_OUT).srec

