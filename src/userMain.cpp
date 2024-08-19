//
// Created by sybilvane on 8/12/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/LinkedList.hpp"
#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
#include "../lib/console.h"
#include "../lib/printing.hpp"

void testBody(void* arg) {
    int lim = *(int*)arg;
    for (int i = 0; i < lim; i++){
        printInt(i);
    }
    printString("testBody has finished execution.\n");
}

void userMain(){
    // C API test
//    thread_t* t = nullptr;
//    int x = 5;
//    thread_create(t, testBody, (void*)&x);
    // Thread tests

    struct Auto {
        int god;
        float cena;

        Auto(int god, float cena){
            this->god = god;
            this->cena = cena;
        }
    };

    Auto* a = new Auto(10, 50.5);
    printString("AUTO: ");
    printInt((uint64)a);
    a->cena = 100;
    delete a;


    // if all went well:
    __putc('-');
    __putc('-');
    __putc('\n');
    __putc('o');
    __putc('k');
    __putc('\n');
    __putc('-');
    __putc('-');
    __putc('\n');
}