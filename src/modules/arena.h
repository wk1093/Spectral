/**
 * @file arena.h
 * Arena allocator header file.
 */

#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

struct sArenaAllocator {
    /// @brief Pointer to the memory buffer.
    uint8_t* buffer;
    /// @brief Size of the memory buffer.
    size_t size;
    /// @brief Current offset in the memory buffer.
    size_t offset;
    /// @brief Tracker string for debugging purposes.
    const char* tracker;

    /**
     * @brief Constructor for the arena allocator.
     * 
     * This constructor initializes the arena allocator with a specified size and tracker string.
     * It allocates memory for the arena and initializes the offset to zero.
     */
    inline sArenaAllocator(const char* tracker, size_t size) : tracker(tracker), size(size), offset(0) {
        buffer = (uint8_t*)malloc(size); // Allocate memory for the arena
        memset(buffer, 0, size);         // Initialize the buffer to zero
    }

    /**
     * @brief Destructor for the arena allocator.
     * 
     * This destructor frees the allocated memory for the arena.
     * It is called when the arena allocator goes out of scope or is deleted.
     */
    inline ~sArenaAllocator() {
        free(buffer); // Free the allocated memory
    }

    /**
     * @brief Allocates memory from the arena.
     * @param bytes The number of bytes to allocate.
     * @return Pointer to the allocated memory, or nullptr if there is not enough space.
     */
    inline void* allocate(size_t bytes) {
        if (offset + bytes > size) {
            printf("Arena allocator '%s' out of memory! Requested %zu bytes, available %zu bytes\n", tracker, bytes, size - offset);
            // return nullptr; // Not enough space in the arena
            printf("Attempting to resize arena allocator '%s' from %zu bytes to %zu bytes\n", tracker, size, size + bytes);
            resize(size + bytes); // Attempt to resize the arena, we will us
        }
        void* ptr = buffer + offset;
        offset += bytes; // Update the offset
        return ptr;
    }

    /**
     * @brief Resets the arena allocator.
     * 
     * This function resets the offset to zero, effectively freeing all allocated memory.
     */
    inline void reset() {
        offset = 0;
    }

    /**
     * @brief Allocates memory for a type T.
     * 
     * This function allocates memory for a type T and returns a pointer to the allocated memory.
     * It is a template function that can be used for any type.
     * 
     * @tparam T The type to allocate memory for.
     * @return Pointer to the allocated memory for type T.
     */
    template<typename T>
    inline T* allocate() {
        return (T*)allocate(sizeof(T)); // Allocate memory for a type T
    }

    /**
     * @brief Allocates memory for an array of type T.
     * 
     * This function allocates memory for an array of type T and returns a pointer to the allocated memory.
     * It is a template function that can be used for any type.
     * 
     * @tparam T the type to allocate memory for.
     * @param count The number of elements to allocate memory for.
     * @return Pointer to the allocated memory for an array of type T.
     */
    template<typename T>
    inline T* allocateArray(size_t count) {
        return (T*)allocate(sizeof(T) * count); // Allocate memory for an array of type T
    }

    /**
     * @brief Resizes the arena allocator.
     * 
     * This function should be avoided, but if you run out of room, you can create more.
     */
    inline void resize(size_t newSize) {
        if (newSize <= size) {
            printf("Arena allocator '%s' resize requested to %zu bytes, but current size is %zu bytes\n", tracker, newSize, size);
            return; // No need to resize if the new size is less than or equal to the current size
        }
        uint8_t* newBuffer = (uint8_t*)realloc(buffer, newSize);
        if (!newBuffer) {
            printf("Arena allocator '%s' failed to resize to %zu bytes\n", tracker, newSize);
            return; // Failed to resize the buffer
        }
        buffer = newBuffer; // Update the buffer pointer
        size = newSize;     // Update the size
    }
};