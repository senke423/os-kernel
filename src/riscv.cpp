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

void riscv::w_sscratch(uint64 sscratch) {
    __asm__ volatile ("csrw sscratch, %[sscratch]" : : [sscratch] "r"(sscratch));
}
