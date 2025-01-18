#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif
#include <cstdio>
#include <fstream>
#include <string>

#ifndef SPECTRAL_PLATFORM
#define SPECTRAL_PLATFORM "unknown"
#endif

#ifdef _WIN32
#define CEXPORT extern "C" __declspec(dllexport)
#define EXPORT __declspec(dllexport)
#else
#define CEXPORT extern "C"
#define EXPORT
#endif

#include <filesystem>

#include <cstring>

std::filesystem::path getexepath() {
#ifdef _WIN32
    wchar_t path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);
    return path;
#else
#define PATH_MAX 4096
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
#endif
}

std::filesystem::path getexedir() {
    return getexepath().parent_path();
}

bool readFile(const char* path, std::string& out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }
    out = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return true;
}

#ifdef _WIN32
const char* outputSuffix = ".dll";
#else
const char* outputSuffix = ".so";
#endif

const char* spectralSuffix = ".splmod";

struct DynamicLibrary {
#ifdef _WIN32
    HINSTANCE handle;
#else
    void* handle;
#endif
public:
    const char* mod_name;
    const char* mod_imp;

private:
    void load(const char* path_in) {
        // add a single . to the end
#ifdef _WIN32
        char* path = (char*)malloc(strlen(path_in) + 2);
        strcpy(path, path_in);
        strcat(path, ".");
        char* setdlldir = (char*)malloc(strlen(SPECTRAL_PLATFORM) + 5);
        strcpy(setdlldir, "lib/");
        strcat(setdlldir, SPECTRAL_PLATFORM);
        if (!SetDllDirectoryA(setdlldir)) {
            printf("Error setting DLL directory\n");
            printf("Error code: %d\n", GetLastError());
        }
#else
        char* path = (char*)malloc(strlen(path_in) + 1);
        strcpy(path, path_in);
#endif
#ifdef _WIN32
        handle = LoadLibraryA(path);
#else
        handle = dlopen(path, RTLD_LAZY);
#endif
        if (!handle) {
            printf("Error loading library %s\n", path);
            handle = NULL;
#ifdef _WIN32
            printf("Error code: %d\n", GetLastError());
#else
            printf("Error: %s\n", dlerror());
#endif
        }
        free(path);
    }
public:

    DynamicLibrary() {
        mod_name = nullptr;
        mod_imp = nullptr;
        handle = NULL;
    }

    DynamicLibrary(const char* path, const char* ident) {
        mod_name = ident;
        mod_imp = path;
        load(makePath(path, ident));
    }

    static char* makePath(const char* path, const char* ident) {

        char* fullPath = (char*)malloc(10+strlen(ident)*2+strlen(path)+1+strlen(SPECTRAL_PLATFORM)+strlen(spectralSuffix)+10);
        sprintf(fullPath, "modules/%s/%s/%s_%s%s", SPECTRAL_PLATFORM, ident, ident, path, spectralSuffix);
        return fullPath;
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
        if (!handle) {
            printf("Error: Library not loaded\n");
            return NULL;
        }
#ifdef _WIN32
        sym = (void*)GetProcAddress(handle, name);
#else
        sym = dlsym(handle, name);
#endif
        if (!sym) {
            printf("Error loading symbol %s\n", name);
#ifdef _WIN32
            printf("Error code: %ld\n", GetLastError());
#else
            printf("Error: %s\n", dlerror());
#endif
        }
        return sym;
    }

    bool valid() {
        return handle != NULL;
    }
};

struct Module {
    DynamicLibrary lib;
    explicit Module(const char* path, const char* ident) : lib(path, ident) {
        if (!lib.valid()) {
            printf("Error loading module %s\n", path);
            return;
        }
        std::filesystem::path p = getexedir();
        char* pth = DynamicLibrary::makePath(path, ident);
        if (!std::filesystem::exists(p / pth)) {
            printf("Error: Module %s.%s not found\n", ident, path);
            return;
        } else {
            printf("Module %s.%s loaded\n", ident, path);
        }
    }
};