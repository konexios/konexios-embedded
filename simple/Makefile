default: all

CC = gcc
LD = gcc

NAME = udp
OUT = ${NAME}.bin

PROJECT = arrow

CSRCS  += $(wildcard $(PWD)/*.c)

COBJS := $(CSRCS:%.c=%.o)

CC_FLAGS += -c -lpthread
LD_FLAGS = -lpthread


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

$(OUT): $(COBJS)
	  @echo "build $(OUT)"
	  $(LD) $(LD_FLAGS) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^)
	
clean:
	$(RM) $(COBJS)
	$(RM) $(OUT)

run: $(OUT)
	@./$(OUT)
	

all: $(COBJS) $(OUT)
