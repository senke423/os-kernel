//
// Created by sybilvane on 7/29/24.
//

#include "../h/mySemaphore.hpp"

int mySemaphore::open(mySemaphore **handle, unsigned int init) {
    *handle = new mySemaphore(init);
    if (!*handle)
        return -1;
    return 0;
}

// does NOT deallocate the space
int mySemaphore::close() {
    if (!is_open)
        return SEM_CLOSE_ERR;
    is_open = false;

    // unblock the threads
    while (blocked_threads.getLen() != 0){
        TCB* tmp = blocked_threads.pop();
        if (tmp->isAsleep()){
            // removes the first sleeping thread from the queue
            Scheduler::removeSleepingThread(tmp);
        } else {
            tmp->setBlocked(false);
            Scheduler::put(tmp);
        }
    }

    return 0;
}

int mySemaphore::wait() {
    // semaphore is deallocated
    if (!is_open)
        return SEM_NULL_ERR;

    if (val > 0){
        --val;
    } else {
        // RUNNING THREAD SHOULD BE BLOCKED,
        // AND ANOTHER THREAD SHOULD BE DISPATCHED
        TCB::running->setBlocked(true);
        blocked_threads.push(TCB::running);
        TCB::dispatch();

        // is the semaphore still opened?
        if (!is_open)
            return SEM_NULL_ERR;
    }
    return 0;
}

int mySemaphore::signal() {
    if (!is_open)
        return SEM_NULL_ERR;

    ++val;

    // UNBLOCK THREAD AND PUSH TO READY QUEUE
    if (blocked_threads.getLen() > 0){
        TCB* temp = blocked_threads.pop();
        temp->setBlocked(false);

        Scheduler::put(temp);
    } else {
        return SEM_UNPAIRED_SIG;
    }

    return 0;
}

int mySemaphore::timed_wait(mySemaphore id, time_t timeout) {

    if (!is_open)
        return SEM_NULL_ERR;

    if (val > 0){
        --val;
    } else {

    }

    return 0;
}

int mySemaphore::try_wait() {
    if (!is_open)
        return SEM_NULL_ERR;

    if (val > 0){
        --val;
        return 0;
    }

    return 1;
}
