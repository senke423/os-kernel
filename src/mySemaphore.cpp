//
// Created by sybilvane on 7/29/24.
//

#include "../h/mySemaphore.hpp"

int mySemaphore::open(mySemaphore **handle, unsigned int init) {
    *handle = new mySemaphore(init);
    if (!*handle)
        return -1;
    return 0;
}

int mySemaphore::close() {
    if (!is_open)
        return SEM_CLOSE_ERR;
    // implement thread logic

    return 0;
}

int mySemaphore::wait() {
    if (val > 0){
        --val;
    } else {
        // BLOCK THREAD UNTIL VAL IS > 0
    }
    return 0;
}

int mySemaphore::signal() {
    ++val;
    return 0;
}
