#include "modules/game.h"
#include "modules/sel/module.h"
// #include "modules/aud/module.h"

#include <iostream>

int main(int argc, char** argv) {
    bool show_config = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --help, -h       Show this help message" << std::endl;
            std::cout << "  --config, -c     Show the startup configuration" << std::endl;
            return 0;
        } else if (strcmp(argv[i], "--config") == 0 || strcmp(argv[i], "-c") == 0) {
            show_config = true;
        }
    }

    // make the engine directory exist
    std::filesystem::path engine_dir = getexedir() / "engine";
    if (!std::filesystem::exists(engine_dir)) {
        std::filesystem::create_directories(engine_dir);
    }
    std::filesystem::path module_config = getexedir() / "engine" / "module_config.splcfg";
    std::map<std::string, std::string> map;
    if (!std::filesystem::exists(module_config) || show_config) {
        auto mods = getModuleDefs();

        std::vector<std::string> mod_types;
        for (auto& mod : mods) {
            if (std::find(mod_types.begin(), mod_types.end(), mod.mod) == mod_types.end()) {
                mod_types.push_back(mod.mod);
            }
        }

        char* sel_gfx = nullptr;
        sSelectModules(filterModules(mods, "gfx"), &sel_gfx);

        if (sel_gfx == nullptr) {
            std::cout << "No graphics module selected" << std::endl;
            return 1;
        }

        auto mods_sub = getSubModules(mods);

        auto reduced = reduceDependencies(mods_sub, findSubDef(mods_sub, "gfx", sel_gfx));

        for (auto& mod : reduced) {
            std::cout << "Module: " << mod;
        }

        std::vector<std::string> mod_types2;

        for (auto& mod : reduced) {
            if (std::find(mod_types2.begin(), mod_types2.end(), mod.mod) == mod_types2.end()) {
                mod_types2.push_back(mod.mod);
            } else {
                std::cout << "Duplicate module found: " << mod.mod << " " << mod.impl << std::endl;
                return 1;
            }
        }

        if (mod_types2.size() != mod_types.size()) {
            std::cout << "Lost some modules in the reduction" << std::endl;
            return 1;
        }

        map = getModuleMap(reduced);

        std::ofstream file(module_config, std::ios::out | std::ios::trunc);
        if (!file) {
            std::cout << "Error opening file for writing: " << module_config << std::endl;
            return 1;
        }
        for (auto& mod : map) {
            file << mod.first << " " << mod.second << std::endl;
        }
        file.close();
    } else {
        std::ifstream file(module_config);
        if (!file) {
            std::cout << "Error opening file for reading: " << module_config << std::endl;
            return 1;
        }
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find(" ");
            if (pos == std::string::npos) {
                std::cout << "Error parsing line: " << line << std::endl;
                continue;
            }
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            map[key] = value;
        }
        file.close();
    }

    WindowModule winm(map["win"].c_str());
    if (!winm.lib.valid()) return 1;
    GraphicsModule gfxm(map["gfx"].c_str());
    if (!gfxm.lib.valid()) return 1;
    ShaderModule shdr(map["shdr"].c_str());
    if (!shdr.lib.valid()) return 1;
    TextureModule texm(map["tex"].c_str());
    if (!texm.lib.valid()) return 1;
    TextModule textm(map["text"].c_str());
    if (!textm.lib.valid()) return 1;
    AssetLoader assetm;
    if (!assetm.lib.valid()) return 1;
    // AudioModule audm(map["aud"].c_str());
    // if (!audm.lib.valid()) return 1;
    
    // audm.init();

    Game game;
    GameContext game_context = {winm, gfxm, shdr, texm, textm, assetm};
    int a = game.main(&game_context);

    // audm.destroy();

    printf("Game returned %d\n", a);

    return a;
}
