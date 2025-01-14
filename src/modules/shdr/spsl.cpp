#include "module.h"

#include <vector>
#include <string>

bool strbegw(const char* a, const char* b) {
    return strncmp(a, b, strlen(b)) == 0;
}
bool strendw(const char* a, const char* b) {
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    if (alen < blen) return false;
    return strncmp(a + alen - blen, b, blen) == 0;
}

sShader compile_glsl(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef);
sShader compile_hlsl(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef);

CEXPORT sShader compile(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef) {
#ifdef SPECTRAL_OUTPUT_GLSL
    return compile_glsl(gfxm, path, type, vertDef);
#elif defined(SPECTRAL_OUTPUT_HLSL)
    return compile_hlsl(gfxm, path, type, vertDef);
#else
#error "Invalid shader output format"
#endif
}

sShader compile_glsl(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef) {
    printf("Compiling SPSL -> GLSL shader %s\n", path);
    std::string source;
    if (!readFile(path, source)) {
        printf("Failed to read file %s\n", path);
        return sShader();
    }

    // first pass: #VS_IN -> layouts
    // second pass: #VS_OUT -> outputs
    // third pass: names
    // fourth pass: uniforms NOT IMPLEMENTED YET

    size_t currentLayout = 0;
    while (true) {
        size_t vsin = source.find("#VS_IN");
        if (vsin == std::string::npos) break;
        source.replace(vsin, 6, "layout(location = " + std::to_string(currentLayout++) + ") in ");
    }

    while (true) {
        size_t vsout = source.find("#VS_OUT");
        if (vsout == std::string::npos) break;
        source.replace(vsout, 7, "out");
    }

    while (true) {
        size_t fsin = source.find("#FS_IN");
        if (fsin == std::string::npos) break;
        source.replace(fsin, 6, "in");
    }

    // spsl_position -> gl_Position
    while (true) {
        size_t pos = source.find("spsl_position");
        if (pos == std::string::npos) break;
        source.replace(pos, 13, "gl_Position");
    }
    if (type == sShaderType::FRAGMENT) {
        source = "out vec4 spsl_fragcolor;\n" + source;
    }

    source = "#version 330 core\n" + source;
    printf("GLSL source:\n%s\n", source.c_str());
    // that is all for now
    return gfxm->createShader(source.c_str(), type, vertDef);
}

