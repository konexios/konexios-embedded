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
GCC4MBED_DIR1    := $(IDK_PATH)
GCC4MBED_DIR    := $(subst \,/,$(GCC4MBED_DIR1))
NO_FLOAT_SCANF  := 1
NO_FLOAT_PRINTF := 1
IDK_LIB			:= 0
MRI_ENABLE		:= 0
INCDIRS			:= Include/ 
LIBS_PREFIX		:= 
MBED_LIBS       := rtos
INCDIRS			+= $(GCC4MBED_DIR)/IDK/include/

 TL_PATH ?= C:\GNUToolchain\bin
# TL_PATH = "C:\Program Files (x86)\GNU Tools ARM Embedded\2015q4\bin"
# TL_PATH = "C:\Program Files (x86)\GNU Tools ARM Embedded\2017-q1\bin"
# TL_PATH = "C:\Program Files (x86)\GNU Tools ARM Embedded\5.4 2016q2\bin"

include $(GCC4MBED_DIR)/build/idk.mk

# You may comment the needless shields
DEFINES += -DUSE_POE_SHIELD
DEFINES += -DUSE_DLBS_MOTOR
DEFINES += -DUSE_STEP_MOTOR
DEFINES += -DUSE_LED_BALLAST
#DEFINES += -DUSE_LCD
DEFINES += -DDEBUG 
DEFINES += -Os 
# -fno-inline-functions
# -Wimplicit-function-declaration -Werror -Wpedantic 
# -Wl,--wrap=malloc -Wl,--wrap=free
# LD_FLAGS += -Wl,--wrap=calloc 

LIBS_PREFIX += $(GCC4MBED_DIR)/acn-embedded/BB-GEVK/NCS36510/acnsdkc/libacnsdkc.a
include $(GCC4MBED_DIR)/acn-embedded/acn-sdk-c/platforms/semiconductor/Makefile.onsemi

include $(GCC4MBED_DIR)/build/gcc4mbed.mk
