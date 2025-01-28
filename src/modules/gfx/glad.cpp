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
    // make sure depth testing is enabled
    glEnable(GL_DEPTH_TEST);
    // wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // make sure backface culling is enabled
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

CEXPORT void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

CEXPORT void clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    sInternalShader* internal = (sInternalShader*)malloc(sizeof(sInternalShader));
    internal->shader = shader;
    internal->vertDef = vertDef;
    return {internal};
}

struct sInternalShaderProgram {
    unsigned int program;
    int texcount;
};

CEXPORT void useShaderProgram(sShaderProgram shader) {
    sInternalShaderProgram* internal = (sInternalShaderProgram*)shader.internal;
    glUseProgram(internal->program);
    internal->texcount = 0;
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

    sInternalShaderProgram* internal = (sInternalShaderProgram*)malloc(sizeof(sInternalShaderProgram));
    internal->program = program;
    internal->texcount = 0;
    return {internal};
}

CEXPORT void present() {

}

struct sInternalUniforms {
    sUniformDefinition def;
    sShaderProgram program;
    int* locations;
};

CEXPORT sUniforms createUniforms(sShaderProgram program, sUniformDefinition def) {
    sInternalUniforms* internal = (sInternalUniforms*)malloc(sizeof(sInternalUniforms));
    internal->def = def;
    internal->program = program;
    internal->locations = (int*)malloc(sizeof(int) * def.count);
    for (size_t i = 0; i < def.count; i++) {
        internal->locations[i] = glGetUniformLocation(*(unsigned int*)program.internal, def.elements[i].name);
    }
    return {internal};
}

CEXPORT void setUniforms(sUniforms uniforms, void* data) {

    sInternalUniforms internal = *(sInternalUniforms*)uniforms.internal;
    sUniformDefinition def = internal.def;

    size_t offset = 0;
    for (size_t i = 0; i < def.count; i++) {
        sUniformElement element = def.elements[i];
        int location = internal.locations[i];
        void* curData = (void*)((size_t)data + offset);
        switch (element.type) {
            case sUniformType::FLOAT:
                if (element.county == 1) {
                    switch (element.countx) {
                    case 1:
                        glUniform1fv(location, 1, (float*)curData);
                        break;
                    case 2:
                        glUniform2fv(location, 1, (float*)curData);

                        break;
                    case 3:
                        glUniform3fv(location, 1, (float*)curData);
                        break;
                    case 4:
                        glUniform4fv(location, 1, (float*)curData);
                        break;
                    default:
                        glUniform1fv(location, element.countx, (float*)curData);
                    }
                    offset += uniformElementSize(element);
                } else if (element.countx == element.county) {
                    switch (element.countx) {
                        case 2:
                            glUniformMatrix2fv(location, 1, GL_FALSE, (float*)curData);
                            offset += uniformElementSize(element);
                            break;
                        case 3:
                            glUniformMatrix3fv(location, 1, GL_FALSE, (float*)curData);
                            offset += uniformElementSize(element);
                            break;
                        case 4:
                            glUniformMatrix4fv(location, 1, GL_FALSE, (float*)curData);
                            offset += uniformElementSize(element);
                            break;
                        default:
                            printf("Invalid matrix size\n");
                            return;
                    }
                } else {
                    printf("Invalid matrix size\n");
                    return;
                }
                break;
            case sUniformType::INT:
                if (element.county != 1) {
                    printf("Invalid uniform type: no int matrices\n");
                    return;
                }
                switch (element.countx) {
                    case 1:
                        glUniform1iv(location, 1, (int*)curData);
                        break;
                    case 2:
                        glUniform2iv(location, 1, (int*)curData);
                        break;
                    case 3:
                        glUniform3iv(location, 1, (int*)curData);
                        break;
                    case 4:
                        glUniform4iv(location, 1, (int*)curData);
                        break;
                    default:
                        glUniform1iv(location, element.countx, (int*)curData);
                }
                offset += uniformElementSize(element);
                break;
            case sUniformType::BOOL:
                if (element.county != 1) {
                    printf("Invalid uniform type: no bool matrices\n");
                    return;
                }
                glUniform1iv(location, element.countx, (int*)curData);
                offset += uniformElementSize(element);
                break;
            default:
                printf("Invalid uniform type\n");
        }
    }
}

struct sInternalTexture {
    unsigned int texture;
};

CEXPORT sTexture createTexture(sTextureDefinition tex) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data);
    GLenum format;
    switch (tex.channels) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            printf("Invalid texture format\n");
            return {nullptr};
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, tex.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    sInternalTexture* internal = (sInternalTexture*)malloc(sizeof(sInternalTexture));
    internal->texture = texture;
    return {internal};
}

CEXPORT void useTexture(sShaderProgram program, sTexture texture, const char* name) {
    sInternalTexture* internal = (sInternalTexture*)texture.internal;
    sInternalShaderProgram* shader = (sInternalShaderProgram*)program.internal;
    glActiveTexture(GL_TEXTURE0 + shader->texcount);
    glBindTexture(GL_TEXTURE_2D, internal->texture);
    glUniform1i(glGetUniformLocation(shader->program, name), shader->texcount);
    shader->texcount++;
}

CEXPORT void freeTexture(sTexture texture) {
    sInternalTexture* internal = (sInternalTexture*)texture.internal;
    glDeleteTextures(1, &internal->texture);
    free(internal);
}

CEXPORT void freeShader(sShader shader) {
    sInternalShader* internal = (sInternalShader*)shader.internal;
    glDeleteShader(internal->shader);
    free(internal);
}

CEXPORT void freeShaderProgram(sShaderProgram program) {
    sInternalShaderProgram* internal = (sInternalShaderProgram*)program.internal;
    glDeleteProgram(internal->program);
    free(internal);
}

CEXPORT void freeMesh(sMesh mesh) {
    sInternalMesh* internal = (sInternalMesh*)mesh.internal;
    glDeleteVertexArrays(1, &internal->vao);
    glDeleteBuffers(1, &internal->vbo);
    glDeleteBuffers(1, &internal->ebo);
    free(internal);
}

CEXPORT void freeUniforms(sUniforms uniforms) {
    sInternalUniforms* internal = (sInternalUniforms*)uniforms.internal;
    free(internal->locations);
    free(internal);
}

CEXPORT void destroy() {

}