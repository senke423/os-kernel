//
// Created by sybilvane on 7/29/24.
//

#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size){
    if (size == 0)
        return nullptr;

    // size argument should be converted to BLOCKS
    size_t no_of_blocks;

    // take ceil of quotient
    size % MEM_BLOCK_SIZE == 0 ? no_of_blocks = size / MEM_BLOCK_SIZE : no_of_blocks = size / MEM_BLOCK_SIZE + 1;

    uint64 code = 0x01;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[no_of_blocks]" : : [no_of_blocks] "r"(no_of_blocks));
    __asm__ volatile ("ecall");

    void* adr;
    __asm__ volatile ("mv %[adr], a0" : [adr] "=r"(adr));
    return adr;
}

int mem_free(void* alloc_space){
    if (alloc_space == nullptr)
        return NULL_PTR;

    uint64 code = 0x02;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[alloc_space]" : : [alloc_space] "r"(alloc_space));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    uint64 code = 0x11;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile ("mv a2, %[start_routine]" : : [start_routine] "r"(start_routine));
    __asm__ volatile ("mv a3, %[arg]" : : [arg] "r"(arg));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int thread_exit() {
    // in case it can't exit, return -1

    uint64 code = 0x12;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("ecall");
    return 0;
}

void thread_dispatch() {
    uint64 code = 0x13;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("ecall");
}

int sem_open(sem_t *handle, unsigned int init) {
    uint64 code = 0x21;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile ("mv a2, %[init]" : : [init] "r"(init));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int sem_close(sem_t handle) {
    uint64 code = 0x22;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int sem_wait(sem_t id) {
    if (id == nullptr)
        return -1;

    uint64 code = 0x23;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int sem_signal(sem_t id) {
    if (id == nullptr)
        return -1;

    uint64 code = 0x24;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int sem_timedwait(sem_t id, time_t timeout) {
    if (id == nullptr)
        return SEMDEAD;

    uint64 code = 0x25;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile ("mv a2, %[timeout]" : : [timeout] "r"(timeout));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int sem_trywait(sem_t id) {
    if (id == nullptr)
        return -1;

    uint64 code = 0x26;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int time_sleep(time_t period) {
    uint64 code = 0x31;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[period]" : : [period] "r"(period));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

char getc() {
    uint64 code = 0x41;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("ecall");

    // this should work; it's supposed to return the next char from the buffer
    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

void putc(char character) {
    uint64 code = 0x42;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r"(code));
    __asm__ volatile ("mv a1, %[character]" : : [character] "r"(character));
    __asm__ volatile ("ecall");
}
