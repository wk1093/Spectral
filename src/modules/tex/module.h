#pragma once

// this file is the module.h for the scriptloading module
// defines common types and functions for the scriptloading module
// everything that is "language-specfic" is in the dll file, and this file is the interface to that


#include "../moduleLib.h"
#include "../gfx/module.h"


namespace texload {
    typedef sTextureDefinition (*LoadTexture)(const char*);
}
struct TextureModule : public Module {
    texload::LoadTexture loadTexture;

    TextureModule(const char* path) : Module(path, "tex") {
        loadTexture = (texload::LoadTexture)lib.getSymbol("loadTexture");
    }
};