/**
 * @file tex/module.h
 * @brief Texture loading module header file.
 */

#pragma once

#include "../moduleLib.h"
#include "../gfx/module.h"

/// @cond DOXYGEN_OMIT
namespace texload {
    typedef sTextureDefinition (*LoadTexture)(const char*);
    typedef void (*FreeTexture)(sTextureDefinition);
    typedef sTextureDefinition (*LoadTextureFromBuffer)(const uint8_t*, size_t);
}
/// @endcond

/**
 * @brief Texture loading module class.
 * 
 * This class represents the texture loading module and provides methods for loading and freeing textures.
 * It is responsible for loading the texture loading library and providing access to its functions.
 */
struct TextureModule : public Module {
private:
    texload::LoadTexture internal_loadTexture;
    texload::FreeTexture internal_freeTexture;
    texload::LoadTextureFromBuffer internal_loadTextureFromBuffer;
public:

    /**
     * @brief Loads a texture from a file path.
     * 
     * This function loads a texture from the specified file path and returns a texture definition.
     * 
     * @param filepath The path to the texture to load
     * @return A texture definition containing the loaded texture data.
     */
    inline sTextureDefinition loadTexture(const char* filepath) {
        return internal_loadTexture(filepath);
    }

    /**
     * @brief Frees a texture definition.
     * 
     * This function frees the specified texture definition. This should be called when the texture is no longer needed to free the resources used by the texture.
     * 
     * @param texture The texture definition to free.
     * @note This function should be called when the texture is no longer needed to free up resources.
     */
    inline void freeTexture(sTextureDefinition texture) {
        internal_freeTexture(texture);
    }

    /**
     * @brief Loads a texture from a buffer.
     * 
     * This function loads a texture from the specified buffer and size, and returns a texture definition.
     * 
     * @param buffer The byte buffer containing the texture data.
     * @param size The size of the byte buffer.
     * @return A texture definition containing the loaded texture data.
     */
    inline sTextureDefinition loadTextureFromBuffer(const uint8_t* buffer, size_t size) {
        return internal_loadTextureFromBuffer(buffer, size);
    }

    /**
     * @brief Constructor for the TextureModule class.
     * 
     * This constructor loads the texture loading library and initializes the function pointers.
     * 
     * @param path The name/id of the implementation to load (ex: "stb").
     */
    TextureModule(const char* path) : Module(path, "tex") {
        internal_loadTexture = (texload::LoadTexture)lib.getSymbol("loadTexture");
        internal_freeTexture = (texload::FreeTexture)lib.getSymbol("freeTexture");
        internal_loadTextureFromBuffer = (texload::LoadTextureFromBuffer)lib.getSymbol("loadTextureFromBuffer");
    }
};