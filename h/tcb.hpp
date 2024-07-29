//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_TCB_H
#define INC_41F_OS_PROJEKAT_TCB_H

#include "../lib/hw.h"
#include "../h/scheduler.hpp"

class tcb {
public:
    static tcb* running;

    // alias for function pointer that points to func that
    // takes no arguments and returns 'void'
    using Body = void (*)();

    bool isFinished() const {
        return finished;
    };
    void setFinished(bool finished) {
        tcb::finished = finished;
    };

    static tcb* createThread(Body body);
    static void yield();


private:
    tcb(Body body) :
        body(body),
        stack(new uint64[STACK_SIZE]),
        context({
            (uint64) body,
            (uint64) &stack[STACK_SIZE] // grows towards decreasing memory
        })
    {
        scheduler::put(this);
    }

    struct Context {
        uint64 ra;
        uint64 sp;
    };
    Body body;
    uint64* stack;
    Context context;
    bool finished;

    // although CLion marks 'contextSwitch' as unimplemented, it is,
    // in contextSwitch.S
    static void contextSwitch(Context* oldContext, Context* runningContext);
    static void dispatch();

    static uint64 constexpr STACK_SIZE = 1024;
};


#endif //INC_41F_OS_PROJEKAT_TCB_H
