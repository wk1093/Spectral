/**
 * @file arena.h
 * Arena allocator header file.
 */

#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

struct sArenaAllocator {
    /// @brief Pointer to the memory buffers.
    uint8_t** buffers;
    /// @brief Size of a memory buffer.
    size_t blockSize;
    /// @brief Number of buffers allocated.
    size_t numBuffers;
    /// @brief Current offset into all of the buffers, using modulo and division to find the current buffer and offset.
    size_t offset;
    /// @brief Tracker string for debugging purposes.
    const char* tracker;
    /// @brief Debug mode, prints every allocation and deallocation.
    bool debugMode = false;

    /**
     * @brief Constructor for the arena allocator.
     * 
     * This constructor initializes the arena allocator with a specified size and tracker string.
     * It allocates memory for the arena and initializes the offset to zero.
     */
    inline sArenaAllocator(const char* tracker, size_t blocksize) : tracker(tracker), blockSize(blocksize), numBuffers(1), offset(0) {
        buffers = (uint8_t**)malloc(sizeof(uint8_t*) * 1); // Allocate memory for the arena
        buffers[0] = (uint8_t*)malloc(blockSize); // Allocate memory for the first buffer
        memset(buffers[0], 0, blockSize); // Initialize the buffer to zero
    }

    /**
     * @brief Destructor for the arena allocator.
     * 
     * This destructor frees the allocated memory for the arena.
     * It is called when the arena allocator goes out of scope or is deleted.
     */
    inline ~sArenaAllocator() {
        for (size_t i = 0; i < numBuffers; i++) {
            free(buffers[i]); // Free the allocated memory for each buffer
        }
        free(buffers); // Free the allocated memory for the arena
    }

    /**
     * @brief Allocates memory from the arena.
     * @param bytes The number of bytes to allocate.
     * @return Pointer to the allocated memory, or nullptr if there is not enough space.
     */
    inline void* allocate(size_t bytes) {
        if (debugMode) {
            printf("Arena '%s' allocating %zu bytes\n", tracker, bytes);
        }
        if (offset + bytes > blockSize * numBuffers) {
            if (debugMode) printf("Arena '%s' is full, attempting to resize from %zu bytes to %zu bytes\n", tracker, blockSize * numBuffers, blockSize * 2);
            size_t newNumBuffers = (offset + bytes) / blockSize + 1; // Calculate the new number of buffers needed
            uint8_t** newBuffers = (uint8_t**)realloc(buffers, sizeof(uint8_t*) * newNumBuffers); // Resize the buffers array
            if (!newBuffers) {
                printf("Arena '%s' failed to resize to %zu bytes\n", tracker, blockSize * 2);
                return nullptr; // Failed to resize the buffer
            }
            buffers = newBuffers; // Update the buffers pointer
            for (size_t i = numBuffers; i < newNumBuffers; i++) {
                buffers[i] = (uint8_t*)malloc(blockSize); // Allocate memory for the new buffers
                memset(buffers[i], 0, blockSize); // Initialize the new buffers to zero
            }
            numBuffers = newNumBuffers; // Update the number of buffers
        }
        // Calculate the current buffer and offset
        size_t currentBuffer = offset / blockSize;
        size_t currentOffset = offset % blockSize;
        // Allocate memory from the current buffer
        void* ptr = buffers[currentBuffer] + currentOffset;
        offset += bytes; // Update the offset

        return ptr; // Return the pointer to the allocated memory
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
};

/**
 * @brief A smart arena allocator that can free memory.
 * 
 * This is a single-type/single-size arena allocator that can reuse memory.
 * This was created to hold dynamic types that can be created and destroyed at runtime, which would cause a normal arena allocator to waste a lot of memory.
 */
template<typename T>
struct sTypedSmartArena {
    /// @brief Pointer to the memory buffers.
    T** buffer;
    /// @brief Size of a memory buffer in number of objects, NOT bytes.
    size_t bufferSize;
    /// @brief Buffer count.
    size_t numBuffers;
    /// @brief List of currently allocated indices. (a special type of index, similar to the offset above)
    std::vector<size_t> allocated;
    /// @brief Tracker string for debugging purposes.
    const char* tracker;
    /// @brief Debug mode, prints every allocation and deallocation.
    bool debugMode = false;

