#include <eogll.h>
#include "glutils.h"

#include "module.h"
#include <fstream>

CEXPORT const char* getShaderType() {
    return "glsl";
}

CEXPORT void init(sWindow* win) {
    if (strcmp(win->creator->lib.mod_imp, "eogll") == 0) {
        auto* w = (EogllWindow*)win->internal;
        glfwMakeContextCurrent(w->window);
    } else if (strcmp(win->creator->lib.mod_imp, "glfw_gl") == 0) {
        auto* w = (GLFWwindow*)win->internal;
        glfwMakeContextCurrent(w);
        if (!gladLoadGL(glfwGetProcAddress)) {
            printf("Error loading OpenGL\n");
        }
    } else if (strcmp(win->creator->lib.mod_imp, "sf") == 0) {
        if (!gladLoadGL((GLADloadfunc)getProcAddress)) {
            printf("Error loading OpenGL\n");
        }
    } else if (strcmp(win->creator->lib.mod_imp, "glfw_noapi") == 0) {
        printf("GLFW with no API can't be used with OpenGL\n");
        printf("Select glfw_gl instead of glfw_noapi\n");
    }
}

CEXPORT void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

CEXPORT void clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

struct sInternalShader {
    unsigned int shader;
    sVertexDefinition* vertDef;
};

CEXPORT sMesh createMesh(sShader vertexShader, void* vertices, size_t vertexSize, sIndex* indices, size_t indexSize) {
    unsigned int vao = eogllGenVertexArray();
    unsigned int vbo = eogllGenBuffer(vao, GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    unsigned int ebo = eogllGenBuffer(vao, GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

    EogllAttribBuilder builder = eogllCreateAttribBuilder();
    sVertexDefinition* vertDef = ((sInternalShader*)vertexShader.internal)->vertDef;
    for (size_t i = 0; i < vertDef->count; i++) {
        eogllAddAttribute(&builder, GL_FLOAT, vertDef->elements[i]);
    }
    eogllBuildAttributes(&builder, vao);

    auto* mesh = (EogllBufferObject*)malloc(sizeof(EogllBufferObject));
    *mesh = eogllCreateBufferObject(vao, vbo, ebo, indexSize, GL_UNSIGNED_INT);

    return {mesh};
}

CEXPORT void drawMesh(sMesh mesh) {
    eogllDrawBufferObject((EogllBufferObject*)mesh.internal, GL_TRIANGLES);
}

CEXPORT sShader createShader(const char* source, sShaderType type, sVertexDefinition* vertDef) {
    unsigned int glType;
    switch (type) {
        case sShaderType::VERTEX:
            glType = GL_VERTEX_SHADER;
            if (vertDef == nullptr) {
                printf("Vertex shader requires vertex definition\n");
                return {nullptr};
            }
            break;
        case sShaderType::FRAGMENT:
            glType = GL_FRAGMENT_SHADER;
            break;
        case sShaderType::GEOMETRY:
            glType = GL_GEOMETRY_SHADER;
            break;
        default:
            printf("Invalid shader type\n");
            return {nullptr};
    }

    unsigned int shader = glCreateShader(glType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
        return {nullptr};
    }

    sInternalShader* internal = (sInternalShader*)malloc(sizeof(sInternalShader));
    internal->shader = shader;
    internal->vertDef = vertDef;
    return {internal};
}

CEXPORT void useShaderProgram(sShaderProgram shader) {
    glUseProgram(((sInternalShader*)shader.internal)->shader);
}

CEXPORT sShaderProgram createShaderProgram(sShader* shaders, size_t count) {
    unsigned int program = glCreateProgram();
    for (size_t i = 0; i < count; i++) {
        glAttachShader(program, *(unsigned int*)shaders[i].internal);
    }
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
        return {nullptr};
    }

    unsigned int* internal = (unsigned int*)malloc(sizeof(unsigned int));
    *internal = program;
    return {internal};
}

CEXPORT void present() {

}

struct sInternalUniforms {
    sUniformDefinition def;
    sShaderProgram program;
    int* locations;
};

CEXPORT sUniforms createUniforms(sShaderProgram program, sUniformDefinition def) {
    auto* uniforms = (sInternalUniforms*)malloc(sizeof(sInternalUniforms));
    uniforms->def = def;
    uniforms->program = program;
    uniforms->locations = (int*)malloc(sizeof(int) * def.count);
    for (size_t i = 0; i < def.count; i++) {
        uniforms->locations[i] = glGetUniformLocation(*(unsigned int*)program.internal, def.elements[i].name);
    }
    return {uniforms};
}

CEXPORT void setUniforms(sUniforms uniforms, void* data) {
    sInternalUniforms* internal = (sInternalUniforms*)uniforms.internal;
    sUniformDefinition def = internal->def;
    int* locations = internal->locations;

    size_t offset = 0;
    for (size_t i = 0; i < def.count; i++) {
        sUniformElement element = def.elements[i];
        int location = locations[i];
        void* curData = (void*)((size_t)data + offset);
        switch (element.type) {
            case sUniformType::FLOAT:
                if (element.county == 1) {
                    switch (element.countx) {
                        case 1:
                            glUniform1fv(location, 1, (float*)curData);
                            break;
                        case 2:
                            glUniform2fv(location, 1, (float*)curData);

                            break;
                        case 3:
                            glUniform3fv(location, 1, (float*)curData);
                            break;
                        case 4:
                            glUniform4fv(location, 1, (float*)curData);
                            break;
                        default:
                            glUniform1fv(location, element.countx, (float*)curData);
                    }
                    offset += uniformElementSize(element);
                } else if (element.countx == element.county) {
                    switch (element.countx) {
                        case 2:
                            glUniform2fv(location, 1, (float*)curData);
                            offset += uniformElementSize(element);
                            break;
                        case 3:
                            glUniform3fv(location, 1, (float*)curData);
                            offset += uniformElementSize(element);
                            break;
                        case 4:
                            glUniform4fv(location, 1, (float*)curData);
                            offset += uniformElementSize(element);
                            break;
                        default:
                            printf("Invalid matrix size\n");
                            return;
                    }
                } else {
                    printf("Invalid matrix size\n");
                    return;
                }
                break;
            case sUniformType::INT:
                if (element.county != 1) {
                    printf("Invalid uniform type: no int matrices\n");
                    return;
                }
                switch (element.countx) {
                    case 1:
                        glUniform1iv(location, 1, (int*)curData);
                        break;
                    case 2:
                        glUniform2iv(location, 1, (int*)curData);
                        break;
                    case 3:
                        glUniform3iv(location, 1, (int*)curData);
                        break;
                    case 4:
                        glUniform4iv(location, 1, (int*)curData);
                        break;
                    default:
                        glUniform1iv(location, element.countx, (int*)curData);
                }
                offset += uniformElementSize(element);
                break;
            case sUniformType::BOOL:
                if (element.county != 1) {
                    printf("Invalid uniform type: no bool matrices\n");
                    return;
                }
                glUniform1iv(location, element.countx, (int*)curData);
                offset += uniformElementSize(element);
                break;
            default:
                printf("Invalid uniform type\n");
        }
    }

}