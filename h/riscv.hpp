//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_RISCV_HPP
#define INC_41F_OS_PROJEKAT_RISCV_HPP

#include "../lib/hw.h"
#include "../lib/console.h"

class riscv {
public:

    static void close_riscv_emulation();

    // pops sstatus.spp and sstatus.spie bits
    static void popSppSpie();

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
    const static uint64 ILLEGAL_INSTR = 0x0000000000000002UL;
    const static uint64 UNAUTH_READ = 0x0000000000000005UL;
    const static uint64 UNAUTH_WRITE = 0x0000000000000007UL;
    const static uint64 ECALL_USER_MODE = 0x0000000000000008UL;
    const static uint64 ECALL_KERNEL_MODE = 0x0000000000000009UL;

    const static uint32 KEYBOARD_INT_NO = 0x0a;

    static void handleSupervisorTrap();

    static bool userMode;
};


#endif //INC_41F_OS_PROJEKAT_RISCV_HPP
