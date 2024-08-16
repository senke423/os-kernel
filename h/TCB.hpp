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

    ~TCB() { delete stack; }

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

private:
    friend class riscv;
    friend class mySemaphore;

    const static int THREAD_CREATE_ERR = -20;

    uint64* stack;
    void* arg;

    subroutine body;
    uint64 timeSlice;
    Context context;
    bool finished;
    bool blocked;
    bool asleep;

    explicit TCB(subroutine body, void* arg, void* stack_space, uint64 timeSlice) :
        stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
        arg(arg),
        body(body),
        timeSlice(timeSlice),
        context({ (uint64) &thread_wrapper,
                  (uint64) stack_space
        }),
        finished(false),
        blocked(false),
        asleep(false)
    {}

    static void dispatch();
    static void yield();
    static void thread_wrapper();

};

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext);


#endif //INC_41F_OS_PROJEKAT_TCB_H
