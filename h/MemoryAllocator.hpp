//
// Created by sybilvane on 7/30/24.
//

#ifndef INC_41F_OS_PROJEKAT_MEMORYALLOCATOR_HPP
#define INC_41F_OS_PROJEKAT_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
    MemoryAllocator(){}

    typedef struct Chunk {
        Chunk* next;
        Chunk* prev;
        size_t size;
    } Chunk;


public:

    static void* mem_alloc(size_t size);
    static int mem_free(void* chunk);
};


#endif //INC_41F_OS_PROJEKAT_MEMORYALLOCATOR_HPP
