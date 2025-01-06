#include <eogll.h>

#include "module.h"

CEXPORT void init(sWindow* win) {
    if (eogllInit() != EOGLL_SUCCESS) {
        printf("Error initializing EOGLL\n");
        return;
    }
    if (strcmp(win->creator->lib.mod_imp, "eogll") == 0) {
        auto* w = (EogllWindow*)win->internal;
        glfwMakeContextCurrent(w->window);
    } else if (strcmp(win->creator->lib.mod_imp, "glfw") == 0) {
        auto* w = (GLFWwindow*)win->internal;
        glfwMakeContextCurrent(w);
        if (!gladLoadGL(glfwGetProcAddress)) {
            printf("Error loading OpenGL\n");
        }
    } else {
        printf("Error: Eogll unsupported window module '%s'\n", win->creator->lib.mod_imp);
        return;
    }


}

CEXPORT void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

CEXPORT void clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

