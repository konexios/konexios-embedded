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
    property string target: "TARGET_LINUX"
    //property string mbed_path: FileInfo.joinPaths(product.sourceDirectory, "mbed/"+target+"/TOOLCHAIN_GCC_ARM/")
    type: ["application", "elf"]
    files: [ "*.h", "*.cpp" , "*.c" ]
    Depends { name: "kronos-c-sdk" }

    property stringList commonDefines: [
        "TOOLCHAIN_GCC"
    ].concat(project.location=="yes"?["GPS_LOC"]:[])

    Properties {
        condition: qbs.buildVariant == "release"
        cpp.optimization: "small"
        cpp.defines: commonDefines.concat("DEBUG")
    }
    cpp.defines: commonDefines.concat("DEBUG");
    cpp.optimization: "none"
    cpp.positionIndependentCode: false
    cpp.visibility: "disable"
    cpp.includePaths: [ "." ]
//    cpp.libraryPaths: [ ]
//    cpp.staticLibraries: [  ]

    cpp.dynamicLibraries: [
        "stdc++",
        "supc++",
        "m", "c",
        "gcc",
        "sensors"
    ]

    //cpp.cLanguageVersion: "gnu99"
//    cpp.cxxLanguageVersion: "c++98"
    cpp.allowUnresolvedSymbols: false
    cpp.useRPaths: false

//    cpp.linkerScripts: mbed_path + "/STM32F401XE.ld"
    cpp.linkerName: "gcc"

//    cpp.linkerFlags : [ ]

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

//    cpp.commonCompilerFlags: ["-mcpu=cortex-m4", "-mthumb"]

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
