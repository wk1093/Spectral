#pragma once
#include "../moduleLib.h"

#include "../win/module.h"

#include <stdio.h>
#include <initializer_list>
#include <cstdint>
#include <cstdlib>
#include <string>

typedef unsigned int sIndex;

struct GraphicsModule;

struct sVertexDefinition {
    int* elements;
    size_t count;
};

size_t vertexDefinitionSize(sVertexDefinition* def) {
    size_t size = 0;
    for (size_t i = 0; i < def->count; i++) {
        size += def->elements[i] * sizeof(float);
    }
    return size;
}

enum class sUniformType {
    FLOAT,
    INT,
    BOOL
};

size_t uniformTypeSize(sUniformType type) {
    switch (type) {
        case sUniformType::FLOAT:
            return sizeof(float);
        case sUniformType::INT:
            return sizeof(int);
        case sUniformType::BOOL:
            return sizeof(bool);
    }
    return 0;
}

enum sShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

struct sUniformElement {
    sShaderType shaderType;
    const char* name;
    sUniformType type;
    size_t countx;
    size_t county=1;

    sUniformElement(sShaderType shaderType, const char* name, sUniformType type, size_t countx, size_t county) : shaderType(shaderType), name(name), type(type), countx(countx), county(county) {}
    sUniformElement(sShaderType shaderType, const char* name, sUniformType type, size_t countx) : shaderType(shaderType), name(name), type(type), countx(countx) {}
};

size_t uniformElementSize(sUniformElement element) {
    return element.countx * element.county * uniformTypeSize(element.type);
}

// typedef sUniformElement sUniformDefinition[];

struct sUniformDefinition {
    sUniformElement* elements;
    size_t count;

    sUniformDefinition(std::initializer_list<sUniformElement> elements) {
        // we cant just cast begin to a pointer, because the array will be destroyed
        this->elements = (sUniformElement*)malloc(sizeof(sUniformElement) * elements.size());
        this->count = elements.size();
        size_t i = 0;
        for (auto it = elements.begin(); it != elements.end(); it++) {
            this->elements[i++] = *it;
        }
    }

    size_t size() {
        size_t size = 0;
        for (size_t i = 0; i < count; i++) {
            size += elements[i].countx * elements[i].county * uniformTypeSize(elements[i].type);
        }
        return size;
    }

    sUniformDefinition() : elements(nullptr), count(0) {}
    sUniformDefinition(sUniformElement* elements, size_t count) : elements(elements), count(count) {}
};

sUniformDefinition getPartialf(sUniformDefinition def, sShaderType type) {
    size_t count = 0;
    for (size_t i = 0; i < def.count; i++) {
        if (def.elements[i].shaderType == type) {
            count++;
        }
    }
    sUniformElement* elements = (sUniformElement*)malloc(sizeof(sUniformElement) * count);
    if (elements == nullptr) {
        printf("ERROR: Malloc failed\n");
    }
    size_t j = 0;
    for (size_t i = 0; i < def.count; i++) {
        if (def.elements[i].shaderType == type) {
            elements[j++] = def.elements[i];
        }
    }
    return {elements, count};
}

struct sUniforms {
    void* internal;
};

struct sMesh {
    void* internal;
    GraphicsModule* creator;
};

struct sShader {
    void* internal;
};

struct GraphicsModule;

struct sShaderProgram {
    void* internal;
    GraphicsModule* creator;
    void* gfx_internal;
};

struct sTextureDefinition {
    size_t width;
    size_t height;
    size_t channels;
    unsigned char* data;
};

struct sTexture {
    void* internal;
};

namespace graphics {
    typedef void (*SetClearColor)(float r, float g, float b, float a);
    typedef void (*Clear)();
    typedef void (*Init)(sWindow* win);
    typedef sMesh (*CreateMesh)(sShader vertexShader, void* vertices, size_t vertexSize, sIndex* indices, size_t indexSize);
    typedef void (*DrawMesh)(sMesh mesh);
    typedef void (*UseShaderProgram)(sShaderProgram shader);
    typedef sShader (*CreateShader)(const char* source, sShaderType type, sVertexDefinition* vertDef);
    typedef sShaderProgram (*CreateShaderProgram)(sShader* shaders, size_t count);
    typedef void (*Present)();
    typedef const char* (*GetShaderType)();
    typedef sUniforms (*CreateUniforms)(sShaderProgram program, sUniformDefinition def);
    typedef void (*SetUniforms)(sUniforms uniforms, void* data);
    typedef sTexture (*CreateTexture)(sTextureDefinition def);
    typedef void (*UseTexture)(sShaderProgram program, sTexture texture, const char* name);
    typedef void (*FreeTexture)(sTexture texture);
    typedef void (*FreeShader)(sShader shader);
    typedef void (*FreeShaderProgram)(sShaderProgram program);
    typedef void (*FreeMesh)(sMesh mesh);
    typedef void (*FreeUniforms)(sUniforms uniforms);
    typedef void (*Destroy)();
    typedef void (*SetScissor)(int x, int y, int width, int height);
    typedef void (*EnableScissor)();
    typedef void (*DisableScissor)();
}

