//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_SCHEDULER_HPP
#define INC_41F_OS_PROJEKAT_SCHEDULER_HPP

class TCB;

class Scheduler {

public:
    static TCB* get();
    static void put(TCB* thread);
};


#endif //INC_41F_OS_PROJEKAT_SCHEDULER_HPP
