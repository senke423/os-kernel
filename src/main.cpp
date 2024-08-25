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
extern "C" void vectorTable();

void mainWrapper(void* arg){
    userMain();
}

void main(){

    MemoryAllocator::initialize();
    riscv::w_stvec((uint64)&vectorTable | 0b01);

    TCB* kernel_thread;
    thread_create(&kernel_thread, nullptr, nullptr);
    TCB::running = kernel_thread;

    riscv::setMode(true);
    TCB* user_thread;
    thread_create(&user_thread, mainWrapper, nullptr);

    while (!user_thread->isFinished()){
        thread_dispatch();
    }

    delete user_thread;
    riscv::setMode(false);

    riscv::close_riscv_emulation();
}