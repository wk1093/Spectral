#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <stdio.h>

#ifdef _WIN32
#define CEXPORT extern "C" __declspec(dllexport)
#else
#define CEXPORT extern "C"
#endif

#ifdef _WIN32
const char* outputSuffix = ".dll";
#else
const char* outputSuffix = ".so";
#endif

const char* spectralSuffix = ".splmod";


struct Module {
    // inherited
};

struct DynamicLibrary {
#ifdef _WIN32
    HINSTANCE handle;
#else
    void* handle;
#endif
    DynamicLibrary(const char* path) {
        load(path);
    }

    private:
        void load(const char* path_in) {
        // add a single . to the end
        char* path = (char*)malloc(strlen(path_in) + 2);
        strcpy(path, path_in);
        strcat(path, ".");
#ifdef _WIN32
        handle = LoadLibraryA(path);
#else
        handle = dlopen(path, RTLD_LAZY);
#endif
        if (!handle) {
            printf("Error loading library %s\n", path);
#ifdef _WIN32
            printf("Error code: %d\n", GetLastError());
#else
            printf("Error: %s\n", dlerror());
#endif
        }
        free(path);
    }
public:

    DynamicLibrary(const char* path, const char* ident) {
        // "modules/" + ident + "_" + path + EXTENSION
        char* fullPath = (char*)malloc(9 + strlen(ident) + 1 + strlen(path) + strlen(spectralSuffix) + 1);
        sprintf(fullPath, "modules/%s_%s%s", ident, path, spectralSuffix);

        load(fullPath);
    }

    ~DynamicLibrary() {
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
    }

    void* getSymbol(const char* name) {
        void* sym;
#ifdef _WIN32
        sym = (void*)GetProcAddress(handle, name);
#else
        sym = dlsym(handle, name);
#endif
        if (!sym) {
            printf("Error loading symbol %s\n", name);
#ifdef _WIN32
            printf("Error code: %d\n", GetLastError());
#else
            printf("Error: %s\n", dlerror());
#endif
        }
        return sym;
    }

    bool valid() {
#ifdef _WIN32
        return handle != NULL;
#else
        return handle != NULL;
#endif
    }
};