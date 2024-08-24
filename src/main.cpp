//
// Created by sybilvane on 5/27/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"
#include "../h/worker.hpp"


extern void userMain();
extern void myUserMain();
extern "C" void supervisorTrap();

void main(){

    MemoryAllocator::initialize();
    riscv::w_stvec((uint64)supervisorTrap);

//    char c = __getc();
//    __putc(c);
//    __putc('\n');

//    userMain();

    TCB* threads[3];
    int ret = thread_create(&threads[0], nullptr, nullptr);
    TCB::running = threads[0];

    ret += thread_create(&threads[1], workerBodyA, nullptr);
    printString("ThreadA created!\n");
    ret += thread_create(&threads[2], workerBodyB, nullptr);
    printString("ThreadB created!\n");

    printString("RET AFTER THREADS HAVE BEEN CREATED: ");
    printInt(ret);
    printString("\n");

    while (!(threads[1]->isFinished() && threads[2]->isFinished())){
        thread_dispatch();
    }

    for (auto &thread : threads){
        delete thread;
    }
    printString("FINISHED!\n");

    riscv::close_riscv_emulation();
}