#include "module.h"

/*
Freetype font rendering is going to use a pretty cool approach
We are going to make a texture atlas with all the characters in it
Then when we create a text object, we will generate a set of vertices containing correct uv coords to select the correct character from the texture atlas
There will be multiple vertices (4 for each character)
*/

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

struct sFreeTypeContext {
    FT_Library ft;
    GraphicsModule* gfxm;
    ShaderModule* shdr;
};

static sFreeTypeContext __freetype_context;

CEXPORT void init(GraphicsModule* gfxm, ShaderModule* shdr) {
    FT_Error error = FT_Init_FreeType(&__freetype_context.ft);
    if (error) {
        printf("Error initializing FreeType\n");
    }
    __freetype_context.gfxm = gfxm;
    __freetype_context.shdr = shdr;
}

struct sInternalFont {
    sVertexDefinition* vertDef;
    sShaderProgram shader;
    sShader vertexShader;
    sUniforms uniforms;
    sTexture atlas;
    struct CharacterDef {
        vec2 offset;
        vec2 size;
        vec2 bearing;
        double advance;
    } characters[128]; // for now just ascii is supported
    int atlasWidth;
    int atlasHeight;
    float scale;
};

struct TextVertex {
    vec2 pos;
    vec2 uv;
};

struct TextUniforms {
    vec3 color;
    mat4 proj;
    mat4 view;
    mat4 model;
    float z;
};

CEXPORT sFont loadFont(const char* path, int size, const char* vertpath, const char* fragpath) {
    FT_Face face;
    FT_Error error = FT_New_Face(__freetype_context.ft, path, 0, &face);
    if (error) {
        printf("Error loading font\n");
        printf("Error code: %d\n", error);
        return {nullptr};
    }

    sInternalFont* internal = (sInternalFont*)malloc(sizeof(sInternalFont));
    if (!internal) {
        printf("Error allocating memory for font\n");
        return {nullptr};
    }

    FT_Set_Char_Size(face, 0, size << 6, 96, 96);
    internal->scale = 1.0f;

    int max_dim = (1+ (face->size->metrics.height >> 6)) * ceilf(sqrtf(128));
    int tex_width = 1;
    while (tex_width < max_dim) tex_width <<= 1;
    int tex_height = tex_width;

    char* pixels = (char*)calloc(tex_width * tex_height, 1);
    int pen_x = 0;
    int pen_y = 0;

    for (int i = 0; i < 128; ++i) {
        FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_NORMAL);
        FT_Bitmap* bmp = &face->glyph->bitmap;

        if (pen_x + bmp->width >= tex_width) {
            pen_x = 0;
            pen_y += ((face->size->metrics.height >> 6) + 1);
        }

        for (int row = 0; row < bmp->rows; ++row) {
            for (int col = 0; col < bmp->width; ++col) {
                int x = pen_x + col;
                int y = pen_y + row;
                pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
            }
        }

        internal->characters[i].offset = vec2(pen_x, pen_y);
        internal->characters[i].size = vec2(bmp->width, bmp->rows);
        internal->characters[i].bearing = vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        // internal->characters[i].advance = face->glyph->advance.x >> 6;
        // we lose some precision by doing that, so let's cast to double
        internal->characters[i].advance = (double)face->glyph->advance.x / 64.0;

        pen_x += bmp->width + 1;
    }

    FT_Done_Face(face);

    sTextureDefinition texDef = {
        .width = tex_width,
        .height = tex_height,
        .channels = 1,
        .data = (uint8_t*)pixels
    };
    internal->atlas = __freetype_context.gfxm->createTexture(texDef);
    internal->atlasWidth = tex_width;
    internal->atlasHeight = tex_height;

    free(pixels);

    internal->vertDef = __freetype_context.gfxm->createVertexDefinition({2, 2});
    if (vertexDefinitionSize(internal->vertDef) != sizeof(TextVertex)) {
        printf("ERROR: Vertex definition size does not match vertex size\n");
        return {nullptr};
    }
    sShader vert = __freetype_context.shdr->compile(__freetype_context.gfxm, vertpath, sShaderType::VERTEX, internal->vertDef);
    sShader frag = __freetype_context.shdr->compile(__freetype_context.gfxm, fragpath, sShaderType::FRAGMENT);
    internal->shader = __freetype_context.gfxm->createShaderProgram({vert, frag});
    internal->vertexShader = vert;

    sUniformDefinition uniformDef = {
        {sShaderType::FRAGMENT, "uColor", sUniformType::FLOAT, 3},
        {sShaderType::VERTEX, "uProj", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uView", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uModel", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uZ", sUniformType::FLOAT, 1}
    };
    if (uniformDef.size() != sizeof(TextUniforms)) {
        printf("ERROR: Uniform definition size does not match shader data size\n");
        return {nullptr};
    }

    internal->uniforms = __freetype_context.gfxm->createUniforms(internal->shader, uniformDef);

    return {internal};
}

struct sInternalText {
    sInternalFont* font;
    char* text;
    TextUniforms uniforms;
    size_t vertexCount; // should be 4 * strlen(text)
    TextVertex* vertices;
    sMesh mesh;
};

