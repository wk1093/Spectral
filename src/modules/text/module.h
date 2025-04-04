/**
 * @file text/module.h
 * @brief Text rendering module header file.
 */

#pragma once
#include "../moduleLib.h"

#include "../math/module.h"
#include "../gfx/module.h"
#include "../shdr/module.h"
#include "../asset.h"

/**
 * @brief Text rendering module class.
 * 
 * This class represents the text rendering module and provides methods for loading fonts, creating text, and rendering it.
 */
struct sFont {
    void* internal;
    int size;
};

/**
 * @brief Text structure.
 * 
 * This structure represents a text object and contains the internal representation of the text.
 * It is used to create and render text using the text rendering module.
 */
struct sText {
    void* internal;
};

/// @cond DOXYGEN_OMIT
namespace text {
    typedef void (*Init)(GraphicsModule* gfxm, ShaderModule* shdr, AssetLoader* assetm);
    typedef sFont (*LoadFont)(const char* path, int size, const char* vertpath, const char* fragpath);
    typedef sFont (*LoadFontAsset)(const char* path, int size, const char* vertpath, const char* fragpath);
    typedef sText (*CreateText)(sFont font, const char* text);
    typedef void (*DrawText)(sText text);
    typedef void (*FreeText)(sText text);
    typedef void (*FreeFont)(sFont font);
    typedef void (*SetTextColor)(sText text, vec3 color);
    typedef void (*SetTextModel)(sText text, mat4 model);
    typedef void (*SetTextView)(sText text, mat4 view);
    typedef void (*SetTextProj)(sText text, mat4 proj);
    typedef vec2 (*MeasureText)(sFont font, const char* text);
    typedef void (*SetTextZ)(sText text, float z);
}
/// @endcond

/**
 * @brief TextModule class.
 * 
 * This class represents the text rendering module and provides methods for loading fonts, creating text, and rendering it.
 * It is responsible for loading the text rendering library and providing access to its functions.
 */
struct TextModule : public Module {
private:
    text::Init internal_init;
    text::LoadFont internal_loadFont;
    text::LoadFontAsset internal_loadFontAsset;
    text::CreateText internal_createText;
    text::DrawText internal_drawText;
    text::FreeText internal_freeText;
    text::FreeFont internal_freeFont;
    text::SetTextColor internal_setTextColor;
    text::SetTextModel internal_setTextModel;
    text::SetTextView internal_setTextView;
    text::SetTextProj internal_setTextProj;
    text::MeasureText internal_measureText;
    text::SetTextZ internal_setTextZ;
public:
    /**
     * @brief Initializes the text rendering module.
     * 
     * This function initializes the text rendering module with the specified graphics module, shader module, and asset loader.
     * 
     * @param gfxm Pointer to the graphics module.
     * @param shdr Pointer to the shader module.
     * @param assetm Pointer to the asset loader.
     */
    inline void init(GraphicsModule* gfxm, ShaderModule* shdr, AssetLoader* assetm) {
        internal_init(gfxm, shdr, assetm);
    }

    /**
     * @brief Loads a font from a file path.
     * 
     * This function loads a font from the specified file path and size, and returns a font object.
     * 
     * @param path The path to the font file.
     * @param size The size of the font.
     * @param vertpath The path to the vertex shader file.
     * @param fragpath The path to the fragment shader file.
     * @return A font object containing the loaded font data.
     */
    inline sFont loadFont(const char* path, int size, const char* vertpath, const char* fragpath) {
        sFont f = internal_loadFont(path, size, vertpath, fragpath);
        f.size = size;
        return f;
    }

    /**
     * @brief Loads a font asset from a file path.
     * 
     * This function loads a font asset from the specified file path and size, and returns a font object.
     * 
     * @param path The path to the font asset file.
     * @param size The size of the font.
     * @param vertpath The path to the vertex shader file.
     * @param fragpath The path to the fragment shader file.
     * @return A font object containing the loaded font asset data.
     */
    inline sFont loadFontAsset(const char* path, int size, const char* vertpath, const char* fragpath) {
        sFont f = internal_loadFontAsset(path, size, vertpath, fragpath);
        f.size = size;
        return f;
    }

    /**
     * @brief Creates a text object from a font and text string.
     * 
     * This function creates a text object from the specified font and text string, and returns a text object.
     * 
     * @param font The font object to use for the text.
     * @param text The text string to create.
     * @return A text object containing the created text data.
     * @note The text object should be freed using the freeText function when it is no longer needed.
     */
    inline sText createText(sFont font, const char* text) {
        return internal_createText(font, text);
    }

