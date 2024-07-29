//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_SYSCALL_C_HPP
#define INC_41F_OS_PROJEKAT_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "tcb.hpp"
#include "sem.hpp"

void* mem_alloc(size_t size);
int mem_free(void*);

typedef tcb* thread_t;
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

typedef sem* sem_t;
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);

typedef unsigned long time_t;
int time_sleep(time_t);

const int EOF = -1;
char getc();
void putc(char character);

#endif //INC_41F_OS_PROJEKAT_SYSCALL_C_HPP
