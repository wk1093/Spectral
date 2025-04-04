/**
 * @file asset.h
 * Asset loading utilities header file.
 */

#pragma once

#include <cstdint>
#include <string>
#include "moduleLib.h"

/// @cond DOXYGEN_OMIT
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
/// @endcond

/**
 * @brief Asset buffer structure.
 * 
 * This structure represents an asset buffer containing the data and length of the asset.
 */
struct AssetBuffer {
    /// @brief Pointer to the asset data.
    const uint8_t* data;
    /// @brief Length of the asset data.
    size_t len;
};
/**
 * @brief Text asset buffer structure.
 * 
 * This structure represents a text asset buffer containing the data and length of the text asset.
 */
struct TextAssetBuffer {
    /// @brief Pointer to the text asset data.
    const char* data;
    /// @brief Length of the text asset data.
    size_t len;
};
/**
 * @brief Asset loader class.
 * 
 * This class represents the asset loader module and provides methods for loading assets and text assets.
 * It is responsible for loading the asset loader library and providing access to its functions.
 * 
 * @note This class makes it seem as if they are loaded from a file, but they are actually all stored in the binary as a big assets.dll file.
 */
struct AssetLoader : Module {

    /**
     * @brief Constructor for the AssetLoader class.
     * 
     * This constructor initializes the asset loader module with the specified library path and sets up the function pointers for loading assets and text assets.
     */
    AssetLoader() : Module("assets", "game", false) {
        // assets are stored in a module, basically a big cpp file with a bunch of variables
    }

    /**
     * @brief Loads an asset from a file path.
     * 
     * This function loads an asset from the specified file path and returns an asset buffer containing the data and length of the asset.
     * 
     * @param path The "path" to the asset file. See @ref AssetLoader for more details.
     * @return An asset buffer containing the loaded asset data.
     */
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

    /**
     * @brief Loads a text asset from a file path.
     * 
     * This function loads a text asset from the specified file path and returns a text asset buffer containing the data and length of the text asset.
     * 
     * @param path The "path" to the text asset file. See @ref AssetLoader for more details.
     * @return A text asset buffer containing the loaded text asset data.
     */
    TextAssetBuffer loadTextAsset(const char* path) {
        AssetBuffer abuf = loadAsset(path);
        return {(const char*)abuf.data, abuf.len};
    }
};