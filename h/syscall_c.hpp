//
// Created by sybilvane on 7/29/24.
//

#ifndef INC_41F_OS_PROJEKAT_SYSCALL_C_HPP
#define INC_41F_OS_PROJEKAT_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "TCB.hpp"
#include "mySemaphore.hpp"

const static int SEMDEAD = -1;
const static int TIMEOUT = -2;
const static int NULL_PTR_ERR = -3;

void* mem_alloc(size_t size);
int mem_free(void* alloc_space);

class TCB;
typedef TCB* thread_t;
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

class mySemaphore;
typedef mySemaphore* sem_t;
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);

typedef unsigned long time_t;
int time_sleep(time_t period);

const int EOF = -1;
char getc();
void putc(char character);

#endif //INC_41F_OS_PROJEKAT_SYSCALL_C_HPP
