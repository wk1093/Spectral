#include "module.h"

bool strbegw(const char* a, const char* b) {
    return strncmp(a, b, strlen(b)) == 0;
}
bool strendw(const char* a, const char* b) {
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    if (alen < blen) return false;
    return strncmp(a + alen - blen, b, blen) == 0;
}

sShader compile(GraphicsModule* gfxm, const char* path, sShaderType type) {
    // This will be done in steps, first we lex/parse/interpret the input SPSL
    // then there is an ifdef for the target output language, and we generate the output code accordingly
    // then we compile the output code using the gfxm object

    // SPSL is designed to be as easy to parse/transpile as possible, and that is why it is mainly for testing the Spectral engine, less for actual use in the future of the Engine
    // The language might be expanded in the future, but for not it is made simply for the purpose of testing the engine
    std::string source;
    if (!readFile(path, source)) {
        printf("Error reading file\n");
        return {nullptr};
    }
    if (source.empty() || source.size() < 16) {
        printf("Invalid shader source: Too Short!\n");
        return {nullptr};
    }
    if (!strbegw(source.c_str(), "#SHADER")) {
        printf("Invalid shader source: Invalid Start!\n");
        return {nullptr};
    }

    switch (type) {
        case sShaderType::VERTEX: {
            if (!strbegw(source.c_str(), "#SHADER VERTEX")) {
                printf("Invalid shader type\n");
                return {nullptr};
            }
        } break;
        case sShaderType::FRAGMENT: {
            if (!strbegw(source.c_str(), "#SHADER FRAGMENT")) {
                printf("Invalid shader type\n");
                return {nullptr};
            }
        } break;
        default:
            printf("Unsupported shader type\n");
            return {nullptr};
    }

    printf("SHADER MODULE UNFINISHED\n");
    printf("USE PLATFORM SPECIFIC SHADERS FOR NOW\n");
    return {nullptr};

}