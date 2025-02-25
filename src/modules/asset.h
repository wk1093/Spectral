#pragma once

#include <cstdint>
#include <string>
#include "moduleLib.h"

std::string replace(std::string src, char from, char to) {
    for (size_t i = 0; i < src.size(); i++) {
        if (src[i] == from) {
            src[i] = to;
        }
    }
    return src;
}

uint64_t hash(std::string str) {
    uint64_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

struct AssetBuffer {
    const uint8_t* data;
    size_t len;
};

struct TextAssetBuffer {
    const char* data;
    size_t len;
};

struct AssetLoader : Module {

    AssetLoader() : Module("assets", "game") {
        // assets are stored in a module, basically a big cpp file with a bunch of variables
    }

    AssetBuffer loadAsset(const char* path) {
        // TODO: Should I use a hash instead of a string?
        // we will try strings first

        std::string assetPath = replace(path, '/', '_');
        assetPath = replace(assetPath, '.', '_');
        assetPath = replace(assetPath, '-', '_');
        assetPath = replace(assetPath, ' ', '_');
        assetPath = replace(assetPath, '\\', '_');
        assetPath = replace(assetPath, ':', '_');
        assetPath = replace(assetPath, '?', '_');
        assetPath = replace(assetPath, '&', '_');
        assetPath = replace(assetPath, '=', '_');
        assetPath = replace(assetPath, '+', '_');
        assetPath = replace(assetPath, '%', '_');
        assetPath = replace(assetPath, '#', '_');
        assetPath = replace(assetPath, '!', '_');
        assetPath = replace(assetPath, '@', '_');
        assetPath = replace(assetPath, '$', '_');
        assetPath = replace(assetPath, '^', '_');
        assetPath = replace(assetPath, '*', '_');
        assetPath = replace(assetPath, '(', '_');
        assetPath = replace(assetPath, ')', '_');
        assetPath = replace(assetPath, '[', '_');
        assetPath = replace(assetPath, ']', '_');
        assetPath = replace(assetPath, '{', '_');
        assetPath = replace(assetPath, '}', '_');
        assetPath = replace(assetPath, '<', '_');
        assetPath = replace(assetPath, '>', '_');
        assetPath = replace(assetPath, '|', '_');
        assetPath = replace(assetPath, ';', '_');
        assetPath = replace(assetPath, ',', '_');

        std::string assetVar = "asset_" + assetPath;
        std::string assetSizeVar = "asset_" + assetPath + "_size";

        // inside of the binary it is declared as extern "C" const uint8_t asset_<path>[];
        // and extern "C" const size_t asset_<path>_size;
        const uint8_t* data = (const uint8_t*)lib.getSymbol(assetVar.c_str());
        size_t len = *(size_t*)lib.getSymbol(assetSizeVar.c_str());

        return {data, len};
    }

    TextAssetBuffer loadTextAsset(const char* path) {
        AssetBuffer abuf = loadAsset(path);
        return {(const char*)abuf.data, abuf.len};
    }
};