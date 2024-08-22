//
// Created by sybilvane on 8/15/24.
//

#include "../h/charBuffer.hpp"

charBuffer::charBuffer(size_t capacity) : capacity(capacity), len(0), head(0), tail(0) {
    buffer = new char[capacity];

    mySemaphore::open(&space_avail, capacity);
    mySemaphore::open(&item_avail, 0);
    mySemaphore::open(&mutex, 1);

}

void charBuffer::append(char c) {
    space_avail->wait();
    mutex->wait();
    buffer[tail] = c;
    tail = (tail + 1) % capacity;
    len++;
    mutex->signal();
    item_avail->signal();
}

char charBuffer::take() {
    item_avail->wait();
    mutex->wait();
    char ret = buffer[head];
    head = (head + 1) % capacity;
    len--;
    mutex->signal();
    space_avail->signal();

    return ret;
}

charBuffer::~charBuffer() {
    space_avail->close();
    item_avail->close();
    mutex->close();

    delete space_avail;
    delete item_avail;
    delete mutex;

    delete buffer;
}

size_t charBuffer::getLen() {
    return len;
}
