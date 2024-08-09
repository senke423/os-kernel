//
// Created by sybilvane on 7/30/24.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::Chunk* MemoryAllocator::free_head = nullptr;

void *MemoryAllocator::mem_alloc(size_t size) {
    size % MEM_BLOCK_SIZE == 0 ? size = size/MEM_BLOCK_SIZE : size = size/MEM_BLOCK_SIZE + 1;

    Chunk* prev = nullptr;
    Chunk* iter = free_head;
    Chunk* sel = nullptr;

    // find best fit
    while (iter != nullptr){
        if (iter->size >= size){
            if (!sel || sel->size > iter->size){
                sel = iter;
                prev = iter->prev;
            }
        }
        iter = iter->next;
    }

    // no space for new chunk
    if (!sel)
        return nullptr;

    Chunk* new_free_fragment = (Chunk*)sel + size * MEM_BLOCK_SIZE;
    new_free_fragment->size = sel->size - size;
    new_free_fragment->next = sel->next;
    new_free_fragment->prev = prev;

    if (new_free_fragment->size == 0){
        // no fragmentation
        if (prev)
            prev->next = new_free_fragment->next;
        else
            free_head = nullptr;
    } else {
        if (prev)
            prev->next = new_free_fragment;
        else
            free_head = new_free_fragment;
    }

    sel->size = size;
    Chunk* ptr = sel + 1;
    return ptr;
}

int MemoryAllocator::mem_free(void* chunk) {
    // It's offset by 1 address because the first block
    // has the linked list header
    Chunk* sel = (Chunk*)chunk - 1;

    if (sel < (Chunk*)HEAP_START_ADDR || sel > (Chunk*)HEAP_END_ADDR)
        return -1;

    // no free space on heap
    if (!free_head){
        // sel->size is already read from the mem. location;
        // setting next and prev to nullptr just in case.
        sel->next = nullptr;
        sel->prev = nullptr;
        free_head = sel;
        return 0;
    }

    // there is additional free space

    bool left_overlap, right_overlap;
    left_overlap = right_overlap = false;
    Chunk* iter = free_head;

    // before & after -> nearest free blocks before and after the selected chunk
    Chunk* before = nullptr;
    Chunk* after = nullptr;
    while (iter != nullptr){
        if (iter < sel)
            before = iter;
        if (iter > sel)
            after = iter;

        if (iter + iter->size == sel)
            left_overlap = true;
        if (sel + sel->size == iter)
            right_overlap = true;

        iter = iter->next;

        if (left_overlap && right_overlap)
            break;
    }

    if (!left_overlap && !right_overlap){
        // No merging needed.
        if (before){
            sel->prev = before;
        } else {
            free_head = sel;
            sel->prev = nullptr;
        }

        sel->next = after;
    }
    else if (left_overlap && right_overlap){
        // Merge to both sides
        // | ... | <empty> | TO BE FREED | <empty> | ... |
        before->size = before->size + sel->size + after->size;
        before->next = after->next;
    }
    else if (left_overlap){
        // | ... | <empty> | TO BE FREED | ... |
        before->size = before->size + sel->size;
    }
    else if (right_overlap){
        // | ... | TO BE FREED | <empty> | ... |
        sel->size = sel->size + after->size;
        sel->prev = after->prev;
        sel->next = after->next;

        if (!after->prev)
            free_head = sel;
    }

    return 0;
}

void MemoryAllocator::initialize() {
    free_head = (Chunk*)HEAP_START_ADDR;
    free_head->next = nullptr;
    free_head->prev = nullptr;
    free_head->size = ((uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR)/MEM_BLOCK_SIZE;
}