    /**
     * @brief Constructor for the smart arena.
     * 
     * This constructor initializes the smart arena with a specified size and tracker string.
     * It allocates memory for the arena and initializes the size to zero.
     * 
     * @param tracker The tracker string for debugging purposes.
     * @param bufferSize The number of objects to allocate memory for, this is the max amount of objects that you expect at any one time.
     */
    inline sTypedSmartArena(const char* tracker, size_t bufferSize) : tracker(tracker), bufferSize(bufferSize), numBuffers(1) {
        buffer = (T**)malloc(sizeof(T*) * 1); // Allocate memory for the arena
        buffer[0] = (T*)malloc(sizeof(T) * bufferSize); // Allocate memory for the first buffer
        memset(buffer[0], 0, sizeof(T) * bufferSize); // Initialize the buffer to zero
    }

    /**
     * @brief Empty constructor for the smart arena.
     * 
     * This constructor initializes the smart arena with a default size of 0 and a null tracker string.
     * It allocates no memory for the arena and initializes the size to zero.
     */
    inline sTypedSmartArena() : tracker(""), bufferSize(0), numBuffers(0) {
        buffer = nullptr; // No memory allocated for the arena
    }

    /**
     * @brief Destructor for the smart arena.
     * 
     * This destructor frees the allocated memory for the arena and all allocated objects.
     * It is called when the smart arena goes out of scope or is deleted.
     */
    inline ~sTypedSmartArena() {
        for (size_t i = 0; i < numBuffers; i++) {
            free(buffer[i]); // Free the allocated memory for each buffer
        }
        free(buffer); // Free the allocated memory for the arena
    }

    /**
     * @brief Allocates memory for an object of type T.
     * 
     * This function allocates memory for an object of type T and returns a pointer to the allocated memory.
     * It is a template function that can be used for any type.
     * 
     * @return Pointer to the allocated memory for an object of type T.
     */
    inline T* allocate() {
        if (allocated.size() >= bufferSize) {
            // We need to allocate a new buffer
            size_t newNumBuffers = numBuffers + 1; // Calculate the new number of buffers needed
            T** newBuffer = (T**)realloc(buffer, sizeof(T*) * newNumBuffers); // Resize the buffers array
            if (!newBuffer) {
                printf("Arena '%s' failed to resize to %zu bytes\n", tracker, bufferSize * 2);
                return nullptr; // Failed to resize the buffer
            }
            buffer = newBuffer; // Update the buffers pointer
            buffer[numBuffers] = (T*)malloc(sizeof(T) * bufferSize); // Allocate memory for the new buffer
            memset(buffer[numBuffers], 0, sizeof(T) * bufferSize); // Initialize the new buffer to zero
            numBuffers++; // Update the number of buffers
        }
        size_t index = allocated.size(); // Get the index of the next available object
        allocated.push_back(index); // Add the index to the allocated list
        return &buffer[index / bufferSize][index % bufferSize]; // Return a pointer to the allocated object
    }

    /**
     * @brief Frees the memory for an object of type T.
     * 
     * This function frees the memory for an object of type T and removes it from the allocated list.
     * It is a template function that can be used for any type.
     * 
     * @param obj Pointer to the object to free.
     */
    inline void free(T* obj) {
        if (debugMode) {
            printf("Arena '%s' freeing object at %p\n", tracker, obj);
        }
        // Find the index of the object in the allocated list
        auto it = std::find(allocated.begin(), allocated.end(), (size_t)(obj - buffer[0]));
        if (it != allocated.end()) {
            allocated.erase(it); // Remove the index from the allocated list
        } else {
            printf("Arena '%s' failed to free object at %p\n", tracker, obj);
        }
    }

    /**
     * @brief Resets the smart arena.
     * 
     * This function resets the allocated list to empty, effectively freeing all allocated memory.
     */
    inline void reset() {
        allocated.clear(); // Clear the allocated list
    }
};
