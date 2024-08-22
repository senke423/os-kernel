//
// Created by sybilvane on 5/27/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/myConsole.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
//#include "../lib/myPrint.hpp"
#include "../test/printing.hpp"


extern void userMain();
extern void myUserMain();
extern "C" void supervisorTrap();

void init_kernel(){
    printString("--- KERNEL INIT STARTING ---\n\n");

    MemoryAllocator::initialize();

    riscv::w_stvec((uint64)supervisorTrap);
//    riscv::ms_sstatus(riscv::SSTATUS_SIE);
//    uint64 mask = riscv::SSTATUS_SIE;
//    __asm__ volatile("csrw sstatus, %0" : : "r" (mask));

//    myConsole::console_init();

    printString("--- KERNEL INIT COMPLETE ---\n\n");
}

void finalize_kernel(){
//    myConsole::console_finalize();
}

void main(){
    init_kernel();

//    userMain();
    myUserMain();

    finalize_kernel();
    riscv::close_riscv_emulation();
}