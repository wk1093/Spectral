#include <eogll.h>
#include "getprocaddress.h"

#include "module.h"
#include <fstream>

CEXPORT void init(sWindow* win) {
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

CEXPORT sMesh createMesh(sVertex* vertices, size_t vertexSize, sIndex* indices, size_t indexSize) {
    unsigned int vao = eogllGenVertexArray();
    unsigned int vbo = eogllGenBuffer(vao, GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    unsigned int ebo = eogllGenBuffer(vao, GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

    EogllAttribBuilder builder = eogllCreateAttribBuilder();
    eogllAddAttribute(&builder, GL_FLOAT, 3);
    eogllAddAttribute(&builder, GL_FLOAT, 3);
    eogllBuildAttributes(&builder, vao);

    auto* mesh = (EogllBufferObject*)malloc(sizeof(EogllBufferObject));
    *mesh = eogllCreateBufferObject(vao, vbo, ebo, indexSize, GL_UNSIGNED_INT);

    return {mesh};
}

CEXPORT void drawMesh(sMesh mesh) {
    eogllDrawBufferObject((EogllBufferObject*)mesh.internal, GL_TRIANGLES);
}

CEXPORT sShader createShader(const char* source, sShaderType type) {
    unsigned int glType;
    switch (type) {
        case sShaderType::VERTEX:
            glType = GL_VERTEX_SHADER;
            break;
        case sShaderType::FRAGMENT:
            glType = GL_FRAGMENT_SHADER;
            break;
        case sShaderType::GEOMETRY:
            glType = GL_GEOMETRY_SHADER;
            break;
        default:
            printf("Invalid shader type\n");
            return {nullptr};
    }

    unsigned int shader = glCreateShader(glType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
        return {nullptr};
    }

    unsigned int* internal = (unsigned int*)malloc(sizeof(unsigned int));
    *internal = shader;
    return {internal};
}

CEXPORT sShader loadShader(const char* path, sShaderType type) {
    std::string source;
    if (!readFile(path, source)) {
        printf("Error reading file\n");
        return {nullptr};
    }
    return createShader(source.c_str(), type);
}

CEXPORT void useShaderProgram(sShaderProgram shader) {
    glUseProgram(*(unsigned int*)shader.internal);
}

CEXPORT sShaderProgram createShaderProgram(sShader* shaders, size_t count) {
    unsigned int program = glCreateProgram();
    for (size_t i = 0; i < count; i++) {
        glAttachShader(program, *(unsigned int*)shaders[i].internal);
    }
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
        return {nullptr};
    }

    unsigned int* internal = (unsigned int*)malloc(sizeof(unsigned int));
    *internal = program;
    return {internal};
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