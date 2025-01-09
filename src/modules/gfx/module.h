#pragma once
#include "../moduleLib.h"

#include "../win/module.h"

#include <stdio.h>
#include <initializer_list>

struct sVertex {
    float position[3];
    float color[3];
};

typedef unsigned int sIndex;

struct GraphicsModule;

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
    typedef sMesh (*CreateMesh)(sShader vertexShader, sVertex* vertices, size_t vertexSize, sIndex* indices, size_t indexSize);
    typedef void (*DrawMesh)(sMesh mesh);
    typedef void (*UseShaderProgram)(sShaderProgram shader);
    typedef sShader (*LoadShader)(const char* path, sShaderType type);
    typedef sShader (*CreateShader)(const char* source, sShaderType type);
    typedef sShaderProgram (*CreateShaderProgram)(sShader* shaders, size_t count);
    typedef void (*Present)();
}

struct GraphicsModule : Module {
    graphics::SetClearColor setClearColor;
    graphics::Clear clear;
    graphics::Init init;
    graphics::CreateMesh internal_createMesh;
    graphics::DrawMesh drawMesh;
    graphics::UseShaderProgram useShaderProgram;
    graphics::LoadShader loadShader;
    graphics::CreateShaderProgram internal_createShaderProgram;
    graphics::CreateShader createShader;
    graphics::Present present;

    sMesh createMesh(sShader vertexShader, sVertex* vertices, size_t vertexCount, sIndex* indices, size_t indexCount) {
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

    explicit GraphicsModule(const char* dynlib) : Module(dynlib, "gfx") {
        setClearColor = (graphics::SetClearColor)lib.getSymbol("setClearColor");
        clear = (graphics::Clear)lib.getSymbol("clear");
        init = (graphics::Init)lib.getSymbol("init");
        internal_createMesh = (graphics::CreateMesh)lib.getSymbol("createMesh");
        drawMesh = (graphics::DrawMesh)lib.getSymbol("drawMesh");
        useShaderProgram = (graphics::UseShaderProgram)lib.getSymbol("useShaderProgram");
        loadShader = (graphics::LoadShader)lib.getSymbol("loadShader");
        internal_createShaderProgram = (graphics::CreateShaderProgram)lib.getSymbol("createShaderProgram");
        createShader = (graphics::CreateShader)lib.getSymbol("createShader");
        present = (graphics::Present)lib.getSymbol("present");
    }
};