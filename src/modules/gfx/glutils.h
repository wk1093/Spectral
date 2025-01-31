#pragma once
#ifdef _WIN32
#include <Windows.h>

#elif __linux__
#include <GL/glx.h>
#include <GL/glxext.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#else
#error "Unsupported platform"
#endif

#ifdef _WIN32
extern "C" void *getProcAddress(const char *name) {
    void *p = (void *)wglGetProcAddress(name);
    if(p == 0 ||
       (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
       (p == (void*)-1) )
    {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void *)GetProcAddress(module, name);
    }

    return p;
}
#elif __linux__
extern "C" void *getProcAddress(const char *name) {
    void *p = (void *)glXGetProcAddress((const GLubyte *)name);
    if(p == 0 ||
       (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
       (p == (void*)-1) )
    {
        void *handle = dlopen("libGL.so.1", RTLD_LAZY);
        p = (void *)dlsym(handle, name);
    }

    return p;
}
#else
#error "Unsupported platform"
#endif