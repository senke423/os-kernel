//
// Created by sybilvane on 8/12/24.
//

#include "../h/myConsole.hpp"

charBuffer* myConsole::in = nullptr;
charBuffer* myConsole::out = nullptr;

void myConsole::console_init() {
    in = new charBuffer(BUFF_CAP);
    out = new charBuffer(BUFF_CAP);
}

void myConsole::put_in(char c) {
    in->append(c);
}

char myConsole::get_in() {
    return in->take();
}

void myConsole::put_out(char c) {
    out->append(c);
}

char myConsole::get_out() {
    return out->take();
}

void myConsole::console_finalize() {
    delete in;
    delete out;
}

void myConsole::handler() {
    // this probabably needs some revision...
    while (*(char *) CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT){
        myConsole::put_in(*((char*)CONSOLE_RX_DATA));
    }
    while (*(char *) CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT){
        *((char*)CONSOLE_TX_DATA) = myConsole::get_out();
    }
}
