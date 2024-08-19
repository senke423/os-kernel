//
// Created by sybilvane on 7/29/24.
//

#include "../h/TCB.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCnt = 0;

void TCB::dispatch() {
    TCB::timeSliceCnt = 0;
    TCB* old = running;
    if (!old->isFinished() && !old->isAsleep() && !old->isBlocked()){
        Scheduler::put(old);
    }

    contextSwitch(&old->context, &running->context);
}

int TCB::createThread(TCB *handle, TCB::subroutine subroutine, void *arg, void *stack_space) {
    printString("TCB createThread\n");
    handle = new TCB(subroutine, arg, stack_space, DEFAULT_TIME_SLICE);
    printString("TCB thread created");

    // put this newly created thread in the Scheduler ready_threads
    Scheduler::put(handle);
    return 0;
}

void TCB::thread_wrapper() {
    riscv::pop_spp_spie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}

void TCB::yield() {
    __asm__ volatile ("ecall");
}

int TCB::exit() {
    running->setFinished(true);
    return 0;
}
