/**
 * @file gfx/module.h
 * @brief Graphics module header file.
 */

#pragma once
#include "../moduleLib.h"

#include "../win/module.h"

#include <stdio.h>
#include <initializer_list>
#include <cstdint>
#include <cstdlib>
#include <string>

/**
 * @brief Index type for vertex buffers.
 * 
 * This type is used to index into vertex buffers and is typically a 32-bit unsigned integer.
 * It is used to represent the indices of vertices in a mesh.
 */
typedef unsigned int sIndex;

struct GraphicsModule;

/**
 * @brief A single abstract vertex definition.
 * 
 * This structure defines the layout of a vertex in a vertex buffer. It contains an array of integers that represent the number of components for each attribute in the vertex, and a count of the number of attributes.
 */
struct sVertexDefinition {
    int* elements;
    size_t count;
};

/**
 * @brief Get the size (in bytes) of a vertex definition.
 * 
 * This function calculates the size of a vertex definition by summing the sizes of each attribute in the vertex. The size is calculated as the sum of the number of components for each attribute multiplied by the size of a float (4 bytes).
 * @todo There should be multiple types allowed in a vertex definition, not just float, but this requires a lot of changes.
 */
inline size_t vertexDefinitionSize(sVertexDefinition* def) {
    size_t size = 0;
    for (size_t i = 0; i < def->count; i++) {
        size += def->elements[i] * sizeof(float);
    }
    return size;
}

/**
 * @brief Uniform type enumeration.
 * 
 * This enumeration defines the types of uniforms that can be used in shaders. It includes float, int, and bool types.
 */
enum class sUniformType {
    FLOAT,
    INT,
    BOOL
};

/**
 * @brief Get the size (in bytes) of a uniform type.
 * 
 * This function returns the size of a uniform type in bytes. This should be multiplied by the number of components if the uniform is an array/vector/matrix.
 */
inline size_t uniformTypeSize(sUniformType type) {
    switch (type) {
        case sUniformType::FLOAT:
            return sizeof(float);
        case sUniformType::INT:
            return sizeof(int);
        case sUniformType::BOOL:
            return sizeof(bool);
    }
    return 0;
}

/**
 * @brief Shader type enumeration.
 * 
 * This enumeration defines the types of shaders that can be used in a shader program. It includes vertex, fragment, and geometry shaders.
 * 
 * @todo Make geometry shaders work.
 * @todo Compute shaders.
 * @todo Tessellation shaders.
 */
enum sShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

/**
 * @brief Uniform element structure.
 * 
 * This structure defines a single uniform element in a shader program. It contains the shader type, name, type, and size of the uniform. The size is defined as countx * county, where countx is the number of components and county is the number of elements in the array.
 * 
 * @todo Add support for arrays like vec3[] and mat4[].
 * @todo Test if uneven matrices work (e.g. mat3x4).
 */
struct sUniformElement {
    sShaderType shaderType;
    const char* name;
    sUniformType type;
    size_t countx;
    size_t county=1;
    /// @brief Construct a matrix uniform element.
    /// @param shaderType The shader that this uniform belongs to.
    /// @param name The name of the uniform in the shader.
    /// @param type The type of the uniform.
    /// @param countx The x size of the matrix (e.g. 4 for a mat4).
    /// @param county The y size of the matrix (e.g. 4 for a mat4).
    inline sUniformElement(sShaderType shaderType, const char* name, sUniformType type, size_t countx, size_t county) : shaderType(shaderType), name(name), type(type), countx(countx), county(county) {}
    
    /// @brief Construct a vector or scalar uniform element.
    /// @param shaderType The shader that this uniform belongs to.
    /// @param name The name of the uniform in the shader.
    /// @param type The type of the uniform.
    /// @param countx The size of the vector (e.g. 4 for a vec4).
    inline sUniformElement(sShaderType shaderType, const char* name, sUniformType type, size_t countx) : shaderType(shaderType), name(name), type(type), countx(countx) {}
};

/**
 * @brief Get the size (in bytes) of a uniform element.
 * 
 * This function returns the size of a uniform element in bytes. This is calculated as countx * county * size of the type.
 */
inline size_t uniformElementSize(sUniformElement element) {
    return element.countx * element.county * uniformTypeSize(element.type);
}

/**
 * @brief Uniform definition structure.
 * 
 * This structure defines a uniform definition, which is a collection of uniform elements. It contains an array of uniform elements and a count of the number of elements.
 */
