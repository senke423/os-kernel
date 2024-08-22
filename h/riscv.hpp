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

    static inline uint64 r_scause(){
        uint64 volatile ret;
        __asm__ volatile ("csrr %[ret], scause" : [ret] "=r"(ret));
        return ret;
    }
    static inline void w_scause(uint64 scause){
        __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
    }
    static inline uint64 r_sepc(){
        uint64 volatile ret;
        __asm__ volatile ("csrr %[ret], sepc" : [ret] "=r"(ret));
        return ret;
    }
    static inline void w_sepc(uint64 sepc){
        __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
    }
    static inline uint64 r_stvec(){
        uint64 volatile ret;
        __asm__ volatile ("csrr %[ret], stvec" : [ret] "=r"(ret));
        return ret;
    }
    static inline void w_stvec(uint64 stvec){
        __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
    }
    static inline uint64 r_stval(){
        uint64 volatile ret;
        __asm__ volatile ("csrr %[ret], stval" : [ret] "=r"(ret));
        return ret;
    }
    static inline void w_stval(uint64 stval){
        __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
    }
    static inline uint64 r_sstatus(){
        uint64 volatile ret;
        __asm__ volatile ("csrr %[ret], sstatus" : [ret] "=r"(ret));
        return ret;
    }
    static inline void w_sstatus(uint64 sstatus){
        __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
    }
    static inline uint64 r_sscratch(){
        uint64 volatile ret;
        __asm__ volatile ("csrr %[ret], sscratch" : [ret] "=r"(ret));
        return ret;
    }
    static inline void w_sscratch(uint64 sscratch){
        __asm__ volatile ("csrw sscratch, %[sscratch]" : : [sscratch] "r"(sscratch));
    }
    static inline uint64 r_sip(){
        uint64 volatile ret;
        __asm__ volatile ("csrr %[ret], sip" : [ret] "=r"(ret));
        return ret;
    }
    static inline void w_sip(uint64 sip){
        __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
    }

    enum BitMaskSip {
        SIP_SSIP = (1 << 1), // software interrupt enable bit
        SIP_STIP = (1 << 5), // supervisor trap enable bit
        SIP_SEIP = (1 << 9), // supervisor external interrupt enable bit
    };

    static inline void ms_sip(uint64 mask){
        __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
    }
    static inline void mc_sip(uint64 mask){
        __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
    }

    enum BitMaskSStatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    static inline void ms_sstatus(uint64 mask){
        asm volatile("csrw sstatus, %0" : : "r" (mask));
    }

    static inline void mc_sstatus(uint64 mask){
        __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
    }

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
//    static void set_a0(uint64 val);
    static void handleSupervisorTrap();

    static bool userMode;
};

inline uint64 riscv::retrieve_param(volatile uint8 index) {
    uint64 volatile param;
    uint64 volatile offset = index*8 + 80;
    __asm__ volatile ("add t0, %[offset], s0" : : [offset]"r"(offset) : "t0");
    __asm__ volatile ("ld %0, 0(t0)" : "=r"(param));
    return param;
}

#endif //INC_41F_OS_PROJEKAT_RISCV_HPP
