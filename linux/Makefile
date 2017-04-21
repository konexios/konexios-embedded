default: all

APP_NAME = linux_arrow
APP_OUT = ${APP_NAME}.elf

PROJECT = arrow

C_SDK_PATH = $(PWD)/../acn-sdk-c
C_SDK_MAKE_PATH = $(C_SDK_PATH)/platforms/linux
-include $(C_SDK_MAKE_PATH)/Makefile.linux.def

CSRCS 	 += $(shell find $(PWD)/acnsdkc -name "*.c")
CPPSRCS  += $(wildcard $(PWD)/*.cpp)

COBJS := $(CSRCS:%.c=%.o)
CPPOBJS := $(CPPSRCS:%.cpp=%.o)
CPPOBJS += $(COBJS)
CPPDEPS := $(CPPSRCS:%.cpp=%.d)
CPPDEPS += $(CSRCS:%.c=%.d)

LIBRARY_PATHS = -L$(C_SDK_PATH) -L$(PWD)/.build
LIBRARIES = -lsensors -lacnsdkc

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

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) $< -o $@
.cpp.o:
	$(CPP) $(CPPC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) $< -o $@

libacnsdkc.a:
	@echo $(PWD)
	if [ ! -e $(PWD)/.build ]; then mkdir $(PWD)/.build; fi
	cd $(C_SDK_PATH) && $(MAKE) -f $(C_SDK_MAKE_PATH)/Makefile.linux LIBDIR="$(PWD)/.build" CPATH="$(PWD)"

$(APP_OUT): $(CPPOBJS) $(SYS_OBJECTS) libacnsdkc.a
	  @echo "build $(APP_OUT)"
	  $(LD) $(LD_FLAGS) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group
	
clean:
	$(RM) $(OBJDIR) 
	$(RM) $(CPPOBJS)
	$(RM) $(CPPDEPS)
	cd $(C_SDK_PATH) && $(MAKE) -f $(C_SDK_MAKE_PATH)/Makefile.linux clean LIBDIR="$(PWD)/.build"
	$(RM) $(APP_OUT)
	
size: $(APP_OUT)
	$(SIZE) $(APP_OUT)

all: libacnsdkc.a $(CPPOBJS) $(APP_OUT)