struct sUniformDefinition {
    sUniformElement* elements;
    size_t count;

    /// @brief Construct a uniform definition from an initializer list of uniform elements.
    /// @param elements The initializer list of uniform elements.
    inline sUniformDefinition(std::initializer_list<sUniformElement> elements) {
        // we cant just cast begin to a pointer, because the array will be destroyed
        this->elements = (sUniformElement*)malloc(sizeof(sUniformElement) * elements.size());
        this->count = elements.size();
        size_t i = 0;
        for (auto it = elements.begin(); it != elements.end(); it++) {
            this->elements[i++] = *it;
        }
    }
    /// @brief Get the size (in bytes) of the uniform definition.
    /// @return The size of the uniform definition in bytes.
    inline size_t size() {
        size_t size = 0;
        for (size_t i = 0; i < count; i++) {
            size += elements[i].countx * elements[i].county * uniformTypeSize(elements[i].type);
        }
        return size;
    }

    /// @brief Construct an empty uniform definition.
    inline sUniformDefinition() : elements(nullptr), count(0) {}

    /// @brief Construct a uniform definition from an array of uniform elements.
    /// @param elements The array of uniform elements.
    /// @param count The number of uniform elements.
    inline sUniformDefinition(sUniformElement* elements, size_t count) : elements(elements), count(count) {}
};

/**
 * @brief Get a partial uniform definition from a uniform definition.
 * 
 * This function returns a new uniform definition that contains only the elements of the specified shader type. This is useful for creating uniform definitions for specific shaders.
 * This function shouldn't really be needed for the user, but is used internally.
 * 
 * @param def The original uniform definition.
 * @param type The shader type to filter by.
 * @return A new uniform definition that contains only the elements of the specified shader type.
 */
inline sUniformDefinition getPartialf(sUniformDefinition def, sShaderType type) {
    size_t count = 0;
    for (size_t i = 0; i < def.count; i++) {
        if (def.elements[i].shaderType == type) {
            count++;
        }
    }
    sUniformElement* elements = (sUniformElement*)malloc(sizeof(sUniformElement) * count);
    if (elements == nullptr) {
        printf("ERROR: Malloc failed\n");
    }
    size_t j = 0;
    for (size_t i = 0; i < def.count; i++) {
        if (def.elements[i].shaderType == type) {
            elements[j++] = def.elements[i];
        }
    }
    return {elements, count};
}

/**
 * @brief Uniforms structure.
 * 
 * This structure represents a collection of uniforms that can be used in a shader program. Implementation defined elsewhere.
 * It is used to set and get uniforms in a shader program.
 */
struct sUniforms {
    void* internal;
};

/**
 * @brief Mesh structure.
 * 
 * This structure represents a mesh that can be used in a shader program. Implementation defined elsewhere.
 * It is used to create and draw meshes to the screen.
 */
struct sMesh {
    void* internal;
    GraphicsModule* creator;
};

/**
 * @brief Shader structure.
 * 
 * This structure represents a shader that can be used in a shader program. Implementation defined elsewhere.
 * It is a single shader (not a shader program) and is used to create shader programs.
 */
struct sShader {
    void* internal;
};

/**
 * @brief Shader program structure.
 * 
 * This structure represents a shader program that can be used to render meshes. Implementation defined elsewhere.
 * It is a collection of shaders that are used together to render a mesh.
 */
struct sShaderProgram {
    void* internal;
    GraphicsModule* creator;
};

/**
 * @brief Texture definition structure.
 * 
 * This structure defines a texture that can be used in a shader program. It contains the width, height, number of channels, and data of the texture.
 * The data is a pointer to the raw texture data, which is an array of unsigned char values.
 * This cannot be used directly, but is used to create a texture.
 * 
 * @warning This is an intermediate structure and should be used only to create a texture.
 */
struct sTextureDefinition {
    size_t width;
    size_t height;
    size_t channels;
    unsigned char* data;
};

/**
 * @brief Texture structure.
 * 
 * This structure represents a texture that can be used in a shader program. Implementation defined elsewhere.
 * It is used to create and use textures in a shader program.
 */
struct sTexture {
    void* internal;
};

