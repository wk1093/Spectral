#include "module.h"

#include <vector>
#include <string>

sArenaAllocator* gArena = nullptr;

CEXPORT size_t getDesiredArenaSize() {
    return 0;
}

CEXPORT void moduleInit(sArenaAllocator* arena) {
    gArena = arena;
}

// #define DEBUG_SHADER

const char* hlsl_header = R"(
#define SAMPLE(tex, coord) spsltexture_##tex.Sample(spslsampler_##tex, coord)
float4x4 inverse(float4x4 m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

float3x3 convm4tom3(float4x4 m) {
    return (float3x3)(m);
}
)";

const char* glsl_header = R"(
#define mul(a,b) ((a)*(b))
#define SAMPLE(name, coord) texture((name), (coord))
mat3 convm4tom3(mat4 m) {
    return mat3(m);
}
)";

bool strbegw(const char* a, const char* b) {
    return strncmp(a, b, strlen(b)) == 0;
}
bool strendw(const char* a, const char* b) {
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    if (alen < blen) return false;
    return strncmp(a + alen - blen, b, blen) == 0;
}

sShader compile_glsl(GraphicsModule* gfxm, const char* shader, sShaderType type, sVertexDefinition* vertDef);
sShader compile_hlsl(GraphicsModule* gfxm, const char* shader, sShaderType type, sVertexDefinition* vertDef);
void preprocess(std::string& source);

CEXPORT sShader compile(GraphicsModule* gfxm, const char* path, sShaderType type, sVertexDefinition* vertDef) {
    std::string shd;
    if (!readFile(path, shd)) {
        printf("Failed to read file %s\n", path);
        return sShader();
    }
#ifdef SPECTRAL_OUTPUT_GLSL
    return compile_glsl(gfxm, shd.c_str(), type, vertDef);
#elif defined(SPECTRAL_OUTPUT_HLSL)
    return compile_hlsl(gfxm, shd.c_str(), type, vertDef);
#else
#error "Invalid shader output format"
#endif
}

CEXPORT sShader createShader(GraphicsModule* gfxm, const char* data, size_t len, sShaderType type, sVertexDefinition* vertDef) {
    char* str = (char*)malloc(len + 1);
    memcpy(str, data, len);
    str[len] = 0;
#ifdef SPECTRAL_OUTPUT_GLSL
    sShader s = compile_glsl(gfxm, str, type, vertDef);
#elif defined(SPECTRAL_OUTPUT_HLSL)
    sShader s = compile_hlsl(gfxm, str, type, vertDef);
#else
#error "Invalid shader output format"
#endif
    free(str);
    return s;
}


sShader compile_glsl(GraphicsModule* gfxm, const char* shader, sShaderType type, sVertexDefinition* vertDef) {
    std::string source = shader;

    preprocess(source);

    // first pass: #VS_IN -> layouts
    // second pass: #VS_OUT -> outputs
    // third pass: names
    // fourth pass: uniforms NOT IMPLEMENTED YET

    source = glsl_header + source;

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

    while (true) {
        size_t unif = source.find("#UNIFORM");
        if (unif == std::string::npos) break;
        source.replace(unif, 8, "uniform");
    }

    while (true) {
        size_t tex = source.find("#TEXTURE");
        if (tex == std::string::npos) break;
        source.replace(tex, 8, "uniform sampler2D");
    }

    // spsl_position -> gl_Position
    while (true) {
        size_t pos = source.find("spsl_position");
        if (pos == std::string::npos) break;
        source.replace(pos, 13, "gl_Position");
    }
    if (type == sShaderType::FRAGMENT) {
        source = "out vec4 spsl_fragcolor;\nlayout(origin_upper_left) in vec4 gl_FragCoord;\n" + source;
        // spsl_fragpos -> gl_FragCoord
        while (true) {
            size_t pos = source.find("spsl_fragpos");
            if (pos == std::string::npos) break;
            source.replace(pos, 12, "gl_FragCoord");
        }
    }

    source = "#version 330 core\n" + source;
    // that is all for now
#ifdef DEBUG_SHADER
    // write to file with same name but with .glsl extension
    std::string outpath = path + std::string(".glsl");
    FILE* f = fopen(outpath.c_str(), "w");
    if (f == nullptr) {
        printf("Failed to write to file %s\n", outpath.c_str());
    } else {
        fwrite(source.c_str(), 1, source.size(), f);
        fclose(f);
    }
#endif
    sShader s = gfxm->createShader(source.c_str(), type, vertDef);
    if (s.internal == nullptr) {
        printf("Failed to compile shader\n");
        printf("Generated source:\n%s\n", source.c_str());
    }
    return s;
}

