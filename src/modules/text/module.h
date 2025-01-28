#pragma once
#include "../moduleLib.h"

#include "../math/module.h"
#include "../gfx/module.h"

struct sFont {
    void* internal;
};

struct sText {
    void* internal;
};

namespace text {
    typedef void (*Init)(GraphicsModule* gfxm);
    typedef sFont (*LoadFont)(const char* path, int size);
    typedef sText (*CreateText)(sFont font, const char* text, vec2 pos, float scale);
    typedef void (*DrawText)(sText text);
}

struct TextModule : public Module {
    text::Init init;
    text::LoadFont loadFont;
    text::CreateText createText;
    text::DrawText drawText;

    TextModule(const char* path) : Module(path, "text") {
        init = (text::Init)lib.getSymbol("init");
        loadFont = (text::LoadFont)lib.getSymbol("loadFont");
        createText = (text::CreateText)lib.getSymbol("createText");
        drawText = (text::DrawText)lib.getSymbol("drawText");
    }
};