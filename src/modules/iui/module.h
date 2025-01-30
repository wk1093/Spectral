#pragma once

#ifdef IUI_IMPLEMENTATION
#define CLAY_IMPLEMENTATION
#endif
#include "clay.h"

#include "../gfx/module.h"
#include "../text/module.h"
#include "../shdr/module.h"

#define CLAY_COLOR_TO_VEC4(color) {(color).r / 255.0f, (color).g / 255.0f, (color).b / 255.0f, (color).a / 255.0f}

struct sIUIGlobalState {
    WindowModule* winm;
    GraphicsModule* gfxm;
    TextModule* textm;
    ShaderModule* shdr;

    sShaderProgram rect_shader;
    sVertexDefinition* rect_vert_def;
    sMesh rect_mesh;
    sUniforms rect_uniforms;

    // rounded rects use the same mesh and vertex defs, but a different shader
    sShaderProgram rounded_rect_shader;
    sUniforms rounded_rect_uniforms;

    sWindow* win;

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
    float z;
};

struct sInternalRoundedRectUniforms {
    vec4 color;
    vec2 topleft;
    vec2 widheight;
    float radius;

    mat4 proj;
    mat4 view;
    mat4 model;
    float z;
};


void clayerr(Clay_ErrorData errorData) {
    printf("UI Error: %s\n", errorData.errorText.chars);
}

Clay_Dimensions Clay_Spectral_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData) {
    if (!__globalIUIState.fonts) {
        return {};
    }
    Clay_Dimensions result = {};
    sFont font = __globalIUIState.fonts[0];
    vec2 size = __globalIUIState.textm->measureText(font, text.chars);
    int fs = config->fontSize;
    float scaleFactor = (float)fs / (float)(2*font.size);
    result.width = size.x * scaleFactor;
    result.height = size.y * scaleFactor;
    return result;
}

void Clay_Spectral_Init(WindowModule* winm, GraphicsModule* gfxm, TextModule* textm, ShaderModule* shdr, sWindow* win, sFont* fonts) {
    __globalIUIState.fonts = fonts;
    __globalIUIState.winm = winm;
    __globalIUIState.gfxm = gfxm;
    __globalIUIState.textm = textm;
    __globalIUIState.shdr = shdr;
    __globalIUIState.win = win;
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena) {
        .capacity = totalMemorySize,
        .memory = (char*)malloc(totalMemorySize)
    };
    Clay_Initialize(clayMemory, (Clay_Dimensions){win->width, win->height}, (Clay_ErrorHandler)clayerr);
    Clay_SetMeasureTextFunction(Clay_Spectral_MeasureText, 0);

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
        {sShaderType::VERTEX, "uModel", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uZ", sUniformType::FLOAT, 1}
    };
    if (rect_uniform_def.size() != sizeof(sInternalRectUniforms)) {
        printf("ERROR: Uniform definition size mismatch\n");
        return;
    }
    __globalIUIState.rect_uniforms = gfxm->createUniforms(rect_shader, rect_uniform_def);

    sShader rounded_rect_vert_shader = shdr->compile(gfxm, "spsl/iui/rounded_rect.spslv", sShaderType::VERTEX, rect_vert_def);
    sShader rounded_rect_frag_shader = shdr->compile(gfxm, "spsl/iui/rounded_rect.spslf", sShaderType::FRAGMENT);
    sShaderProgram rounded_rect_shader = gfxm->createShaderProgram({rounded_rect_vert_shader, rounded_rect_frag_shader});
    __globalIUIState.rounded_rect_shader = rounded_rect_shader;

    sUniformDefinition rounded_rect_uniform_def = {
        {sShaderType::FRAGMENT, "uColor", sUniformType::FLOAT, 4},
        {sShaderType::FRAGMENT, "uTopLeft", sUniformType::FLOAT, 2},
        {sShaderType::FRAGMENT, "uWidthHeight", sUniformType::FLOAT, 2},
        {sShaderType::FRAGMENT, "uRadius", sUniformType::FLOAT, 1},
        {sShaderType::VERTEX, "uProj", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uView", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uModel", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uZ", sUniformType::FLOAT, 1}
    };
    if (rounded_rect_uniform_def.size() != sizeof(sInternalRoundedRectUniforms)) {
        printf("ERROR: Uniform definition size mismatch\n");
        return;
    }

    __globalIUIState.rounded_rect_uniforms = gfxm->createUniforms(rounded_rect_shader, rounded_rect_uniform_def);
}

