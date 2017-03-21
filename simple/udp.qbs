import qbs

Project {
  CppApplication {
    type: "application"
    consoleApplication: true
    cpp.includePaths: ["include"]
    cpp.dynamicLibraries: ["pthread"]

    files: [
      "main.c"
    ]
  }
}
