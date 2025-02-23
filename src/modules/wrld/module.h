/*
This module is one of the most complicated, even compared to gfx
This module handles the world, level data, and pretty much everything related to the game
It is stored in a custom format, as well as a custom format for things contained within it
This format actually is a zip file, containing even more sub-formats
The editor is completely separate from the engine, and is used to edit, create, convert, and view these files
The editor (as of now) doesn't even exist, but it is planned to be made in the future

Format specification:
filename (description) [format] // if the format is [zip], [png], [json], or any other format, it is identical to that format, just renamed
Format:
- gmae.spgam (game data) [zip] // this file might eng up being split up into multiple files if it gets too big (idk how i'm gonna do that yet)
    - world.spwld (world data) [json]
    - models/
        - MODEL.spmdl (model data) [similar to obj]
    - textures/
        - TEXTURE.(png/jpg/etc) (texture data) [png/jpg/etc, but png is is recommended]
    - materials/
        - MATERIAL.spmat (material data) [json]
    - levels/
        - LEVEL.splvl (level data) [json]
    - scripts/
        - SCRIPT.spscr (script meta) [json]
        - SCRIPT.(cpp/py/etc) (script code) [whatever language the script is in, many languages are supported (although cpp is the most common, and most performant)]
    - audio/
        - AUDIO.(wav/mp3/etc) (audio data) [wav/mp3/etc but wav is recommended]
    - etc/
        - ETC.spetc (etc meta) [json]
        - ETC.(etc) (etc data) [etc, any custom data can be stored here. WARNING: This is STATIC data, and cannot be changed during runtime, this is for things like configuration files that don't need to be changed once the game is compiled]

File structure of compiled game:
- game.spgam
- Spectral.PLATFORM.exe
- modules/
    - Contains all the modules needed to run the game
- data/
    - Contains all NON-STATIC data, such as logs, save files (only if the game enables portable saving, otherwise it is stored in appdata), etc
    - This folder can be used to store configuration files that the user can change, and it will probably be used for settings (if they aren't stored in the save)
    - This folder can also be used for modding purposes if the game wants to support mods (DISCLAIMER: Mods are NOT supported by the engine, and would have to be implemented by the developer)
- lib/
    - Contains all the libraries needed to run the game
- logs/
    - Log files are stored here, and are used for debugging purposes
- engine/
    - Contains engine settings, and is used to store engine data that the user is allowed to change
    - This includes settings like resolution, graphics settings, etc
    - This folder will also contain configs that allow the user to modify which modules are loaded (for example, choosing OpenGL instead of DirectX)
- cache/
    - Contains cached data (maybe, in the future)

most structs in the engine are prefixed with "s" (for Spectral), but this module is prefixed with "sw" (for Spectral World)
*/

