//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_RISCV_HPP
#define INC_41F_OS_PROJEKAT_RISCV_HPP

#include "../lib/hw.h"

class riscv {
public:

    static uint64 r_scause();
    static void w_scause(uint64 scause);
    static uint64 r_sepc();
    static void w_sepc(uint64 sepc);
    static uint64 r_stvec();
    static void w_stvec(uint64 stvec);
    static uint64 r_stval();
    static void w_stval(uint64 stval);

private:
    static void handleSupervisorTrap();
};


#endif //INC_41F_OS_PROJEKAT_RISCV_HPP
