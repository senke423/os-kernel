//
// Created by sybilvane on 7/29/24.
//

#include "../h/riscv.hpp"

bool riscv::userMode = false;

void riscv::handleSupervisorTrap() {

    uint64 scause = r_scause();

    if (scause == ILLEGAL_INSTR){

    }
    else if (scause == UNAUTH_READ){

    }
    else if (scause == UNAUTH_WRITE){

    }
    else if (scause == ECALL_USER_MODE || scause == ECALL_KERNEL_MODE){
        uint64 sepc = r_sepc() + 4; // +4 so that we return to the address behind the "ecall" address
        uint64 sstatus = r_sstatus();

        uint64 a0 = retrieve_param(0); // code
        uint64 a1 = retrieve_param(1); // 1st param
        uint64 a2 = retrieve_param(2); // 2nd param
        uint64 a3 = retrieve_param(3); // 3rd param
        uint64 a4 = retrieve_param(4); // 4th param

        volatile void* ptr;
        char c;
        int ret;
        mySemaphore* handle;

        switch(a0){
            case MEM_ALLOC:
                a1 *= MEM_BLOCK_SIZE; // convert to bytes
                ptr = MemoryAllocator::mem_alloc(a1);
                set_a0((uint64)ptr);
                break;
            case MEM_FREE:
                ret = MemoryAllocator::mem_free((void*)a1);
                set_a0((uint64)ret);
                break;
            case THREAD_CREATE:
                // a1 -> handle
                // a2 -> start_routine
                // a3 -> arg
                // a4 -> stack space

                if ((TCB::subroutine) a2 == nullptr){
                    ret = -1;
                    set_a0(ret);
                } else {
                    ret = (uint64)TCB::createThread((TCB**)a1, (TCB::subroutine)a2, (void*)a3, (void*)a4);
                    set_a0(ret);
                }

                break;
            case THREAD_EXIT:
                ret = TCB::exit();
                set_a0(ret);
                TCB::dispatch();
                break;
            case THREAD_DISPATCH:
                TCB::dispatch();
                break;
            case SEM_OPEN:
                ret = mySemaphore::open((mySemaphore**)a1, a2);
                set_a0(ret);
                break;
            case SEM_CLOSE:
                handle = (mySemaphore*)a1;
                ret = handle->close();
                set_a0(ret);
                break;
            case SEM_WAIT:
                handle = (mySemaphore*)a1;
                ret = handle->wait();
                set_a0(ret);
                break;
            case SEM_SIGNAL:
                handle = (mySemaphore*)a1;
                ret = handle->signal();
                set_a0(ret);
                break;
            case SEM_TIMEDWAIT:
                handle = (mySemaphore*)a1;
                ret = handle->timed_wait((time_t)a2);
                set_a0(ret);
                break;
            case SEM_TRYWAIT:
                handle = (mySemaphore*)a1;
                ret = handle->try_wait();
                set_a0(ret);
                break;
            case TIME_SLEEP:
                Scheduler::putSleeping(TCB::running, (size_t)a1);
                TCB::dispatch();
                set_a0(0);
                break;
            case GETC:
                c = myConsole::get_out();
                set_a0(c);
                break;
            case PUTC:
                myConsole::put_out(a1);
                myConsole::handler();
                break;
            default:
                // unknown code
//                __putc('E');
//                __putc('R');
//                __putc('R');
                break;
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
        // unexpected trap cause
    }
}

uint64 riscv::retrieve_param(uint8 index) {
    uint64 volatile param;
    uint64 volatile offset = index*8 + 80;
    __asm__ volatile ("add t0, %[offset], s0" : : [offset]"r"(offset) : "t0");
    __asm__ volatile ("ld %0, 0(t0)" : "=r"(param));
    return param;
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

void riscv::set_a0(uint64 val) {
    __asm__ volatile ("mv a0, %[val]" : : [val] "r"(val));
}