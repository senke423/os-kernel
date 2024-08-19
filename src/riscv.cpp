//
// Created by sybilvane on 7/29/24.
//

#include "../h/riscv.hpp"
#include "../lib/console.h"

bool riscv::userMode = false;

void riscv::handleSupervisorTrap() {

    uint64 volatile a0; // code
    __asm__ volatile ("mv %0, a0" : "=r"(a0));
    uint64 volatile a1; // 1st param
    __asm__ volatile ("lw %0, 88(fp)" : "=r"(a1));
    uint64 volatile a2; // 2nd param
    __asm__ volatile ("lw %0, 96(fp)" : "=r"(a2));
    uint64 volatile a3; // 3rd param
    __asm__ volatile ("lw %0, 104(fp)" : "=r"(a3));
    uint64 volatile a4; // 4th param
    __asm__ volatile ("lw %0, 112(fp)" : "=r"(a4));

    uint64 scause = r_scause();
    uint64 stvec = r_stvec();
    uint64 sepc = r_sepc();

    printString("\nPrekidna rutina\n");
    printInt(scause, 10, 0);

    if (scause == ILLEGAL_INSTR){
        printString("UNAUTHORIZED READ!\nstvec: ");
        printInt(stvec, 10, 0);
        printString("sepc: ");
        printInt(sepc, 10, 0);

        riscv::close_riscv_emulation();
    }
    else if (scause == UNAUTH_READ){
        printString("UNAUTHORIZED READ!\nstvec: ");
        printInt(stvec, 10, 0);
        printString("sepc: ");
        printInt(sepc, 10, 0);

        riscv::close_riscv_emulation();
    }
    else if (scause == UNAUTH_WRITE){
        printString("UNAUTHORIZED WRITE!\nstvec: ");
        printInt(stvec, 10, 0);
        printString("sepc: ");
        printInt(sepc, 10, 0);

        riscv::close_riscv_emulation();
    }
    else if (scause == ECALL_USER_MODE || scause == ECALL_KERNEL_MODE){
        printString("ECALL SUCCESS\n");

        uint64 sepc = r_sepc() + 4; // +4 so that we return to the address behind the "ecall" address
        uint64 sstatus = r_sstatus();


        printString("a0 code: ");
        printInt(a0, 16, 0);
        __putc('\n');

        if (a0 == MEM_ALLOC){
            a1 *= MEM_BLOCK_SIZE; // convert to bytes
            printString("How many bytes, converted? ");
            void* ptr = MemoryAllocator::mem_alloc(a1);
            __asm__ volatile ("mv t0, %0" : : "r"(ptr));
            __asm__ volatile ("sw t0, 80(fp)");
        }
        else if (a0 == MEM_FREE){
            printInt(a1);
            int ret = MemoryAllocator::mem_free((void*)a1);
            __asm__ volatile ("mv t0, %0" : : "r"(ret));
            __asm__ volatile ("sw t0, 80(fp)");
        }
        else if (a0 == THREAD_CREATE){
            // a1 -> handle
            // a2 -> start_routine
            // a3 -> arg
            // a4 -> stack space

            if ((TCB::subroutine) a2 == nullptr){
                int ret = -1;
                set_a0(ret);
            } else {
                int ret = TCB::createThread((TCB*)a1, (TCB::subroutine)a2, (void*)a3, (void*)a4);
                set_a0(ret);
            }
        }
        else if (a0 == THREAD_EXIT){
            int ret = TCB::exit();
            set_a0(ret);
            TCB::dispatch();
        }
        else if (a0 == THREAD_DISPATCH){
            TCB::dispatch();
        }
        else if (a0 == SEM_OPEN){
            int ret = mySemaphore::open((mySemaphore**)a1, a2);
            set_a0(ret);
        }
        else if (a0 == SEM_CLOSE){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->close();
            set_a0(ret);
        }
        else if (a0 == SEM_WAIT){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->wait();
            set_a0(ret);
        }
        else if (a0 == SEM_SIGNAL){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->signal();
            set_a0(ret);
        }
        else if (a0 == SEM_TIMEDWAIT){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->timed_wait((time_t)a2);
            set_a0(ret);
        }
        else if (a0 == SEM_TRYWAIT){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->try_wait();
            set_a0(ret);
        }
        else if (a0 == TIME_SLEEP){
            Scheduler::putSleeping(TCB::running, (size_t)a1);
            TCB::dispatch();
            set_a0(0);
        }
        else if (a0 == GETC){
            char c = myConsole::get_out();
            set_a0(c);
        }
        else if (a0 == PUTC){
            myConsole::put_out(a1);
            myConsole::handler();
        }
        else {
            // unknown code
            printString("UNKNOWN CODE!\n");
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == SUP_SOFT_INT){
        // interrupt: yes, supervisor software interrupt (timer)
        // timer frequency: 10 Hz
        TCB::timeSliceCnt++;
        Scheduler::update();

        if (TCB::timeSliceCnt >= TCB::running->getTimeSlice()){
            uint64 sepc = r_sepc();
            uint64 sstatus = r_sstatus();
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }

        mc_sip(SIP_SSIP);
    }
    else if (scause == SUP_EXT_INT){
        // interrupt: yes, supervisor external interrupt (console)
        int ret = plic_claim();
        if (ret == KEYBOARD_INT_NO){
            myConsole::handler();
            plic_complete(KEYBOARD_INT_NO);
        }
    }
    else {
        __putc('C');
        // unexpected trap cause
    }
}

void riscv::pop_spp_spie() {
    if (userMode){
        mc_sstatus(SSTATUS_SPP);
    } else {
        ms_sstatus(SSTATUS_SPP);
    }
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void riscv::setMode(bool mode) {
    userMode = mode;
}

void riscv::close_riscv_emulation() {
    uint32 close_code = 0x5555;
    __asm__ volatile ("addi t0, %[close_code], 0" : : [close_code] "r"(close_code));
    __asm__ volatile ("li t1, 0x100000");
    __asm__ volatile ("sw t0, 0(t1)");
}

void riscv::set_a0(volatile uint64 val) {
    __asm__ volatile ("mv a0, %[val]" : : [val] "r"(val));
}
