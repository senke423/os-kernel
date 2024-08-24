//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_TCB_H
#define INC_41F_OS_PROJEKAT_TCB_H

#include "../lib/hw.h"
#include "Scheduler.hpp"
#include "riscv.hpp"

class TCB {
public:
    static TCB* running;
    using subroutine = void (*)(void*);

    bool isFinished() const { return finished; }
    void setFinished(bool val) { finished = val; }
    bool isBlocked() const { return blocked; }
    void setBlocked(bool val) { blocked = val; }
    bool isAsleep() const { return asleep; }
    void setAsleep(bool val) { asleep = val; }

    uint64 getTimeSlice() const { return timeSlice; }
    static int createThread(TCB** handle, subroutine subroutine, void* arg, void* stack_space);

    struct Context {
        uint64 ra;
        uint64 sp;
    };
    static uint64 timeSliceCnt;
    static void dispatch();
    static void yield();
private:
    static int cnt;
    int id;
    friend class riscv;
    friend class mySemaphore;

    void* arg;

    subroutine body;
    uint64 timeSlice;
    uint64* stack;
    Context context;
    bool finished;
    bool blocked;
    bool asleep;

    TCB(subroutine body, void* arg, uint64* stack_space, uint64 timeSlice) :
        id(cnt++),
        arg(arg),
        body(body),
        timeSlice(timeSlice),
        stack(stack_space),
        context({body != nullptr ? (uint64)&thread_wrapper : 0,
                 stack_space != nullptr ? (uint64)((char*)stack_space + DEFAULT_STACK_SIZE) : 0}),
        finished(false),
        blocked(false),
        asleep(false)
    {
    }

    static int exit();

    static void thread_wrapper();

};

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext);


#endif //INC_41F_OS_PROJEKAT_TCB_H
