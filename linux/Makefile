default: all

APP_NAME = arrow
APP_OUT = ${APP_NAME}.bin

PROJECT = arrow 
LIBDIR = .build
WOLFSSL = yes
CJSON = yes
GCC_BIN =
GCC_PREFIX =
PLATFORM = linux

SDK_PATH = $(abspath ../acn-sdk-c/)
SDK_IMPL = $(PWD)/acnsdkc

COMMON_FLAGS = -Os \
               -Wall \
               -Wextra \
               -Wno-unused-parameter \
               -Wno-missing-field-initializers \
               -fmessage-length=0 \
               -fno-exceptions \
               -fno-builtin \
               -ffunction-sections \
               -fdata-sections \
               -funsigned-char \
               -MMD \
               -fno-delete-null-pointer-checks \
               -fomit-frame-pointer
CC_FLAGS = $(COMMON_FLAGS) -std=gnu11 -MP
CPPC_FLAGS = $(COMMON_FLAGS) -std=gnu++98 -fno-rtti -MP
ASM_FLAGS = -x assembler-with-cpp $(COMMON_FLAGS)
CC_SYMBOLS = -DTOOLCHAIN_GCC \
        -DDEBUG -Wextra -DNO_SOFTWARE_UPDATE \
	-DARCH_TYPE -DARCH_SOCK\
	-Werror -Wpedantic -Wno-variadic-macros #-O0 -g -DVALGRIND_TEST=100

ifdef PROBOOK
CC_SYMBOLS += -D__probook_4540s__
endif

-include $(SDK_PATH)/Makefile.sdk

$(info $(CC))

CSRCS	+= $(shell find $(PWD)/acnsdkc -name "*.c")
CPPSRCS += $(wildcard $(PWD)/*.cpp)


COBJS := $(CSRCS:%.c=%.o)
CPPOBJS := $(CPPSRCS:%.cpp=%.o)
CPPOBJS += $(COBJS)
CPPDEPS := $(CPPSRCS:%.cpp=%.d)
CPPDEPS += $(CSRCS:%.c=%.d)

LIBRARY_PATHS = -L$(PWD)/$(LIBDIR)
LIBRARIES = -lsensors -lacnsdkc

LD_FLAGS = 
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc

# LD_SYS_LIBS += -lcurl 
# -Wl,--wrap=malloc,--wrap=free,--wrap=realloc,--wrap=calloc,--wrap=valloc,--wrap=aligned_alloc,--wrap=memalign,--wrap=pvalloc,--wrap=posix_memalign,--wrap=strdup


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

INCLUDES += $(SDK_INCLUDES)

.c.o:
	$(CC) $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@

.cpp.o:
	$(CPP) $(CPPC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) -c $< -o $@
	
updater: updater.o
	@echo "build updater"
	$(LD) $(LD_FLAGS) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group

$(APP_OUT): check_settings app_version $(SDK_TARGET) $(CPPOBJS) $(SYS_OBJECTS)
	  @echo "build $(APP_OUT)"
	  $(LD) $(LD_FLAGS) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group
	  cp $(APP_OUT) $(LIBDIR)/ota_${VERSION}.bin
	
clean: sdk_clean
	$(RMDIR) $(LIBDIR)
	$(RM) $(CPPOBJS)
	$(RM) $(CPPDEPS)
	$(RM) $(APP_OUT)
	
size: $(APP_OUT) app_settings
	$(SIZE) $(APP_OUT)
	@echo -e "$(DEV_CONF)"

run: $(APP_OUT)
	@if [ -e update.file ]; then mv update.file $(APP_OUT); chmod a+x $(APP_OUT); fi
	@./$(APP_OUT)

test: $(SDK_TARGET) $(CPPOBJS) $(SYS_OBJECTS)
	@echo "Run Test..."

all: $(APP_OUT) $(CPPOBJS) size updater
