import qbs
import qbs.FileInfo

Project {
    property string version: "0.1"
    property string location: "no"

    references: [
        "../acn-sdk-c/kronos-c-sdk.qbs"
    ]

CppApplication {
    property string projectName: ("arrow-").concat(project.version)
    name: projectName+".elf"
    property string target: "TARGET_NUCLEO_F401RE"
    property string mbed_path: FileInfo.joinPaths(product.sourceDirectory, "mbed/"+target+"/TOOLCHAIN_GCC_ARM/")
    type: ["application", "elf"]
    Depends { name: "kronos-c-sdk" }
    files: [ "*.h", "*.cpp" , "*.c" ]

    property stringList commonDefines: [
        target,
        "DEVICE_I2CSLAVE=1",
        "TARGET_LIKE_MBED",
        "TARGET_RTOS_M4_M7",
        "TARGET_FF_ARDUINO",
        "TARGET_STM32F4",
        "TARGET_CORTEX_M",
        "TARGET_LIKE_CORTEX_M4",
        "TARGET_M4",
        "TARGET_UVISOR_UNSUPPORTED",
        "TARGET_STM32F401RE",
        "TARGET_FF_MORPHO",
        "TARGET_RELEASE",
        "TARGET_STM",
        "__CORTEX_M4",
        "DEVICE_PWMOUT=1",
        "DEVICE_INTERRUPTIN=1",
        "DEVICE_I2C=1",
        "DEVICE_PORTOUT=1",
        "DEVICE_STDIO_MESSAGES=1",
        "DEVICE_PORTIN=1",
        "DEVICE_SERIAL_FC=1",
        "DEVICE_PORTINOUT=1",
        "DEVICE_SLEEP=1",
        "DEVICE_SPI=1",
        "DEVICE_ERROR_RED=1",
        "DEVICE_SPISLAVE=1",
        "DEVICE_ANALOGIN=1",
        "DEVICE_SERIAL=1",
        "TOOLCHAIN_GCC",
        "TOOLCHAIN_GCC_ARM",
        "TOOLCHAIN_object",
        "DEVICE_RTC=1",
        "__MBED__=1",
        "__CMSIS_RTOS",
        "__FPU_PRESENT=1",
        "__MBED_CMSIS_RTOS_CM",
        "MBED_BUILD_TIMESTAMP=1471326725.12",
        "ARM_MATH_CM4"
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
    cpp.includePaths: [".",
        "../kronos-c-sdk/include",
        "../kronos-c-sdk/src/wolfSSL",
        "../kronos-c-sdk/src/wolfSSL/wolfssl",
        "mbed-rtos",
        "mbed-rtos/rtos",
        "mbed-rtos/rtx/TARGET_CORTEX_M",
        "mbed",
        "mbed/"+target,
        "mbed/"+target+"/TARGET_STM",
        "mbed/"+target+"/TARGET_STM/TARGET_STM32F4",
        "mbed/"+target+"/TARGET_STM/TARGET_STM32F4/TARGET_NUCLEO_F401RE",
        "X_NUCLEO_IDW01M1",
        "X_NUCLEO_IDW01M1/Spwf/inc", "X_NUCLEO_IDW01M1/Spwf/utils", "X_NUCLEO_IDW01M1/Spwf_API",
        "NetworkSocketAPI", "DnsQuery",
        "FP", "MQTTPacket",
        "Components",
        "X_NUCLEO_COMMON/DevI2C",
        "Components/lis3mdl",
        "Components/hts221",
        "Components/Common",
        "Components/Interfaces",
        mbed_path
    ]
    cpp.libraryPaths: [ mbed_path ]
    cpp.staticLibraries: [
        mbed_path + "stm32f4xx_hal_flash_ramfunc.o",
        mbed_path + "board.o",
        mbed_path + "mbed_board.o",
        mbed_path + "cmsis_nvic.o",
        mbed_path + "hal_tick.o",
        mbed_path + "mbed_overrides.o",
        mbed_path + "retarget.o",
        mbed_path + "startup_stm32f401xe.o",
        mbed_path + "stm32f4xx_hal.o",
        mbed_path + "stm32f4xx_hal_adc.o",
        mbed_path + "stm32f4xx_hal_adc_ex.o",
        mbed_path + "stm32f4xx_hal_can.o",
        mbed_path + "stm32f4xx_hal_cec.o",
        mbed_path + "stm32f4xx_hal_cortex.o",
        mbed_path + "stm32f4xx_hal_crc.o",
        mbed_path + "stm32f4xx_hal_cryp.o",
        mbed_path + "stm32f4xx_hal_cryp_ex.o",
        mbed_path + "stm32f4xx_hal_dac.o",
        mbed_path + "stm32f4xx_hal_dac_ex.o",
        mbed_path + "stm32f4xx_hal_dcmi.o",
        mbed_path + "stm32f4xx_hal_dcmi_ex.o",
        mbed_path + "stm32f4xx_hal_dma.o",
        mbed_path + "stm32f4xx_hal_dma2d.o",
        mbed_path + "stm32f4xx_hal_dma_ex.o",
        mbed_path + "stm32f4xx_hal_dsi.o",
        mbed_path + "stm32f4xx_hal_eth.o",
        mbed_path + "stm32f4xx_hal_flash.o",
        mbed_path + "stm32f4xx_hal_flash_ex.o",
        mbed_path + "stm32f4xx_hal_fmpi2c_ex.o",
        mbed_path + "stm32f4xx_hal_fmpi2c.o",
        mbed_path + "stm32f4xx_hal_msp_template.o",
        mbed_path + "stm32f4xx_hal_gpio.o",
        mbed_path + "stm32f4xx_hal_hash.o",
        mbed_path + "stm32f4xx_hal_hash_ex.o",
        mbed_path + "stm32f4xx_hal_hcd.o",
        mbed_path + "stm32f4xx_hal_i2c.o",
        mbed_path + "stm32f4xx_hal_i2c_ex.o",
        mbed_path + "stm32f4xx_hal_i2s.o",
        mbed_path + "stm32f4xx_hal_i2s_ex.o",
        mbed_path + "stm32f4xx_hal_irda.o",
        mbed_path + "stm32f4xx_hal_iwdg.o",
        mbed_path + "stm32f4xx_hal_lptim.o",
        mbed_path + "stm32f4xx_hal_ltdc.o",
        mbed_path + "stm32f4xx_hal_ltdc_ex.o",
        mbed_path + "stm32f4xx_hal_smartcard.o",
        mbed_path + "stm32f4xx_hal_nand.o",
        mbed_path + "stm32f4xx_hal_nor.o",
        mbed_path + "stm32f4xx_hal_pccard.o",
        mbed_path + "stm32f4xx_hal_pcd.o",
        mbed_path + "stm32f4xx_hal_pcd_ex.o",
        mbed_path + "stm32f4xx_hal_pwr.o",
        mbed_path + "stm32f4xx_hal_pwr_ex.o",
        mbed_path + "stm32f4xx_hal_qspi.o",
        mbed_path + "stm32f4xx_hal_rcc.o",
        mbed_path + "stm32f4xx_hal_rcc_ex.o",
        mbed_path + "stm32f4xx_hal_rng.o",
        mbed_path + "stm32f4xx_hal_rtc.o",
        mbed_path + "stm32f4xx_hal_rtc_ex.o",
        mbed_path + "stm32f4xx_hal_sai.o",
        mbed_path + "stm32f4xx_hal_sai_ex.o",
        mbed_path + "stm32f4xx_hal_sd.o",
        mbed_path + "stm32f4xx_hal_sdram.o",
        mbed_path + "stm32f4xx_hal_spdifrx.o",
        mbed_path + "stm32f4xx_hal_spi.o",
        mbed_path + "stm32f4xx_hal_sram.o",
        mbed_path + "stm32f4xx_hal_tim.o",
        mbed_path + "stm32f4xx_hal_tim_ex.o",
        mbed_path + "stm32f4xx_hal_uart.o",
        mbed_path + "stm32f4xx_hal_usart.o",
        mbed_path + "stm32f4xx_hal_wwdg.o",
        mbed_path + "stm32f4xx_ll_fmc.o",
        mbed_path + "stm32f4xx_ll_fsmc.o",
        mbed_path + "stm32f4xx_ll_sdmmc.o",
        mbed_path + "stm32f4xx_ll_usb.o",
        mbed_path + "system_stm32f4xx.o",
        "mbed"
    ]

    cpp.dynamicLibraries: [
        "stdc++",
        "supc++",
        "m", "c",
        "gcc",
        "nosys"
    ]

    //cpp.cLanguageVersion: "gnu99"
//    cpp.cxxLanguageVersion: "c++98"
    cpp.allowUnresolvedSymbols: false
    cpp.useRPaths: false

    cpp.linkerScripts: mbed_path + "/STM32F401XE.ld"
    cpp.linkerName: "gcc"

    cpp.linkerFlags : [
        "-Wl,--gc-sections",
        "-Wl,--wrap,main",
        "-Wl,--wrap,_malloc_r",
        "-Wl,--wrap,_free_r",
        "-Wl,--wrap,_realloc_r",
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

    cpp.commonCompilerFlags: ["-mcpu=cortex-m4", "-mthumb"]

//    Group {
//        name: "mbed-rtos"
//        prefix: name+"/"
//        files: [
//            "rtos/*.cpp",
//            "rtos/*.h",
//            "rtx/TARGET_CORTEX_M/*.c",
//            "rtx/TARGET_CORTEX_M/*.h",
//        ]
//    }

//    Group {
//        name: "assembler"
//        files: "mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_M0P/TOOLCHAIN_GCC/*.S"
//        cpp.compilerName: "gcc"
//    }

    Group {
        name: "iks"
        files: [
            "X_NUCLEO_COMMON/D*/*.h",
            "Components/*/*.h",
            "Components/*/*.cpp"
        ]
    }

    Group {
        name: "net"
        //prefix: ""
        files: [ "NetworkSocketAPI/*.h",
            "NetworkSocketAPI/*.cpp",
            "DnsQuery/*.h",
            "DnsQuery/*.cpp"
        ]
    }

    Group {
        name: "wifi"
        prefix: "X_NUCLEO_IDW01M1/"
        files: [ "*.h", "*.cpp",
            "Spwf/*.c", "Spwf/inc/*.h", "Spwf/utils/*.h", "Spwf/utils/*.c",
            "Spwf_API/*.h", "Spwf_API/*.cpp", "Spwf_API/utils/*.h", "Spwf_API/utils/*.cpp" ]
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
