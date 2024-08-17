//
// Created by sybilvane on 7/30/24.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::Chunk* MemoryAllocator::free_head = nullptr;

void *MemoryAllocator::mem_alloc(size_t size) {
//    size % MEM_BLOCK_SIZE == 0 ? size = size/MEM_BLOCK_SIZE : size = size/MEM_BLOCK_SIZE + 1;

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

    if (sel->size == size) {
        // no fragmentation
        if (prev){
            prev->next = sel->next;
        }
        else {
            free_head = nullptr;
        }
    } else {
        Chunk* new_free_fragment = (Chunk*)((char*)sel + size * MEM_BLOCK_SIZE);
        new_free_fragment->size = sel->size - size;
        new_free_fragment->next = sel->next;
        new_free_fragment->prev = prev;
        if (prev)
            prev->next = new_free_fragment;
        else
            free_head = new_free_fragment;
    }

    sel->size = size;
    return sel;
}

int MemoryAllocator::mem_free(void* chunk) {
    if (chunk == nullptr)
        return 0;

    Chunk* sel = (Chunk*)chunk;

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
        if (iter > sel && after == nullptr)
            after = iter;

        if ((char*)iter + iter->size*MEM_BLOCK_SIZE == (char*)sel)
            left_overlap = true;
        if ((char*)sel + sel->size*MEM_BLOCK_SIZE == (char*)iter)
            right_overlap = true;

        iter = iter->next;

        if (left_overlap && right_overlap)
            break;
    }

    if (!left_overlap && !right_overlap){
        // No merging needed.
        if (before){
            sel->prev = before;
            before->next = sel;
        } else {
            free_head = sel;
            sel->prev = nullptr;
        }

        if (after)
            after->prev = sel;

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
        else
            sel->prev->next = sel;
    }

    return 0;
}

void MemoryAllocator::initialize() {
    free_head = (Chunk*)HEAP_START_ADDR;
    free_head->next = nullptr;
    free_head->prev = nullptr;
    free_head->size = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof(Chunk))/MEM_BLOCK_SIZE;
}
