//
// Created by sybilvane on 7/29/24.
//

#include "../h/Scheduler.hpp"

LinkedList<TCB*> Scheduler::ready_threads = LinkedList<TCB*>();
LinkedList<TCB*> Scheduler::sleeping_threads = LinkedList<TCB*>();

TCB *Scheduler::get() {
    return ready_threads.remove((size_t)0);
}

void Scheduler::put(TCB *thread) {
    ready_threads.push(thread);
}

size_t Scheduler::getLen() {
    return ready_threads.getLen();
}

TCB *Scheduler::getSleeping() {
    return sleeping_threads.remove((size_t)0);
}

void Scheduler::putSleeping(TCB *thread) {
    sleeping_threads.push(thread);
}

size_t Scheduler::getLenSleeping() {
    return sleeping_threads.getLen();
}

TCB *Scheduler::removeThread(TCB *thread) {
    return ready_threads.remove(thread);
}

TCB *Scheduler::removeSleepingThread(TCB *thread) {
    return sleeping_threads.remove(thread);
}
