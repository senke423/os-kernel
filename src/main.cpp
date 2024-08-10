//
// Created by sybilvane on 5/27/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"

void userMain(){

}

void main(){
    MemoryAllocator::initialize();

    userMain();

    __putc('\n');
    riscv::close_riscv_emulation();
}