## Getting started

It's needed a keys (api and secret) for any application in the acn-embedded repo.
You may define it into an acn-sdk-c/private.h file:

#ifndef ACN_SDK_C_PRIVATE_H_
#define ACN_SDK_C_PRIVATE_H_

#define DEFAULT_API_KEY "xxxxxx"
#define DEFAULT_SECRET_KEY "yyyyy"
#define DEV_ENV

#endif


or when execute make
make API_KEY="xxxx" SEC_KEY="yyyyyy" DEV_ENV=1

where DEV_ENV - define the development environment not a production environment:
API address: pgsdev01.arrowconnect.io
MQTT address: pgsdev01.arrowconnect.io

Setting up the environment
Follow the Internet of Everything Wireless System Release (I-WSR) 4.1 CS Hostless SDK Release Notes for install environment
(80-YA116-4 Rev. B April 1, 2016) 4.2-4.2.1
Do not forget to export the system variables!
For example:
export XTENSA_INST=/home/danila/xtensa
export XTENSA_CORE=KF
export XTENSA_ROOT=$XTENSA_INST/XtDevTools/install/builds/RE-2013.3-linux/KF
export XTENSA_SYSTEM=$XTENSA_ROOT/config
export XTENSA_TOOLS_ROOT=$XTENSA_INST/XtDevTools/install/tools/RE-2013.3-linux/XtensaTools
export LM_LICENSE_FILE=~/xtensa/license.dat
export PATH=$PATH:$XTENSA_TOOLS_ROOT/bin

Assumed you unpacked 14apr_qca4010.tx_.1.1_4.0.1.24.tgz into home directory
tar -xf 14apr_qca4010.tx_.1.1_4.0.1.24.tgz -C ~/

I would recommend to fix some files into the building environment
sed "s/-objdump/-xt-objdump/" -i ~/4010.tx.1.1_sdk/target/demo/sdk_flash/make_flash_hostless.sh
sed "s/-obj/-xt-obj/g" -i ~/4010.tx.1.1_sdk/target/tool/makeseg.sh

Enter the target folder and export more system variables
cd ~/4010.tx.1.1_sdk/target/
source sdkenv.sh

Now environment ready to building the firmware

## How to build 

cd ~/4010.tx.1.1_sdk/target/
git clone git@bitbucket.org:arrows3/eos.git
(Or you can use a https if you wish)

Getting the kronos-c-sdk as submodule
cd eos
git clone git@bitbucket.org:arrows3/kronos-c-sdk.git

If needed checkout into the tested versions
cd kronos-c-sdk
git checkout git checkout v_QCA4010_1.0
cd ..
git checkout QCA4010_1.0

cd ..

Getting a fixed json library
cp eos/xtensa/config/libjson.a ./lib/
cp eos/xtensa/config/index.html ./demo/sdk_flash/

Copy the config file for firmware building, it's need to replace target path /home/somebody/Arrow/QCA to $SDK_ROOT
cat eos/xtensa/config/tunable_input.txt | sed 's@\/home\/somebody\/Arrow\/QCA\/4010.tx.1.1_sdk\/target@'"$SDK_ROOT"'@g' > ./tool/tunable/tunable_input.txt

Compile an application
cd eos/xtensa
make

or 

make API_KEY="xxxx" SEC_KEY="yyyyyy"

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
