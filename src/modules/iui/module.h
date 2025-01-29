#pragma once

#ifdef IUI_IMPLEMENTATION
#define CLAY_IMPLEMENTATION
#endif
#include "clay.h"

#include "../gfx/module.h"
#include "../text/module.h"
#include "../shdr/module.h"

struct sIUIGlobalState {
    GraphicsModule* gfxm;
    TextModule* textm;
    ShaderModule* shdr;

    sShaderProgram rect_shader;
    sVertexDefinition* rect_vert_def;
    sMesh rect_mesh;
    sUniforms rect_uniforms;

    sShaderProgram text_shader;
    sVertexDefinition* text_vert_def;
    sMesh text_mesh;

    sFont* fonts=nullptr;
};

sIUIGlobalState __globalIUIState;

struct sInternalRectVertex {
    vec2 pos;
};
// the rectangle will actually be stretched using a model matrix, so we can just put a basic rectangle here
sInternalRectVertex __rect_vertices[] = {
    {{0.0f, 0.0f}},
    {{1.0f, 0.0f}},
    {{1.0f, 1.0f}},
    {{0.0f, 1.0f}}
};

sIndex __rect_indices[] = {
    0, 1, 2,
    2, 3, 0
};
struct sInternalRectUniforms {
    vec4 color;
    mat4 proj;
    mat4 view;
    mat4 model;
};

void clayerr(Clay_ErrorData errorData) {
    printf("UI Error: %s\n", errorData.errorText.chars);
}

Clay_Dimensions Clay_Spectral_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData) {
    if (!__globalIUIState.fonts) {
        return {};
    }
    Clay_Dimensions result = {};
    vec2 size = __globalIUIState.textm->measureText(__globalIUIState.fonts[0], text.chars);
    result.width = size.x;
    result.height = size.y;
    return result;
}

void Clay_Spectral_Init(GraphicsModule* gfxm, TextModule* textm, ShaderModule* shdr, sWindow* win, sFont* fonts) {
    __globalIUIState.fonts = fonts;
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena) {
        .capacity = totalMemorySize,
        .memory = (char*)malloc(totalMemorySize)
    };
    Clay_Initialize(clayMemory, (Clay_Dimensions){win->width, win->height}, (Clay_ErrorHandler)clayerr);
    Clay_SetMeasureTextFunction(Clay_Spectral_MeasureText, 0);
    __globalIUIState.gfxm = gfxm;
    __globalIUIState.textm = textm;
    __globalIUIState.shdr = shdr;
    // pos, color
    sVertexDefinition* rect_vert_def = gfxm->createVertexDefinition({2});
    if (vertexDefinitionSize(rect_vert_def) != sizeof(sInternalRectVertex)) {
        printf("ERROR: Vertex definition size mismatch\n");
        return;
    }
    __globalIUIState.rect_vert_def = rect_vert_def;
    sShader rect_vert_shader = shdr->compile(gfxm, "spsl/iui/rect.spslv", sShaderType::VERTEX, rect_vert_def);
    sShader rect_frag_shader = shdr->compile(gfxm, "spsl/iui/rect.spslf", sShaderType::FRAGMENT);
    sShaderProgram rect_shader = gfxm->createShaderProgram({rect_vert_shader, rect_frag_shader});
    __globalIUIState.rect_shader = rect_shader;
    __globalIUIState.rect_mesh = gfxm->createMesh(rect_vert_shader, __rect_vertices, sizeof(__rect_vertices), __rect_indices, sizeof(__rect_indices));

    sUniformDefinition rect_uniform_def = {
        {sShaderType::FRAGMENT, "uColor", sUniformType::FLOAT, 4},
        {sShaderType::VERTEX, "uProj", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uView", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uModel", sUniformType::FLOAT, 4, 4}
    };
    if (rect_uniform_def.size() != sizeof(sInternalRectUniforms)) {
        printf("ERROR: Uniform definition size mismatch\n");
        return;
    }
    __globalIUIState.rect_uniforms = gfxm->createUniforms(rect_shader, rect_uniform_def);
}

// custom clay implementation using our graphics library
void Clay_Spectral_Render(sWindow* win, Clay_RenderCommandArray renderCommands, mat4 proj, mat4 view) {
    for (uint32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand* renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;
        switch (renderCommand->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
                Clay_Color color = config->color;

                sInternalRectUniforms uniforms = {};
                uniforms.color = {color.r, color.g, color.b, color.a};
                uniforms.proj = proj;
                uniforms.view = view;
                uniforms.model = translate({boundingBox.x, boundingBox.y, 0.0f}) * scale({boundingBox.width, boundingBox.height, 1.0f});

                __globalIUIState.gfxm->useShaderProgram(__globalIUIState.rect_shader);
                __globalIUIState.gfxm->setUniforms(__globalIUIState.rect_uniforms, &uniforms);
                __globalIUIState.gfxm->drawMesh(__globalIUIState.rect_mesh);
            } break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextElementConfig *config = renderCommand->config.textElementConfig;
                Clay_StringSlice text = renderCommand->text;
                char *cloned = (char*)malloc(text.length + 1);
                memcpy(cloned, text.chars, text.length);
                cloned[text.length] = '\0';
                sText textel = __globalIUIState.textm->createText(__globalIUIState.fonts[0], cloned);
                __globalIUIState.textm->setTextProj(textel, proj);
                __globalIUIState.textm->setTextView(textel, view);
                __globalIUIState.textm->setTextModel(textel, translate({boundingBox.x, boundingBox.y, 0.0f}) * scale({boundingBox.width, boundingBox.height, 1.0f}));
                __globalIUIState.textm->drawText(textel);
                __globalIUIState.textm->freeText(textel);
            }
        }
    }
}