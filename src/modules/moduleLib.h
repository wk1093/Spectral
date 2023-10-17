#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <stdio.h>

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
        sym = GetProcAddress(handle, name);
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
};