/// @cond DOXYGEN_OMIT
namespace graphics {
    typedef void (*SetClearColor)(float r, float g, float b, float a);
    typedef void (*Clear)();
    typedef void (*Init)(sWindow* win);
    typedef sMesh (*CreateMesh)(sShader vertexShader, void* vertices, size_t vertexSize, sIndex* indices, size_t indexSize);
    typedef void (*DrawMesh)(sMesh mesh);
    typedef void (*UseShaderProgram)(sShaderProgram shader);
    typedef sShader (*CreateShader)(const char* source, sShaderType type, sVertexDefinition* vertDef);
    typedef sShaderProgram (*CreateShaderProgram)(sShader* shaders, size_t count);
    typedef void (*Present)();
    typedef const char* (*GetShaderType)();
    typedef sUniforms (*CreateUniforms)(sShaderProgram program, sUniformDefinition def);
    typedef void (*SetUniforms)(sUniforms uniforms, void* data);
    typedef sTexture (*CreateTexture)(sTextureDefinition def);
    typedef void (*UseTexture)(sShaderProgram program, sTexture texture, const char* name);
    typedef void (*FreeTexture)(sTexture texture);
    typedef void (*FreeShader)(sShader shader);
    typedef void (*FreeShaderProgram)(sShaderProgram program);
    typedef void (*FreeMesh)(sMesh mesh);
    typedef void (*FreeUniforms)(sUniforms uniforms);
    typedef void (*Destroy)();
    typedef void (*SetScissor)(int x, int y, int width, int height);
    typedef void (*EnableScissor)();
    typedef void (*DisableScissor)();
}
/// @endcond

/**
 * @brief Graphics module class.
 * 
 * This class represents the graphics module that is used to render graphics to the screen. It contains all the functions that are used to create and use shaders, meshes, and textures.
 * It is used to initialize the graphics module and create graphics objects.
 */
struct GraphicsModule : Module {
private:
    graphics::SetClearColor internal_setClearColor;
    graphics::Clear internal_clear;
    graphics::Init internal_init;
    graphics::CreateMesh internal_createMesh;
    graphics::DrawMesh internal_drawMesh;
    graphics::UseShaderProgram internal_useShaderProgram;
    graphics::CreateShaderProgram internal_createShaderProgram;
    graphics::CreateShader internal_createShader;
    graphics::Present internal_present;
    graphics::GetShaderType internal_getShaderType;
    graphics::CreateUniforms internal_createUniforms;
    graphics::SetUniforms internal_setUniforms;
    graphics::CreateTexture internal_createTexture;
    graphics::UseTexture internal_useTexture;
    graphics::FreeTexture internal_freeTexture;
    graphics::FreeShader internal_freeShader;
    graphics::FreeShaderProgram internal_freeShaderProgram;
    graphics::FreeMesh internal_freeMesh;
    graphics::FreeUniforms internal_freeUniforms;
    graphics::Destroy internal_destroy;
    graphics::SetScissor internal_setScissor;
    graphics::EnableScissor internal_enableScissor;
    graphics::DisableScissor internal_disableScissor;
public:

    sWindow* win;

    /**
     * @brief Initialize the graphics module.
     * 
     * This function initializes the graphics module with the specified window.
     * 
     * @param win The window to initialize the graphics module with.
     */
    inline void init(sWindow* win) {
        this->win = win;
        internal_init(win);
    }

    /**
     * @brief Set the clear color for the graphics module.
     * 
     * This function sets the clear color for the graphics module. This color is used to clear the screen before rendering.
     * 
     * @param r The red component of the clear color (0-1).
     * @param g The green component of the clear color (0-1).
     * @param b The blue component of the clear color (0-1).
     * @param a The alpha component of the clear color (0-1).
     */
    inline void setClearColor(float r, float g, float b, float a) {
        internal_setClearColor(r, g, b, a);
    }

    /**
     * @brief Clear the screen.
     * 
     * This function clears the screen with the clear color set by setClearColor. This MUST be called before rendering a new frame.
     * 
     * @warning This function must be called before rendering a new frame because it does more than just clearing the screen. It also sets up the viewport and other graphics state (on some platforms).
     */
    inline void clear() {
        internal_clear();
    }

    /**
     * @brief Present the rendered frame to the screen.
     * 
     * This function presents the rendered frame to the screen. This is typically called at the end of the rendering loop to display the rendered frame.
     * This function does not swap buffers, it just presents the frame. The actual buffer swap is done by the windowing system.
     * 
     * @warning This function must be called at the end of the rendering loop to display the rendered frame (on some platforms).
     */
    inline void present() {
        internal_present();
    }
    
