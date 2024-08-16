//
// Created by sybilvane on 8/12/24.
//

#ifndef INC_41F_OS_PROJEKAT_MYCONSOLE_HPP
#define INC_41F_OS_PROJEKAT_MYCONSOLE_HPP

#include "charBuffer.hpp"

class myConsole {
public:
    myConsole(const myConsole& console) = delete;
    myConsole& operator=(const myConsole& console) = delete;

    static void put_in(char c);
    static char get_in();
    static void put_out(char c);
    static char get_out();

    static void console_init();
    static void console_finalize();
    static void handler();

    static const size_t BUFF_CAP = 256;
private:
    myConsole(){}

    static charBuffer* in; // stdin
    static charBuffer* out; // stdout
};


#endif //INC_41F_OS_PROJEKAT_MYCONSOLE_HPP
