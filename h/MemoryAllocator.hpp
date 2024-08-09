//
// Created by sybilvane on 7/30/24.
//

#ifndef INC_41F_OS_PROJEKAT_MEMORYALLOCATOR_HPP
#define INC_41F_OS_PROJEKAT_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
public:

    static void initialize();
    [[nodiscard]] static void* mem_alloc(size_t size);
    static int mem_free(void* chunk);

    typedef struct Chunk {
        Chunk* next;
        Chunk* prev;
        size_t size; // in BLOCKS
    } Chunk;

    static Chunk* free_head;

    MemoryAllocator(const MemoryAllocator& memAlloc) = delete;
    MemoryAllocator& operator=(const MemoryAllocator& memAlloc) = delete;
private:
    const static int ALG_ERR = -5;

    MemoryAllocator() {};
    static void merge();
};


#endif //INC_41F_OS_PROJEKAT_MEMORYALLOCATOR_HPP