    /**
     * @brief Set the scissor rectangle for rendering.
     * 
     * This function sets the scissor rectangle for rendering. The scissor rectangle is used to limit the area of the screen that is rendered to. This is useful for rendering to a specific area of the screen or for rendering to a texture.
     * 
     * @param x The x coordinate of the scissor rectangle.
     * @param y The y coordinate of the scissor rectangle.
     * @param width The width of the scissor rectangle.
     * @param height The height of the scissor rectangle.
     */
    inline void setScissor(int x, int y, int width, int height) {
        internal_setScissor(x, y, width, height);
    }

    /**
     * @brief Enable the scissor rectangle for rendering.
     * 
     * This function enables the scissor rectangle for rendering. When the scissor rectangle is enabled, only the area inside the rectangle is rendered to.
     */
    inline void enableScissor() {
        internal_enableScissor();
    }

    /**
     * @brief Disable the scissor rectangle for rendering.
     * 
     * This function disables the scissor rectangle for rendering. When the scissor rectangle is disabled, the entire screen is rendered to.
     */
    inline void disableScissor() {
        internal_disableScissor();
    }

    /**
     * @brief Use a shader program for rendering.
     * 
     * This function uses the specified shader program for rendering. This must be called before rendering a mesh to use the shader program.
     * 
     * @param program The shader program to use for rendering.
     * @warning This function must be called before rendering a mesh to use the shader program.
     */
    inline void useShaderProgram(sShaderProgram program) {
        internal_useShaderProgram(program);
    }

    /**
     * @brief Draw a mesh to the screen.
     * 
     * This function draws the specified mesh to the screen. This must be called after using a shader program to render the mesh.
     * 
     * @param mesh The mesh to draw to the screen.
     */
    inline void drawMesh(sMesh mesh) {
        internal_drawMesh(mesh);
    }

    /**
     * @brief Free a mesh.
     * 
     * This function frees the specified mesh. This should be called when the mesh is no longer needed to free the resources used by the mesh.
     * 
     * @param mesh The mesh to free.
     */
    inline void freeMesh(sMesh mesh) {
        internal_freeMesh(mesh);
    }

    /**
     * @brief Free a shader.
     * 
     * This function frees the specified shader. This should be called when the shader is no longer needed to free the resources used by the shader.
     * 
     * @param shader The shader to free.
     */
    inline void freeShader(sShader shader) {
        internal_freeShader(shader);
    }

    /**
     * @brief Free a shader program.
     * 
     * This function frees the specified shader program. This should be called when the shader program is no longer needed to free the resources used by the shader program.
     */
    inline void freeShaderProgram(sShaderProgram program) {
        internal_freeShaderProgram(program);
    }

    /**
     * @brief Free a uniforms object.
     * 
     * This function frees the specified uniforms object. This should be called when the uniforms object is no longer needed to free the resources used by the uniforms object.
     */
    inline void freeUniforms(sUniforms uniforms) {
        internal_freeUniforms(uniforms);
    }

    /**
     * @brief Free a texture.
     * 
     * This function frees the specified texture. This should be called when the texture is no longer needed to free the resources used by the texture.
     */
    inline void freeTexture(sTexture texture) {
        internal_freeTexture(texture);
    }

    /**
     * @brief Destroy the graphics module.
     * 
     * This function destroys the graphics module and frees all resources used by the module.
     */
    inline void destroy() {
        internal_destroy();
    }

    /**
     * @brief Create a texture from a texture definition.
     * 
     * This function creates a texture from the specified texture definition.
     * 
     * @param def The texture definition to create the texture from.
     * @return The created texture.
     */
    inline sTexture createTexture(sTextureDefinition def) {
        sTexture texture = internal_createTexture(def);
        return texture;
    }

    /**
     * @brief Create a uniforms object from a shader program and a uniform definition.
     * 
     * This function creates a uniforms object from the specified shader program and uniform definition. This is used to set and get uniforms in a shader program.
     * 
     * @param program The shader program to create the uniforms object for.
     * @param def The uniform definition to create the uniforms object from.
     * @return The created uniforms object.
     */
    inline sUniforms createUniforms(sShaderProgram program, sUniformDefinition def) {
        sUniforms uniforms = internal_createUniforms(program, def);
        return uniforms;
    }

