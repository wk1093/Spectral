// TODO: use the following link to compile glsl shaders into hlsl
// maybe https://github.com/alaingalvan/CrossShader if Spirv-Cross doesn't work how I want it to

#include "module.h"

#include <spirv_cross_c.h>
#include <filesystem>
#include <stdio.h>


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

    sShaderReflection refl = {};

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
    // spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_EMIT_UNIFORM_BUFFER_AS_PLAIN_UNIFORMS, true);
    // spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_FORCE_FLATTENED_IO_BLOCKS , true);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_VULKAN_SEMANTICS, false);

    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ENABLE_420PACK_EXTENSION, false);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_FORCE_ZERO_INITIALIZED_VARIABLES, true);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_EMIT_LINE_DIRECTIVES, true);
    
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

    // Reflect uniforms
    spvc_resources resources;
    spvc_compiler_create_shader_resources(compiler, &resources);
    size_t numUniformBlocks = 0;
    std::vector<sUniformElement> uniformElements;
    const spvc_reflected_resource* uniformBlocks = nullptr;
    spvc_resources_get_resource_list_for_type(resources, SPVC_RESOURCE_TYPE_UNIFORM_BUFFER, &uniformBlocks, &numUniformBlocks);
    if (numUniformBlocks > 1) {
        printf("Shader must have only one uniform block\n");
        spvc_context_destroy(context);
        return {};
    } else if (numUniformBlocks == 1) {
        spvc_variable_id id = uniformBlocks[0].id;
        printf("UNIF '%s'\n", uniformBlocks[0].name);
        refl.uniformName = strdup(uniformBlocks[0].name);
        spvc_type_id typeId = uniformBlocks[0].type_id;
        spvc_type baseType = spvc_compiler_get_type_handle(compiler, typeId);
        spvc_compiler_get_declared_struct_size(compiler, baseType, &refl.uniformSize);
        printf("UNIF SIZE %zu\n", refl.uniformSize);

        std::vector<size_t> elementSizes;
        elementSizes.push_back(0);
        spvc_compiler_get_declared_struct_member_size(compiler, baseType, elementSizes.size()-1, &elementSizes[elementSizes.size()-1]);
        while (elementSizes[elementSizes.size()-1] != 0) {
            elementSizes.push_back(0);
            spvc_compiler_get_declared_struct_member_size(compiler, baseType, elementSizes.size()-1, &elementSizes[elementSizes.size()-1]);
            // if sum of elementSizes is equal to or greater than refl.uniformSize, break
            size_t sum = 0;
            for (size_t i = 0; i < elementSizes.size(); i++) {
                sum += elementSizes[i];
            }
            if (sum >= refl.uniformSize) {
                break;
            }
        }
        elementSizes.pop_back();
        size_t numElements = elementSizes.size();
        printf("UNIF ELEMENTS %zu\n", numElements);
        for (size_t i = 0; i < numElements; i++) {
            printf("UNIF ELEMENT %zu SIZE %zu\n", i, elementSizes[i]);
        }
        
    } else {
        printf("No uniform blocks found\n");
    }

    // reflect in/outs
    const spvc_reflected_resource* inputs = nullptr;
    const spvc_reflected_resource* outputs = nullptr;
    size_t numInputs = 0;
    size_t numOutputs = 0;
    spvc_resources_get_resource_list_for_type(resources, SPVC_RESOURCE_TYPE_STAGE_INPUT, &inputs, &numInputs);
    spvc_resources_get_resource_list_for_type(resources, SPVC_RESOURCE_TYPE_STAGE_OUTPUT, &outputs, &numOutputs);
    if (numInputs > 0) {
        for (size_t i = 0; i < numInputs; i++) {
            printf("IN '%s'\n", inputs[i].name);

        }
    }
    if (numOutputs > 0) {
        for (size_t i = 0; i < numOutputs; i++) {
            printf("OUT '%s'\n", outputs[i].name);
        }
    }
    


#ifdef SPECTRAL_OUTPUT_SPIRV
    outputLen = spirvlen;
    outputdata = spirvdata;
#else
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
#endif


    spvc_context_destroy(context); // this destroys the outputdata, so we need to copy it
    // printf("SHADER CONTENT:\n%s\n", outputdata);

#ifdef DEBUG_BUILD
    static int shaderCount = 0;
    // write the shader to a file
    std::filesystem::path path = std::filesystem::current_path() / "temp";
    std::filesystem::create_directories(path);
    // std::filesystem::path shaderPath = path / (type == sShaderType::VERTEX ? "vert.spv" : "frag.spv");
    std::string filename = (type == sShaderType::VERTEX ? "vert" : "frag") + std::to_string(shaderCount++) + ".shader";
    std::filesystem::path shaderPath = path / filename;
    FILE* file = fopen(shaderPath.string().c_str(), "wb");
    if (file) {
        fwrite(outputdata, 1, spirvlen, file);
        fclose(file);
    } else {
        fprintf(stderr, "Failed to write shader to file: %s\n", shaderPath.string().c_str());
    }
#endif



    sShader shader = gfx->createShader(outputdata, type, vertDef, spirvlen);
    shader.reflection = refl;
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