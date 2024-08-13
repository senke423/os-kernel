//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_MYSEMAPHORE_HPP
#define INC_41F_OS_PROJEKAT_MYSEMAPHORE_HPP

#include "../h/TCB.hpp"

class mySemaphore {
public:
    int open(mySemaphore** handle, unsigned int init);
    int close();
    int wait();
    int signal();
    int timed_wait(mySemaphore id, time_t timeout);
    int try_wait();

private:
    const static int SEM_CLOSE_ERR = -10;
    const static int SEM_NULL_ERR = -11;

    explicit mySemaphore(unsigned int val) : val(val), is_open(true) {
        blocked_threads = LinkedList<TCB*>();
    };

    unsigned int val;
    bool is_open;
    LinkedList<TCB*> blocked_threads;
};


#endif //INC_41F_OS_PROJEKAT_MYSEMAPHORE_HPP
