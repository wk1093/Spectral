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
#include <vector>
#include <unordered_map>

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
 namespace smod {
    typedef size_t (*GetDesiredArenaSize)(void);
    typedef void (*ModuleInit)(sArenaAllocator* allocator);
}
struct Module {
    /// @brief The dynamic library handle for the module.
    DynamicLibrary lib;
    /// @brief The allocator that will be used for the module.
    sArenaAllocator* allocator = nullptr;
private:
    smod::GetDesiredArenaSize getDesiredArenaSize = nullptr;
    smod::ModuleInit moduleInit = nullptr;
public:
    /**
     * @brief Default constructor for the Module class.
     * 
     * This constructor initializes the library. The inherited module should provide functionality to use that library.
     * The ident field should be filled out by any inherited module, and the path will be passed in to select the implementation.
     * 
     * @param path The implementation name of the module. This is the name of the module to load.
     * @param ident The name of the module. This is the type of the module to load.
     * @param useAlloc If true, the module will use the arena allocator. If false, it will not.
     */
    inline explicit Module(const char* path, const char* ident, bool useAlloc=true) : lib(path, ident) {
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

        if (useAlloc) {

            char* mod = (char*)malloc(strlen(ident) + strlen(path) + 2);
            strcpy(mod, ident);
            strcat(mod, ".");
            strcat(mod, path);


            getDesiredArenaSize = (smod::GetDesiredArenaSize)lib.getSymbol("getDesiredArenaSize");

            if (getDesiredArenaSize) {
                size_t size = getDesiredArenaSize();
                if (size > 0) {
                    allocator = new sArenaAllocator(mod, size);
                } else {
                    allocator = new sArenaAllocator(mod, 0);
                }
            } else {
                printf("Module %s.%s does not have a getDesiredArenaSize function\n", ident, path);
                allocator = new sArenaAllocator(mod, 0);
            }

            moduleInit = (smod::ModuleInit)lib.getSymbol("moduleInit");
            if (moduleInit) {
                moduleInit(allocator);
            } else {
                printf("Module %s.%s does not have a moduleInit function\n", ident, path);
            }

            free(mod);
        }
    }
};

/**
 * @brief Get a list of all modules that match the given identifier.
 * 
 * This function searches the current directory for all files with the ".splmod" extension and checks if their names contain the given identifier.
 * It returns a vector of strings containing the names of the matching modules.
 * 
 * @param ident The identifier to search for in the module names.
 * @return A vector of strings containing the names of the matching modules.
 */
std::vector<std::string> getModuleImpls(const char* ident) {
    std::vector<std::string> out;
    std::filesystem::path p = getexedir() / "modules" / SPECTRAL_PLATFORM;
    for (auto& entry : std::filesystem::recursive_directory_iterator(p)) {
        if (entry.path().extension() == ".splmod") {
            std::string name = entry.path().stem().string();
            if (name.find(ident) != std::string::npos) {
                out.push_back(name);
            }
        }
    }
    return out;
}

// a module that depends on windows that have the "gl" tag would have the following line in the submod def area of the file (the line starting with !dep makes it a dep instead of a submod):
// !dep win gl
// a module that depends on windows with either "noapi" OR "dx" would have this:
// !dep win noapi,dx
// a module that depends on windows with "noapi" AND "dx" would have this:
// !dep win noapi
// !dep win dx
// a module that depends on windows with "gl" AND shaders with "gl":
// !dep win gl
// !dep shdr gl


struct sModuleDef {
    std::string mod;
    std::string impl;
    std::string dispname;

    struct sModuleDep {
        std::string mod;
        std::vector<std::string> tags;
    };

    struct sSubModuleDef {
        std::string mod;
        std::string impl;
        std::vector<std::string> tags;
        std::vector<sModuleDep> deps;
    };

    std::vector<sSubModuleDef> submods;
};

inline std::ostream& operator<<(std::ostream& os, const sModuleDef::sModuleDep& dep) {
    os << "dep " << dep.mod << ": ";
    for (const auto& tag : dep.tags) {
        os << tag << " ";
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const sModuleDef::sSubModuleDef& submod) {
    os << "submod " << submod.mod << " " << submod.impl << ": ";
    for (const auto& tag : submod.tags) {
        os << tag << " ";
    }
    os << ", deps: ";
    for (const auto& dep : submod.deps) {
        os << dep << " ";
    }
    os << "\n";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const sModuleDef& def) {
    os << "mod " << def.mod << " " << def.impl << "\n";
    os << "dispname " << def.dispname << "\n";
    for (const auto& submod : def.submods) {
        os << submod << "\n";
    }
    return os;
}

inline sModuleDef getModuleDef(const char* filepath) {
    // example input:
    // v1 // version
    // win glfw // mod impl
    // GLFW // dispname
    // win glfw_gl gl // submod: mod impl tags
    // win glfw_noapi vulk,dx,noapi // submod: mod impl tags

    sModuleDef def;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        printf("Error opening file %s\n", filepath);
        return def;
    }
    std::string line;
    std::getline(file, line); // version
    if (line != "v1") {
        printf("Error: Invalid version %s\n", line.c_str());
        return def;
    }
    std::getline(file, line); // mod impl
    size_t pos = line.find(" ");
    if (pos == std::string::npos) {
        printf("Error: Invalid module definition %s\n", line.c_str());
        return def;
    }
    def.mod = line.substr(0, pos);
    def.impl = line.substr(pos + 1);
    std::getline(file, line); // dispname
    def.dispname = line;
    while (std::getline(file, line)) {
        sModuleDef::sSubModuleDef submod;
        size_t pos = line.find(" ");
        if (pos == std::string::npos) {
            printf("Error: Invalid submodule definition %s\n", line.c_str());
            continue;
        }
        submod.mod = line.substr(0, pos);
        size_t pos2 = line.find(" ", pos + 1);
        if (pos2 == std::string::npos) {
            printf("Error: Invalid submodule definition %s\n", line.c_str());
            continue;
        }
        submod.impl = line.substr(pos + 1, pos2 - pos - 1);
        std::string tags = line.substr(pos2 + 1);
        size_t start = 0;
        size_t end = tags.find(",");
        while (end != std::string::npos) {
            submod.tags.push_back(tags.substr(start, end - start));
            start = end + 1;
            end = tags.find(",", start);
        }
        submod.tags.push_back(tags.substr(start));
        if (submod.mod == "!dep") {
            // this is a dependency, not a submodule
            sModuleDef::sModuleDep dep;
            dep.mod = submod.impl;
            if (submod.tags.size() == 0) {
                printf("Error: Invalid dependency definition %s\n", line.c_str());
                continue;
            }
            dep.tags = submod.tags;
            def.submods[def.submods.size() - 1].deps.push_back(dep);
            continue;
        }
        def.submods.push_back(submod);
    }
    return def;
}