sShader compile_hlsl(GraphicsModule* gfxm, const char* shader, sShaderType type, sVertexDefinition* vertDef) {
    std::string source = shader;
    preprocess(source);


    std::string unifs;

    while (true) {
        size_t unif = source.find("#UNIFORM");
        if (unif == std::string::npos) break;
        size_t eof0 = source.find("\r", unif);
        size_t eof1 = source.find("\n", unif);
        size_t eof = eof0 < eof1 ? eof0 : eof1;
        std::string line = source.substr(unif + 8, eof - unif - 8);
        unifs += line + "\n";
        source.replace(unif, eof - unif, "");
    }

    source = "cbuffer SPSL_Uniforms {\n" + unifs + "};\n" + source;
    source = hlsl_header + source;
    // vecX -> floatX
    size_t offset = 0;
    while (true) {
        size_t vec = source.find("vec", offset);
        if (vec == std::string::npos) break;
        // make sure the next character is a digit and then after that cannot be a letter/digit/underscore/identifier
        // if there is any identifier-like character before or after the name, skip it
        if (vec > 0 && (isalnum(source[vec - 1]) || source[vec - 1] == '_')) {
            offset = vec + 1;
            continue;
        }
        if (vec + 4 < source.size() && (isalnum(source[vec + 4]) || source[vec + 4] == '_')) {
            offset = vec + 1;
            continue;
        }
        if (isdigit(source[vec + 3])) {
            source.replace(vec, 3, "float");
        }
        offset = vec + 1;
    }

    // matX -> floatXxX
    offset = 0;
    while (true) {
        size_t mat = source.find("mat", offset);
        if (mat == std::string::npos) break;
        if (mat > 0 && (isalnum(source[mat - 1]) || source[mat - 1] == '_')) {
            offset = mat + 1;
            continue;
        }
        if (mat + 4 < source.size() && (isalnum(source[mat + 4]) || source[mat + 4] == '_')) {
            offset = mat + 1;
            continue;
        }
        if (isdigit(source[mat + 3])) {
            source.replace(mat, 3, "float");
            source.insert(mat + 5, "x");
            source.insert(mat + 5, 1, source[mat + 6]);
        }
        offset = mat + 1;
    }

    if (type == sShaderType::VERTEX) {
        std::string inputs;
        std::string outputs;

        // second pass: #VS_IN -> inputs
        size_t input_count = 0;
        std::vector<std::string> input_names;
        while (true) {
            size_t vsin = source.find("#VS_IN");
            if (vsin == std::string::npos) break;
            // find end of line
            // size_t eol = source.find("\n", vsin);
            size_t eol0 = source.find("\r", vsin);
            size_t eol1 = source.find("\n", vsin);
            size_t eol = eol0 < eol1 ? eol0 : eol1;
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
            // size_t eol = source.find("\n", vsout);
            size_t eol0 = source.find("\r", vsout);
            size_t eol1 = source.find("\n", vsout);
            size_t eol = eol0 < eol1 ? eol0 : eol1;

            std::string line = source.substr(vsout+7, eol-vsout-7);
            size_t semicolon = line.find(";");
            line.replace(semicolon, 1, ":");
            // find the last identifier before the semicolon
            size_t last_space = line.rfind(" ", semicolon);
            std::string name = line.substr(last_space + 1, semicolon - last_space - 1);
            output_names.push_back(name);
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
        for (size_t i = 0; i < output_names.size(); i++) {
            size_t offset = 0;
            while (true) {
                size_t pos = source.find(output_names[i], offset);
                if (pos == std::string::npos) break;
                if (pos > 0 && source[pos - 1] == '.') {
                    offset = pos + 1;
                    continue;
                }
                // if there is any identifier-like character before or after the name, skip it
                if (pos > 0 && (isalnum(source[pos - 1]) || source[pos - 1] == '_')) {
                    offset = pos + output_names[i].size();
                    continue;
                }
                if (pos + output_names[i].size() < source.size() && (isalnum(source[pos + output_names[i].size()]) || source[pos + output_names[i].size()] == '_')) {
                    offset = pos + output_names[i].size();
                    continue;
                }
                source.replace(pos, output_names[i].size(), "output." + output_names[i]);
            }
        }

        for (size_t i = 0; i < input_names.size(); i++) {
            size_t offset = 0;
            while (true) {
                size_t pos = source.find(input_names[i], offset);
                if (pos == std::string::npos) break;
                // if the name has input. in front of it, skip it
                if (pos > 0 && source[pos - 1] == '.') {
                    offset = pos + input_names[i].size();
                    continue;
                }
                // if there is any identifier-like character before or after the name, skip it
                if (pos > 0 && (isalnum(source[pos - 1]) || source[pos - 1] == '_')) {
                    offset = pos + input_names[i].size();
                    continue;
                }
                if (pos + input_names[i].size() < source.size() && (isalnum(source[pos + input_names[i].size()]) || source[pos + input_names[i].size()] == '_')) {
                    offset = pos + input_names[i].size();
                    continue;
                }
                source.replace(pos, input_names[i].size(), "input." + input_names[i]);
            }
        }

        // sixth pass: turn all output names into output.name

        source = "struct SPSL_VS_Input {\n" + inputs + "};\nstruct SPSL_VS_Output {\nfloat4 position : SV_POSITION;" + outputs + "};\n" + source;


#ifdef DEBUG_SHADER
        // write to file with same name but with .hlsl extension
        std::string outpath = path + std::string(".hlsl");
        FILE* f = fopen(outpath.c_str(), "w");
        if (f == nullptr) {
            printf("Failed to write to file %s\n", outpath.c_str());
        } else {
            fwrite(source.c_str(), 1, source.size(), f);
            fclose(f);
        }
#endif
        sShader s = gfxm->createShader(source.c_str(), type, vertDef);
        if (s.internal == nullptr) {
            printf("Failed to compile shader\n");
            printf("Generated source:\n%s\n", source.c_str());
        }
        return s;
    } else {
        // std::string inputs;
        std::stringstream inputs_stream;

        // second pass: #FS_IN -> inputs
        std::vector<std::string> input_names;
        while (true) {
            size_t fsin = source.find("#FS_IN");
            if (fsin == std::string::npos) break;
            // size_t eol = source.find("\n", fsin);
            // we want to use \r or \n
            size_t eol0 = source.find("\r", fsin);
            size_t eol1 = source.find("\n", fsin);
            size_t eol = eol0 < eol1 ? eol0 : eol1;
            std::string line = source.substr(fsin+6, eol - fsin-6);
            size_t semicolon = line.find(";");
            line.replace(semicolon, 1, ":");
            size_t last_space = line.rfind(" ", semicolon);
            std::string name = line.substr(last_space + 1, semicolon - last_space - 1);
            input_names.push_back(name);
            source.replace(fsin, eol - fsin, "");
            // inputs += line + "C_" + name + "e;\n";
            // the above line seems to corrupt in rare cases and the inputs string becomes gibberish
            inputs_stream << line << "C_" << name << "e;\n";
        }

        std::string inputs = inputs_stream.str();

        // for some reason there is \r and wierd characters in the inputs string

        // spsl_fragpos -> input.position
        while (true) {
            size_t pos = source.find("spsl_fragpos");
            if (pos == std::string::npos) break;
            source.replace(pos, 12, "input.position");
        }

        // #TEXTURE test ->
        // Texture2D spsltexture_test : register(t0);
        // SamplerState spslsampler_test : register(s0);
        size_t texs = 0;
        while (true) {
            size_t tex = source.find("#TEXTURE");
            if (tex == std::string::npos) break;
            // size_t eol = source.find("\n", tex);
            size_t eol0 = source.find("\r", tex);
            size_t eol1 = source.find("\n", tex);
            size_t eol = eol0 < eol1 ? eol0 : eol1;
            std::string name = source.substr(tex + 9, eol - tex - 10);
            
            source.replace(tex, eol - tex, "Texture2D spsltexture_" + name + " : register(t" + std::to_string(texs) + ");\nSamplerState spslsampler_" + name + " : register(s" + std::to_string(texs) + ");\n");
        }
        // input names
        for (size_t i = 0; i < input_names.size(); i++) {
            size_t offset = 0;
            while (true) {
                size_t pos = source.find(input_names[i], offset);
                if (pos == std::string::npos) break;
                if (pos > 0 && source[pos - 1] == '.') {
                    offset = pos + input_names[i].size();
                    continue;
                }
                // if there is any identifier-like character before or after the name, skip it
                if (pos > 0 && (isalnum(source[pos - 1]) || source[pos - 1] == '_')) {
                    offset = pos + input_names[i].size();
                    continue;
                }
                if (pos + input_names[i].size() < source.size() && (isalnum(source[pos + input_names[i].size()]) || source[pos + input_names[i].size()] == '_')) {
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
        // size_t close_brace = source.find("}", open_brace);
        // the above line breaks if there is a } in a comment or a string
        size_t close_brace = source.rfind("}");

        std::string main_body = source.substr(open_brace + 1, close_brace - open_brace - 1);
        source.replace(main, close_brace - main + 1, "float4 main(SPSL_FS_Input input) : SV_TARGET { float4 spsl_fragcolor; " + main_body + " return spsl_fragcolor; }");
        
        source = "struct SPSL_FS_Input {\nfloat4 position : SV_POSITION;" + inputs + "};\n" + source;

#ifdef DEBUG_SHADER
        // write to file with same name but with .hlsl extension
        std::string outpath = path + std::string(".hlsl");
        FILE* f = fopen(outpath.c_str(), "w");
        if (f == nullptr) {
            printf("Failed to write to file %s\n", outpath.c_str());
        } else {
            fwrite(source.c_str(), 1, source.size(), f);
            fclose(f);
        }
#endif
        sShader s = gfxm->createShader(source.c_str(), type, vertDef);
        if (s.internal == nullptr) {
            printf("Failed to compile shader\n");
        }
        return s;
    }

}

void preprocess(std::string& source) {
    // only one little thing we do here for now
    // #ifgl and #endifgl
    // #ifhl and #endifhl
    // everything in between is removed if the output format is not GLSL or HLSL respectively
#ifdef SPECTRAL_OUTPUT_GLSL
    std::string to_remove = "#ifhl";
    std::string to_remove_end = "#endifhl";
    std::string to_keep = "#ifgl";
    std::string to_keep_end = "#endifgl";
#elif defined(SPECTRAL_OUTPUT_HLSL)
    std::string to_remove = "#ifgl";
    std::string to_remove_end = "#endifgl";
    std::string to_keep = "#ifhl";
    std::string to_keep_end = "#endifhl";
#else
#error "Invalid shader output format"
#endif
    while (true) {
        size_t ifremove = source.find(to_remove);
        if (ifremove == std::string::npos) break;
        size_t ifremove_end = source.find(to_remove_end, ifremove);
        if (ifremove_end == std::string::npos) {
            printf("Unterminated %s\n", to_remove.c_str());
            break;
        }
        source.replace(ifremove, ifremove_end - ifremove + to_remove_end.size(), "");
    }

    while (true) {
        size_t ifkeep = source.find(to_keep);
        if (ifkeep == std::string::npos) break;
        size_t ifkeep_end = source.find(to_keep_end, ifkeep);
        if (ifkeep_end == std::string::npos) {
            printf("Unterminated %s\n", to_keep.c_str());
            break;
        }
        // just remove the #if and #endif, but keep the inside
        source.replace(ifkeep, to_keep.size(), "");
        source.replace(ifkeep_end, to_keep_end.size(), "");
    }

    // remove everything after any '//' until the end of the line
    while (true) {
        size_t comment = source.find("//");
        if (comment == std::string::npos) break;
        // size_t eol = source.find("\n", comment);
        size_t eol0 = source.find("\r", comment);
        size_t eol1 = source.find("\n", comment);
        size_t eol = eol0 < eol1 ? eol0 : eol1;
        source.replace(comment, eol - comment, "");
    }

}