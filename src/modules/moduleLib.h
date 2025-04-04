#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

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

#include "arena.h"

/**
 * @brief Get the path to the current executable.
 * 
 * @todo Ensure this works on all platforms
 * 
 * @return The path to the current executable.
 */
inline std::filesystem::path getexepath() {
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

/**
 * @brief Get the directory of the current executable.
 * 
 * @return The directory containing the current executable.
 */
inline std::filesystem::path getexedir() {
    return getexepath().parent_path();
}

/**
 * @brief Read a file into a string.
 * 
 * A helper function to read a file into a string.
 * 
 * @param path The path to the file.
 * @param out The string to store the contents of the file.
 * 
 * @return True if the file was read successfully, false otherwise.
 */
inline bool readFile(const char* path, std::string& out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }
    out = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return true;
}

/// @cond DOXYGEN_OMIT
#ifdef _WIN32
const char* outputSuffix = ".dll";
#else
const char* outputSuffix = ".so";
#endif

const char* spectralSuffix = ".splmod";
/// @endcond

/**
 * @brief Dynamic library loader class.
 * 
 * This class is used to load dynamic libraries at runtime.
 * It provides functions to load a library, get a symbol from the library, and check if the library is valid.
 */
struct DynamicLibrary {
#ifdef _WIN32
    HINSTANCE handle;
#else
    void* handle;
#endif
public:
    /// @brief The name of the module. tex, shdr, gfx, win, etc.
    const char* mod_name;
    /// @brief The implementation name of the module. stb (tex_stb), glad (gfx_glad), etc.
    const char* mod_imp;

private:
    inline void load(const char* path_in) {
#ifdef _WIN32
        char* path = (char*)malloc(strlen(path_in) + 2);
        strcpy(path, path_in);
        strcat(path, ".");
        char* setdlldir = (char*)malloc(strlen(SPECTRAL_PLATFORM) + 7);
        strcpy(setdlldir, "./lib/");
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
            printf("libdir: %s\n", setdlldir);
#else
            printf("Error: %s\n", dlerror());
#endif
        }
        free(path);
        free(setdlldir);
    }
public:

    /**
     * @brief Default constructor for the DynamicLibrary class.
     * 
     * This constructor initializes the module name, implementation name, and handle to NULL.
     * @note This constructor does not load a library. And the resulting object is invalid.
     */
    inline DynamicLibrary() {
        mod_name = nullptr;
        mod_imp = nullptr;
        handle = NULL;
    }

    /**
     * @brief Constructor for the DynamicLibrary class.
     * 
     * This constructor loads a library from the specified path and sets the module name and implementation name.
     * @param path The implementation name of the module. This is the name of the module to load.
     * @param ident The name of the module. This is the type of the module to load.
     */
    inline DynamicLibrary(const char* path, const char* ident) {
        mod_name = ident;
        mod_imp = path;
        char* p = makePath(path, ident);
        load(p);
        free(p);
    }

    /**
     * @brief Create a valid path to a module from the module type and implementation name.
     * 
     * This function creates a valid path to a module from the module type and implementation name.
     * The path is in the format "modules/<platform>/<ident>/<ident>_<path>_<platform>.splmod".
     * 
     * @param path The implementation name of the module. This is the name of the module to load.
     * @param ident The name of the module. This is the type of the module to load.
     * @return A path to the module file.
     */
    inline static char* makePath(const char* path, const char* ident) {

        char* fullPath = (char*)malloc(10+strlen(ident)*2+strlen(path)+1+strlen(SPECTRAL_PLATFORM)+strlen(spectralSuffix)+10);
        sprintf(fullPath, "modules/%s/%s/%s_%s%s", SPECTRAL_PLATFORM, ident, ident, path, spectralSuffix);
        return fullPath;
    }

    /**
     * @brief Destructor for the DynamicLibrary class.
     */
    ~DynamicLibrary() {
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
    }

    /**
     * @brief Get a symbol from the loaded library.
     * 
     * This function retrieves a symbol from the loaded library.
     * It returns a pointer to the symbol if it is found, or NULL if it is not found.
     * 
     * @param name The name of the symbol to retrieve.
     * @return A pointer to the symbol if it is found, or NULL if it is not found.
     */
    inline void* getSymbol(const char* name) {
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

    /**
     * @brief Check if the library is valid.
     * 
     * This function checks if the library is valid by checking if the handle is not NULL.
     * 
     * @return True if the library is valid, false otherwise.
     */
    inline bool valid() {
        return handle != NULL;
    }
};

/**
 * @brief A module structure.
 * 
 * This structure is a small wrapper around the DynamicLibrary that creates a standardized interface for modules.
 */
struct Module {
    /// @brief The dynamic library handle for the module.
    DynamicLibrary lib;
    /// @brief The allocator that will be used for the module.
    sArenaAllocator* allocator = nullptr;
    /**
     * @brief Default constructor for the Module class.
     * 
     * This constructor initializes the library. The inherited module should provide functionality to use that library.
     * The ident field should be filled out by any inherited module, and the path will be passed in to select the implementation.
     * 
     * @param path The implementation name of the module. This is the name of the module to load.
     * @param ident The name of the module. This is the type of the module to load.
     */
    inline explicit Module(const char* path, const char* ident) : lib(path, ident) {
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
        free(pth);
    }
};