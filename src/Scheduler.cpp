//
// Created by sybilvane on 7/29/24.
//

#include "../h/Scheduler.hpp"

LinkedList<TCB*> Scheduler::queue = LinkedList<TCB*>();

TCB *Scheduler::get() {
    return queue.remove(0);
}

void Scheduler::put(TCB *thread) {
    queue.push(thread);
}

size_t Scheduler::getLen() {
    return queue.getLen();
}
