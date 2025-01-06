#pragma once
#ifdef _WIN32
#include <Windows.h>
#else
#error NOT SUPPORTED YET
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
#else
#error NOT SUPPORTED YET
#endif