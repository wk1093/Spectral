// TODO: use the following link to compile glsl shaders into hlsl
// maybe https://github.com/alaingalvan/CrossShader if Spirv-Cross doesn't work how I want it to

#include "module.h"

#include <spirv_cross_c.h>

void compileToSpirv(const char* data, size_t len, char** spirvOut, size_t* spirvLen) {
    
}

CEXPORT sShader createShader(GraphicsModule* gfx, const char* data, size_t len, sShaderType type, sVertexDefinition* vertDef) {
    char* spirvdata = nullptr;
    size_t spirvlen = 0;
    compileToSpirv(data, len, &spirvdata, &spirvlen);
#ifndef SPECTRAL_OUTPUT_SPIRV
    spvc_context context;
    spvc_context_create(&context);

    spvc_parsed_ir ir;
    spvc_context_parse_spirv(context, (const uint32_t*)spirvdata, spirvlen / sizeof(uint32_t), &ir);

    spvc_compiler compiler;
    spvc_compiler_options options;
#ifdef SPECTRAL_OUTPUT_GLSL330
    spvc_context_create_compiler(context, SPVC_BACKEND_GLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler);
    spvc_compiler_create_compiler_options(compiler, &options);
    spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, 330);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ES, false);
    
#elif defined(SPECTRAL_OUTPUT_GLSL450)
    spvc_context_create_compiler(context, SPVC_BACKEND_GLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler);
    spvc_compiler_create_compiler_options(compiler, &options);
    spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, 450);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ES, false);
#elif defined(SPECTRAL_OUTPUT_GLES)
    spvc_context_create_compiler(context, SPVC_BACKEND_GLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler);
    spvc_compiler_create_compiler_options(compiler, &options);
    spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, 200);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ES, true);
#elif defined(SPECTRAL_OUTPUT_HLSL)
    spvc_context_create_compiler(context, SPVC_BACKEND_HLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler);
    spvc_compiler_create_compiler_options(compiler, &options);
    spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_HLSL_SHADER_MODEL, 50);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_HLSL_ENABLE_16BIT_TYPES, true);
#endif
    spvc_compiler_install_compiler_options(compiler, options);
    spvc_compiler_compile(compiler, &spirvdata);
    spirvlen = strlen(spirvdata);
    spvc_context_destroy(context);
#endif
    sShader shader = gfx->createShader(spirvdata, type, vertDef, spirvlen);
    free(spirvdata);
    return shader;
}

CEXPORT sShader compile(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return {};
    }
    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* data = (char*)malloc(len + 1);
    fread(data, 1, len, file);
    fclose(file);
    data[len] = '\0';
    
    sShader shader = createShader(gfxm, data, len, type, vertDef);
    
    free(data);
    
    return shader;
}