    /**
     * @brief Set the uniforms for a shader program.
     * 
     * This function sets the uniforms for the specified shader program. This must be called after creating a uniforms object to set the uniforms in the shader program.
     * 
     * @param uniforms The uniforms object to set the uniforms for.
     * @param data The data to set the uniforms to. This should be a pointer to the data that matches the uniform definition.
     * @warning This function must be called after creating a uniforms object to set the uniforms in the shader program.
     * @warning This void* data must be the same size as the uniform definition. This is not checked, so it is up to the user to ensure that the data is the correct size.
     */
    inline void setUniforms(sUniforms uniforms, void* data) {
        internal_setUniforms(uniforms, data);
    }

    /**
     * @brief Use a texture in a shader program.
     * 
     * This function uses the specified texture in the specified shader program. This must be called after using a shader program to use the texture in the shader program.
     * 
     * @param program The shader program to use the texture in.
     * @param texture The texture to use in the shader program.
     * @param name The name of the texture in the shader program.
     */
    inline void useTexture(sShaderProgram program, sTexture texture, const char* name) {
        internal_useTexture(program, texture, name);
    }

    /**
     * @brief Get the shader type as a string.
     * 
     * This function returns the shader language format as a string. This is used to determine the shader language required for the shader compilation module.
     */
    inline const char* getShaderType() {
        return internal_getShaderType();
    }

    /**
     * @brief Create a mesh from a vertex shader, vertices, and indices.
     * 
     * This function creates a mesh from the specified vertex shader, vertices, and indices. This is used to create a mesh that can be rendered to the screen.
     * 
     * @param vertexShader The vertex shader to use for the mesh.
     * @param vertices The vertices to use for the mesh. This should be a pointer to the vertex data.
     * @param vertexCount The number of vertices in the mesh.
     * @param indices The indices to use for the mesh. This should be a pointer to the index data.
     * @param indexCount The number of indices in the mesh.
     * @return The created mesh.
     */
    inline sMesh createMesh(sShader vertexShader, void* vertices, size_t vertexCount, sIndex* indices, size_t indexCount) {
        sMesh mesh = internal_createMesh(vertexShader, vertices, vertexCount, indices, indexCount);
        mesh.creator = this;
        return mesh;
    }

    /**
     * @brief Create a shader program from an array of shaders.
     * 
     * This function creates a shader program from the specified array of shaders. This is used to create a shader program that can be used to render meshes.
     * 
     * @param shaders The array of shaders to use for the shader program.
     * @param count The number of shaders in the array.
     * @return The created shader program.
     */
    inline sShaderProgram createShaderProgram(sShader* shaders, size_t count) {
        sShaderProgram program = internal_createShaderProgram(shaders, count);
        program.creator = this;
        return program;
    }

    /**
     * @brief Create a shader program from an initializer list of shaders.
     * 
     * This function creates a shader program from the specified initializer list of shaders. This is used to create a shader program that can be used to render meshes.
     * 
     * @param shaders The initializer list of shaders to use for the shader program.
     * @return The created shader program.
     */
    inline sShaderProgram createShaderProgram(std::initializer_list<sShader> shaders) {
        return createShaderProgram((sShader*)shaders.begin(), shaders.size());
    }

    /**
     * @brief Create a vertex definition from an array of elements.
     * 
     * This function creates a vertex definition from the specified array of elements. This is used to define the layout of a vertex in a vertex buffer.
     * 
     * @param elements The array of elements to use for the vertex definition. This should be a pointer to the element data.
     * @param count The number of elements in the array.
     * @return The created vertex definition.
     */
    inline sVertexDefinition* createVertexDefinition(int* elements, size_t count) {
        sVertexDefinition* def = (sVertexDefinition*)malloc(sizeof(sVertexDefinition));
        // def->elements = elements;
        def->elements = (int*)malloc(sizeof(int) * count);
        if (def->elements == nullptr) {
            printf("ERROR: Malloc failed\n");
        }
        for (size_t i = 0; i < count; i++) {
            def->elements[i] = elements[i];
        }
        def->count = count;
        return def;
    }

    /**
     * @brief Create a vertex definition from an initializer list of elements.
     * 
     * This function creates a vertex definition from the specified initializer list of elements. This is used to define the layout of a vertex in a vertex buffer.
     * 
     * @param elements The initializer list of elements to use for the vertex definition.
     * @return The created vertex definition.
     */
    inline sVertexDefinition* createVertexDefinition(std::initializer_list<int> elements) {
        return createVertexDefinition((int*)elements.begin(), elements.size());
    }