    /**
     * @brief Draws a text object.
     * 
     * This function draws the specified text object using the graphics module.
     * 
     * @param text The text object to draw.
     * @return void
     * @see createText
     */
    inline void drawText(sText text) {
        internal_drawText(text);
    }

    /**
     * @brief Frees a text object.
     * 
     * This function frees the specified text object. This should be called when the text is no longer needed to free the resources used by the text.
     * 
     * @param text The text object to free.
     */
    inline void freeText(sText text) {
        internal_freeText(text);
    }

    /**
     * @brief Frees a font object.
     * 
     * This function frees the specified font object. This should be called when the font is no longer needed to free the resources used by the font.
     * 
     * @param font The font object to free.
     */
    inline void freeFont(sFont font) {
        internal_freeFont(font);
    }

    /**
     * @brief Sets the color of a text object.
     * 
     * This function sets the color of the specified text object.
     * 
     * @param text The text object to set the color for.
     * @param color The color to set for the text object.
     */
    inline void setTextColor(sText text, vec3 color) {
        internal_setTextColor(text, color);
    }

    /**
     * @brief Sets the model matrix of a text object.
     * 
     * This function sets the model matrix of the specified text object.
     * 
     * @param text The text object to set the model matrix for.
     * @param model The model matrix to set for the text object.
     */
    inline void setTextModel(sText text, mat4 model) {
        internal_setTextModel(text, model);
    }

    /**
     * @brief Sets the view matrix of a text object.
     * 
     */
    inline void setTextView(sText text, mat4 view) {
        internal_setTextView(text, view);
    }

    /**
     * @brief Sets the projection matrix of a text object.
     * 
     * This function sets the projection matrix of the specified text object.
     * 
     * @param text The text object to set the projection matrix for.
     * @param proj The projection matrix to set for the text object.
     */
    inline void setTextProj(sText text, mat4 proj) {
        internal_setTextProj(text, proj);
    }

    /**
     * @brief Measures the size of a text string using a font.
     * 
     * This function measures the size of the specified text string using the specified font and returns the size as a vec2.
     * 
     * @param font The font object to use for measuring the text.
     * @param text The text string to measure.
     * @return The size of the text string as a vec2.
     * @note This function does not create a text object and does not require freeing.
     */
    inline vec2 measureText(sFont font, const char* text) {
        return internal_measureText(font, text);
    }

    /**
     * @brief Sets the Z coordinate of a text object.
     * 
     * This function sets the Z coordinate of the specified text object.
     * 
     * @param text The text object to set the Z coordinate for.
     * @param z The Z coordinate to set for the text object.
     */
    inline void setTextZ(sText text, float z) {
        internal_setTextZ(text, z);
    }

    /**
     * @brief Constructor for the TextModule class.
     * 
     * This constructor initializes the text rendering module with the specified library path and sets up the function pointers for the text rendering functions.
     * 
     * @param path The implementation name/id to load (ex: "freetype").
     * @note This argument shouldn't be named "path" as it is not a path, but an identifier for the implementation to load.
     * @note This name should be the second part of the module name (ex "freetype" for "text_freetype.splmod").
     */
    inline TextModule(const char* path) : Module(path, "text") {
        internal_init = (text::Init)lib.getSymbol("init");
        internal_loadFont = (text::LoadFont)lib.getSymbol("loadFont");
        internal_loadFontAsset = (text::LoadFontAsset)lib.getSymbol("loadFontAsset");
        internal_createText = (text::CreateText)lib.getSymbol("createText");
        internal_drawText = (text::DrawText)lib.getSymbol("drawText");
        internal_freeText = (text::FreeText)lib.getSymbol("freeText");
        internal_freeFont = (text::FreeFont)lib.getSymbol("freeFont");
        internal_setTextColor = (text::SetTextColor)lib.getSymbol("setTextColor");
        internal_setTextModel = (text::SetTextModel)lib.getSymbol("setTextModel");
        internal_setTextView = (text::SetTextView)lib.getSymbol("setTextView");
        internal_setTextProj = (text::SetTextProj)lib.getSymbol("setTextProj");
        internal_measureText = (text::MeasureText)lib.getSymbol("measureText");
        internal_setTextZ = (text::SetTextZ)lib.getSymbol("setTextZ");
    }
};