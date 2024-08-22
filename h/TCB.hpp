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
    static int createThread(TCB* handle, subroutine subroutine, void* arg, void* stack_space);
    static TCB* createThread(subroutine subroutine);

    struct Context {
        uint64 ra;
        uint64 sp;
    };
    static uint64 timeSliceCnt;
    static void dispatch();
    static void yield();
private:
    friend class riscv;
    friend class mySemaphore;

    void* arg;

    subroutine body;
    uint64 timeSlice;
    Context context;
    bool finished;
    bool blocked;
    bool asleep;

    explicit TCB(subroutine body, void* arg, void* stack_space, uint64 timeSlice) :
        arg(arg),
        body(body),
        timeSlice(timeSlice),

        finished(false),
        blocked(false),
        asleep(false)
    {

        if (stack_space == nullptr){
            stack_space = new uint64[DEFAULT_STACK_SIZE];
        }
        context = {(uint64)&thread_wrapper, (uint64)stack_space};
    }

    static int exit();

    static void thread_wrapper();

};

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext);


#endif //INC_41F_OS_PROJEKAT_TCB_H
