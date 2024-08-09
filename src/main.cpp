//
// Created by sybilvane on 5/27/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"

void userMain(){
    MemoryAllocator::initialize();
    void* ptr = MemoryAllocator::mem_alloc(100);
    MemoryAllocator::mem_free(ptr);

    __putc('o');
    __putc('k');
    __putc('\n');
}

void main(){
    userMain();
    riscv::close_riscv_emulation();
}