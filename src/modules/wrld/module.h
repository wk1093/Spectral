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
