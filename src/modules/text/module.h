#pragma once
#include "../moduleLib.h"

#include "../math/module.h"
#include "../gfx/module.h"
#include "../shdr/module.h"
#include "../asset.h"

struct sFont {
    void* internal;
    int size;
};

struct sText {
    void* internal;
};

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

struct TextModule : public Module {
    text::Init init;
    text::LoadFont internal_loadFont;
    text::LoadFontAsset internal_loadFontAsset;
    text::CreateText createText;
    text::DrawText drawText;
    text::FreeText freeText;
    text::FreeFont freeFont;
    text::SetTextColor setTextColor;
    text::SetTextModel setTextModel;
    text::SetTextView setTextView;
    text::SetTextProj setTextProj;
    text::MeasureText measureText;
    text::SetTextZ setTextZ;

    sFont loadFont(const char* path, int size, const char* vertpath, const char* fragpath) {
        sFont f = internal_loadFont(path, size, vertpath, fragpath);
        f.size = size;
        return f;
    }

    sFont loadFontAsset(const char* path, int size, const char* vertpath, const char* fragpath) {
        sFont f = internal_loadFontAsset(path, size, vertpath, fragpath);
        f.size = size;
        return f;
    }

    TextModule(const char* path) : Module(path, "text") {
        init = (text::Init)lib.getSymbol("init");
        internal_loadFont = (text::LoadFont)lib.getSymbol("loadFont");
        internal_loadFontAsset = (text::LoadFontAsset)lib.getSymbol("loadFontAsset");
        createText = (text::CreateText)lib.getSymbol("createText");
        drawText = (text::DrawText)lib.getSymbol("drawText");
        freeText = (text::FreeText)lib.getSymbol("freeText");
        freeFont = (text::FreeFont)lib.getSymbol("freeFont");
        setTextColor = (text::SetTextColor)lib.getSymbol("setTextColor");
        setTextModel = (text::SetTextModel)lib.getSymbol("setTextModel");
        setTextView = (text::SetTextView)lib.getSymbol("setTextView");
        setTextProj = (text::SetTextProj)lib.getSymbol("setTextProj");
        measureText = (text::MeasureText)lib.getSymbol("measureText");
        setTextZ = (text::SetTextZ)lib.getSymbol("setTextZ");
    }
};