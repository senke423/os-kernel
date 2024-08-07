//
// Created by sybilvane on 7/30/24.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::Chunk* MemoryAllocator::free_head = nullptr;

void *MemoryAllocator::mem_alloc(size_t size) {

    Chunk* iter = free_head;
    while (iter != nullptr){
        if (iter->size >= size)
            break;
    }

    // no space for new chunk
    if (!iter)
        return nullptr;

    Chunk* new_chunk =

}

int MemoryAllocator::mem_free(void *chunk) {
    return 0;
}

void MemoryAllocator::initialize() {
    free_head = (Chunk*)HEAP_START_ADDR;
    free_head->next = nullptr;
    free_head->prev = nullptr;
    free_head->size = ((uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR)/MEM_BLOCK_SIZE;
}

void MemoryAllocator::merge() {

}
