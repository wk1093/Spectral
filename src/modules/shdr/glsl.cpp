// TODO: use the following link to compile glsl shaders into hlsl
// maybe https://github.com/alaingalvan/CrossShader if Spirv-Cross doesn't work how I want it to

#include "module.h"

#include <spirv_cross_c.h>

sArenaAllocator* gArena = nullptr;

CEXPORT size_t getDesiredArenaSize() {
    return 0;
}

CEXPORT void moduleInit(sArenaAllocator* arena) {
    gArena = arena;
}

CEXPORT const char* shaderExtension(sShaderType type) {
    switch (type) {
        case sShaderType::VERTEX:
            return "vert.spv";
        case sShaderType::FRAGMENT:
            return "frag.spv";
        default:
            return nullptr;
    }
}

CEXPORT sShader createShader(GraphicsModule* gfx, const char* data, size_t len, sShaderType type, sVertexDefinition* vertDef) {
    const char* spirvdata = data;
    size_t spirvlen = len;

    const char* outputdata = nullptr;
    size_t outputLen = 0;

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
    // flatten_uniform_buffer_blocks
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_EMIT_UNIFORM_BUFFER_AS_PLAIN_UNIFORMS, true);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_FORCE_FLATTENED_IO_BLOCKS , true);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_VULKAN_SEMANTICS, false);

    const spvc_reflected_resource* uniformBlocks = nullptr;
    spvc_resources resources;
    spvc_compiler_create_shader_resources(compiler, &resources);
    size_t numUniformBlocks = 0;
    spvc_resources_get_resource_list_for_type(resources, SPVC_RESOURCE_TYPE_UNIFORM_BUFFER, &uniformBlocks, &numUniformBlocks);
    for (size_t i = 0; i < numUniformBlocks; i++) {
        spvc_variable_id id = uniformBlocks[i].id;
        spvc_compiler_flatten_buffer_block(compiler, id);
    }
    // we want to disable the GL_ARB_shading_language_420pack
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ENABLE_420PACK_EXTENSION, false);
    
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
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_HLSL_FORCE_STORAGE_BUFFER_AS_UAV, true);
#endif
    spvc_compiler_install_compiler_options(compiler, options);
    spvc_result res = spvc_compiler_compile(compiler, (const char**)(&outputdata));
    if (res != SPVC_SUCCESS) {
        fprintf(stderr, "Failed to compile shader: %d\n", (int)res);
        spvc_context_destroy(context);
        return {};
    }
    spirvlen = strlen(outputdata);
    char* copied_data = (char*)malloc(spirvlen + 1);
    memcpy(copied_data, outputdata, spirvlen);
    copied_data[spirvlen] = 0;
    outputdata = copied_data;

    spvc_context_destroy(context); // this destroys the outputdata, so we need to copy it
    printf("SHADER CONTENT:\n%s\n", outputdata);
#else
    outputLen = spirvlen;
    outputdata = spirvdata;
#endif
    sShader shader = gfx->createShader(outputdata, type, vertDef, spirvlen);
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