/*
The above comment is outdated, as I have decided to make some changes as to how the game will be stored
The game (on the developer's side) will be in a folder, with basically the same structure as the above specs
But when the game is compiled, much of this data will be converted into C++ code and compiled into dlls
Each level/scene will be a separate dll, and the user scripts will all be compiled into a single dll
The main change, is that a level, asset, or script can be marked as a "moddable" asset, and it will not be compiled into anything, it will be stored in the final game folder under a user/ folder
The engine will also make a mods/ folder, where the user can place mods which will contain replacements for the assets in the moddable folder. The engine will hash every dll (modules, user scripts, and levels) as well as the moddable scripts. and will prevent the game from running if any of these files are modified
When the engine loads (on computers only, consoles do not support mods and selection of api), it will open up a popup letting the user select which modules to use, and mods to enable (if any)
This will allow the user to select things like OpenGL or DirectX. and will allow them to add custom modules. For example: If the user wanted to use vulkan, and someone made a vulkan module, they could add the vulkan module to the modules folder, and select it in the engine settings (this will give a warning that the module is not official, and could be dangerous)
The engine will never select a non-official module by default, and will always open the config popup if the user doesn't click "don't show this again"
If the user selects a non-official module, and then "don't show this again", it will still be shown to give the user a warning that they are using a non-official module
This popup menu is completely seperate from the rest of the engine, and will be run before anything else in the engine is loaded.
So the engine can recognize official modules, when the engine is compiled, it will hash all the official modules, and the hash will be stored inside of the engine executable. When the engine is run, it will verify that the official modules have not been modified, and if they have, it will act as if that module is non-official
The engine executable and official modules will all be the same for every game, since the game is actually just a few dlls.

All assets (non-moddable) will be compiled into an asset dll, which will store them as C++ code (just an array of bytes), and will be loaded into the game when the game starts
The assets that are moddable will be in the user/ folder.

This part of the engine (runtime) has no idea how to deal with the json format of the game, only the compiled format

The engine is split into 3 parts:
- Runtime: The part of the engine that runs the game. This contains all the modules, and the main executable. This part is compiled into the engine exe, and the module dlls
- Editor: This is the last part that I will make, and it will be used to create and edit the game file (in the dev format json). This part will be a completely separate program, but will borrow some of the modules from the runtime
- Compiler: This part is used to compile the game into the runtime format. This part will integrate with CMake, and will be used to make a game project into a game dll. This part will be put in the game/ directory of the output from the first part.


So a final compiled game will look like this:ABC
- game/
  - assets.dll
  - levels/
    - level1.dll
    - level2.dll
  - scripts.dll
- modules/
  - all the modules needed to run the game
- user/
  - assets/
    - moddable assets
  - levels/
    - moddable levels
  - scripts/
    - moddable scripts
- mods/
    - mod1/
        - assets/
        - moddable assets
        - levels/
        - moddable levels
        - scripts/
        - moddable scripts
    - mod2/
        - assets/
        - moddable assets
        - levels/
        - moddable levels
        - scripts/
        - moddable scripts
- Spectral.PLATFORM.exe
- data/
    - logs/
    - save/
    - settings/
- lib/
    - all the libraries needed to run the game
- engine/
    - engine settings
- cache/
    - cached data
*/

#pragma once

#include "../moduleLib.h"

#include <string>
#include <vector>
#include <cstdint>

#include "../math/module.h"

struct swWorld { // spwld [json]
    std::string name;
    std::string author;
    std::string description;
};

struct swModel { // spmdl [similar to obj]
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

struct swMaterial { // spmat [json]
    std::string shader;
    std::vector<std::string> samplers;
    std::vector<std::string> vertexUniforms;
    std::vector<std::string> fragmentUniforms;
};

struct swLevelObject {
    sModelTransform transform;
    void* ecsObject; // TODO: Implement ECS
};

struct swLevel { // splvl [json]
    std::vector<swLevelObject> objects;
};

struct swScript { // spscr [json]
    std::string ext; // the extension of the script file
    std::string mod; // the name of the scriptloader module to load this language
};

struct swEtc { // spetc [json]
    enum swEtcType {
        PRE_LOAD, // this data is required before the game's initial load/init (like settings and other data that is required to start the game)
        PRE_LOOP, // this data is required before the game's main loop starts (like level data and other data that is required to run the game)
        POST_LOOP, // this data can be lazy-loaded after the game's main loop starts (like extra data or dlc content that can be loaded after the game starts)
        POST_GAME // this data is loaded right before the game's cleanup (not used very often, but could be used for save templates, but remember, this can't be modified)
    } type;
};

struct swTexture {
    std::string path;
    std::vector<uint8_t> data;
};

struct swAudio {
    std::string path;
    std::vector<uint8_t> data;
};

struct swGame { // spgam [zip]
    swWorld world;
    std::vector<swModel> models;
    std::vector<swMaterial> materials;
    std::vector<swLevel> levels;
    std::vector<swScript> scripts;
    std::vector<swEtc> etc;
    std::vector<swTexture> textures;
    std::vector<swAudio> audio;
};

namespace world {
    // we use pointers to swGame instead of swGame itself because the swGame struct is pretty big
    // and we don't want to copy it around a lot

    typedef swGame* (*LoadGame)(const char*);
    typedef void (*FreeGame)(swGame*);
    typedef void (*SaveGame)(swGame*, const char*);
}

struct WorldModule : public Module {
    world::LoadGame loadGame;
    world::FreeGame freeGame;
    world::SaveGame saveGame;

    WorldModule() : Module("main", "wrld") {
        loadGame = (world::LoadGame)lib.getSymbol("loadGame");
        freeGame = (world::FreeGame)lib.getSymbol("freeGame");
        saveGame = (world::SaveGame)lib.getSymbol("saveGame");
    }
};
