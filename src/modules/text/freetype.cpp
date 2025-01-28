#include "module.h"

/*
Freetype font rendering is going to use a pretty cool approach
We are going to make a texture atlas with all the characters in it
Then when we create a text object, we will generate a set of vertices containing correct uv coords to select the correct character from the texture atlas
There will be multiple vertices (4 for each character)
*/

#include <ft2build.h>
#include FT_FREETYPE_H

struct sFreeTypeContext {
    FT_Library ft;
    GraphicsModule* gfxm;
};

static sFreeTypeContext __freetype_context;

CEXPORT void init(GraphicsModule* gfxm) {
    FT_Error error = FT_Init_FreeType(&__freetype_context.ft);
    if (error) {
        printf("Error initializing FreeType\n");
    }
    __freetype_context.gfxm = gfxm;
}

struct sInternalFont {
    FT_Face face;
    sVertexDefinition* vertDef;
    sShaderProgram shader;
    sUniforms uniforms;
    sTexture atlas;
    struct CharacterDef {
        vec2 offset;
        vec2 size;
        vec2 bearing;
        uint32_t advance;
    } characters[128]; // for now just ascii is supported
};

CEXPORT sFont loadFont(const char* path, int size) {
    FT_Face face;
    FT_Error error = FT_New_Face(__freetype_context.ft, path, 0, &face);
    if (error) {
        printf("Error loading font\n");
        return {nullptr};
    }
    error = FT_Set_Pixel_Sizes(face, 0, size);
    if (error) {
        printf("Error setting font size\n");
        return {nullptr};
    }

    sInternalFont* internal = (sInternalFont*)malloc(sizeof(sInternalFont));
    if (!internal) {
        printf("Error allocating memory for font\n");
        return {nullptr};
    }
    internal->face = face;

    // create a texture atlas
    // HOW
    return {internal};
}