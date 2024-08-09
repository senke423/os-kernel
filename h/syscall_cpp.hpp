//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_SYSCALL_CPP_HPP
#define INC_41F_OS_PROJEKAT_SYSCALL_CPP_HPP

#include "syscall_c.hpp"

class Thread {
public:
    Thread(void (*body)(void*), void* arg);
    virtual ~Thread();

    int start();

    static void dispatch();
    static int sleep(time_t);

protected:
    Thread();
    virtual void run(){}

private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
};

class Semaphore {
public:

    Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();
    int timedWait(time_t);
    int tryWait();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate();

protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation(){}

private:
    time_t period;
};

class Console {
public:
    static char getc();
    static void putc(char);
};

#endif //INC_41F_OS_PROJEKAT_SYSCALL_CPP_HPP
