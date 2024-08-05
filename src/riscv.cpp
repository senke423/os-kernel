//
// Created by sybilvane on 7/29/24.
//

#include "../h/riscv.hpp"

void riscv::handleSupervisorTrap() {
//    uint64 scause;
}

uint64 riscv::r_scause() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], scause" : [ret] "=r"(ret));
    return ret;
}

void riscv::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

uint64 riscv::r_sepc() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], sepc" : [ret] "=r"(ret));
    return ret;
}

void riscv::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

uint64 riscv::r_stvec() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], stvec" : [ret] "=r"(ret));
    return ret;
}

void riscv::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

uint64 riscv::r_stval() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], stval" : [ret] "=r"(ret));
    return ret;
}

void riscv::w_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}
