import qbs
import qbs.Environment
import qbs.FileInfo

Project {
    property string location: "no"
    references: [
        "../kronos-c-sdk/kronos-c-sdk.qbs"
    ]

CppApplication {
    property string projectName: ("iot_arrow")
    name: projectName+".elf"
    property string target: "QCA4010"
    property string FW: Environment.getEnv("FW")
    property string THREADXDIR: Environment.getEnv("THREADXDIR")
    property string INTERNALDIR: Environment.getEnv("INTERNALDIR")
    property string IMAGEDIR: Environment.getEnv("IMAGEDIR")
    property string LIBDIR: Environment.getEnv("LIBDIR")
    type: ["application", "elf"]
    files: [ "*.h", "*.c" ]

//     " -TARG:unaligned_loads=1 \"
//    -include $(INTERNALDIR)/include/fwconfig_AR6006.h \
//    -Wno-return-type \
    property stringList commonDefines: [
        "__XCC__", // useless
        "AR6K",
        "AR6002",
        "IOT_BUILD_FLAG",
        "AR6002_REV7",
        "ATHOS",
        "P2P_ENABLED",
        "SWAT_WMICONFIG_P2P",
        "APP_P2P_SUPPORT",
        "ATH_TARGET",
        "SWAT_WMICONFIG_MISC_EXT",
        "SWAT_SSL",
        "SWAT_I2C",
        "SWAT_I2S",
        "SWAT_WMICONFIG_SNTP",
        "SWAT_OTA",
        "ENABLE_HTTP_CLIENT",
        "ENABLE_HTTP_SERVER",
        "SWAT_DNS_ENABLED",
        "HTTP_ENABLED",
        "DNS_ENABLED",
        "BRIDGE_ENABLED",
        "CONFIG_HOSTLESS",
        "SSL_ENABLED",
        "SWAT_CRYPTO",
        "SWAT_BENCH_RAW",
        "ENABLE_JSON",
        "SSDP_ENABLED",
        "NUM_OF_VDEVS=2",
        "AR6002_REV76"
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
        "../kronos-c-sdk",
        "../kronos-c-sdk/include",
        "../kronos-c-sdk/src/wolfSSL",
        "../kronos-c-sdk/src/wolfSSL/wolfssl",
        FW+"/include/AR6002/hw/include",
        FW+"/include/AR6002/hw/include",
        FW+"/include/qcom",
        FW+"/include",
        FW+"/include/AR6002/",
        FW+"/include/AR6K/",
        THREADXDIR+"/include",
        INTERNALDIR+"/include",
        INTERNALDIR+"/include/os",
        INTERNALDIR+"/include/whal",
        FW+"/demo/ezxm"
    ]
//    cpp.libraryPaths: [ LIBDIR ]
//    cpp.staticLibraries: [
//        "mbed"
//    ]

    cpp.dynamicLibraries: [
        "gcc"
    ]

    property stringList libs: [
        LIBDIR+"/lib${APP_NAME}.a",
        LIBDIR+"/libadc.a",
        LIBDIR+"/libramcust.a",
        LIBDIR+"/libserport.a",
        LIBDIR+"/libuartserp.a",
        LIBDIR+"/libconsole.a",
        LIBDIR+"/libpatches_iot.a",
        LIBDIR+"/libpatches.a",
        LIBDIR+"/libhost_dset_api.a",
        LIBDIR+"/libhttppatches.a",
        LIBDIR+"/libsntppatches.a",
        LIBDIR+"/libdnspatches.a",
        LIBDIR+"/libotapatches.a",
        LIBDIR+"/libstrrclpatches.a",
        LIBDIR+"/libipv6patches.a",
        LIBDIR+"/libsslpatches.a",
        LIBDIR+"/libqcomcrypto.a",
        LIBDIR+"/libcryptopatches.a",
        LIBDIR+"/libcrypto_mgmt.a",
        LIBDIR+"/libcrypto_aes.a",
        LIBDIR+"/libcrypto_chacha20_poly1305.a",
        LIBDIR+"/libcrypto_des.a",
        LIBDIR+"/libcrypto_srp.a",
        LIBDIR+"/libcrypto_hmacsha384.a",
        LIBDIR+"/libcrypto_hmacsha256.a",
        LIBDIR+"/libcrypto_hmacsha1.a",
        LIBDIR+"/libwhalpatches.a",
        LIBDIR+"/libjson.a",
        LIBDIR+"/lib_ezxml.a",
        LIBDIR+"/libssdp.a"
    ]

    //cpp.cLanguageVersion: "gnu99"
//    cpp.cxxLanguageVersion: "c++98"
    cpp.allowUnresolvedSymbols: false
    cpp.useRPaths: false

    cpp.linkerPath: "sdk_shell.ld"
    cpp.linkerName: "gcc"

    cpp.linkerFlags : [
        "-g -nostdlib -Wl,-EL -Wl,--gc-sections",
        "-u app_start",
        "-Wl,-static",
        "-Wl,--start-group"
    ], libs, [
        "-Wl,--end-group",
        "-mthumb",
        IMAGEDIR+"/rom.addrs.ld"
    ]

    property stringList commonFlags: [
        "-g3",
        "-O3",
        "-Wall",
        "-Wpointer-arith",
        "-Wundef",
        "-Werror",
        "-Wl,-EL",
        "-fno-inline-functions",
        "-nostdlib",
        "-mlongcalls",
        "-ffunction-sections"
        ]

    cpp.cxxFlags: [
//        "-std=gnu++98",
//        "-fno-rtti"
    ].concat(commonFlags);

    cpp.cFlags: [
//        "-std=gnu99"
    ].concat(commonFlags);

    cpp.commonCompilerFlags: []

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

//    Group {
//            name: "firmware"
////            fileTags: "cpp"
////            prefix: "../kronos-c-sdk/"
//            files: [
//                "*.h",
//                "*.c"
//            ]
//        }

//    Group {
//        name: "iks"
//        files: [
//            "X_NUCLEO_COMMON/D*/*.h",
//            "Components/*/*.h",
//            "Components/*/*.cpp"
//        ]
//    }

//    Group {
//            name: "HTTP"
//            fileTags: "cpp"
//            excludeFiles: ["include/http/miniserver.h", "src/http/miniserver.c"]
//            prefix: "../kronos-c-sdk/"
//            files: [
//                "include/http/*.h",
//                "src/http/*.c"
//            ]
//        }

//    Group {
//            name: "mqtt"
//            fileTags: "cpp"
//            prefix: "../kronos-c-sdk/"
//            files: [
//                "include/mqtt/client/MQTTClient.h",
//                "include/mqtt/client/MQTTmbed.h",
//                "include/mqtt/packet/*.h",
//                "src/mqtt/client/src/*.c",
//                "src/mqtt/packet/src/*.c",
//                "src/mqtt/client/src/mbed/*.c"
//            ]
//        }

//        Group {
//            name: "arrow"
//            fileTags: "cpp"
//            prefix: "../kronos-c-sdk/"
//            files: [
//                "src/arrow/*.c",
//                "include/arrow/*.h",
//                "include/config.h"
//            ]
//        }

//        Group {
//            name: "json"
//            fileTags: "cpp"
//            prefix: "../kronos-c-sdk/"
//            files: [
//                "src/json/*.c",
//                "include/json/*.h"
//            ]
//        }

//        Group {
//            name: "ssl"
//            prefix: "../kronos-c-sdk/src/wolfSSL/"
//            files: [
//                "*.h",
//                "src/*.c",
//                "wolfcrypt/src/*.c",
//                "wolfssl/*.h",
//                "wolfssl/wolfcrypt/*.h"
//            ]
//        }

//    Group {
//        name: "net"
//        //prefix: ""
//        files: [ "NetworkSocketAPI/*.h",
//            "NetworkSocketAPI/*.cpp",
//            "DnsQuery/*.h",
//            "DnsQuery/*.cpp"
//        ]
//    }

//    Group {
//        name: "wifi"
//        prefix: "X_NUCLEO_IDW01M1/"
//        files: [ "*.h", "*.cpp",
//            "Spwf/*.c", "Spwf/inc/*.h", "Spwf/utils/*.h", "Spwf/utils/*.c",
//            "Spwf_API/*.h", "Spwf_API/*.cpp", "Spwf_API/utils/*.h", "Spwf_API/utils/*.cpp" ]
//    }


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
