//
// Created by sybilvane on 7/29/24.
//

#include "../h/Scheduler.hpp"

LinkedList<TCB*> Scheduler::ready_threads = LinkedList<TCB*>();
LinkedList<Scheduler::SleepingThread*> Scheduler::sleeping_threads = LinkedList<SleepingThread*>();

TCB *Scheduler::get() {
    return ready_threads.remove((size_t)0);
}

void Scheduler::put(TCB *thread) {
    if (!thread)
        return;

    ready_threads.push(thread);
}

size_t Scheduler::getLen() {
    return ready_threads.getLen();
}

size_t Scheduler::getLenSleeping() {
    return sleeping_threads.getLen();
}

TCB *Scheduler::removeThread(TCB *thread) {
    return ready_threads.remove(thread);
}

void Scheduler::update() {
    sleeping_threads.cur_set_first();
    SleepingThread* iter = sleeping_threads.get_cur();
    while (iter){
        if (--iter->time_left == 0){
            iter->thread->setAsleep(false);
            Scheduler::put(iter->thread);
            sleeping_threads.remove_cur();
        }

        sleeping_threads.cur_next();
        iter = sleeping_threads.get_cur();
    }
}

void Scheduler::putSleeping(TCB *thread, time_t timeout) {
    if (timeout == 0 || !thread)
        return;

    thread->setAsleep(true);
    sleeping_threads.push(new SleepingThread(thread, timeout));
}

void Scheduler::removeSleepingThread(TCB *thread) {
    SleepingThread* iter = sleeping_threads.get_cur();
    while (iter){
        if (iter->thread == thread){
            sleeping_threads.remove(iter);
            break;
        }

        sleeping_threads.cur_next();
        iter = sleeping_threads.get_cur();
    }
}
