//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_SCHEDULER_HPP
#define INC_41F_OS_PROJEKAT_SCHEDULER_HPP

#include "../h/tcb.hpp"

class scheduler {

public:
    static tcb* get();
    static void put(tcb* thread);
};


#endif //INC_41F_OS_PROJEKAT_SCHEDULER_HPP
