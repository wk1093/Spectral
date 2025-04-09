/**
 * @file game.h
 * @brief Game loader header file.
 */

#pragma once
#include "win/module.h"
#include "gfx/module.h"
#include "shdr/module.h"
#include "tex/module.h"
#include "text/module.h"

#include "asset.h"

/**
 * @brief Game context structure.
 * 
 * This structure contains the context for the game, including the window, graphics, shader, texture, text, and asset loader modules.
 * This is passed to the "main" function of the game.
 */
struct GameContext {
    /// @brief Window module.
    WindowModule winm;
    /// @brief Graphics module.
    GraphicsModule gfxm;
    /// @brief Shader module.
    ShaderModule shdr;
    /// @brief Texture module.
    TextureModule texm;
    /// @brief Text module.
    TextModule textm;
    /// @brief Asset loader module.
    AssetLoader assetm;
};

/**
 * @brief Game module class.
 * 
 * This class represents the game being loaded from the "game.dll" library.
 * It provides access to the main function of the game, which is called to start the game.
 */
class Game : Module {
private:
    typedef int (*GameMain)(GameContext*);
    GameMain internal_main;
public:
    /**
     * @brief Main function of the game.
     * 
     * This function is called to start the game. It takes a pointer to the game context as an argument.
     * 
     * @param ctx Pointer to the game context.
     * @return An integer representing the exit code of the game.
     */
    inline int main(GameContext* ctx) {
        return internal_main(ctx);
    }

    /**
     * @brief Constructor for the Game class.
     * 
     * This constructor loads the game library and retrieves the main function of the game.
     * It initializes the game module and checks for errors in loading the library and main function.
     */
    Game() : Module("game", "game", false) {
        if (!lib.valid()) {
            printf("Error loading main game module\n");
            return;
        }
        internal_main = (GameMain)lib.getSymbol("game_main");
        if (!internal_main) {
            printf("Error loading main game function\n");
            return;
        }
    }
};