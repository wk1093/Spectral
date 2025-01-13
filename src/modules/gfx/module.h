#pragma once
#include "../moduleLib.h"

#include "../win/module.h"

#include <stdio.h>
#include <initializer_list>

typedef unsigned int sIndex;

struct GraphicsModule;

struct sVertexDefinition {
    int* elements;
    size_t count;
};

struct sMesh {
    void* internal;
    GraphicsModule* creator;
};

struct sShader {
    void* internal;
};

enum sShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

struct GraphicsModule;

struct sShaderProgram {
    void* internal;
    GraphicsModule* creator;
    void* gfx_internal;
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
        def->elements = elements;
        def->count = count;
        return def;
    }

    sVertexDefinition* createVertexDefinition(std::initializer_list<int> elements) {
        return createVertexDefinition((int*)elements.begin(), elements.size());
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
    }
};