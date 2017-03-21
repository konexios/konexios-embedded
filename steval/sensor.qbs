import qbs
import qbs.FileInfo

Project {
    property string version: "0.1"
    property string location: "no"

CppApplication {
    property string projectName: ("sensor-").concat(project.version)
    name: projectName+".elf"
    property string target: "STM32L476xx"
    type: ["application", "elf"]
    files: [ "Projects/SensorTile/Applications/DataLog/Src/*.c" ]

    property stringList commonDefines: [
        "STM32L4",
        target,
        "DEVICE_I2C=1",
        "DEVICE_I2CSLAVE=1",
        "DEVICE_PORTINOUT=1",
        "DEVICE_RTC=1",
        "TOOLCHAIN_object",
        "__CMSIS_RTOS",
        "DEVICE_ANALOGOUT=1",
        "TOOLCHAIN_GCC",
        "DEVICE_CAN=1",
        "ARM_MATH_CM4",
        "DEVICE_PWMOUT=1",
        "DEVICE_INTERRUPTIN=1",
        "DEVICE_PORTOUT=1",
        "DEVICE_STDIO_MESSAGES=1",
        "__FPU_PRESENT=1",
        "DEVICE_PORTIN=1",
        "DEVICE_SERIAL_FC=1",
        "DEVICE_SLEEP=1",
        "TOOLCHAIN_GCC_ARM",
        "DEVICE_SPI=1",
        "DEVICE_SPISLAVE=1",
        "DEVICE_ANALOGIN=1",
        "DEVICE_SERIAL=1",
        "USE_HAL_DRIVER"
    ].concat(project.location=="yes"?["GPS_LOC"]:[])

    Properties {
        condition: qbs.buildVariant == "release"
        cpp.optimization: "small"
        cpp.defines: commonDefines.concat("NDEBUG")
    }
    cpp.defines: commonDefines.concat("DEBUG")
    cpp.optimization: "none"
    cpp.positionIndependentCode: false
    cpp.visibility: "disable"
    cpp.includePaths: [
        "Projects/SensorTile/Applications/DataLog/Inc",
        "Drivers/STM32L4xx_HAL_Driver/Inc/",
        "Drivers/CMSIS/Device/ST/STM32L4xx/Include",
        "Drivers/CMSIS/Include/",
        "Drivers/BSP/SensorTile/",
        "Drivers/BSP/Components/Common/",
        "Drivers/BSP/Components/hts221",
        "Drivers/BSP/Components/lps22hb",
        "Drivers/BSP/Components/lsm303agr",
        "Drivers/BSP/Components/lsm6dsm",
        "Drivers/BSP/Components/pcm1774",
        "Middlewares/ST/STM32_USB_Device_Library/Core/Inc/",
        "Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/",
        "Middlewares/Third_Party/FatFs/src/",
        "Middlewares/Third_Party/FatFs/src/drivers/"
    ]
//    cpp.libraryPaths: [ mbed_path ]
//    cpp.staticLibraries: []

    cpp.dynamicLibraries: [
        "m", "c",
        "gcc",
        "nosys"
    ]

    //cpp.cLanguageVersion: "gnu99"
//    cpp.cxxLanguageVersion: "c++98"
    cpp.allowUnresolvedSymbols: false
    cpp.useRPaths: false

    cpp.linkerScripts: "Projects/SensorTile/Applications/DataLog/SW4STM32/STM32L4xx-SensorTile/LinkerScript.ld"
    cpp.linkerName: "gcc"

    cpp.linkerFlags : [
        "-Wl,--gc-sections",
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfpu=fpv4-sp-d16",
        "-flto",
        "-ffunction-sections","-Wl,--gc-sections",
        "-mfloat-abi=softfp"
    ]

    property stringList commonFlags: [
        "-c",
        "-Wno-unused-parameter",
        "-Wno-missing-field-initializers",
        "-fmessage-length=0",
        "-fno-exceptions",
        "-fno-builtin",
        "-ffunction-sections", "-Wl,--gc-sections",
        "-fdata-sections",
        "-funsigned-char",
        "-fno-delete-null-pointer-checks",
        "-fomit-frame-pointer",
        "-flto",
        "-fno-asynchronous-unwind-tables",
        "-Wl,--strip-all",
        "-MMD", "-MP"
        ]

    cpp.cxxFlags: [
        "-std=gnu++98",
        "-fno-rtti"
    ].concat(commonFlags);

    cpp.cFlags: [
        "-std=gnu99"
    ].concat(commonFlags);

    cpp.commonCompilerFlags: ["-mcpu=cortex-m4", "-mthumb", "-mfpu=fpv4-sp-d16", "-mfloat-abi=softfp"]

    Group {
            name: "HAL_Driver"
            prefix: "Drivers/STM32L4xx_HAL_Driver/Src/"
            files: [
                "*.c"
            ]

        }
    Group {
        name: "BSP"
        prefix: "Drivers/BSP/SensorTile/"
        excludeFiles: [ "SensorTile_BlueNRG.c" ]
        files: [
            "*.c",
            "*.h"
        ]
    }

    Group {
            name: "lsm303agr"
            prefix: "Drivers/BSP/Components/lsm303agr/"
            files: [
                "*.h",
                "*.c"
            ]

        }

    Group {
            name: "lsm6dsm"
            prefix: "Drivers/BSP/Components/lsm6dsm/"
            files: [
                "*.h",
                "*.c"
            ]

        }

        Group {
            name: "lps22hb"
            prefix: "Drivers/BSP/Components/lps22hb/"
            files: [
                "*.c",
                "*.h"
            ]
        }

        Group {
            name: "hts221"
            prefix: "Drivers/BSP/Components/hts221/"
            files: [
                "*.h",
                "*.c"
            ]
        }

    Group {
        name: "sturtup"
        prefix: "Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/gcc/"
        files: [ "startup_stm32l476xx.s" ]
    }

    Group {
        name: "STM32_USB_Device_Library"
        prefix: "Middlewares/ST/STM32_USB_Device_Library/"
        excludeFiles: ["Core/Src/usbd_conf_template.c"]
        files: [ "Class/CDC/Src/*.c", "Core/Src/*.c" ]
    }

    Group {
        name: "FatFs"
        prefix: "Middlewares/Third_Party/FatFs/src/"
        files: [
            "ff_gen_drv.c",
            "diskio.c",
            "ff.c",
            "drivers/sd_diskio.c",
            "option/ccsbcs.c",
            "option/syscall.c"
        ]
    }


    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: "/opt/flash/"
    }

    property string objcopy: cpp.objcopyName

    Rule {
        multiplex: true
        inputs: "application"
        Artifact {
            fileTags: "elf"
            filePath: product.name+".bin"
        }
        outputFileTags: "bin"
        prepare: {
            var args = ["-O", "binary", input.filePath, output.filePath];
            var cmd = new Command(product.objcopy, args);
            cmd.description = "elf converting to bin";
            return cmd;
        }
    }
}
}
