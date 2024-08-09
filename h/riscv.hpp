//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_RISCV_HPP
#define INC_41F_OS_PROJEKAT_RISCV_HPP

#include "../lib/hw.h"
#include "../lib/console.h"
#include "MemoryAllocator.hpp"

class riscv {
public:

    static void close_riscv_emulation();

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

    static void supervisorTrap();

    static void setMode(bool mode);
private:
    const static uint64 SUP_SOFT_INT = 0x8000000000000001UL;
    const static uint64 SUP_EXT_INT = 0x8000000000000009UL;
    const static uint64 ILLEGAL_INSTR = 0x2UL;
    const static uint64 UNAUTH_READ = 0x5UL;
    const static uint64 UNAUTH_WRITE = 0x7UL;
    const static uint64 ECALL_USER_MODE = 0x8UL;
    const static uint64 ECALL_KERNEL_MODE = 0x9UL;

    const static uint64 MEM_ALLOC = 0x01;
    const static uint64 MEM_FREE = 0x02;
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

    const static uint32 KEYBOARD_INT_NO = 0x0a;

    static uint64 retrieve_param(uint8 offset);
    static void handleSupervisorTrap();

    static bool userMode;
};


#endif //INC_41F_OS_PROJEKAT_RISCV_HPP