struct GraphicsModule : Module {
    graphics::SetClearColor setClearColor;
    graphics::Clear clear;
    graphics::Init init;
    graphics::CreateMesh internal_createMesh;
    graphics::DrawMesh drawMesh;
    graphics::UseShaderProgram useShaderProgram;
    graphics::CreateShaderProgram internal_createShaderProgram;
    graphics::CreateShader internal_createShader;
    graphics::Present present;
    graphics::GetShaderType getShaderType;
    graphics::CreateUniforms createUniforms;
    graphics::SetUniforms setUniforms;
    graphics::CreateTexture createTexture;
    graphics::UseTexture useTexture;
    graphics::FreeTexture freeTexture;
    graphics::FreeShader freeShader;
    graphics::FreeShaderProgram freeShaderProgram;
    graphics::FreeMesh freeMesh;
    graphics::FreeUniforms freeUniforms;
    graphics::Destroy destroy;
    graphics::SetScissor setScissor;
    graphics::EnableScissor enableScissor;
    graphics::DisableScissor disableScissor;

    sMesh createMesh(sShader vertexShader, void* vertices, size_t vertexCount, sIndex* indices, size_t indexCount) {
        sMesh mesh = internal_createMesh(vertexShader, vertices, vertexCount, indices, indexCount);
        mesh.creator = this;
        return mesh;
    }

    sShaderProgram createShaderProgram(sShader* shaders, size_t count) {
        sShaderProgram program = internal_createShaderProgram(shaders, count);
        program.creator = this;
        return program;
    }

    sShaderProgram createShaderProgram(std::initializer_list<sShader> shaders) {
        return createShaderProgram((sShader*)shaders.begin(), shaders.size());
    }

    sVertexDefinition* createVertexDefinition(int* elements, size_t count) {
        sVertexDefinition* def = (sVertexDefinition*)malloc(sizeof(sVertexDefinition));
        // def->elements = elements;
        def->elements = (int*)malloc(sizeof(int) * count);
        if (def->elements == nullptr) {
            printf("ERROR: Malloc failed\n");
        }
        for (size_t i = 0; i < count; i++) {
            def->elements[i] = elements[i];
        }
        def->count = count;
        return def;
    }

    sVertexDefinition* createVertexDefinition(std::initializer_list<int> elements) {
        return createVertexDefinition((int*)elements.begin(), elements.size());
    }

    void freeVertexDefinition(sVertexDefinition* def) {
        free(def->elements);
        free(def);
    }

    sShader createShader(const char* source, sShaderType type, sVertexDefinition* vertDef) {
        sShader shader = internal_createShader(source, type, vertDef);
        return shader;
    }

    sShader createShader(const char* source, sShaderType type) {
        if (type == sShaderType::VERTEX) {
            printf("ERROR: Vertex shader must have a vertex definition\n");
        }
        return createShader(source, type, nullptr);
    }

    sShader loadShader(const char* path, sShaderType type, sVertexDefinition* vertDef) {
        std::string source;
        if (!readFile(path, source)) {
            printf("Error reading file\n");
            return {nullptr};
        }
        return createShader(source.c_str(), type, vertDef);
    }

    sShader loadShader(const char* path, sShaderType type) {
        return loadShader(path, type, nullptr);
    }

    explicit GraphicsModule(const char* dynlib) : Module(dynlib, "gfx") {
        setClearColor = (graphics::SetClearColor)lib.getSymbol("setClearColor");
        clear = (graphics::Clear)lib.getSymbol("clear");
        init = (graphics::Init)lib.getSymbol("init");
        internal_createMesh = (graphics::CreateMesh)lib.getSymbol("createMesh");
        drawMesh = (graphics::DrawMesh)lib.getSymbol("drawMesh");
        useShaderProgram = (graphics::UseShaderProgram)lib.getSymbol("useShaderProgram");
        internal_createShaderProgram = (graphics::CreateShaderProgram)lib.getSymbol("createShaderProgram");
        internal_createShader = (graphics::CreateShader)lib.getSymbol("createShader");
        present = (graphics::Present)lib.getSymbol("present");
        getShaderType = (graphics::GetShaderType)lib.getSymbol("getShaderType");
        createUniforms = (graphics::CreateUniforms)lib.getSymbol("createUniforms");
        setUniforms = (graphics::SetUniforms)lib.getSymbol("setUniforms");
        createTexture = (graphics::CreateTexture)lib.getSymbol("createTexture");
        useTexture = (graphics::UseTexture)lib.getSymbol("useTexture");
        freeTexture = (graphics::FreeTexture)lib.getSymbol("freeTexture");
        freeShader = (graphics::FreeShader)lib.getSymbol("freeShader");
        freeShaderProgram = (graphics::FreeShaderProgram)lib.getSymbol("freeShaderProgram");
        freeMesh = (graphics::FreeMesh)lib.getSymbol("freeMesh");
        freeUniforms = (graphics::FreeUniforms)lib.getSymbol("freeUniforms");
        destroy = (graphics::Destroy)lib.getSymbol("destroy");
        setScissor = (graphics::SetScissor)lib.getSymbol("setScissor");
        enableScissor = (graphics::EnableScissor)lib.getSymbol("enableScissor");
        disableScissor = (graphics::DisableScissor)lib.getSymbol("disableScissor");
    }
};