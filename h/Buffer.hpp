//
// Created by os on 6/24/26.
//

#ifndef PROJECT_BASE_V1_1_BUFFER_HPP
#define PROJECT_BASE_V1_1_BUFFER_HPP

#include "kSemaphore.hpp"
#include "kMemAlloc.hpp"
#include "riscv.hpp"
#include "../lib/hw.h"

class BoundedBuffer {
public:
    explicit BoundedBuffer(int capacity = 256);
    ~BoundedBuffer();

    void append(char value);
    char take();

    int appendTry(char value);
    int takeTry(char& value);

    void* operator new(size_t size) {
        return MemAlloc::malloc(size);
    }

    void operator delete(void* ptr) {
        MemAlloc::free(ptr);
    }
private:
    uint64 lock();
    void unlock(uint64 sstatus);

    kSemaphore spaceAvailable;
    kSemaphore itemAvailable;
    char* buffer;
    int capacity;
    int count;
    int head, tail;
};

#endif //PROJECT_BASE_V1_1_BUFFER_HPP
