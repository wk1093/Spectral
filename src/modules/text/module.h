#pragma once
#include "../moduleLib.h"

#include "../math/module.h"
#include "../gfx/module.h"
#include "../shdr/module.h"

struct sFont {
    void* internal;
};

struct sText {
    void* internal;
};

namespace text {
    typedef void (*Init)(GraphicsModule* gfxm, ShaderModule* shdr);
    typedef sFont (*LoadFont)(const char* path, int size, const char* vertpath, const char* fragpath);
    typedef sText (*CreateText)(sFont font, const char* text);
    typedef void (*DrawText)(sText text);
    typedef void (*FreeText)(sText text);
    typedef void (*FreeFont)(sFont font);
    typedef void (*SetTextColor)(sText text, vec3 color);
    typedef void (*SetTextModel)(sText text, mat4 model);
    typedef void (*SetTextView)(sText text, mat4 view);
    typedef void (*SetTextProj)(sText text, mat4 proj);
}

struct TextModule : public Module {
    text::Init init;
    text::LoadFont loadFont;
    text::CreateText createText;
    text::DrawText drawText;
    text::FreeText freeText;
    text::FreeFont freeFont;
    text::SetTextColor setTextColor;
    text::SetTextModel setTextModel;
    text::SetTextView setTextView;
    text::SetTextProj setTextProj;

    TextModule(const char* path) : Module(path, "text") {
        init = (text::Init)lib.getSymbol("init");
        loadFont = (text::LoadFont)lib.getSymbol("loadFont");
        createText = (text::CreateText)lib.getSymbol("createText");
        drawText = (text::DrawText)lib.getSymbol("drawText");
        freeText = (text::FreeText)lib.getSymbol("freeText");
        freeFont = (text::FreeFont)lib.getSymbol("freeFont");
        setTextColor = (text::SetTextColor)lib.getSymbol("setTextColor");
        setTextModel = (text::SetTextModel)lib.getSymbol("setTextModel");
        setTextView = (text::SetTextView)lib.getSymbol("setTextView");
        setTextProj = (text::SetTextProj)lib.getSymbol("setTextProj");
    }
};