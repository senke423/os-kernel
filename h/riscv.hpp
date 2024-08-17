//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_RISCV_HPP
#define INC_41F_OS_PROJEKAT_RISCV_HPP

#include "../lib/hw.h"
#include "../lib/console.h"
#include "MemoryAllocator.hpp"
#include "TCB.hpp"
#include "myConsole.hpp"
#include "../lib/printing.hpp"

extern "C" void pushRegisters();
extern "C" void popRegisters();

class riscv {
public:

    static void close_riscv_emulation();

    // defined in registersUtil.asm
    static void pushRegisters();
    static void popRegisters();

    // pops sstatus.spp and sstatus.spie bits
    static void pop_spp_spie();

    static uint64 r_scause();
    static void w_scause(uint64 scause);
    static uint64 r_sepc();
    static void w_sepc(uint64 sepc);
    static uint64 r_stvec();
    static void w_stvec(uint64 stvec);
    static uint64 r_stval();
    static void w_stval(uint64 stval);
    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);
    static uint64 r_sscratch();
    static void w_sscratch(uint64 sscratch);
    static uint64 r_sip();
    static void w_sip(uint64 sip);

    enum BitMaskSip {
        SIP_SSIP = (1 << 1), // software interrupt enable bit
        SIP_STIP = (1 << 5), // supervisor trap enable bit
        SIP_SEIP = (1 << 9), // supervisor external interrupt enable bit
    };

    static void ms_sip(uint64 mask);
    static void mc_sip(uint64 mask);

    enum BitMaskSStatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    static void ms_sstatus(uint64 mask);
    static void mc_sstatus(uint64 mask);

    static void setMode(bool mode);
private:
    const static uint64 SUP_SOFT_INT = 0x8000000000000001UL;
    const static uint64 SUP_EXT_INT = 0x8000000000000009UL;
    const static uint64 ILLEGAL_INSTR = 0x2UL;
    const static uint64 UNAUTH_READ = 0x5UL;
    const static uint64 UNAUTH_WRITE = 0x7UL;
    const static uint64 ECALL_USER_MODE = 0x8UL;
    const static uint64 ECALL_KERNEL_MODE = 0x9UL;

    const static uint64 MEM_ALLOC = 0x1;
    const static uint64 MEM_FREE = 0x2;
    const static uint64 THREAD_CREATE = 0x11;
    const static uint64 THREAD_EXIT = 0x12;
    const static uint64 THREAD_DISPATCH = 0x13;
    const static uint64 SEM_OPEN = 0x21;
    const static uint64 SEM_CLOSE = 0x22;
    const static uint64 SEM_WAIT = 0x23;
    const static uint64 SEM_SIGNAL = 0x24;
    const static uint64 SEM_TIMEDWAIT = 0x25;
    const static uint64 SEM_TRYWAIT = 0x26;
    const static uint64 TIME_SLEEP = 0x31;
    const static uint64 GETC = 0x41;
    const static uint64 PUTC = 0x42;

    const static uint32 KEYBOARD_INT_NO = 0xa;

    static uint64 retrieve_param(uint8 offset);
    static void set_a0(uint64 val);
    static void handleSupervisorTrap();

    static bool userMode;
};

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

inline uint64 riscv::r_sscratch() {
    uint64 volatile ret;
    __asm__ volatile ("csrr %[ret], sscratch" : [ret] "=r"(ret));
    return ret;
}

inline void riscv::w_sscratch(uint64 sscratch) {
    __asm__ volatile ("csrw sscratch, %[sscratch]" : : [sscratch] "r"(sscratch));
}

inline uint64 riscv::retrieve_param(volatile uint8 index) {
    uint64 volatile param;
    uint64 volatile offset = index*8 + 80;
    __asm__ volatile ("add t0, %[offset], s0" : : [offset]"r"(offset) : "t0");
    __asm__ volatile ("ld %0, 0(t0)" : "=r"(param));
    return param;
}

#endif //INC_41F_OS_PROJEKAT_RISCV_HPP
