//
// Created by sybilvane on 7/29/24.
//

#include "../h/syscall_cpp.hpp"

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
