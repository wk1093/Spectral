#pragma once

// this file is the module.h for the scriptloading module
// defines common types and functions for the scriptloading module
// everything that is "language-specfic" is in the dll file, and this file is the interface to that


#include "../moduleLib.h"
#include "../gfx/module.h"


namespace texload {
    typedef sTextureDefinition (*LoadTexture)(const char*);
    typedef void (*FreeTexture)(sTextureDefinition);
    typedef sTextureDefinition (*LoadTextureFromBuffer)(const uint8_t*, size_t);
}
struct TextureModule : public Module {
    texload::LoadTexture loadTexture;
    texload::FreeTexture freeTexture;
    texload::LoadTextureFromBuffer loadTextureFromBuffer;

    TextureModule(const char* path) : Module(path, "tex") {
        loadTexture = (texload::LoadTexture)lib.getSymbol("loadTexture");
        freeTexture = (texload::FreeTexture)lib.getSymbol("freeTexture");
        loadTextureFromBuffer = (texload::LoadTextureFromBuffer)lib.getSymbol("loadTextureFromBuffer");
    }
};