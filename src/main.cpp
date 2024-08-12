//
// Created by sybilvane on 5/27/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/riscv.hpp"

extern void userMain();
extern "C" void supervisorTrap();

void init_kernel(){
    MemoryAllocator::initialize();
//    riscv::w_stvec((uint64)supervisorTrap);
}

void main(){
    init_kernel();

    userMain();

    __putc('\n');
    riscv::close_riscv_emulation();
}