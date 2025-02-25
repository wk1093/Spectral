#pragma once
#include "win/module.h"
#include "gfx/module.h"
#include "shdr/module.h"
#include "tex/module.h"
#include "text/module.h"

#include "asset.h"

struct GameContext {
    WindowModule winm;
    GraphicsModule gfxm;
    ShaderModule shdr;
    TextureModule texm;
    TextModule textm;
    AssetLoader assetm;
};

class Game : Module {
    typedef int (*GameMain)(GameContext*);
    public:
    GameMain main;
    Game() : Module("game", "game") {
        if (!lib.valid()) {
            printf("Error loading main game module\n");
            return;
        }
        main = (GameMain)lib.getSymbol("game_main");
        if (!main) {
            printf("Error loading main game function\n");
            return;
        } else {
            printf("Loaded main game\n");
        }
    }
};