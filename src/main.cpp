//
// Created by sybilvane on 5/27/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/myConsole.hpp"
#include "../h/riscv.hpp"

extern void userMain();
extern "C" void supervisorTrap();

void init_kernel(){
    MemoryAllocator::initialize();
    myConsole::console_init();
    riscv::w_stvec((uint64)supervisorTrap);
}

void finalize_kernel(){
    myConsole::console_finalize();
}

void main(){
    init_kernel();

    userMain();

    __putc('\n');

    finalize_kernel();
    riscv::close_riscv_emulation();
}