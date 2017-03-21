ST-LINK programmer - https://github.com/texane/stlink.git
Use it for flashing Nucleo board and SensorTile Cradle

flashing The Nucleo board
st-flash write arrow-0.1.elf.bin 0x8000000

arrow-0.1.elf.bin - mbed firmware


flashing The SensorTile
in a folder steval/Utilities/BootLoader/STM32L476RG
st-flash write BootLoaderL4.bin 0x08000000

and

in a folder steval/Projects/SensorTile/Applications/DataLog/gcc after compiling st firmware
st-flash write .build/DataLog_USB_gcc.bin 0x08004000

DataLog_USB_gcc.bin - stm32cube based firmware


