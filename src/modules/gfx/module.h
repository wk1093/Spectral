#pragma once
#include "../moduleLib.h"

#include "../win/module.h"

#include <stdio.h>

namespace graphics {
    typedef void (*SetClearColor)(float r, float g, float b, float a);
    typedef void (*Clear)();
    typedef void (*Init)(sWindow* win);
    typedef void (*DebugTriangle)();
}

struct GraphicsModule : Module {
    graphics::SetClearColor setClearColor;
    graphics::Clear clear;
    graphics::Init init;
    graphics::DebugTriangle debugTriangle;

    explicit GraphicsModule(const char* dynlib) : Module(dynlib, "gfx") {
        setClearColor = (graphics::SetClearColor)lib.getSymbol("setClearColor");
        clear = (graphics::Clear)lib.getSymbol("clear");
        init = (graphics::Init)lib.getSymbol("init");
        debugTriangle = (graphics::DebugTriangle)lib.getSymbol("debugTriangle");
    }
};