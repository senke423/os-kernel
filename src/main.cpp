//
// Created by sybilvane on 5/27/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/myConsole.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../lib/printing.hpp"

extern void userMain();
extern "C" void supervisorTrap();

void init_kernel(){
    printString("--- KERNEL INIT STARTING ---\n\n");

    MemoryAllocator::initialize();

    riscv::w_stvec((uint64)&supervisorTrap);
//    riscv::ms_sstatus(riscv::SSTATUS_SIE);

//    myConsole::console_init();

    printString("--- KERNEL INIT COMPLETE ---\n\n");
}

void finalize_kernel(){
//    myConsole::console_finalize();
}

void main(){
    init_kernel();

    userMain();

    finalize_kernel();
    riscv::close_riscv_emulation();
}