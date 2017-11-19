PROGRAM=arrow

PROGRAM_CFLAGS += -D__esp8266__ -DDEBUG -DNO_SOFTWARE_UPDATE -DNO_RELEASE_UPDATE

# -DNO_EVENTS
EXTRA_CFLAGS += $(PROGRAM_CFLAGS)

ACN_INC_DIR += ${PWD}/acnsdkc

PROGRAM_EXTRA_SRC_FILES += $(wildcard $(ACN_INC_DIR)/bsd/*.c)
PROGRAM_EXTRA_SRC_FILES += $(wildcard $(ACN_INC_DIR)/time/*.c)
PROGRAM_EXTRA_SRC_FILES += $(wildcard $(ACN_INC_DIR)/arrow/*.c)
PROGRAM_EXTRA_SRC_FILES += $(wildcard $(ACN_INC_DIR)/json/*.c)
PROGRAM_EXTRA_SRC_FILES += $(wildcard $(ACN_INC_DIR)/*.c)


EXTRA_COMPONENTS = ../acn-embedded/acn-sdk-c extras/onewire extras/ds18b20
include ~/esp/esp-open-rtos/common.mk

INC_DIRS += /home/git_user_1/esp/esp-open-rtos/libc/xtensa-lx106-elf/include 
