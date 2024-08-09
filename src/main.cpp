//
// Created by sybilvane on 5/27/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"

void userMain(){
    int n = 10;
    char* niz = (char*)MemoryAllocator::mem_alloc(10*sizeof(char));
    if(niz == nullptr) {
        __putc('?');
    }

    for(int i = 0; i < n; i++) {
        niz[i] = 'k';
    }

    for(int i = 0; i < n; i++) {
        __putc(niz[i]);
        __putc(' ');
    }

    int status = MemoryAllocator::mem_free(niz);
    if(status != 0) {
        __putc('?');
    }

}

void main(){
    userMain();
    riscv::close_riscv_emulation();
}