# Overview #

This project is a demo of the ArrowConnect system running on the Microchip SAMW25, an ARM Cortex M0+ and WINC1500 WiFi enabled COM.

Information about ArrowConnect APIKey and SecretKey are stored on the SAMW25 chip.  Defaults can be defined in the ac_data.c file.

ArrowConnect SDK expects a private.h header file with fallback definitions for ArrowConnect values.  These values will not be used by the application and can be defined as empty.  An empty example private.h file is included in the inc/ folder.

## Hardware ##

SAMW25 Xplained PRO Development board - https://www.arrow.com/en/reference-designs/atsamw25-xpro-sam-w25-xplained-pro-evaluation-kit-based-on-the-industry-leading-low-power-24ghz-ieee-80211-bgn-wi-fi-atwinc1500-soc/bbdb6688c9017c58f9465dbef8667705  
CRYPTOAUTH-XPRO-B - https://www.arrow.com/en/products/atcryptoauth-xpro-b/microchip-technology  

The jumper settings for the CRYPTOAUTH board must enable the ECC608A chip.  The ECC608A must also be listening for communication on address 0xCC on the I2C bus. Programming software for the address byte on the ECC608A chip will be uploaded to this repository as it is finalized.


## Build ##
The project can be built with Atmel Studio 7.0.  An Atmel Studio Solution file is provided in the root of the project directory.  Load this Solution into Atmel Studio to review code. 

## Flash ##
Release files are located in the releases/ folder in the root of the project directory.  The firmware binary for the SAMW25 board is SAMW25Demo.elf and can be loaded by the atprogram.exe program in the Atmel Studio folder.

On most Windows 10 systems, atprogram.exe is located in the following directory:

* C:\Program Files (x86)\Atmel\Studio\7.0\atbackend

Example of commands used to program the SAMW25 are:

1. ./atprogram.exe -t edbg -i swd -d atsamd21g18a chiperase
2. ./atprogram.exe -t edbg -i swd -d atsamd21g18a program -f <PATHTO/SAMW25Demo.elf>


## Interacting with system ##

The system includes a UART based terminal application that allow a user to set variables, display variables, and command the system to execute different functions.  By default this menu is shown on startup

The UART settings are 115200 baud 8-N-1.  Putty is the preferred terminal application to connect to the example system. https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html

Instructions on connecting to the device can be found here: http://bit.ly/2mP6S5k

Instructions on the operation of the system can be found here: http://bit.ly/2Lx3iM1

## Setup WiFi ##

The WiFi subsystem on the dev board is configured to automatically connect to the following access point:

* SSID: Arrow Connect
* Key: ArrowDemo0

To change the WiFi SSID, type 'wifissid' and press enter

To change the WiFi key, type 'wifikey' and press enter

To connect to a WiFi AP, type 'connect' and press enter

To disconnect, type 'disconnect' and press enter


## Connect to ArrowConnect ##

To connect to ArrowConnect, type 'start' and press enter

To disconnect from ArrowConnect, type 'stop' and press enter


## Send a Message ##

To send a message to ArrowConnect, press the USER button, or type 'send' and press enter

A dashboard showing all messages and the number of connections from devices, can be found here http://bit.ly/2LTx6BL

