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
    MemoryAllocator::initialize();

    riscv::w_stvec((uint64)&supervisorTrap);
//    riscv::ms_sstatus(riscv::SSTATUS_SIE);

    myConsole::console_init();

    printString("INIT COMPLETE ---\n");
}

void finalize_kernel(){
    myConsole::console_finalize();
}

void main(){
//    init_kernel();
    MemoryAllocator::initialize();


//    userMain();
    char* niz = new char[3];
    niz[0] = 'a';
    niz[1] = 'b';
    niz[2] = 'c';

    __putc(niz[0]);
    __putc(niz[1]);
    __putc(niz[2]);

//    delete[] niz;

//    finalize_kernel();
    riscv::close_riscv_emulation();
}