//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_SCHEDULER_HPP
#define INC_41F_OS_PROJEKAT_SCHEDULER_HPP

#include "../h/TCB.hpp"
#include "../h/LinkedList.hpp"

class TCB;

class Scheduler {
    static LinkedList<TCB*> queue;

    Scheduler() {}
public:
    Scheduler(const Scheduler& scheduler) = delete;
    Scheduler& operator=(const Scheduler& scheduler) = delete;

    static TCB* get();
    static void put(TCB* thread);
    static size_t getLen();
};


#endif //INC_41F_OS_PROJEKAT_SCHEDULER_HPP
