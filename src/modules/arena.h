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

/**
 * @brief A smart arena allocator that can free memory.
 * 
 * This is a single-type/single-size arena allocator that can reuse memory.
 * This was created to hold dynamic types that can be created and destroyed at runtime, which would cause a normal arena allocator to waste a lot of memory.
 */
template<typename T>
struct sTypedSmartArena {
    /// @brief Pointer to the memory buffer.
    T* buffer;
    /// @brief Size of the memory buffer in number of objects, NOT bytes.
    size_t size;
    /// @brief List of currently allocated indices.
    std::vector<size_t> allocated;
    /// @brief Tracker string for debugging purposes.
    const char* tracker;

    /**
     * @brief Constructor for the smart arena.
     * 
     * This constructor initializes the smart arena with a specified size and tracker string.
     * It allocates memory for the arena and initializes the size to zero.
     * 
     * @param tracker The tracker string for debugging purposes.
     * @param num The number of objects to allocate memory for, this is the max amount of objects that you expect at any one time.
     */
    inline sTypedSmartArena(const char* tracker, size_t num) : tracker(tracker), size(num) {
        buffer = (T*)malloc(sizeof(T) * num); // Allocate memory for the arena
        memset(buffer, 0, sizeof(T) * num);   // Initialize the buffer to zero
        // allocated.reserve(num);               // Reserve space for the allocated indices
    }

    /**
     * @brief Empty constructor for the smart arena.
     * 
     * This constructor initializes the smart arena with a default size of 0 and a null tracker string.
     * It allocates no memory for the arena and initializes the size to zero.
     */
    inline sTypedSmartArena() : tracker(nullptr), size(0) {
        buffer = nullptr; // No memory allocated for the arena
    }

    /**
     * @brief Destructor for the smart arena.
     * 
     * This destructor frees the allocated memory for the arena and all allocated objects.
     * It is called when the smart arena goes out of scope or is deleted.
     */
    inline ~sTypedSmartArena() {
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
        if (allocated.size() >= size) {
            printf("Smart arena '%s' is full, attempting to resize from %zu objects to %zu objects\n", tracker, size, size * 2);
            resize(size * 2); // Attempt to resize the arena
        }
        // we have to find an index that is not in the allocated list
        size_t index = 0;
        for (size_t i = 0; i < size; i++) {
            if (std::find(allocated.begin(), allocated.end(), i) == allocated.end()) {
                index = i;
                break;
            }
        }
        allocated.push_back(index); // Add the index to the allocated list
        return &buffer[index]; // Return a pointer to the allocated object
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
        size_t index = (size_t)(obj - buffer); // Calculate the index of the object
        auto it = std::find(allocated.begin(), allocated.end(), index); // Find the index in the allocated list
        if (it != allocated.end()) {
            allocated.erase(it); // Remove the index from the allocated list
        }
        memset(obj, 0, sizeof(T)); // Clear the object, not really necessary, but helps for future issues to come
    }

    /**
     * @brief Resets the smart arena.
     * 
     * This function resets the allocated list to empty, effectively freeing all allocated memory.
     */
    inline void reset() {
        allocated.clear(); // Clear the allocated list
        memset(buffer, 0, sizeof(T) * size); // Clear the buffer
    }

    /**
     * @brief Resizes the smart arena.
     * 
     * This function resizes the smart arena to a new size.
     * It is called when the smart arena goes out of scope or is deleted.
     * 
     * @param newSize The new size of the smart arena.
     */
    inline void resize(size_t newSize) {
        if (newSize <= size) {
            printf("Smart arena '%s' resize requested to %zu bytes, but current size is %zu bytes\n", tracker, newSize, size);
            return; // No need to resize if the new size is less than or equal to the current size
        }
        T* newBuffer = (T*)realloc(buffer, sizeof(T) * newSize);
        if (!newBuffer) {
            printf("Smart arena '%s' failed to resize to %zu bytes\n", tracker, newSize);
            return; // Failed to resize the buffer
        }
        buffer = newBuffer; // Update the buffer pointer
        size = newSize;     // Update the size
    }
};

// ### WIP code below ###


// struct sSmartArena {
//     /// @brief Pointer to the memory buffer.
//     uint8_t* buffer;
//     /// @brief Size of the memory buffer.
//     size_t size;
//     struct sArenaBlock {
//         size_t offset;
//         size_t size;
//     };
//     /// @brief List of currently allocated blocks.
//     std::vector<sArenaBlock> allocated;
//     /// @brief Tracker string for debugging purposes.
//     const char* tracker;

//     sArenaBlock findFreeBlock(size_t bytes) {
//         // find an area in the buffer that has no bytes contained in a current block
//         // we will just try each offset, starting from 0 (if there is no block there) and then if it cant fit, or if a block already exists there, we go to the next closest block and try to fit it right at the end of that block
//         size_t offset = 0;
//         bool needsResize = false;
//         while (true) {
//             while (offset < size) {
//                 // we want to check if the current offset "intersects" with any of the allocated blocks
//                 // if it does, we add to the offset the size of the block, and check again
//                 for (size_t i = 0; i < allocated.size(); i++) {
//                     if (offset >= allocated[i].offset && offset < allocated[i].offset + allocated[i].size) {
//                         offset = allocated[i].offset + allocated[i].size;
//                         break;
//                     }
//                 }
//             }
//             // now, either we have found an empty spot, or we have reached the end of the buffer
//             // if we have reached the end of the buffer, it is easy, we just resize and return the block.
//             if (offset + bytes > size) {
//                 needsResize = true;
//                 break;
//             }
//             bool needRestart = false;
//             // if we have found an empty spot, check if it is big enough, if any offset is in between offset and offset + bytes, then this check has failed and we continue finding another empty block
//             for (size_t i = 0; i < allocated.size(); i++) {
//                 if (offset < allocated[i].offset && offset + bytes >= allocated[i].offset) {
//                     offset = allocated[i].offset + allocated[i].size;
//                     needRestart = true;
//                     break;
//                 }
//             }
//             if (!needRestart) {
//                 continue; // dang it, block isn't big enough, try again
//             }

//             // if we get here, we have found a block that is big enough, so we can return it
//             sArenaBlock block = {offset, bytes};
//             allocated.push_back(block); // Add the block to the allocated list
//             return block;
//         }

//         if (needsResize) {
//             printf("Smart arena '%s' is full, attempting to resize from %zu bytes to %zu bytes\n", tracker, size, size * 2);
//             size_t newSize;
//             if (size * 2 > offset + bytes) {
//                 newSize = size * 2;
//             } else {
//                 newSize = offset + bytes + size / 2;
//             }
//             resize(newSize);
//         }
//         // now that it is resized, we can just create the block
//         sArenaBlock block = {offset, bytes};
//         allocated.push_back(block); // Add the block to the allocated list
//         return block;
//     }
// };