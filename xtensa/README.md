# Getting started

## Setting up the environment

###### Connecting the SX-ULPGN-EVK board

**_Following points based on the SX-ULPGN-EVK Tutorial (https://drive.google.com/open?id=0BzSl3gduBcnuTHRmMFFpcEl3Qnc)._**

There are two connection to your PC: FTDI and UART.
- JTAG connection
The C232HM-DDHSL-0 cable was used. The individual pins must be connected to corresponding pins in 
JP12 as Figure 1-3 (see the tutorial). Note that connection to pin11 (VCC) is optional. Though you can 
power the EVK through JTAG VCC, you don't have 5V power out J4-1. Also you cannot measure power consumption using J5.



| C232HM cable  |  ULPGN JP12  |
|---------------|--------------|
| TDI (yellow)  |       1      |
| TDO (green)   |       3      |
| TCK (orange)  |       5      |
| GND (black)   |       6      |
| RST (purple)  |       9      |
| TMS (brown)   |       10     |
| VCC (red)     |       11     |



- UART connection 
It's possible to use USB-UART adaptor with 3.3V logic voltage (not RS-232C voltage) interface.
Figure 1-4 (see the tutorial) shows how to connect USB-UART cable with EVK. J1 DEBUG UART is mandatory, JP4/JP6 is optional (only when you want to test HSUART1 or HSUART2).


The following connection map was used:


| USB-UART cable |  ULPGN JP1  |
|----------------|-------------|
|  TXD (green)   |      6      |
|  GND (black)   |      7      |
|  RXD (white)   |      8      |



###### Xtensa ecosystem deploying

- Cadence Xtensa development tools

Follow the Internet of Everything Wireless System Release (I-WSR) 4.1 CS Hostless SDK Release Notes for install environment
(https://drive.google.com/open?id=0BzSl3gduBcnuVDhVMVhmNnlaaVU)

1. Download the Xplorer-5.0.3-linux-installer.bin from https://drive.google.com/open?id=0BzSl3gduBcnuZnR3ZVVCWWdyZTQ
2. Execute the Xtensa Xplorer Setup Wizard Xplorer-5.0.3-linux-installer.bin and install this. May use a by-default setting but choose Xtensa Xplorer Root Directory (assume it would be **/home/user/xtensa**)
3. Download the license.dat file int home directory
4. Start the Xplorer and install the software keys (locate the license file). Finish and close.
 (In the Xplorer GUI, click Install Software Keys and navigate to the license file. Click Finish and close the window.)
5. Download the KF_2013_3_linux.tgz from https://drive.google.com/open?id=0BzSl3gduBcnuTXVocGUxUERCb00
6. In the Xplorer workbench, right click Configurations in the System Overview panel.
Click Find and Install a Configuration Build. In the popup window, next to Browse 
for build download to install, click Browse... and navigate to target/RE-2013.3-Xplorer/KF_2013_3_linux.tgz. Click OK, and wait for the install to complete.
7. Close Xtensa Xplorer


**Do not forget to export the system variables!**

Place the following variables either in a script to be sourced later, or in a .bashrc (or
equivalent) startup file.

```
export XTENSA_INST=/home/user/xtensa
export XTENSA_CORE=KF
export XTENSA_ROOT=$XTENSA_INST/XtDevTools/install/builds/RE-2013.3-linux/KF
export XTENSA_SYSTEM=$XTENSA_ROOT/config
export XTENSA_TOOLS_ROOT=$XTENSA_INST/XtDevTools/install/tools/RE-2013.3-linux/XtensaTools
export LM_LICENSE_FILE=~/license.dat
export PATH=$PATH:$XTENSA_TOOLS_ROOT/bin
```

###### Building environment

Assumed the 14apr_qca4010.tx_.1.1_4.0.1.24.tgz was unpacked into a directory /home/user/

```
tar -xf 14apr_qca4010.tx_.1.1_4.0.1.24.tgz -C /home/user/
```

I would recommend to fix some files into the building environment

```
sed "s/-objdump/-xt-objdump/" -i ~/4010.tx.1.1_sdk/target/demo/sdk_flash/make_flash_hostless.sh
sed "s/-obj/-xt-obj/g" -i ~/4010.tx.1.1_sdk/target/tool/makeseg.sh
```

Enter the target folder and export more system variables
```
cd ~/4010.tx.1.1_sdk/target/
source sdkenv.sh
```

Now environment ready to building the firmware

## How to build 

###### Setting up the SDK

It's needed a keys (api and secret) for any application in the acn-embedded repo.
You may define it into an acn-sdk-c/private.h file:

```
#ifndef ACN_SDK_C_PRIVATE_H_
#define ACN_SDK_C_PRIVATE_H_

#define DEFAULT_API_KEY "xxxxxx"
#define DEFAULT_SECRET_KEY "yyyyy"
#define DEV_ENV

#endif
```

where DEV_ENV - define the development environment not a production environment:
API address: pgsdev01.arrowconnect.io
MQTT address: pgsdev01.arrowconnect.io

###### Building

```
cd /home/user/4010.tx.1.1_sdk/target/
git clone https://github.com/arrow-acs/acn-embedded.git --depth 1 --recursive
```
(Or you can use a SSL if you wish)

Getting a fixed json library
```
cp acn-embedded/xtensa/config/libjson.a ./lib/
```
Fixed the default index page for AP mode:
```
cp acn-embedded/xtensa/config/index.html ./demo/sdk_flash/
```

Copy the config file for firmware building, it's need to replace target path /home/somebody/Arrow/QCA to $SDK_ROOT
```
cat acn-embedded/xtensa/config/tunable_input.txt | sed 's@\/home\/somebody\/Arrow\/QCA\/4010.tx.1.1_sdk\/target@'"$SDK_ROOT"'@g' > ./tool/tunable/tunable_input.txt
```

Compile an application
```
cd acn-embedded/xtensa
make
```

After this in a ~/target/bin/ folder should be the raw_flashimage_AR401X_REV6_IOT_MP1_hostless_unidev_singleband_iot_arrow.bin file.

## How to flash
Do not forget to replace TEST jumper to 2-3 place on the JP11 connector!
Run the OCD
./xt-ocd

Go to ~/target/image directory
cd ~/target/image

And run xt-gdb
xt-gdb -x gdb.sdk_flash

Connect to the target
(xt-gdb) target remote 127.0.0.1:20000

And then flash the bin file in a ~/target/bin/ folder
(xt-gdb) sdk_flash ../bin/raw_flashimage_AR401X_REV6_IOT_MP1_hostless_unidev_singleband_iot_arrow.bin
