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
        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();

        uint64 a0 = retrieve_param(0); // code
        uint64 a1 = retrieve_param(1);
//        uint64 a2 = retrieve_param(2);
//        uint64 a3 = retrieve_param(3);
//        uint64 a4 = retrieve_param(4);
        switch(a0){
            case MEM_ALLOC:
                MemoryAllocator::mem_alloc(a1);
                w_sepc(sepc);
                w_sstatus(sstatus);
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

inline uint64 riscv::r_scause() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], scause" : [ret] "=r"(ret));
    return ret;
}

inline void riscv::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 riscv::r_sepc() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], sepc" : [ret] "=r"(ret));
    return ret;
}

inline void riscv::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 riscv::r_stvec() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], stvec" : [ret] "=r"(ret));
    return ret;
}

inline void riscv::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 riscv::r_stval() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], stval" : [ret] "=r"(ret));
    return ret;
}

inline void riscv::w_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline uint64 riscv::r_sstatus() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], sstatus" : [ret] "=r"(ret));
    return ret;
}

inline void riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline uint64 riscv::r_sip() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], sip" : [ret] "=r"(ret));
    return ret;
}

inline void riscv::w_sip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void riscv::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void riscv::mc_sip(uint64 mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline void riscv::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void riscv::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
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

inline uint64 riscv::r_sscratch() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], sscratch" : [ret] "=r"(ret));
    return ret;
}

inline void riscv::w_sscratch(uint64 sscratch) {
    __asm__ volatile ("csrw sscratch, %[sscratch]" : : [sscratch] "r"(sscratch));
}

inline uint64 riscv::retrieve_param(uint8 index) {
    uint64 volatile param;
    uint64 volatile offset = index*8 + 80;
    __asm__ volatile ("add t0, %[offset], s0" : : [offset]"r"(offset) : "t0");
    __asm__ volatile ("ld %0, 0(t0)" : "=r"(param));
    return param;
}
