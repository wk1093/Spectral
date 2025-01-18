#include "module.h"

#include <stb_image.h>

CEXPORT sTextureDefinition loadTexture(const char* path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data == nullptr) {
        printf("Error loading texture\n");
        return {0, 0, 0, nullptr};
    }
    if (width <= 0 || height <= 0 || channels <= 0) {
        printf("Error loading texture\n");
        return {0, 0, 0, nullptr};
    }
    return {static_cast<size_t>(width), static_cast<size_t>(height), static_cast<size_t>(channels), data};
}