    /**
     * @brief Free a vertex definition.
     * 
     * This function frees the specified vertex definition. This should be called when the vertex definition is no longer needed to free the resources used by the vertex definition.
     * 
     * @param def The vertex definition to free.
     */
    inline void freeVertexDefinition(sVertexDefinition* def) {
        // free(def->elements);
        free(def);
    }

    /**
     * @brief Create a shader from a source string and shader type.
     * 
     * This function creates a shader from the specified source string and shader type, and a vertex definition if the shader is a vertex shader. This is used to create a shader that can be used in a shader program.
     * 
     * @param source The source string to use for the shader. This should be a pointer to the source string.
     * @param type The type of the shader to create. This should be one of the shader types defined in sShaderType.
     * @param vertDef The vertex definition to use for the shader. This should be a pointer to the vertex definition. This is only used if the shader is a vertex shader.
     * @return The created shader.
     */
    inline sShader createShader(const char* source, sShaderType type, sVertexDefinition* vertDef=nullptr) {
        sShader shader = internal_createShader(source, type, vertDef);
        return shader;
    }

    /**
     * @brief Create a shader from a file path and shader type.
     * 
     * This function creates a shader from the specified file path and shader type, and a vertex definition if the shader is a vertex shader. This is used to create a shader that can be used in a shader program.
     * 
     * @param path The file path to the shader source file. This should be a pointer to the file path string.
     * @param type The type of the shader to create. This should be one of the shader types defined in sShaderType.
     * @param vertDef The vertex definition to use for the shader. This should be a pointer to the vertex definition. This is only used if the shader is a vertex shader.
     * @return The created shader.
     */
    inline sShader loadShader(const char* path, sShaderType type, sVertexDefinition* vertDef=nullptr) {
        std::string source;
        if (!readFile(path, source)) {
            printf("Error reading file\n");
            return {nullptr};
        }
        return createShader(source.c_str(), type, vertDef);
    }

    /**
     * @brief Construct a graphics module from a dynamic library.
     * 
     * This function constructs a graphics module from the specified dynamic library. This is used to load the graphics module at runtime.
     * 
     * @param dynlib The implementation string to use for the graphics module.
     */
    inline explicit GraphicsModule(const char* dynlib) : Module(dynlib, "gfx") {
        internal_setClearColor = (graphics::SetClearColor)lib.getSymbol("setClearColor");
        internal_clear = (graphics::Clear)lib.getSymbol("clear");
        internal_init = (graphics::Init)lib.getSymbol("init");
        internal_createMesh = (graphics::CreateMesh)lib.getSymbol("createMesh");
        internal_drawMesh = (graphics::DrawMesh)lib.getSymbol("drawMesh");
        internal_useShaderProgram = (graphics::UseShaderProgram)lib.getSymbol("useShaderProgram");
        internal_createShaderProgram = (graphics::CreateShaderProgram)lib.getSymbol("createShaderProgram");
        internal_createShader = (graphics::CreateShader)lib.getSymbol("createShader");
        internal_present = (graphics::Present)lib.getSymbol("present");
        internal_getShaderType = (graphics::GetShaderType)lib.getSymbol("getShaderType");
        internal_createUniforms = (graphics::CreateUniforms)lib.getSymbol("createUniforms");
        internal_setUniforms = (graphics::SetUniforms)lib.getSymbol("setUniforms");
        internal_createTexture = (graphics::CreateTexture)lib.getSymbol("createTexture");
        internal_useTexture = (graphics::UseTexture)lib.getSymbol("useTexture");
        internal_freeTexture = (graphics::FreeTexture)lib.getSymbol("freeTexture");
        internal_freeShader = (graphics::FreeShader)lib.getSymbol("freeShader");
        internal_freeShaderProgram = (graphics::FreeShaderProgram)lib.getSymbol("freeShaderProgram");
        internal_freeMesh = (graphics::FreeMesh)lib.getSymbol("freeMesh");
        internal_freeUniforms = (graphics::FreeUniforms)lib.getSymbol("freeUniforms");
        internal_destroy = (graphics::Destroy)lib.getSymbol("destroy");
        internal_setScissor = (graphics::SetScissor)lib.getSymbol("setScissor");
        internal_enableScissor = (graphics::EnableScissor)lib.getSymbol("enableScissor");
        internal_disableScissor = (graphics::DisableScissor)lib.getSymbol("disableScissor");
    }
};