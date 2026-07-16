#include "../h/kMemAlloc.hpp"
#include "../test/mojprint.hpp"

MemAlloc::FreeMem *MemAlloc::fmem_head = nullptr;

void MemAlloc::init() {
    fmem_head = (FreeMem*) HEAP_START_ADDR;
    if (fmem_head == nullptr) return;
    fmem_head->next = nullptr;
    fmem_head->prev = nullptr;
    fmem_head->size = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR;
}

void* MemAlloc::malloc(size_t size) {
    if (size<=0) return nullptr;
    //size_t blocksToAlloc = (size + sizeof(FreeMem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    size_t blocksToAlloc = size*MEM_BLOCK_SIZE + MEM_BLOCK_SIZE;

    for (FreeMem* cur = fmem_head; cur != nullptr; cur = cur->next) {
        if (cur->size == blocksToAlloc) {
            if (cur->prev) cur->prev->next = cur->next;
            else fmem_head = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            return (void*)((char*)cur+MEM_BLOCK_SIZE);
        }else if (cur->size > blocksToAlloc + MEM_BLOCK_SIZE) {
            FreeMem* newCur = (FreeMem*)((char*)cur+blocksToAlloc);
            if (cur->prev) cur->prev->next = newCur;
            else fmem_head = newCur;
            if (cur->next) cur->next->prev = newCur;

            newCur->next = cur->next;
            newCur->prev = cur->prev;
            newCur->size = cur->size - blocksToAlloc;
            cur->size = blocksToAlloc-MEM_BLOCK_SIZE;
            return (void*)((char*)cur+MEM_BLOCK_SIZE);
        }
    }
    return nullptr;
}

int MemAlloc::free(void* adr) {

    if (!adr || adr >= HEAP_END_ADDR || adr < HEAP_START_ADDR) return -1;

    FreeMem* cur = (FreeMem*)((char*)adr - MEM_BLOCK_SIZE);

    if (fmem_head == nullptr) {
        fmem_head = cur;
        fmem_head->next = nullptr;
        fmem_head->prev = nullptr;
    } else if (cur > fmem_head) {
        FreeMem* curFree = fmem_head;
        while (curFree->next && curFree->next < cur) curFree = curFree->next;
        cur->next = curFree->next;
        cur->prev = curFree;
        if (cur->next) cur->next->prev = cur;
        curFree->next = cur;
        tryToJoin(cur);
        tryToJoin(curFree);
    } else {
        cur->next = fmem_head;
        cur->prev = nullptr;
        fmem_head->prev = cur;
        fmem_head = cur;
        tryToJoin(cur);
    }

    return 0;


}

void MemAlloc::tryToJoin(FreeMem* current) {
    if (current->next) {
        size_t end = (size_t)current + current->size + MEM_BLOCK_SIZE;
        size_t nextStart = (size_t)current->next;
        if (end == nextStart) {
            current->size += current->next->size + MEM_BLOCK_SIZE;
            current->next = current->next->next;
            if (current->next) current->next->prev = current;
        }
    }
}

void MemAlloc::printFreeMem() {
    FreeMem* cur = fmem_head;
    while (cur) {
        printHexa((size_t)cur);
        printStr(" size of ");
        printInteger((size_t)cur->size);
        printStr("\n");
        cur = cur->next;
    }
    printStr("\n");
}


// void MemAlloc::tryToJoin(FreeMem* cur) {
//     if (!cur)return;
//     if (cur->next && (char*)cur+cur->size * MEM_BLOCK_SIZE == (char*)(cur->next)) {
//         cur->size += cur->next->size;
//         cur->next = cur->next->next;
//         if (cur->next) cur->next->prev = cur;
//     }
// }

// if (!adr) return -1;
//
// FreeMem* cur = nullptr;
//
// if (fmem_head) {
//     for (cur = fmem_head; cur->next != nullptr
//         && adr>(char*)(cur->next); cur = cur->next) {}
// }
//
// FreeMem* newSeg = (FreeMem*)((char*)adr - sizeof(FreeMem));
// newSeg->size = cur->size;
// newSeg->prev = cur;
// if (cur) newSeg->next = cur;
// else newSeg->next = fmem_head;
// if (newSeg->next) newSeg->next->prev = newSeg;
// if (cur)cur->next = newSeg;
// else fmem_head = newSeg;
//
// tryToJoin(newSeg);
// if (prev) tryToJoin(cur);
// return 0;