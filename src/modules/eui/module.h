#pragma once
#include "../moduleLib.h"
#include "../win/module.h"

CEXPORT const char* required_files[] = {
};
CEXPORT size_t required_files_count = 0;

struct EngineUI {
    void* internal;
};

namespace engineui {
    typedef EngineUI (*EngineUILoader)(const WindowModule& wm, const sWindow& window);
    typedef void (*EngineUIDestructor)(EngineUI ui);
    typedef void (*EngineUIDraw)(EngineUI ui);
}

struct EngineUIModule : public Module {
    engineui::EngineUILoader loadUI;
    engineui::EngineUIDestructor destroyUI;
    engineui::EngineUIDraw draw;

    explicit EngineUIModule(const char* dynlib) : Module(dynlib, "eui") {
        loadUI = (engineui::EngineUILoader)lib.getSymbol("loadUI");
        destroyUI = (engineui::EngineUIDestructor)lib.getSymbol("destroyUI");
        draw = (engineui::EngineUIDraw)lib.getSymbol("draw");
    }
};