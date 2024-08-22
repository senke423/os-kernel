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
#include "../h/worker.hpp"


extern void userMain();
extern void myUserMain();
extern "C" void supervisorTrap();

void init_kernel(){
    MemoryAllocator::initialize();
    riscv::w_stvec((uint64)supervisorTrap);
}

void finalize_kernel(){
    riscv::close_riscv_emulation();
}

void main(){
    init_kernel();

//    userMain();
//    myUserMain();

    TCB* threads[3];
    int ret = thread_create(&threads[0], nullptr, nullptr);
    TCB::running = threads[0];

    ret += thread_create(&threads[1], workerBodyA, nullptr);
    printString("ThreadA created!\n");
    ret += thread_create(&threads[2], workerBodyB, nullptr);
    printString("ThreadB created!\n");

    printString("RET AFTER THREADS HAVE BEEN CREATED: ");
    printInt(ret);

    while (!(threads[1]->isFinished() && threads[2]->isFinished())){
        thread_dispatch();
    }

    for (auto &thread : threads){
        delete thread;
    }
    printString("FINISHED!\n");


    finalize_kernel();
}