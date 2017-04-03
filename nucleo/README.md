## Preparation ##

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

## Build ##

And also you need to export the right PATH for compiler (arm-none-eabi-gcc)
export PATH=$PATH:/opt/gcc-arm-none-eabi-5_2-2015q4/bin/

I use the following compiler:
arm-none-eabi-gcc -v
Using built-in specs.
COLLECT_GCC=arm-none-eabi-gcc
COLLECT_LTO_WRAPPER=/home/danila/gcc-arm-none-eabi-5_2-2015q4/bin/../lib/gcc/arm-none-eabi/5.2.1/lto-wrapper
Target: arm-none-eabi
Configured with: /home/build/work/GCC-5-0-build/src/gcc/configure --target=arm-none-eabi --prefix=/home/build/work/GCC-5-0-build/install-native --libexecdir=/home/build/work/GCC-5-0-build/install-native/lib --infodir=/home/build/work/GCC-5-0-build/install-native/share/doc/gcc-arm-none-eabi/info --mandir=/home/build/work/GCC-5-0-build/install-native/share/doc/gcc-arm-none-eabi/man --htmldir=/home/build/work/GCC-5-0-build/install-native/share/doc/gcc-arm-none-eabi/html --pdfdir=/home/build/work/GCC-5-0-build/install-native/share/doc/gcc-arm-none-eabi/pdf --enable-languages=c,c++ --enable-plugins --disable-decimal-float --disable-libffi --disable-libgomp --disable-libmudflap --disable-libquadmath --disable-libssp --disable-libstdcxx-pch --disable-nls --disable-shared --disable-threads --disable-tls --with-gnu-as --with-gnu-ld --with-newlib --with-headers=yes --with-python-dir=share/gcc-arm-none-eabi --with-sysroot=/home/build/work/GCC-5-0-build/install-native/arm-none-eabi --build=i686-linux-gnu --host=i686-linux-gnu --with-gmp=/home/build/work/GCC-5-0-build/build-native/host-libs/usr --with-mpfr=/home/build/work/GCC-5-0-build/build-native/host-libs/usr --with-mpc=/home/build/work/GCC-5-0-build/build-native/host-libs/usr --with-isl=/home/build/work/GCC-5-0-build/build-native/host-libs/usr --with-cloog=/home/build/work/GCC-5-0-build/build-native/host-libs/usr --with-libelf=/home/build/work/GCC-5-0-build/build-native/host-libs/usr --with-host-libstdcxx='-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm' --with-pkgversion='GNU Tools for ARM Embedded Processors' --with-multilib-list=armv6-m,armv7-m,armv7e-m,armv7-r,armv8-m.base,armv8-m.main
Thread model: single
gcc version 5.2.1 20151202 (release) [ARM/embedded-5-branch revision 231848] (GNU Tools for ARM Embedded Processors)

After this:
cd nucleo
make

or

make API_KEY="xxxx" SEC_KEY="yyyyyy"

## Flash ##

ST-LINK programmer - https://github.com/texane/stlink.git
Use it for flashing Nucleo board and SensorTile Cradle

flashing The Nucleo board
st-flash write arrow-0.1.elf.bin 0x8000000

arrow-0.1.elf.bin - mbed firmware

or

make flash


flashing The SensorTile
in a folder steval/Utilities/BootLoader/STM32L476RG
st-flash write BootLoaderL4.bin 0x08000000

and

in a folder steval/Projects/SensorTile/Applications/DataLog/gcc after compiling st firmware
st-flash write .build/DataLog_USB_gcc.bin 0x08004000

DataLog_USB_gcc.bin - stm32cube based firmware


