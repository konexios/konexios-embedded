## Linux ##

This linux project is just one way of using an SDK. 
Dependens is a libsensors4-devel package; 

You can install this library on SUSE like distribution:
zypper in libsensors4-devel
or
apt-get install libsensors4-dev
for Debian like distribution

Actually I wrote it for my laptop. I have a two temperature sensors on CPUs.
There is no guarantee that this application will work correctly with your machine.

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

cd linux
make

or

make API_KEY="xxxx" SEC_KEY="yyyyyy"

## Execute ##
./linux_arrow.elf

