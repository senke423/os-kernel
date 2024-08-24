//
// Created by sybilvane on 7/29/24.
//

#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"


bool riscv::userMode = false;

void riscv::handleSupervisorTrap() {

    uint64 scause = r_scause();
    uint64 stvec = r_stvec();
    uint64 sepc = r_sepc();

    if (scause == ECALL_USER_MODE || scause == ECALL_KERNEL_MODE){
        uint64 volatile a0; // code
        __asm__ volatile ("ld %0, 80(fp)" : "=r"(a0));
        uint64 volatile a1; // 1st param
        __asm__ volatile ("ld %0, 88(fp)" : "=r"(a1));
        uint64 volatile a2; // 2nd param
        __asm__ volatile ("ld %0, 96(fp)" : "=r"(a2));
        uint64 volatile a3; // 3rd param
        __asm__ volatile ("ld %0, 104(fp)" : "=r"(a3));
        uint64 volatile a4; // 4th param
        __asm__ volatile ("ld %0, 112(fp)" : "=r"(a4));

        uint64 sepc = r_sepc() + 4; // +4 so that we return to the address behind the "ecall" address
        uint64 sstatus = r_sstatus();


        if (a0 == MEM_ALLOC){
            a1 *= MEM_BLOCK_SIZE; // convert to bytes
            MemoryAllocator::mem_alloc(a1);
            __asm__ volatile ("sd a0, 80(fp)");
        }
        else if (a0 == MEM_FREE){
            int ret = MemoryAllocator::mem_free((void*)a1);
            __asm__ volatile ("mv t0, %0" : : "r"(ret));
            __asm__ volatile ("sd t0, 80(fp)");
        }
        else if (a0 == THREAD_CREATE){
            // a1 -> handle
            // a2 -> start_routine
            // a3 -> arg
            // a4 -> stack space

            TCB::createThread((TCB**)a1, (TCB::subroutine)a2, (void*)a3, (uint64*)a4);
            __asm__ volatile ("sd a0, 80(fp)");

        }
        else if (a0 == THREAD_EXIT){
            int ret = TCB::exit();
            __asm__ volatile ("mv t0, %0" : : "r"(ret));
            __asm__ volatile ("sd t0, 80(fp)");
            TCB::dispatch();
        }
        else if (a0 == THREAD_DISPATCH){
            TCB::dispatch();
        }
        else if (a0 == SEM_OPEN){
            int ret = mySemaphore::open((mySemaphore**)a1, a2);
            __asm__ volatile ("mv t0, %0" : : "r"(ret));
            __asm__ volatile ("sd t0, 80(fp)");
        }
        else if (a0 == SEM_CLOSE){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->close();
            __asm__ volatile ("mv t0, %0" : : "r"(ret));
            __asm__ volatile ("sd t0, 80(fp)");
        }
        else if (a0 == SEM_WAIT){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->wait();
            __asm__ volatile ("mv t0, %0" : : "r"(ret));
            __asm__ volatile ("sd t0, 80(fp)");
        }
        else if (a0 == SEM_SIGNAL){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->signal();
            __asm__ volatile ("mv t0, %0" : : "r"(ret));
            __asm__ volatile ("sd t0, 80(fp)");
        }
        else if (a0 == SEM_TRYWAIT){
            mySemaphore* handle = (mySemaphore*)a1;
            int ret = handle->try_wait();
            __asm__ volatile ("mv t0, %0" : : "r"(ret));
            __asm__ volatile ("sd t0, 80(fp)");
        }
        else if (a0 == GETC){
            char c = __getc();
            __asm__ volatile ("mv t0, %0" : : "r"(c));
            __asm__ volatile ("sd t0, 80(fp)");
        }
        else if (a0 == PUTC){
            __putc(a1);
        }
        else {
            // unknown code
            close_riscv_emulation();
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == ILLEGAL_INSTR){
        printString("UNAUTHORIZED READ!\nstvec: ");
        printInt(stvec, 16, 0);
        printString("sepc: ");
        printInt(sepc, 16, 0);

        riscv::close_riscv_emulation();
    }
    else if (scause == UNAUTH_READ){
        printString("UNAUTHORIZED READ!\nstvec: ");
        printInt(stvec, 16, 0);
        printString("sepc: ");
        printInt(sepc, 16, 0);

        riscv::close_riscv_emulation();
    }
    else if (scause == UNAUTH_WRITE){
        printString("UNAUTHORIZED WRITE!\nstvec: ");
        printInt(stvec, 16, 0);
        printString("sepc: ");
        printInt(sepc, 16, 0);

        riscv::close_riscv_emulation();
    }
    else if (scause == SUP_EXT_INT){
        printString("SUP EXT INT!\n");
        console_handler();
    }
    else {
        printString("\nInstruction access fault.\n");
        printString("sepc value: ");
        printInt((uint64)sepc, 16, 0);
        // unexpected trap cause
        close_riscv_emulation();
    }
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