CEXPORT sText createText(sFont font, const char* text) {
    sInternalFont* internalFont = (sInternalFont*)font.internal;

    sInternalText* internal = (sInternalText*)malloc(sizeof(sInternalText));
    if (!internal) {
        printf("Error allocating memory for text\n");
        return {nullptr};
    }

    internal->font = internalFont;
    internal->text = strdup(text);
    internal->uniforms.model = identity();
    internal->uniforms.view = identity();
    internal->uniforms.proj = identity();
    internal->uniforms.color = vec3{1.0f, 1.0f, 1.0f};
    internal->uniforms.z = 0.0f;


    internal->vertexCount = 4 * strlen(text);
    internal->vertices = (TextVertex*)malloc(sizeof(TextVertex) * internal->vertexCount);
    if (!internal->vertices) {
        printf("Error allocating memory for text vertices\n");
        return {nullptr};
    }

    double x = 0.0f;
    double y = font.size / (2);

    float scale = internalFont->scale;

    std::vector<uint32_t> indices;

    for (size_t i = 0; i < strlen(text); ++i) {
        char c = text[i];
        sInternalFont::CharacterDef def = internalFont->characters[c];

        if (c == ' ') {
            x += def.advance;
            continue;
        }

        float xpos = x + def.bearing.x;
        float ypos = y - (def.size.y - def.bearing.y);

        float w = def.size.x;
        float h = def.size.y;

        TextVertex vertices[4] = {
            {{xpos*scale, (ypos + h)*scale}, {def.offset.x / internalFont->atlasWidth, def.offset.y / internalFont->atlasHeight}},
            {{xpos*scale, ypos*scale}, {def.offset.x / internalFont->atlasWidth, (def.offset.y + h) / internalFont->atlasHeight}},
            {{(xpos + w)*scale, ypos*scale}, {(def.offset.x + w) / internalFont->atlasWidth, (def.offset.y + h) / internalFont->atlasHeight}},
            {{(xpos + w)*scale, (ypos + h)*scale}, {(def.offset.x + w) / internalFont->atlasWidth, def.offset.y / internalFont->atlasHeight}}
        };

        indices.push_back(i * 4);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 3);


        memcpy(&internal->vertices[i * 4], vertices, sizeof(vertices));

        x += def.advance;
    }

    internal->mesh = __freetype_context.gfxm->createMesh(internalFont->vertexShader, internal->vertices, sizeof(TextVertex) * internal->vertexCount, indices.data(), indices.size() * sizeof(uint32_t));

    return {internal};
}


CEXPORT void drawText(sText text) {
    sInternalText* internal = (sInternalText*)text.internal;

    __freetype_context.gfxm->useShaderProgram(internal->font->shader);
    __freetype_context.gfxm->setUniforms(internal->font->uniforms, &internal->uniforms);
    __freetype_context.gfxm->useTexture(internal->font->shader, internal->font->atlas, "tex0");
    __freetype_context.gfxm->drawMesh(internal->mesh);
}

CEXPORT void freeText(sText text) {
    sInternalText* internal = (sInternalText*)text.internal;

    __freetype_context.gfxm->freeMesh(internal->mesh);
    free(internal->vertices);
    free(internal->text);
    free(internal);
}

CEXPORT void freeFont(sFont font) {
    sInternalFont* internal = (sInternalFont*)font.internal;

    __freetype_context.gfxm->freeTexture(internal->atlas);
    __freetype_context.gfxm->freeUniforms(internal->uniforms);
    __freetype_context.gfxm->freeShader(internal->vertexShader);
    __freetype_context.gfxm->freeShaderProgram(internal->shader);
    __freetype_context.gfxm->freeVertexDefinition(internal->vertDef);
    free(internal);
}

CEXPORT void setTextColor(sText text, vec3 color) {
    sInternalText* internal = (sInternalText*)text.internal;
    internal->uniforms.color = color;
}

CEXPORT void setTextModel(sText text, mat4 model) {
    sInternalText* internal = (sInternalText*)text.internal;
    internal->uniforms.model = model;
}

CEXPORT void setTextView(sText text, mat4 view) {
    sInternalText* internal = (sInternalText*)text.internal;
    internal->uniforms.view = view;
}

CEXPORT void setTextProj(sText text, mat4 proj) {
    sInternalText* internal = (sInternalText*)text.internal;
    internal->uniforms.proj = proj;
}

CEXPORT vec2 measureText(sFont font, const char* text) {
    sInternalFont* internal = (sInternalFont*)font.internal;

    float x = 0.0f;   // Current x position
    float maxTextWidth = 0.0f;
    float maxY = 0.0f;
    for (size_t i = 0; i < strlen(text); ++i) {
        char c = text[i];
        sInternalFont::CharacterDef def = internal->characters[c];

        // X position for the glyph
        float xpos = x + def.advance;

        if (c == ' ') {
            maxTextWidth += def.advance / 2;
            break;
        }

        maxTextWidth = fmaxf(maxTextWidth, xpos);
        maxY = fmaxf(maxY, def.bearing.y + def.size.y);


        x += def.advance;
    }

    return {maxTextWidth * internal->scale, maxY * internal->scale};
}

CEXPORT void setTextZ(sText text, float z) {
    sInternalText* internal = (sInternalText*)text.internal;
    internal->uniforms.z = z;
}