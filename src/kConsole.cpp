//
// Created by os on 6/23/26.
//

#include "../h/kConsole.hpp"

BoundedBuffer* kConsole::inputBuffer = nullptr;
BoundedBuffer* kConsole::outputBuffer = nullptr;

void kConsole::init() {
    inputBuffer = new BoundedBuffer(CAP);
    outputBuffer = new BoundedBuffer(CAP);
}

char kConsole::getc() {
    return inputBuffer->take();
}

void kConsole::putc(char c) {
    //if (c == 13) c = '\n';
    outputBuffer->append(c);
    drainOutput();
}

void kConsole::drainOutput() {
    volatile char* status = (char*)CONSOLE_STATUS;
    volatile char* tx = (char*)CONSOLE_TX_DATA;

    while ((*status & CONSOLE_TX_STATUS_BIT) != 0) {
        char c;
        if (outputBuffer->takeTry(c) < 0) break;
        *tx = c;
    }
}

void kConsole::handleInterrupt() {
    int irq = plic_claim();

    if (irq == CONSOLE_IRQ) {
        volatile char* status = (char*)CONSOLE_STATUS;
        volatile char* rx = (char*)CONSOLE_RX_DATA;
        volatile char* tx = (char*)CONSOLE_TX_DATA;

        while ((*status & CONSOLE_RX_STATUS_BIT) != 0) {
            char c = *rx;
            if (inputBuffer->appendTry(c) < 0) break;
        }

        while ((*status & CONSOLE_TX_STATUS_BIT) != 0) {
            char c;
            if (outputBuffer->takeTry(c) < 0) break;
            *tx = c;
        }
    }

    if (irq) {
        plic_complete(irq);
    }
}
