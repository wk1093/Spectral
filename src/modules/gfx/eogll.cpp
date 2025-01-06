#include <eogll.h>

#include "module.h"

CEXPORT void init(sWindow* win) {

    if (strcmp(win->creator->lib.mod_imp, "eogll") == 0) {
        auto* w = (EogllWindow*)win->internal;
        glfwMakeContextCurrent(w->window);
    } else if (strcmp(win->creator->lib.mod_imp, "glfw") == 0) {
        auto* w = (GLFWwindow*)win->internal;
        glfwMakeContextCurrent(w);
    } else {
        printf("Error: Eogll unsupported window module '%s'\n", win->creator->lib.mod_imp);
        return;
    }

    if (!gladLoadGL(glfwGetProcAddress)) { // since this is in a dynamic library, we have to load them again
        printf("Error loading OpenGL\n");
    }

}

CEXPORT void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

CEXPORT void clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}