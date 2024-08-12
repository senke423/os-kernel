//
// Created by sybilvane on 7/29/24.
//

#include "../h/TCB.hpp"

TCB* TCB::running = nullptr;

TCB* TCB::createThread(TCB::Body body) {
//    return new TCB(body);
    return nullptr;
}

void TCB::yield() {
//    pushRegisters();
//    dispatch();
//    popRegisters();
}

void TCB::dispatch() {
    TCB* old = running;
    if (!old->isFinished()){
        Scheduler::put(old);
    }
//    contextSwitch(&old->context, &running->context);
}