# Overview #

This project is a demo of the Arrow Connect system running on the Microchip SAMW25, an ARM Cortex M0+ and WINC1500 WiFi enabled COM.

Information about Arrow Connect APIKey and SecretKey are stored on the SAMW25 chip.  Defaults can be defined in the ac_data.c file.

Arrow Connect SDK expects a private.h header file with fallback definitions for Arrow Connect values.  These values will not be used by the application and can be defined as empty.  An empty example private.h file is included in the inc/ folder.

## Hardware ##

SAMW25 Xplained PRO Development board - https://www.arrow.com/en/reference-designs/atsamw25-xpro-sam-w25-xplained-pro-evaluation-kit-based-on-the-industry-leading-low-power-24ghz-ieee-80211-bgn-wi-fi-atwinc1500-soc/bbdb6688c9017c58f9465dbef8667705  

CRYPTOAUTH-XPRO-B - https://www.arrow.com/en/products/atcryptoauth-xpro-b/microchip-technology  

The jumper settings for the CRYPTOAUTH board must enable the ECC608A chip.  The ECC608A must also be listening for communication on address 0xCC on the I2C bus. Programming software for the address byte on the ECC608A chip is contained in the tools/ECC608A_Updater/ directory.  Instructions for updating the ECC608A are contained the tools/ECC608A_Updater/README.md


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

The system includes a UART based terminal application that allows a user to set variables, display variables, and command the system to execute different functions.  By default this menu is shown on startup

The UART settings are 115200 baud 8-N-1.  Putty is the preferred terminal application to connect to the example system. https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html

Instructions on connecting to the device can be found here: http://bit.ly/2mP6S5k

Instructions on the operation of the system can be found here: http://bit.ly/2Lx3iM1

## Setup WiFi ##

The WiFi subsystem on the dev board is configured to automatically connect to the following access point:

* SSID: Arrow Connect
* Key: Arrowdem0123!

The demo application supports WPA2 and Open security methods.

To change the WiFi SSID, type 'wifissid' and press enter

To change the WiFi key, type 'wifikey' and press enter

To connect to a WiFi AP, type 'connect' and press enter

To disconnect, type 'disconnect' and press enter


## Connect to Arrow Connect ##

To connect to Arrow Connect, type 'start' and press enter

To disconnect from Arrow Connect, type 'stop' and press enter

## Automatic connection ##

The demo system has a statemachine based 'application' which will connect to the network, update the system time, and connect to Arrow Connect.  By default this application is enabled on the system.

To enable/disable the automatic network connection type 'autostart' and press enter

To manually connect the system to the network type 'appstart' and press enter

## Send a Message ##

Once the system is connected to the Arrow Connect network, messages can be sent to the server and on to the demo display page.  There is a user defined message that will be sent with all messages.

To change the user mesage type 'message' and press enter.

To send the message to Arrow Connect, press the USER button on the board, or on the command line type 'send' and press enter

A dashboard showing all messages and the number of connections from devices, can be found here http://bit.ly/2LTx6BL

