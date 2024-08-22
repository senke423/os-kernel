//
// Created by sybilvane on 7/29/24.
//

#include "../h/TCB.hpp"
#include "../test/printing.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCnt = 0;

void TCB::dispatch() {
    TCB::timeSliceCnt = 0;
    TCB* old = running;
    if (!old->isFinished() && !old->isAsleep() && !old->isBlocked()){
        if (body != nullptr)
            Scheduler::put(old);

    }
    running = Scheduler::get();

    if (old != running){
        contextSwitch(&old->context, &running->context);
    }
}

int TCB::createThread(TCB *handle, TCB::subroutine subroutine, void *arg, void *stack_space) {
    handle = new TCB(subroutine, arg, stack_space, DEFAULT_TIME_SLICE);

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
    volatile uint64 code = 0x13;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("ecall");
}

int TCB::exit() {
    running->setFinished(true);
    return 0;
}

TCB *TCB::createThread(TCB::subroutine subroutine) {
    return new TCB(subroutine, nullptr, nullptr, DEFAULT_TIME_SLICE);
}
