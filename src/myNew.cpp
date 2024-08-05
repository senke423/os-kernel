//
// Created by sybilvane on 8/1/24.
//

#include "../h/syscall_c.hpp"

void* operator new(size_t size){
    return mem_alloc(size);
}

void* operator new[](size_t size){
    return mem_alloc(size);
}

void operator delete(void* alloc_space) noexcept {
    mem_free(alloc_space);
}

void operator delete[](void* alloc_space) noexcept {
    mem_free(alloc_space);
}