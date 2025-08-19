#include <glad/glad.h>
#include "glutils.h"

#include "module.h"

#include <cmath>

sArenaAllocator* gArena = nullptr;

struct sInternalMesh {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    size_t indexCount;
};

sTypedSmartArena<sInternalMesh>* gMeshArena = nullptr;

CEXPORT size_t getDesiredArenaSize() {
    return 1024 * 1024; // Assume 1MB for now, this should be enough for some cases, but bigger games will reallocate this anyway
}

CEXPORT void moduleInit(sArenaAllocator* arena) {
    gArena = arena;
    gMeshArena = new sTypedSmartArena<sInternalMesh>(arena->tracker, 1024 * 1024); // 1MB for meshes
}

CEXPORT const char* getShaderType() {
    return "glsl";
}

sWindow* __window_global;

CEXPORT void init(sWindow* win) {
    if (!gladLoadGLLoader((GLADloadproc)getProcAddress)) {
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
    __window_global = win;

}

CEXPORT void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

CEXPORT void clear() {
    if (__window_global->did_resize) {
        glViewport(0, 0, __window_global->width, __window_global->height);
        __window_global->did_resize = false;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

struct sInternalShader {
    unsigned int shader;
    sVertexDefinition* vertDef;
    sShaderType type;
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

    // sInternalMesh* internalMesh = (sInternalMesh*)malloc(sizeof(sInternalMesh));
    // sInternalMesh* internalMesh = gArena->allocate<sInternalMesh>();
    sInternalMesh* internalMesh = gMeshArena->allocate();
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

    // sInternalShader* internal = (sInternalShader*)malloc(sizeof(sInternalShader));
    sInternalShader* internal = gArena->allocate<sInternalShader>();
    internal->shader = shader;
    internal->vertDef = vertDef;
    internal->type = type;
    if (type == sShaderType::VERTEX) {
        internal->vertDef = vertDef;
    } else {
        internal->vertDef = nullptr;
    }
    return {internal};
}

struct sInternalShaderProgram {
    unsigned int program;
    int texcount;
    sShaderReflection vert;
    sShaderReflection frag;
};

CEXPORT void useShaderProgram(sShaderProgram shader) {
    sInternalShaderProgram* internal = (sInternalShaderProgram*)shader.internal;
    glUseProgram(internal->program);
    internal->texcount = 0;
}

CEXPORT sShaderProgram createShaderProgram(sShader* shaders, size_t count) {
    unsigned int program = glCreateProgram();
    sShaderReflection vert;
    sShaderReflection frag;
    for (size_t i = 0; i < count; i++) {
        glAttachShader(program, ((sInternalShader*)(shaders[i].internal))->shader);
        sInternalShader* shader = (sInternalShader*)shaders[i].internal;
        if (shader->type == sShaderType::VERTEX) {
            vert = shaders[i].reflection;
        } else if (shader->type == sShaderType::FRAGMENT) {
            frag = shaders[i].reflection;
        }
        
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

    // sInternalShaderProgram* internal = (sInternalShaderProgram*)malloc(sizeof(sInternalShaderProgram));
    sInternalShaderProgram* internal = gArena->allocate<sInternalShaderProgram>();
    internal->program = program;
    internal->texcount = 0;
    internal->vert = vert;
    internal->frag = frag;

    return {internal};
}

CEXPORT void present() {

}

struct sInternalUniforms {
    sUniformDefinition def;
    sShaderProgram program;
    int vert_loc;
    int frag_loc;
    GLuint vert_ubo;
    GLuint frag_ubo;
    sUniformDefinition vertdef;
    sUniformDefinition fragdef;
};

CEXPORT sUniforms createUniforms(sShaderProgram program, sUniformDefinition def) {
    // sInternalUniforms* internal = (sInternalUniforms*)malloc(sizeof(sInternalUniforms));
    sInternalUniforms* internal = gArena->allocate<sInternalUniforms>();
    internal->def = def;
    internal->program = program;
    internal->vertdef = getPartialf(def, sShaderType::VERTEX);
    internal->fragdef = getPartialf(def, sShaderType::FRAGMENT);

    sInternalShaderProgram* internalProgram = (sInternalShaderProgram*)program.internal;

    if (internal->vertdef.size() != internalProgram->vert.uniformSize) {
        printf("Vertex uniform size mismatch: %zu != %zu\n", internal->vertdef.size(), internalProgram->vert.uniformSize);
    }
    if (internal->fragdef.size() != internalProgram->frag.uniformSize) {
        printf("Fragment uniform size mismatch: %zu != %zu\n", internal->fragdef.size(), internalProgram->frag.uniformSize);
    }
    
    internal->vert_loc = glGetUniformBlockIndex(internalProgram->program, internalProgram->vert.uniformName);
    if (internal->vert_loc == GL_INVALID_INDEX) {
        printf("Vertex uniform block not found: %s\n", internalProgram->vert.uniformName);
    }
    internal->frag_loc = glGetUniformBlockIndex(internalProgram->program, internalProgram->frag.uniformName);
    if (internal->frag_loc == GL_INVALID_INDEX) {
        printf("Fragment uniform block not found: %s\n", internalProgram->frag.uniformName);
    }
    glUniformBlockBinding(internalProgram->program, internal->vert_loc, 0);
    glUniformBlockBinding(internalProgram->program, internal->frag_loc, 1);

    glGenBuffers(1, &internal->vert_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, internal->vert_ubo);
    glBufferData(GL_UNIFORM_BUFFER, internal->vertdef.size(), nullptr, GL_DYNAMIC_DRAW);
    glGenBuffers(1, &internal->frag_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, internal->frag_ubo);
    glBufferData(GL_UNIFORM_BUFFER, internal->fragdef.size(), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, internal->vert_ubo);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, internal->frag_ubo);


    return {internal};
}

CEXPORT void setUniforms(sUniforms uniforms, void* data) {

    sInternalUniforms* internal = (sInternalUniforms*)uniforms.internal;
    sUniformDefinition def = internal->def;

    size_t vertSize = internal->vertdef.size();
    size_t fragSize = internal->fragdef.size();

    glBindBuffer(GL_UNIFORM_BUFFER, internal->frag_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, fragSize, data);
    glBindBuffer(GL_UNIFORM_BUFFER, internal->vert_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, vertSize, (char*)data+fragSize);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
    // sInternalTexture* internal = (sInternalTexture*)malloc(sizeof(sInternalTexture));
    sInternalTexture* internal = gArena->allocate<sInternalTexture>();
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
    // free(internal);
}

CEXPORT void freeShader(sShader shader) {
    sInternalShader* internal = (sInternalShader*)shader.internal;
    glDeleteShader(internal->shader);
    // free(internal);
}

CEXPORT void freeShaderProgram(sShaderProgram program) {
    sInternalShaderProgram* internal = (sInternalShaderProgram*)program.internal;
    glDeleteProgram(internal->program);
    // free(internal);
}

CEXPORT void freeMesh(sMesh mesh) {
    sInternalMesh* internal = (sInternalMesh*)mesh.internal;
    glDeleteVertexArrays(1, &internal->vao);
    glDeleteBuffers(1, &internal->vbo);
    glDeleteBuffers(1, &internal->ebo);
    // free(internal);
    gMeshArena->free(internal);
}

CEXPORT void freeUniforms(sUniforms uniforms) {
    sInternalUniforms* internal = (sInternalUniforms*)uniforms.internal;
    // free(internal->locations);
    // free(internal);
}

CEXPORT void destroy() {

}

CEXPORT void setScissor(int x, int y, int width, int height) {
    glScissor(x, y, width, height);
}

CEXPORT void enableScissor() {
    glEnable(GL_SCISSOR_TEST);
}

CEXPORT void disableScissor() {
    glDisable(GL_SCISSOR_TEST);
}