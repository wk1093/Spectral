/**
 * @file shdr/module.h
 * @brief Shader module interface.
 */

#pragma once
#include "moduleLib.h"

#include "gfx/module.h"

#include <stdio.h>
#include <stdint.h>

/// @cond DOXYGEN_OMIT
inline const char* combine_strs_with_delim(const char* a, const char* b, char delim) {
    size_t len = strlen(a) + strlen(b) + 2;
    char* out = (char*)malloc(len);
    snprintf(out, len, "%s%c%s", a, delim, b);
    return out;
}

namespace shader {
     typedef sShader (*Compile)(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef);
     typedef sShader (*CreateShader)(GraphicsModule* gfxm, const char* data, size_t len, sShaderType type, sVertexDefinition* vertDef);
     typedef const char* (*ShaderExtension)(sShaderType type);
}
/// @endcond

/**
 * @brief ShaderModule is a module that provides functionality for compiling and creating shaders.
 * 
 * @details The ShaderModule is a dynamic library that provides functionality for compiling and creating shaders. It uses the GraphicsModule to compile the shader. The shader is compiled from the file at the given path.
 * 
 * @note This is helpful because it allows us to write a single shader in a single language, instead of having to rewrite it to support each graphics API.
 */
struct ShaderModule : Module {
private:
    shader::Compile internal_compile;
    shader::CreateShader internal_createShader;
    shader::ShaderExtension internal_shaderExtension;

public:

    /**
     * @brief Compiles a shader from a file.
     * 
     * @details Uses the GraphicsModule to compile the shader. The shader is compiled from the file at the given path.
     * 
     * @param gfxm The GraphicsModule to use for compilation.
     * @param path The path to the shader file.
     * @param type The type of shader to compile.
     * @param vertDef The vertex definition to use for the shader. Only required for vertex shaders.
     * @return The compiled shader.
     * @note This function will compile from any type of shader file (glsl, hlsl, spsl, etc.) depending on which implementation is used.
     * @see #sShaderType
     */
    inline sShader compile(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef=nullptr) {
        return internal_compile(gfxm, path, type, vertDef);
    }

    /**
     * @brief Creates a shader from a string.
     * 
     * @details Uses the GraphicsModule to create the shader. The shader is created from the string data.
     * 
     * @param gfxm The GraphicsModule to use for creating the shader.
     * @param data The shader data.
     * @param len The length of the shader data.
     * @param type The type of shader to create.
     * @param vertDef The vertex definition to use for the shader. Only required for vertex shaders.
     * @return The created shader.
     * @see ShaderModule::compile
     */
    inline sShader createShader(GraphicsModule* gfxm, const char* data, size_t len, sShaderType type, sVertexDefinition* vertDef=nullptr) {
        return internal_createShader(gfxm, data, len, type, vertDef);
    }

    /**
     * @brief Gets the shader extension for a given shader type.
     * 
     * @details Uses the GraphicsModule to get the shader extension. The shader extension is used to determine the file extension for the shader file.
     * 
     * @param type The type of shader to get the extension for.
     * @return The shader extension.
     */
    inline const char* shaderExtension(sShaderType type) {
        return internal_shaderExtension(type);
    }

    /**
     * @brief Add a shader extension to a shader name.
     * 
     * @details This is used to add the shader extension to the shader name. This is used to determine the file extension for the shader file.
     * 
     * @param name The shader name.
     * @param type The type of shader to get the extension for.
     * @return The shader name with the extension added.
     */
    inline const char* addShaderExtension(const char* name, sShaderType type) {
        const char* ext = shaderExtension(type);
        size_t slen = strlen(name) + strlen(ext) + 2;
        char* out = (char*)malloc(slen);
        snprintf(out, slen, "%s.%s", name, ext);
        return out;
    }

    /**
     * @brief Constructs a ShaderModule object.
     * 
     * @details The ShaderModule is a dynamic library that provides functionality for compiling and creating shaders. It uses the GraphicsModule to compile the shader. The shader is compiled from the file at the given path.
     * 
     * @param dynlib The dynamic library implementation to use.
     * @param dynp2 The output type. This is given as a string, normally straight from the graphics module's getShaderType function.
     * @see #GraphicsModule::getShaderType
     * @todo update this once the graphics module has docs
     */
    inline explicit ShaderModule(const char* dynlib, const char* dynp2) : Module(combine_strs_with_delim(dynlib, dynp2, '_'), "shdr") {
        internal_compile = (shader::Compile)lib.getSymbol("compile");
        internal_createShader = (shader::CreateShader)lib.getSymbol("createShader");
        internal_shaderExtension = (shader::ShaderExtension)lib.getSymbol("shaderExtension");
    }

    inline explicit ShaderModule(const char* dynlib) : Module(dynlib, "shdr") {
        internal_compile = (shader::Compile)lib.getSymbol("compile");
        internal_createShader = (shader::CreateShader)lib.getSymbol("createShader");
        internal_shaderExtension = (shader::ShaderExtension)lib.getSymbol("shaderExtension");
    }
};