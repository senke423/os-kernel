//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_TCB_H
#define INC_41F_OS_PROJEKAT_TCB_H

#include "../lib/hw.h"
#include "Scheduler.hpp"
#include "riscv.hpp"
#include "../lib/mem.h"

class TCB {
public:
    static TCB* running;
    using subroutine = void (*)(void*);

    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }
    uint64 getTimeSlice() const { return timeSlice; }
    static TCB* createThread(subroutine body);
    static void yield();

    struct Context {
        uint64 ra;
        uint64 sp;
    };

private:

    subroutine body;
    uint64* stack;
    uint64 timeSlice;
    Context context;
    bool finished;

    explicit TCB(subroutine body, uint64 timeSlice) :
        body(body),
        stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
        timeSlice(timeSlice),
        context({ body != nullptr ? (uint64) body : 0,
                  stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
        }),
        finished(false)
    {
        if (body != nullptr) {
//            Scheduler::put(this);
        }
    }

    static void dispatch();
};

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext);

#endif //INC_41F_OS_PROJEKAT_TCB_H