inline std::vector<sModuleDef> getModuleDefs() {
    std::vector<sModuleDef> out;
    std::filesystem::path p = getexedir() / "modules/defs";
    for (auto& entry : std::filesystem::recursive_directory_iterator(p)) {
        if (entry.path().extension() == ".spldef") {
            sModuleDef def = getModuleDef(entry.path().string().c_str());
            if (!def.mod.empty()) {
                out.push_back(def);
            }
        }
    }
    return out;
}

inline std::vector<sModuleDef::sSubModuleDef> getSubModules(const std::vector<sModuleDef>& mods) {
    std::vector<sModuleDef::sSubModuleDef> out;
    for (const auto& mod : mods) {
        for (const auto& submod : mod.submods) {
            out.push_back(submod);
        }
    }
    return out;
}

inline sModuleDef::sSubModuleDef findSubDef(const std::vector<sModuleDef::sSubModuleDef>& defs, const char* mod, const char* impl) {
    for (const auto& submod : defs) {
        if (submod.mod == mod && submod.impl == impl) {
            return submod;
        }
    }
    return sModuleDef::sSubModuleDef();
}

inline std::vector<sModuleDef::sSubModuleDef> reduceDependencies(const std::vector<sModuleDef::sSubModuleDef>& defs, sModuleDef::sSubModuleDef selectedDef) {
    // remove any defs that contain a dependency that is the same module as the given one, but isn't the same selection/implementaiton
    // we will first find the sModuledef for the given mod

    // now we will remove any defs that have a dependency that is the same module as the given one, but isn't the same selection/implementation
    std::vector<sModuleDef::sSubModuleDef> out;
    std::vector<sModuleDef::sSubModuleDef> explicitlyDependedOn;
    out.push_back(selectedDef);
    for (const auto& moduleDefinition : defs) {
        if (moduleDefinition.mod == selectedDef.mod) {
            continue;
        }
        bool foundAll = true;
        bool wasDep = false;
        for (const auto& dep : selectedDef.deps) {
            if (dep.mod == moduleDefinition.mod) {
                bool found = false;
                for (const auto& tag : moduleDefinition.tags) {
                    for (const auto& submodTag : dep.tags) {
                        if (submodTag == tag) {
                            found = true;       
                            wasDep = true;
                            break;
                        }
                    }
                }
                if (!found) {
                    foundAll = false;
                }
            }
        }
        if (foundAll) {
            out.push_back(moduleDefinition);
            if (wasDep) {
                explicitlyDependedOn.push_back(moduleDefinition);
            }
        }
    }
    // all explicitly depended on modules should also be reduced
    for (const auto& moduleDefinition : explicitlyDependedOn) {
        out = reduceDependencies(out, moduleDefinition);
    }

    return out;
}

inline std::unordered_map<std::string, std::string> getModuleMap(const std::vector<sModuleDef::sSubModuleDef>& defs) {
    std::unordered_map<std::string, std::string> out;
    for (const auto& def : defs) {
        out[def.mod] = def.impl;
    }
    return out;
}

inline std::vector<sModuleDef> filterModules(const std::vector<sModuleDef>& mods, const char* type) {
    std::vector<sModuleDef> out;
    for (const auto& mod : mods) {
        if (mod.mod == type) {
            out.push_back(mod);
        }
    }
    return out;
}

inline std::vector<sModuleDef::sSubModuleDef> filterSubModules(const std::vector<sModuleDef::sSubModuleDef>& mods, const char* type) {
    std::vector<sModuleDef::sSubModuleDef> out;
    for (const auto& mod : mods) {
        if (mod.mod == type) {
            out.push_back(mod);
        }
    }
    return out;
}

inline sModuleDef createDefFromSubDef(const sModuleDef::sSubModuleDef& submod) {
    sModuleDef def;
    def.mod = submod.mod;
    def.impl = submod.impl;
    def.submods.push_back(submod);
    def.dispname = submod.mod + " " + submod.impl;
    return def;
}

inline std::vector<sModuleDef> getSubModuleDefs(const std::vector<sModuleDef::sSubModuleDef>& mods) {
    std::vector<sModuleDef> out;
    for (const auto& mod : mods) {
        sModuleDef def = createDefFromSubDef(mod);
        out.push_back(def);
    }
    
    return out;
}