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
        - TEXTURE.sptex (texture meta) [json]
        - TEXTURE.(png/jpg/etc) (texture data) [png/jpg/etc, but png is the most common, and is recommended]
    - materials/
        - MATERIAL.spmat (material data) [json]
    - levels/
        - LEVEL.splvl (level data) [json]
    - scripts/
        - SCRIPT.spscr (script meta) [json]
        - SCRIPT.(cpp/py/etc) (script code) [whatever language the script is in, many languages are supported (although cpp is the most common, and most performant)]
    - audio/
        - AUDIO.spaud (audio meta) [json]
        - AUDIO.wav (audio data) [wav]
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
*/