sShader compile_hlsl(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef) {
    printf("Compiling SPSL -> HLSL shader %s\n", path);
    std::string source;
    
    if (!readFile(path, source)) {
        printf("Failed to read file %s\n", path);
        return sShader();
    }
    if (type == sShaderType::VERTEX) {
        std::string inputs;
        std::string outputs;

        // first pass: vecX -> floatX
        while (true) {
            size_t vec = source.find("vec");
            if (vec == std::string::npos) break;
            // make sure the next character is a digit and then after that cannot be a letter/digit/underscore/identifier
            if (isdigit(source[vec + 3]) && !isalnum(source[vec + 4]) && source[vec + 4] != '_') {
                source.replace(vec, 3, "float");
            }
        }

        // second pass: #VS_IN -> inputs
        size_t input_count = 0;
        std::vector<std::string> input_names;
        while (true) {
            size_t vsin = source.find("#VS_IN");
            if (vsin == std::string::npos) break;
            // find end of line
            size_t eol = source.find("\n", vsin);
            std::string line = source.substr(vsin+6, eol - vsin-6);
            // find ; at the end and replace it with :
            size_t semicolon = line.find(";");
            line.replace(semicolon, 1, ":");
            // find the last identifier before the semicolon
            size_t last_space = line.rfind(" ", semicolon);
            std::string name = line.substr(last_space + 1, semicolon - last_space - 1);
            input_names.push_back(name);
            source.replace(vsin, eol - vsin, "");
            inputs += line + "C_" + std::to_string(input_count++) + "e;\n";
        }

        // second pass #VS_OUT -> outputs
        // outputs will use a name instead of a number
        // so #VS_OUT vec3 testout; will become float3 testout : C_testoute;
        std::vector<std::string> output_names;
        while (true) {
            size_t vsout = source.find("#VS_OUT");
            if (vsout == std::string::npos) break;
            size_t eol = source.find("\n", vsout);
            std::string line = source.substr(vsout+7, eol-vsout-7);
            size_t semicolon = line.find(";");
            line.replace(semicolon, 1, ":");
            // find the last identifier before the semicolon
            size_t last_space = line.rfind(" ", semicolon);
            std::string name = line.substr(last_space + 1, semicolon - last_space - 1);
            output_names.push_back(name);
            printf("Output name: '%s'\n", name.c_str());
            source.replace(vsout, eol - vsout, "");
            outputs += line + "C_" + name + "e;\n";
        }

        // third pass: names
        // spsl_position -> spsl_output.position
        while (true) {
            size_t pos = source.find("spsl_position");
            if (pos == std::string::npos) break;
            source.replace(pos, 13, "output.position");
        }


        // fourth pass: main function
        // turn void main() { ... } into SPSL_VS_Output main(SPSL_VS_Input input) { SPSL_VS_Output output; ... return output; }

        size_t main = source.find("void main()");
        if (main == std::string::npos) {
            printf("No main function found\n");
            return sShader();
        }
        size_t open_brace = source.find("{", main);
        size_t close_brace = source.find("}", open_brace);
        std::string main_body = source.substr(open_brace + 1, close_brace - open_brace - 1);
        source.replace(main, close_brace - main + 1, "SPSL_VS_Output main(SPSL_VS_Input input) { SPSL_VS_Output output; " + main_body + " return output; }");

        // fifth pass: turn all input names into input.name
        size_t offset = 0;
        for (size_t i = 0; i < output_names.size(); i++) {
            while (true) {
                size_t pos = source.find(output_names[i], offset);
                if (pos == std::string::npos) break;
                if (pos > 0 && source[pos - 1] == '.') {
                    offset = pos + output_names[i].size();
                    continue;
                }
                source.replace(pos, output_names[i].size(), "output." + output_names[i]);
            }
        }

        offset = 0;
        for (size_t i = 0; i < input_names.size(); i++) {
            while (true) {
                size_t pos = source.find(input_names[i], offset);
                if (pos == std::string::npos) break;
                // if the name has input. in front of it, skip it
                if (pos > 0 && source[pos - 1] == '.') {
                    offset = pos + input_names[i].size();
                    continue;
                }
                source.replace(pos, input_names[i].size(), "input." + input_names[i]);
            }
        }

        // sixth pass: turn all output names into output.name

        source = "struct SPSL_VS_Input {\n" + inputs + "};\nstruct SPSL_VS_Output {\nfloat4 position : SV_POSITION;" + outputs + "};\n" + source;

        printf("HLSL source:\n%s\n", source.c_str());
        return gfxm->createShader(source.c_str(), type, vertDef);
    } else {
        std::string inputs;

        // first pass: vecX -> floatX
        while (true) {
            size_t vec = source.find("vec");
            if (vec == std::string::npos) break;
            // make sure the next character is a digit and then after that cannot be a letter/digit/underscore/identifier
            if (isdigit(source[vec + 3]) && !isalnum(source[vec + 4]) && source[vec + 4] != '_') {
                source.replace(vec, 3, "float");
            }
        }

        // second pass: #FS_IN -> inputs
        std::vector<std::string> input_names;
        while (true) {
            size_t fsin = source.find("#FS_IN");
            if (fsin == std::string::npos) break;
            size_t eol = source.find("\n", fsin);
            std::string line = source.substr(fsin+6, eol - fsin-6);
            size_t semicolon = line.find(";");
            line.replace(semicolon, 1, ":");
            size_t last_space = line.rfind(" ", semicolon);
            std::string name = line.substr(last_space + 1, semicolon - last_space - 1);
            input_names.push_back(name);
            source.replace(fsin, eol - fsin, "");
            inputs += line + "C_" + name + "e;\n";
        }

        // spsl_fragpos -> input.position
        while (true) {
            size_t pos = source.find("spsl_fragpos");
            if (pos == std::string::npos) break;
            source.replace(pos, 13, "input.position");
        }

        // input names
        size_t offset = 0;
        for (size_t i = 0; i < input_names.size(); i++) {
            while (true) {
                size_t pos = source.find(input_names[i], offset);
                if (pos == std::string::npos) break;
                if (pos > 0 && source[pos - 1] == '.') {
                    offset = pos + input_names[i].size();
                    continue;
                }
                source.replace(pos, input_names[i].size(), "input." + input_names[i]);
            }
        }

        size_t main = source.find("void main()");
        // void main() { ... } ->
        // float4 main(SPSL_FS_Input input) { float4 spsl_fragcolor; ... return spsl_fragcolor; }
        size_t open_brace = source.find("{", main);
        size_t close_brace = source.find("}", open_brace);
        std::string main_body = source.substr(open_brace + 1, close_brace - open_brace - 1);
        source.replace(main, close_brace - main + 1, "float4 main(SPSL_FS_Input input) : SV_TARGET { float4 spsl_fragcolor; " + main_body + " return spsl_fragcolor; }");
        
        source = "struct SPSL_FS_Input {\nfloat4 position : SV_POSITION;" + inputs + "};\n" + source;

        printf("HLSL source:\n%s\n", source.c_str());
        sShader s = gfxm->createShader(source.c_str(), type, vertDef);
        printf("Shader created\n");
        return s;
    }

}