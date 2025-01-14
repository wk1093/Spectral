#pragma once
#include "../moduleLib.h"

#include "../gfx/module.h"

#include <stdio.h>

const char* combine_strs_with_delim(const char* a, const char* b, char delim) {
    size_t len = strlen(a) + strlen(b) + 2;
    char* out = (char*)malloc(len);
    snprintf(out, len, "%s%c%s", a, delim, b);
    return out;
}

namespace shader {
     typedef sShader (*Compile)(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef);
}

struct ShaderModule : Module {
    shader::Compile internal_compile;

    sShader compile(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef=nullptr) {
        return internal_compile(gfxm, path, type, vertDef);
    }

    explicit ShaderModule(const char* dynlib, const char* dynp2) : Module(combine_strs_with_delim(dynlib, dynp2, '_'), "shdr") {
        internal_compile = (shader::Compile)lib.getSymbol("compile");
    }
};