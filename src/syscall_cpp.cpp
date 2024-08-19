//
// Created by sybilvane on 7/29/24.
//

#include "../h/syscall_cpp.hpp"

void* operator new(size_t size){
    printInt(size);
    return mem_alloc(size);
}

void* operator new[](size_t size){
    printInt(size);
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