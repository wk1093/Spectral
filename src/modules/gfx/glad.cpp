#include <glad/gl.h>
#include "getprocaddress.h"

#include "module.h"

CEXPORT void init(sWindow* win) {
    if (!gladLoadGL((GLADloadfunc)getProcAddress)) {
        printf("Error loading OpenGL\n");
    }
}

CEXPORT void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

CEXPORT void clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

static const float __debugTriangleVertices[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
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

bool __debugTriangleBufferInitialized = false;
unsigned int __debugTriangleVao;
unsigned int __debugTriangleShader;

CEXPORT void debugTriangle() {
    if (!__debugTriangleBufferInitialized) {
        glGenVertexArrays(1, &__debugTriangleVao);
        glBindVertexArray(__debugTriangleVao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(__debugTriangleVertices), __debugTriangleVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        __debugTriangleShader = glCreateProgram();
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &__debugTriangleVertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &__debugTriangleFragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        glAttachShader(__debugTriangleShader, vertexShader);
        glAttachShader(__debugTriangleShader, fragmentShader);
        glLinkProgram(__debugTriangleShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        __debugTriangleBufferInitialized = true;
    }

    glUseProgram(__debugTriangleShader);
    glBindVertexArray(__debugTriangleVao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}