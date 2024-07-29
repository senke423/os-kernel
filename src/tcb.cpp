//
// Created by sybilvane on 7/29/24.
//

#include "../h/tcb.hpp"


void tcb::yield() {
    pushRegisters();
    dispatch();
    popRegisters();
}

void tcb::dispatch() {
    tcb* old = running;
    if (!old->isFinished())
        scheduler::put(old);
    running = scheduler::get();

    tcb::contextSwitch(&old->context, &running->context);
}

tcb *tcb::createThread(tcb::Body body) {
    return new tcb(body);
}

