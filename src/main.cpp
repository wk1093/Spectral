#include "modules/game.h"
#include "modules/sel/module.h"
// #include "modules/aud/module.h"

#include <iostream>

int main(int argc, char** argv) {
    // std::vector<std::string> win_mods = getModuleImpls("win");
    // std::vector<std::string> gfx_mods = getModuleImpls("gfx");

    // the main selection will be the graphics module, since it's def file should have dependencies set to window and shader modules. We shouldn't really have much options after choosing the graphics module.

    // if we have only one graphics module, we can skip the selection
    auto map = getModuleDefs();

    for (auto& mod : map) {
        std::cout << "Module: \n" << mod.mod << " " << mod.impl << std::endl;
    }

    auto reduced = reduceDependencies(map, "gfx", "glad");

    for (auto& mod : reduced) {
        std::cout << "Reduced Module: \n" << mod.mod << " " << mod.impl << std::endl;
    }

    // Display a simple popup to select the window and graphics modules
    // default to the window and graphics modules selected above
    // char* winmod=nullptr, *gfxmod=nullptr;
    // if (win_mods.size() == 1 && gfx_mods.size() == 1) {
    //     winmod = (char*)win_mods[0].c_str();
    //     gfxmod = (char*)gfx_mods[0].c_str();
    // } else {
    //     sSelectModules(win_mods, &winmod, gfx_mods, &gfxmod);
    // }
    // if (!winmod || !gfxmod) {
    //     printf("exiting\n");
    //     return 1;
    // }

    // remove everythin before the first '_' including the '_'
    // char* winmod_ = strchr(winmod, '_');
    // if (winmod_) {
    //     winmod_++;
    // } else {
    //     winmod_ = winmod;
    // }
    // char* gfxmod_ = strchr(gfxmod, '_');
    // if (gfxmod_) {
    //     gfxmod_++;
    // } else {
    //     gfxmod_ = gfxmod;
    // }

    // WindowModule winm(winmod_);
    // if (!winm.lib.valid()) return 1;
    // GraphicsModule gfxm(gfxmod_);
    // if (!gfxm.lib.valid()) return 1;
    // ShaderModule shdr("spsl", gfxm.getShaderType());
    // if (!shdr.lib.valid()) return 1;
    // TextureModule texm("stb");
    // if (!texm.lib.valid()) return 1;
    // TextModule textm("ft");
    // if (!textm.lib.valid()) return 1;
    // AssetLoader assetm;
    // if (!assetm.lib.valid()) return 1;
    // AudioModule audm("openal");
    // if (!audm.lib.valid()) return 1;
    
    // audm.init();

    // Game game;
    // GameContext game_context = {winm, gfxm, shdr, texm, textm, assetm};
    // int a = game.main(&game_context);

    // audm.destroy();

    // printf("Game returned %d\n", a);

    // return a;
}
