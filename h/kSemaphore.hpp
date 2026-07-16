#ifndef PROJECT_BASE_V1_1_KSEMAPHORE_HPP
#define PROJECT_BASE_V1_1_KSEMAPHORE_HPP

#include "kThread.hpp"
#include "../lib/hw.h"
#include "kMemAlloc.hpp"
#include "list.hpp"

class kSemaphore {
public:
    kSemaphore(int value=1);
    ~kSemaphore() = default;

    int kSem_wait();
    int kSem_signal();
    int kSem_close();
    static kSemaphore* create(unsigned init);

    int kSem_wait_N_time(unsigned n);
    int kSem_signal_N_time(unsigned n);

    void* operator new(size_t size) {
        return MemAlloc::malloc(size);
    }

    void operator delete(void* ptr) {
        MemAlloc::free(ptr);
    }


    unsigned getValue() const { return value; }

protected:
    void block();
    void unblock();

private:
    int value;
    bool isClosed;
    List<kThread> readyThreads;
    unsigned sem_needed = 0;
};

#endif
