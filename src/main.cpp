#include "modules/game.h"



// TODO: Make a sample game to test the engine
// Make a basic 3D player controller with gravity and collision
// and make a basic platformer/obby/parkour game
// maybe also make a 3D shooter game
// make all these using a new higher-level API that abstracts the modules and handles everything for you
// make a player controller library with a lot of settings and options (so this will be easier to use than Unity)
// also make a UI library
// also make a basic file format (or use an existing one) to store world/level data and make an editor for it (like Unity but simpler)
// this engine will not be structured like Unity, most things will be in code, unlike Unity where most things are in the editor
// the editor will be used to create levels and worlds, and to set up models, textures, and materials

// TODO: Support for more advanced forms of rendering like instancing, compute shaders, and ray tracing
// TODO: Support for more advanced forms of input like gamepads, touchscreens, and VR
// TODO: Audio
// TODO: Networking
// TODO: Physics
// TODO: AI
// TODO: Scripting
// TODO: ECS
// TODO: Multi-threading



int main(int argc, char** argv) {
    const char* window_module;
    const char* graphics_module;
    bool use_dx = true;
    if (use_dx) {
        window_module = "glfw_noapi";
        graphics_module = "d3d11_1";
    } else {
        window_module = "glfw_gl";
        graphics_module = "glad";
    }
    if (argc == 3) {
        window_module = argv[1];
        graphics_module = argv[2];
    }

    WindowModule winm(window_module);
    if (!winm.lib.valid()) return 1;
    GraphicsModule gfxm(graphics_module);
    if (!gfxm.lib.valid()) return 1;
    ShaderModule shdr("spsl", gfxm.getShaderType());
    if (!shdr.lib.valid()) return 1;
    TextureModule texm("stb");
    if (!texm.lib.valid()) return 1;
    TextModule textm("ft");
    if (!textm.lib.valid()) return 1;
    AssetLoader assetm;
    if (!assetm.lib.valid()) return 1;
    
    Game game;
    GameContext game_context = {winm, gfxm, shdr, texm, textm, assetm};
    return (game.main(&game_context));
}
