default: all

APP_NAME = linux_arrow
APP_OUT = ${APP_NAME}.elf

GCC_BIN = 
PROJECT = arrow

KRONOS_C_SDK_PATH = $(PWD)/../kronos-c-sdk
CPPSRCS  = $(wildcard $(PWD)/*.cpp)

COBJS := $(CSRCS:%.c=%.o)
CPPOBJS := $(CPPSRCS:%.cpp=%.o)
CPPOBJS += $(COBJS)
CPPDEPS := $(CPPSRCS:%.cpp=%.d)

INCLUDES = -I$(PWD)/.

LIBRARY_PATHS = -L$(KRONOS_C_SDK_PATH) -L$(PWD)/.build
LIBRARIES = -lsensors -lkronoscsdk

INCLUDES += -I$(KRONOS_C_SDK_PATH) \
	    -I$(KRONOS_C_SDK_PATH)/include

############################################################################### 
AS      = $(GCC_BIN)as
CC      = $(GCC_BIN)gcc
CPP     = $(GCC_BIN)g++
LD      = $(GCC_BIN)gcc
OBJCOPY = $(GCC_BIN)objcopy
OBJDUMP = $(GCC_BIN)objdump
SIZE    = $(GCC_BIN)size 

CPU = 
CC_FLAGS = -c -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Os -std=gnu99 -MMD -MP
CPPC_FLAGS = -c -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Os -std=gnu++98 -fno-rtti -Wvla -MMD -MP
ASM_FLAGS = -x assembler-with-cpp -c -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Os
CC_SYMBOLS = -DDEBUG

LD_FLAGS = 
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc


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

# all $(PROJECT).bin $(PROJECT).hex size

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) $< -o $@
.cpp.o:
	$(CPP) $(CPPC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) $< -o $@

libkronoscsdk.a:
	@echo $(PWD)
	cd $(KRONOS_C_SDK_PATH) && $(MAKE) -f Makefile.linux LIBDIR="$(PWD)/.build" CPATH="$(PWD)"

$(APP_OUT): $(CPPOBJS) $(SYS_OBJECTS) libkronoscsdk.a
	  @echo "build $(APP_OUT)"
	  $(LD) $(LD_FLAGS) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group
	
clean:
	$(RM) $(OBJDIR) 
	$(RM) $(CPPOBJS)
	cd $(KRONOS_C_SDK_PATH) && $(MAKE) -f Makefile.mbed clean
	
$(APP_OUT).bin: $(APP_OUT)
	$(OBJCOPY) -O binary $< $@

$(APP_OUT).hex: $(APP_OUT)
	@$(OBJCOPY) -O ihex $< $@

$(APP_OUT).lst: $(APP_OUT)
	@$(OBJDUMP) -Sdh $< > $@

lst: $(APP_OUT).lst

size: $(APP_OUT)
	$(SIZE) $(APP_OUT)

all: libkronoscsdk.a $(CPPOBJS) $(APP_OUT) $(APP_OUT).bin
