#include <glad/gl.h>
#include "glutils.h"

#include "module.h"

CEXPORT const char* getShaderType() {
    return "glsl";
}

CEXPORT void init(sWindow* win) {
    if (!gladLoadGL((GLADloadfunc)getProcAddress)) {
        printf("Error loading OpenGL\n");
        printf("Make sure you have a valid OpenGL context\n");
        printf("Using window module '%s'\n", win->creator->lib.mod_imp);
    }
}

CEXPORT void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

CEXPORT void clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

struct sInternalMesh {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    size_t indexCount;
};

struct sInternalShader {
    unsigned int shader;
    sVertexDefinition* vertDef;
};

CEXPORT sMesh createMesh(sShader vertexShader, void* vertices, size_t vertexSize, sIndex* indices, size_t indexSize) {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

    sVertexDefinition* vertDef = ((sInternalShader*)vertexShader.internal)->vertDef;
    size_t stride = 0;
    for (size_t i = 0; i < vertDef->count; i++) {
        stride += vertDef->elements[i];
    }

    size_t offset = 0;
    for (size_t i = 0; i < vertDef->count; i++) {
        glVertexAttribPointer(i, vertDef->elements[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += vertDef->elements[i];
    }

    sInternalMesh* internalMesh = (sInternalMesh*)malloc(sizeof(sInternalMesh));
    internalMesh->vao = vao;
    internalMesh->vbo = vbo;
    internalMesh->ebo = ebo;
    internalMesh->indexCount = indexSize / sizeof(sIndex);

    return {internalMesh};
}

CEXPORT void drawMesh(sMesh mesh) {
    sInternalMesh* internalMesh = (sInternalMesh*)mesh.internal;
    glBindVertexArray(internalMesh->vao);
    glDrawElements(GL_TRIANGLES, internalMesh->indexCount, GL_UNSIGNED_INT, nullptr);
}


CEXPORT sShader createShader(const char* source, sShaderType type, sVertexDefinition* vertDef) {
    unsigned int glType;
    switch (type) {
        case sShaderType::VERTEX:
            glType = GL_VERTEX_SHADER;
            if (vertDef == nullptr) {
                printf("Vertex shader requires vertex definition\n");
                return {nullptr};
            }
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

    // unsigned int* internal = (unsigned int*)malloc(sizeof(unsigned int));
    // *internal = shader;
    // return {internal};
    sInternalShader* internal = (sInternalShader*)malloc(sizeof(sInternalShader));
    internal->shader = shader;
    internal->vertDef = vertDef;
    return {internal};
}

CEXPORT void useShaderProgram(sShaderProgram shader) {
    glUseProgram(*(unsigned int*)shader.internal);
}

CEXPORT sShaderProgram createShaderProgram(sShader* shaders, size_t count) {
    unsigned int program = glCreateProgram();
    for (size_t i = 0; i < count; i++) {
        glAttachShader(program, ((sInternalShader*)(shaders[i].internal))->shader);
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

CEXPORT void present() {

}
