//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_SCHEDULER_HPP
#define INC_41F_OS_PROJEKAT_SCHEDULER_HPP

#include "../h/TCB.hpp"
#include "../h/LinkedList.hpp"

class TCB;

class Scheduler {
public:
    typedef struct SleepingThread {
        TCB* thread;
        time_t time_left;

        SleepingThread(TCB* thread, time_t time_left) : thread(thread), time_left(time_left) {}
    } SleepingThread;


    Scheduler(const Scheduler& scheduler) = delete;
    Scheduler& operator=(const Scheduler& scheduler) = delete;

    static TCB* get();
    static TCB* removeThread(TCB* thread);
    static void put(TCB* thread);
    static size_t getLen();

    static void putSleeping(TCB* thread, time_t timeout);
    static void removeSleepingThread(TCB* thread);
    static void update();
    static size_t getLenSleeping();

private:
    static LinkedList<TCB*> ready_threads;
    static LinkedList<SleepingThread*> sleeping_threads;

    Scheduler() {}
};


#endif //INC_41F_OS_PROJEKAT_SCHEDULER_HPP
