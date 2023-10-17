#pragma once

// this file is the module.h for the scriptloading module
// defines common types and functions for the scriptloading module
// everything that is "language-specfic" is in the dll file, and this file is the interface to that


#include "../moduleLib.h"
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#define CEXPORT extern "C" __declspec(dllexport)
#else
#define CEXPORT extern "C"
#endif

typedef void (*ScriptInit)();
typedef void (*ScriptUpdate)(float dt);

struct Script {
    void* internal;
    ScriptInit init;
    ScriptUpdate update;
};

// TODO: module loader
namespace scrload {
    typedef Script (*ScriptLoader)(const char *path);

    typedef void (*ScriptCompiler)(const char *scriptPath, const char *outputPath);

    typedef void (*MultiScriptCompiler)(const char **paths, size_t paths_num, const char *outputPath);
}
struct ScriptLoaderModule : public Module {
    scrload::ScriptLoader loadScript;
    scrload::ScriptCompiler compileScript;
    scrload::MultiScriptCompiler compileScripts;
    char* inputExtension;
    char* outputExtension;
    DynamicLibrary lib;

    explicit ScriptLoaderModule(const char* dynlib) : lib(dynlib) {
        // TODO: load functions and extensions
        loadScript = (scrload::ScriptLoader)lib.getSymbol("loadScript");
        compileScript = (scrload::ScriptCompiler)lib.getSymbol("compileScript");
        compileScripts = (scrload::MultiScriptCompiler)lib.getSymbol("compileScripts");
        auto iext = (const char**)lib.getSymbol("inputExtension");
        auto oext = (const char**)lib.getSymbol("outputExtension");
        // copy strings because the dll will be unloaded
        inputExtension = (char*)malloc(strlen(*iext) + 1);
        outputExtension = (char*)malloc(strlen(*oext) + 1);
        strcpy((char*)inputExtension, *iext);
        strcpy((char*)outputExtension, *oext);
    }
};