//
// Created by sybilvane on 7/29/24.
//

#include "../h/riscv.hpp"
#include "../lib/printing.hpp"

bool riscv::userMode = false;

void riscv::handleSupervisorTrap() {

    printInt(80, 10, 0);

    uint64 scause = r_scause();

    if (scause == ILLEGAL_INSTR){

    }
    else if (scause == UNAUTH_READ){

    }
    else if (scause == UNAUTH_WRITE){

    }
    else if (scause == ECALL_USER_MODE || scause == ECALL_KERNEL_MODE){
        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();

        uint64 a0 = retrieve_param(0); // code
        uint64 a1 = retrieve_param(1);
//        uint64 a2 = retrieve_param(2);
//        uint64 a3 = retrieve_param(3);
//        uint64 a4 = retrieve_param(4);
        switch(a0){
            case MEM_ALLOC:
                a1 *= MEM_BLOCK_SIZE; // convert to bytes
                MemoryAllocator::mem_alloc(a1);
                break;
            case MEM_FREE:
                break;
            case THREAD_CREATE:
                break;
            case THREAD_EXIT:
                break;
            case THREAD_DISPATCH:
                break;
            case SEM_OPEN:
                break;
            case SEM_CLOSE:
                break;
            case SEM_WAIT:
                break;
            case SEM_SIGNAL:
                break;
            case SEM_TIMEDWAIT:
                break;
            case SEM_TRYWAIT:
                break;
            case TIME_SLEEP:
                break;
            case GETC:
                break;
            case PUTC:
                break;
            default:
                // unknown code
                __putc('E');
                __putc('R');
                __putc('R');
                break;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == SUP_SOFT_INT){
        // interrupt: yes, supervisor software interrupt (timer)
        // timer frequency: 10 Hz
        mc_sip(SIP_SSIP);
    }
    else if (scause == SUP_EXT_INT){
        // interrupt: yes, supervisor external interrupt (console)
        int ret = plic_claim();
        if (ret == KEYBOARD_INT_NO){

            // do stuff...

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