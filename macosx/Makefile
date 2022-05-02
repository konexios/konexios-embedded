default: all

APP_NAME = konexios
APP_OUT = ${APP_NAME}.bin

PROJECT = konexios 
LIBDIR = .build
WOLFSSL = yes
CJSON = yes
GCC_BIN =
GCC_PREFIX =
PLATFORM = macosx

SDK_PATH = $(abspath ../../konexios-sdk-c/)
SDK_IMPL = $(PWD)/konexiossdkc

COMMON_FLAGS = -g3 -O0 \
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
               -fomit-frame-pointer \
			   -Wgnu-binary-literal \
			   -Wunused-function
CC_FLAGS = $(COMMON_FLAGS) -std=gnu11 -MP
CPPC_FLAGS = $(COMMON_FLAGS) -std=gnu++98 -fno-rtti -MP
ASM_FLAGS = -x assembler-with-cpp $(COMMON_FLAGS)
CC_SYMBOLS = -DTOOLCHAIN_GCC \
        -DDEBUG -Wextra -DNO_SOFTWARE_UPDATE \
	-DARCH_TYPE -DARCH_SOCK -DARCH_SSL \
  -Wpedantic -Wno-variadic-macros 
#	-Werror -Wpedantic -Wno-variadic-macros 

#CC_SYMBOLS += -O0 -g -DVALGRIND_TEST=5
#CC_SYMBOLS += -O0 -g3 -DVALGRIND_TEST=5
CC_SYMBOLS += -O0 -g3

-include $(SDK_PATH)/Makefile.sdk

$(info $(CC))

CSRCS	+= $(shell find $(PWD)/konexiossdkc -name "*.c")
CPPSRCS += $(wildcard $(PWD)/*.cpp)


COBJS := $(CSRCS:%.c=%.o)
CPPOBJS := $(CPPSRCS:%.cpp=%.o)
CPPOBJS += $(COBJS)
CPPDEPS := $(CPPSRCS:%.cpp=%.d)
CPPDEPS += $(CSRCS:%.c=%.d)

LIBRARY_PATHS = -L$(PWD)/$(LIBDIR)
LIBRARIES = -lkonexiossdkc -lpthread

# LD_FLAGS = -Wl,--wrap=malloc,--wrap=realloc,--wrap=calloc,--wrap=strdup,--wrap=strndup 
LD_SYS_LIBS = -lstdc++ -lm -lc -Wl

# LD_SYS_LIBS += -lcurl 
# -Wl,--wrap=malloc,--wrap=free,--wrap=realloc,--wrap=calloc,--wrap=valloc,--wrap=aligned_alloc,--wrap=memalign,--wrap=pvalloc,--wrap=posix_memalign,--wrap=strdup


ifeq ($(DEBUG), 1)
#  CC_FLAGS += -DDEBUG -O0
   CC_FLAGS += -DDEBUG -O0
else
#  CC_FLAGS += -DNDEBUG -Os
   CC_FLAGS += -DDEBUG -O0
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
	$(LD) $(LD_FLAGS) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) $(LIBRARIES) $(LD_SYS_LIBS)

$(APP_OUT): check_settings app_version $(SDK_TARGET) $(CPPOBJS) $(SYS_OBJECTS)
	  @echo "build $(APP_OUT)"
	  $(LD) $(LD_FLAGS) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) $(LIBRARIES) $(LD_SYS_LIBS)
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
