#include <eogll.h>
#include "getprocaddress.h"

#include "module.h"

CEXPORT void init(sWindow* win) {
//    if (eogllInit() != EOGLL_SUCCESS) {
//        printf("Error initializing EOGLL\n");
//        return;
//    }
    if (strcmp(win->creator->lib.mod_imp, "eogll") == 0) {
        auto* w = (EogllWindow*)win->internal;
        glfwMakeContextCurrent(w->window);
    } else if (strcmp(win->creator->lib.mod_imp, "glfw") == 0) {
        auto* w = (GLFWwindow*)win->internal;
        glfwMakeContextCurrent(w);
        if (!gladLoadGL(glfwGetProcAddress)) {
            printf("Error loading OpenGL\n");
        }
    } else if (strcmp(win->creator->lib.mod_imp, "sf") == 0) {
        if (!gladLoadGL((GLADloadfunc)getProcAddress)) {
            printf("Error loading OpenGL\n");
        }
    }


}

CEXPORT void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

CEXPORT void clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

EogllBufferObject __debugTriangleBuffer;
bool __debugTriangleBufferInitialized = false;
EogllShaderProgram* __debugTriangleShader;

static const float __debugTriangleVertices[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};
static const unsigned int __debugTriangleIndices[] = {
    0, 1, 2
};

static const char* __debugTriangleVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

static const char* __debugTriangleFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

CEXPORT void debugTriangle() {
    if (!__debugTriangleBufferInitialized) {
        __debugTriangleShader = eogllLinkProgram(__debugTriangleVertexShaderSource, __debugTriangleFragmentShaderSource);

        EogllAttribBuilder builder = eogllCreateAttribBuilder();
        eogllAddAttribute(&builder, GL_FLOAT, 3);
        unsigned int vao = eogllGenVertexArray();
        unsigned int vbo = eogllGenBuffer(vao, GL_ARRAY_BUFFER, sizeof(__debugTriangleVertices), __debugTriangleVertices, GL_STATIC_DRAW);
        unsigned int ebo = eogllGenBuffer(vao, GL_ELEMENT_ARRAY_BUFFER, sizeof(__debugTriangleIndices), __debugTriangleIndices, GL_STATIC_DRAW);
        eogllBuildAttributes(&builder, vao);

        __debugTriangleBuffer = eogllCreateBufferObject(vao, vbo, ebo, sizeof(__debugTriangleIndices), GL_UNSIGNED_INT);
        __debugTriangleBufferInitialized = true;
    }

    eogllUseProgram(__debugTriangleShader);
    eogllDrawBufferObject(&__debugTriangleBuffer, GL_TRIANGLES);


}