// custom clay implementation using our graphics library
void Clay_Spectral_Render(sWindow* win, Clay_RenderCommandArray renderCommands, mat4 proj, mat4 view) {
    float mousex, mousey;
    __globalIUIState.winm->getMousePosition(*win, &mousex, &mousey);
    Clay_Vector2 mousePos = {mousex, mousey};
    bool mouse = __globalIUIState.winm->isMouseButtonPressed(*win, 0);
    Clay_SetPointerState(mousePos, mouse);

    
    float z = -1.0f+0.01f;
    for (uint32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand* renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;
        boundingBox.y = win->height - boundingBox.y - boundingBox.height;
        switch (renderCommand->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
                Clay_Color color = config->color;

                if (config->cornerRadius.topLeft == 0) {
                    sInternalRectUniforms uniforms = {};
                    uniforms.color = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
                    uniforms.proj = proj;
                    uniforms.view = view;
                    uniforms.model = scale({boundingBox.width, boundingBox.height, 1.0f}) * translate({boundingBox.x, boundingBox.y, 0.0f});
                    uniforms.z = z;
                    z += 0.01f;

                    __globalIUIState.gfxm->useShaderProgram(__globalIUIState.rect_shader);
                    __globalIUIState.gfxm->setUniforms(__globalIUIState.rect_uniforms, &uniforms);
                    __globalIUIState.gfxm->drawMesh(__globalIUIState.rect_mesh);
                } else {
                    sInternalRoundedRectUniforms uniforms = {};
                    uniforms.color = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
                    uniforms.proj = proj;
                    uniforms.view = view;
                    uniforms.model = scale({boundingBox.width, boundingBox.height, 1.0f}) * translate({boundingBox.x, boundingBox.y, 0.0f});
                    uniforms.z = z;
                    uniforms.topleft = {boundingBox.x, renderCommand->boundingBox.y};
                    uniforms.widheight = {boundingBox.width, boundingBox.height};
                    uniforms.radius = config->cornerRadius.topLeft;
                    z += 0.01f;

                    __globalIUIState.gfxm->useShaderProgram(__globalIUIState.rounded_rect_shader);
                    __globalIUIState.gfxm->setUniforms(__globalIUIState.rounded_rect_uniforms, &uniforms);
                    __globalIUIState.gfxm->drawMesh(__globalIUIState.rect_mesh);

                }
            } break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextElementConfig *config = renderCommand->config.textElementConfig;
                Clay_StringSlice text = renderCommand->text;
                int fs = config->fontSize;
                char *cloned = (char*)malloc(text.length + 1);
                memcpy(cloned, text.chars, text.length);
                cloned[text.length] = '\0';
                sFont font = __globalIUIState.fonts[0];
                float scaleFactor = (float)fs / (float)(2*font.size);
                sText textel = __globalIUIState.textm->createText(font, cloned);
                __globalIUIState.textm->setTextProj(textel, proj);
                __globalIUIState.textm->setTextView(textel, view);
                __globalIUIState.textm->setTextModel(textel, scale({scaleFactor, scaleFactor, 1.0f}) * translate({boundingBox.x, boundingBox.y, 0.0f}));
                __globalIUIState.textm->setTextColor(textel, vec3{config->textColor.r / 255.0f, config->textColor.g / 255.0f, config->textColor.b / 255.0f});
                __globalIUIState.textm->setTextZ(textel, z);
                z += 0.01f;
                __globalIUIState.textm->drawText(textel);
                __globalIUIState.textm->freeText(textel);
            } break;
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderElementConfig *config = renderCommand->config.borderElementConfig;

                sInternalRectUniforms uniforms = {};
                uniforms.proj = proj;
                uniforms.view = view;
                uniforms.z = z;

                if (config->bottom.width > 0) {
                    // draw a rect
                    uniforms.color = CLAY_COLOR_TO_VEC4(config->bottom.color);
                    uniforms.model = scale({boundingBox.width, (float)config->bottom.width, 1.0f}) * translate({boundingBox.x, boundingBox.y, 0.0f});
                    __globalIUIState.gfxm->useShaderProgram(__globalIUIState.rect_shader);
                    __globalIUIState.gfxm->setUniforms(__globalIUIState.rect_uniforms, &uniforms);
                    __globalIUIState.gfxm->drawMesh(__globalIUIState.rect_mesh);
                }
                if (config->left.width > 0) {
                    // draw a rect
                    uniforms.color = CLAY_COLOR_TO_VEC4(config->left.color);
                    uniforms.model = scale({(float)config->left.width, boundingBox.height, 1.0f}) * translate({boundingBox.x, boundingBox.y, 0.0f});
                    __globalIUIState.gfxm->useShaderProgram(__globalIUIState.rect_shader);
                    __globalIUIState.gfxm->setUniforms(__globalIUIState.rect_uniforms, &uniforms);
                    __globalIUIState.gfxm->drawMesh(__globalIUIState.rect_mesh);
                }
                if (config->right.width > 0) {
                    // draw a rect
                    uniforms.color = CLAY_COLOR_TO_VEC4(config->right.color);
                    uniforms.model = scale({(float)config->right.width, boundingBox.height, 1.0f}) * translate({boundingBox.x + boundingBox.width - config->right.width, boundingBox.y, 0.0f});
                    __globalIUIState.gfxm->useShaderProgram(__globalIUIState.rect_shader);
                    __globalIUIState.gfxm->setUniforms(__globalIUIState.rect_uniforms, &uniforms);
                    __globalIUIState.gfxm->drawMesh(__globalIUIState.rect_mesh);
                }
                if (config->top.width > 0) {
                    // draw a rect
                    uniforms.color = CLAY_COLOR_TO_VEC4(config->top.color);
                    uniforms.model = scale({boundingBox.width, (float)config->top.width, 1.0f}) * translate({boundingBox.x, boundingBox.y + boundingBox.height - config->top.width, 0.0f});
                    __globalIUIState.gfxm->useShaderProgram(__globalIUIState.rect_shader);
                    __globalIUIState.gfxm->setUniforms(__globalIUIState.rect_uniforms, &uniforms);
                    __globalIUIState.gfxm->drawMesh(__globalIUIState.rect_mesh);
                }
                z += 0.01f;
            } break;
        }
    }
}