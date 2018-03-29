# Copyright 2015 Adam Green (http://mbed.org/users/AdamGreen/)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
PROJECT_ORG     := BB-GEVK
NOTHING         :=
SPACE           :=$(NOTHING) $(NOTHING)
PROJECT         :=$(subst $(SPACE),_,$(PROJECT_ORG))
DEVICES         := NCS36510
GCC4MBED_DIR1   := $(IDK_PATH)
GCC4MBED_DIR    := $(subst \,/,$(GCC4MBED_DIR1))
NO_FLOAT_SCANF  := 1
NO_FLOAT_PRINTF := 1
IDK_LIB		:= 0
MRI_ENABLE	:= 0
INCDIRS		:= Include/ 
LIBS_PREFIX	:= 
MBED_LIBS       := rtos
INCDIRS		+= $(GCC4MBED_DIR)/IDK/include/
GCC4MBED_TYPE   = Release

#TL_PATH ?= C:\GNUToolchain\bin
# TL_PATH = "C:\Program Files (x86)\GNU Tools ARM Embedded\2015q4\bin"
 TL_PATH = "C:\Program Files (x86)\GNU Tools ARM Embedded\2017-q1\bin"
# TL_PATH = "C:\Program Files (x86)\GNU Tools ARM Embedded\5.4 2016q2\bin"


include $(GCC4MBED_DIR)/build/idk.mk

WOLFSSL = yes
CJSON = yes
GCC_BIN = $(TL_PATH)
GCC_PREFIX = arm-none-eabi
PLATFORM = semiconductor
SDK_PATH = $(GCC4MBED_DIR)/acn-embedded/acn-sdk-c
SDK_IMPL = $(GCC4MBED_DIR)/acn-embedded/BB-GEVK/acnsdkc
LIBDIR = $(GCC4MBED_DIR)/acn-embedded/BB-GEVK/NCS36510/acnsdkc

# You may comment the needless shields
DEFINES += -DUSE_QUADRO_SHIELD
#DEFINES += -DUSE_WIFI_SHIELD
#DEFINES += -DUSE_POE_SHIELD
#DEFINES += -DUSE_DLBS_MOTOR
DEFINES += -DUSE_STEP_MOTOR
DEFINES += -DUSE_LED_BALLAST
#DEFINES += -DUSE_LCD
DEFINES += -DDEBUG
DEFINES += -Os
DEFINES += -DNO_SOFTWARE_UPDATE 
DEFINES += -DNO_SOFTWARE_RELEASE
DEFINES += -DARCH_SSL
DEFINES += -D__$(PLATFORM)__
# -fno-inline-functions
# -Wimplicit-function-declaration -Werror -Wpedantic 
# -Wl,--wrap=malloc -Wl,--wrap=free
# LD_FLAGS += -Wl,--wrap=calloc

CC_FLAGS += $(C_FLAGS)
CC_SYMBOLS += $(DEFINES) -std=gnu11 -Os

SHELL_BASH := no

include $(SDK_PATH)/Makefile.sdk
inc = -I
INCDIRS += $(subst $(inc),,$(SDK_INCLUDES))
LIBS_PREFIX += $(SDK_TARGET)

EXCLUDE_INC += -I./acnsdkc/time
EXCLUDE_INC += -I./acnsdkc/arrow
EXCLUDE_INC += -I./acnsdkc/bsd

# workaround
include $(GCC4MBED_DIR)/build/gcc4mbed.mk
$(MBED_DEVICE): CPP_FLAGS := $(filter-out $(EXCLUDE_INC),${CPP_FLAGS})
$(MBED_DEVICE): C_FLAGS := $(filter-out $(EXCLUDE_INC),${C_FLAGS})

default: all
clean: sdk_clean
