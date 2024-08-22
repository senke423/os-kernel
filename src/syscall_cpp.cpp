//
// Created by sybilvane on 7/29/24.
//

#include "../h/syscall_cpp.hpp"

void* operator new(size_t size){
    return mem_alloc(size);
}

void* operator new[](size_t size){
    return mem_alloc(size);
}

void operator delete(void* alloc_space) noexcept {
    mem_free(alloc_space);
}

void operator delete[](void* alloc_space) noexcept {
    mem_free(alloc_space);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->myHandle = nullptr;
    this->body = body;
    this->arg = arg;
}

Thread::~Thread() {

}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::start() {
    if (myHandle != nullptr)
        return -1;

    return thread_create(&myHandle, body, arg);
}

Thread::Thread() {

}

int Thread::sleep(time_t) {
    return 0;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t time) {
    return sem_timedwait(myHandle, time);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

char Console::getc() {
    return __getc();
}

void Console::putc(char c) {
    __putc(c);
}
