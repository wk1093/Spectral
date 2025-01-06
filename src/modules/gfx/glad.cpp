#include <glad/gl.h>
#ifdef _WIN32
#include <Windows.h>
#else
#error NOT SUPPORTED YET
#endif

#include "module.h"

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

CEXPORT void init(sWindow* win) {
    if (strcmp(win->creator->lib.mod_imp, "eogll") == 0) {

    } else if (strcmp(win->creator->lib.mod_imp, "glfw") == 0) {

    } else {
        printf("Error: Eogll unsupported window module '%s'\n", win->creator->lib.mod_imp);
        return;
    }

    if (!gladLoadGL((GLADloadfunc)getProcAddress)) {
        printf("Error loading OpenGL\n");
    }
}

#include "opengl.h"