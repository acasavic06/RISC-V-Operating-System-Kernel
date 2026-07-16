#ifndef PROJECT_BASE_V1_1_KMEMALLOC_HPP
#define PROJECT_BASE_V1_1_KMEMALLOC_HPP

#include "../lib/hw.h"

class MemAlloc {
public:
    struct FreeMem {
        FreeMem* next;
        FreeMem* prev;
        size_t size;
    };

    MemAlloc(MemAlloc &) = delete;
    void operator=(const MemAlloc&) = delete;

    static void init();
    static void* malloc (size_t size);
    static int free (void* adr);
    static void printFreeMem();

    //moglo je i u private realno
    static FreeMem* fmem_head;

private:
    MemAlloc(){}
    static void tryToJoin(FreeMem* cur);
};


#endif //PROJECT_BASE_V1_1_KMEMALLOC_HPP
