//
// Created by os on 5/17/26.
//

#ifndef PROJECT_BASE_V1_1_KCONSOLE_HPP
#define PROJECT_BASE_V1_1_KCONSOLE_HPP

#include "../lib/hw.h"
#include "Buffer.hpp"

class kConsole {
public:
    static void init();
    static void handleInterrupt();

    static char getc();
    static void putc(char c);

private:
    static const int CAP = 256;

    static BoundedBuffer* inputBuffer;
    static BoundedBuffer* outputBuffer;

    static void drainOutput();
};

#endif
