#include "module.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <nlohmann/json.hpp>
#include <zip.h>

using json = nlohmann::json;

CEXPORT void saveGame(swGame* game, const char* path) {
    zip_t* zip = zip_open(path, ZIP_CREATE | ZIP_TRUNCATE, NULL);
    if (!zip) {
        std::cerr << "Failed to open zip file" << std::endl;
        return;
    }

    zip_error_t* error = zip_get_error(zip);
    if (zip_error_code_zip(error) != 0) {
        std::cerr << "Error: " << zip_error_strerror(error) << std::endl;
    }

    // add a mock file for now
    std::string mockFile = "This is a mock file";
    zip_source_t* source = zip_source_buffer(zip, mockFile.c_str(), mockFile.size(), 0);
    if (!source) {
        std::cerr << "Failed to create source" << std::endl;
        return;
    }

    zip_int64_t index = zip_file_add(zip, "mock.txt", source, ZIP_FL_OVERWRITE);
    if (index < 0) {
        std::cerr << "Failed to add file to zip" << std::endl;
        return;
    }

    zip_close(zip);
}

CEXPORT swGame* loadGame(const char* path) {
    // open the zip and verify the mock file
    zip_t* zip = zip_open(path, 0, NULL);
    if (!zip) {
        std::cerr << "Failed to open zip file" << std::endl;
        return nullptr;
    }

    zip_int64_t numEntries = zip_get_num_entries(zip, 0);

    if (numEntries < 1) {
        std::cerr << "No entries in zip file" << std::endl;
        return nullptr;
    }

    zip_file_t* file = zip_fopen_index(zip, 0, 0);

    if (!file) {
        std::cerr << "Failed to open file in zip" << std::endl;
        return nullptr;
    }

    zip_stat_t stat;
    zip_stat_init(&stat);
    zip_stat(zip, zip_get_name(zip, 0, 0), 0, &stat);

    char* buffer = new char[stat.size + 1];
    zip_fread(file, buffer, stat.size);
    buffer[stat.size] = '\0';

    std::cout << "File contents: " << buffer << std::endl;

    zip_fclose(file);
    zip_close(zip);

    return nullptr;
}

CEXPORT void freeGame(swGame* game) {
    // nothing to free
}