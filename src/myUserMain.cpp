//
// Created by sybilvane on 8/12/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
#include "../lib/console.h"
//#include "../lib/myPrint.hpp"
#include "../h/worker.hpp"
#include "../test/printing.hpp"

void myUserMain(){

    TCB* threads[3];
    threads[0] = TCB::createThread(nullptr);
    TCB::running = threads[0];

    threads[1] = TCB::createThread(workerBodyA);
    printString("ThreadA created!\n");
    threads[2] = TCB::createThread(workerBodyB);
    printString("ThreadB created!\n");


    while (!(threads[1]->isFinished() && threads[2]->isFinished())){
        TCB::yield();
    }

    for (auto &thread : threads){
        delete thread;
    }
    printString("FINISHED!\n");

//    int* niz = new int[5];
//    printString("Ovo je u myUserMain: ");
//    niz[0] = 'A';
//    __putc(niz[0]);
//
//    delete[] niz;
//
//    // if all went well:
//    __putc('-');
//    __putc('-');
//    __putc('\n');
//    __putc('o');
//    __putc('k');
//    __putc('\n');
//    __putc('-');
//    __putc('-');
//    __putc('\n');
}