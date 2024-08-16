//
// Created by sybilvane on 8/15/24.
//

#ifndef INC_41F_OS_PROJEKAT_CHARBUFFER_HPP
#define INC_41F_OS_PROJEKAT_CHARBUFFER_HPP

#include "mySemaphore.hpp"
#include "../lib/hw.h"
typedef mySemaphore* sem_t;

class charBuffer {
public:
    explicit charBuffer(size_t capacity);
    ~charBuffer();

    void append(char c);
    char take();
    size_t getLen();
private:

    size_t capacity;
    size_t len;
    sem_t space_avail;
    sem_t item_avail;
    sem_t mutex;

    char* buffer;
    size_t head, tail;
};


#endif //INC_41F_OS_PROJEKAT_CHARBUFFER_HPP
