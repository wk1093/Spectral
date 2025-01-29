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

    sShaderProgram text_shader;
    sVertexDefinition* text_vert_def;
    sMesh text_mesh;
};

void Clay_Spectral_Init(GraphicsModule* gfxm, TextModule* textm, ShaderModule* shdr) {
    
}

// custom clay implementation using our graphics library
void Clay_Spectral_Render(sWindow* win, Clay_RenderCommandArray renderCommands, sFont* fonts) {
    for (uint32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand* renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        switch (renderCommand->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
                Clay_Color color = config->color;

            } break;
        }
    }
}