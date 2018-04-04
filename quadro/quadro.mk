#
# Copyright 2017, Cypress Semiconductor Corporation or a subsidiary of 
 # Cypress Semiconductor Corporation. All Rights Reserved.
 # This software, including source code, documentation and related
 # materials ("Software"), is owned by Cypress Semiconductor Corporation
 # or one of its subsidiaries ("Cypress") and is protected by and subject to
 # worldwide patent protection (United States and foreign),
 # United States copyright laws and international treaty provisions.
 # Therefore, you may use this Software only as provided in the license
 # agreement accompanying the software package from which you
 # obtained this Software ("EULA").
 # If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 # non-transferable license to copy, modify, and compile the Software
 # source code solely for use in connection with Cypress's
 # integrated circuit products. Any reproduction, modification, translation,
 # compilation, or representation of this Software except as specified
 # above is prohibited without the express written permission of Cypress.
 #
 # Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 # EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 # WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 # reserves the right to make changes to the Software without notice. Cypress
 # does not assume any liability arising out of the application or use of the
 # Software or any product or circuit described in the Software. Cypress does
 # not authorize its products for use in any products where a malfunction or
 # failure of the Cypress product may reasonably be expected to result in
 # significant property damage, injury or death ("High Risk Product"). By
 # including Cypress's product in a High Risk Product, the manufacturer
 # of such system or application assumes all risk of such use and in doing
 # so agrees to indemnify Cypress against all liability.
#

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(dir $(mkfile_path))
SDK_PATH = $(current_dir)/../acn-sdk-c
SDK_IMPL = $(current_dir)/acnsdkc
include $(SDK_PATH)/Makefile.wolf
NAME := App_Arrow_Quadro

$(NAME)_SOURCES    := quadro.c
$(NAME)_SOURCES    += acnsdkc/bsd/socket.c
$(NAME)_SOURCES    += acnsdkc/time/time.c
$(NAME)_SOURCES    += acnsdkc/debug.c
$(NAME)_SOURCES    += acnsdkc/sys/mac.c
$(NAME)_SOURCES    += acnsdkc/sys/reboot.c
$(NAME)_SOURCES    += acnsdkc/arrow/storage.c
$(NAME)_SOURCES    += acnsdkc/arrow/update.c
$(NAME)_SOURCES    += acnsdkc/json/telemetry.c

$(NAME)_SOURCES    += ../acn-sdk-c/src/ntp/client.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/ntp/ntp.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/sys/watchdog_weak.c
#$(NAME)_SOURCES    += ../acn-sdk-c/src/sys/reboot_weak.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/time/time.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/bsd/inet.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/routine.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/device.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/gateway.c
# $(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/request.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/sign.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/api/device/device.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/api/device/info.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/api/gateway/gateway.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/software_release.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/software_update.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/events.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/utf8.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/http/client.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/http/request.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/http/response.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/http/routine.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/events.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/state.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/device_command.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/json/json.c

$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/mqtt.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/mqtt/acn.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/gateway_payload_sign.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/client/src/timer.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/client/src/network.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/client/src/MQTTClient.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTConnectClient.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTConnectServer.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTDeserializePublish.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTFormat.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTPacket.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTSerializePublish.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTSubscribeClient.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTSubscribeServer.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTUnsubscribeClient.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/mqtt/packet/src/MQTTUnsubscribeServer.c

$(NAME)_SOURCES    += ../acn-sdk-c/src/debug.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/data/linkedlist.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/data/ringbuffer.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/data/property.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/data/propmap.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/data/find_by.c
$(NAME)_SOURCES    += ../acn-sdk-c/src/arrow/api/json/parse.c
#$(NAME)_SOURCES    += ../acn-sdk-c/src/ssl/crypt.c
#$(NAME)_SOURCES    += ../acn-sdk-c/src/ssl/md5sum.c
#$(NAME)_SOURCES    += ../acn-sdk-c/src/ssl/ssl.c
$(NAME)_SOURCES    += $(patsubst $(SDK_PATH)%,../acn-sdk-c/%,$(WOLF_SRC))

$(info $($(NAME)_SOURCES))

WIFI_CONFIG_DCT_H := wifi_config_dct.h

$(NAME)_DEFINES    += __quadro__
$(NAME)_DEFINES    += DEBUG
$(NAME)_DEFINES    += USER_BYTE_CONVERTER
$(NAME)_DEFINES    += ARCH_SSL

$(NAME)_INCLUDES += .
$(NAME)_INCLUDES += ../acn-sdk-c
$(NAME)_INCLUDES += ../acn-sdk-c/include
$(NAME)_INCLUDES += ../acn-sdk-c/src/wolfSSL
$(NAME)_INCLUDES += ../acn-sdk-c/src/wolfSSL/wolfssl
$(NAME)_INCLUDES += ../acn-sdk-c/src/mqtt/client/src
$(NAME)_INCLUDES += ../acn-sdk-c/src/mqtt/packet/src
$(NAME)_INCLUDES += ./acnsdkc

$(info $($(NAME)_INCLUDES))

# $(NAME)_COMPONENTS := protocols/HTTP

# < Please add client certificate and private key here if you want to enable client authentication >
#CERTIFICATE := $(SOURCE_ROOT)resources/certificates/brcm_demo_server_cert.cer
#PRIVATE_KEY := $(SOURCE_ROOT)resources/certificates/brcm_demo_server_cert_key.key
