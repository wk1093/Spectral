
// cross platform dynamic library runtime loading
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "module.h"


CEXPORT const char* inputExtension = ".cpp";
CEXPORT const char* outputExtension = outputSuffix;


typedef void (*ScriptInit)();
typedef void (*ScriptUpdate)(float dt);

struct DynamicScript { // represent a dynamic library
#ifdef _WIN32
    HINSTANCE handle;
#else
    void* handle;
#endif
};

char* getBaseFilename(const char* path) {
    // give a/b/name.ext.b, return name.ext
    const char* base = strrchr(path, '/');
    if (!base) {
        base = path;
    } else {
        base++;
    }
    char* name = (char*)malloc(strlen(base) + 1);
    strcpy(name, base);
    char* dot = strrchr(name, '.');
    if (dot) {
        *dot = '\0';
    }
    return name;
}

CEXPORT Script loadScript(const char* path, const char* scriptName) {

    // this mangling works with gcc, idk about other compilers, so I display an error message on others
#if defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__) || defined(__clang__)
    // _ZN10testScript4initEv
    // _ZN + length of name + 4initEV
    // _ZN10testScript6updateEf
    // _ZN + length of name + 6updateEf
    char* length = (char*)malloc(10);
    sprintf(length, "%lu", strlen(scriptName));


    char* initName = (char*)malloc(strlen(scriptName)+strlen(length)+11);
    strcpy(initName, "_ZN");
    strcat(initName, length);
    strcat(initName, scriptName);
    strcat(initName, "4initEv");
    char* updateName = (char*)malloc(strlen(scriptName)+strlen(length)+13);
    strcpy(updateName, "_ZN");
    strcat(updateName, length);
    strcat(updateName, scriptName);
    strcat(updateName, "6updateEf");
#else
#error "This compiler is not supported"
#endif


#ifdef _WIN32
    HINSTANCE handle = LoadLibrary(path);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", GetLastError());
    }
    DynamicScript dscript{handle};
    Script script = {(void*)&dscript};
    script.init = (ScriptInit)GetProcAddress(handle, initName);
    if (!script.init) {
        fprintf(stderr, "Error: %s\n", GetLastError());
    }
    script.update = (ScriptUpdate)GetProcAddress(handle, updateName);
    if (!script.update) {
        fprintf(stderr, "Error: %s\n", GetLastError());
    }
#else
    void* handle = dlopen(path, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
    }
    DynamicScript dscript{handle};
    Script script = {(void*)&dscript};
    script.init = (ScriptInit)dlsym(handle, initName);
    if (!script.init) {
        fprintf(stderr, "Error: %s\n", dlerror());
    }
    script.update = (ScriptUpdate)dlsym(handle, updateName);
    if (!script.update) {
        fprintf(stderr, "Error: %s\n", dlerror());
    }
#endif
    return script;
}

CEXPORT void compileScript(const char* scriptPath, const char* outputPath, const char* includeDir) {
    // given location of cpp file, compile it to a shared library
    char* command = (char*)malloc(strlen(scriptPath) + strlen(outputPath) + 100);
#ifndef SCRIPT_INCLUDE_DIR
    sprintf(command, "gcc -I%s -fPIC -shared -o %s %s", includeDir, outputPath, scriptPath);
#else
    sprintf(command, "gcc -I%s -I%s -fPIC -shared -o %s %s", includeDir, SCRIPT_INCLUDE_DIR, outputPath, scriptPath);
#endif
    system(command);

}

CEXPORT void compileScripts(const char** paths, size_t paths_num, const char* outputPath, const char* includeDir) {
    // given location of cpp files, compile it to a shared library
    size_t paths_size = 1;
    for (int i = 0; i < paths_num; i++) {
        paths_size += strlen(paths[i]) + 1;
    }
    char* paths_str = (char*)malloc(paths_size);
    paths_str[0] = '\0';
    // put them all together with a space in between
    for (int i = 0; i < paths_num; i++) {
        strcat(paths_str, paths[i]);
        strcat(paths_str, " ");
    }
    char* command = (char*)malloc(strlen(paths_str) + strlen(outputPath) + 100);
#ifndef SCRIPT_INCLUDE_DIR
    sprintf(command, "gcc -I%s -fPIC -shared -o %s %s", includeDir, outputPath, paths_str);
#else
    sprintf(command, "gcc -I%s -I%s -fPIC -shared -o %s %s", includeDir, SCRIPT_INCLUDE_DIR, outputPath, paths_str);
#endif
    system(command);
}