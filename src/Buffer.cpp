#include "../h/Buffer.hpp"

BoundedBuffer::BoundedBuffer(int capacity)
    : spaceAvailable(capacity), itemAvailable(0), capacity(capacity), count(0), head(0), tail(0) {
    size_t blocks = (capacity + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    buffer = (char*)MemAlloc::malloc(blocks);
}

BoundedBuffer::~BoundedBuffer() {
    MemAlloc::free(buffer);
}

uint64 BoundedBuffer::lock() {
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    return sstatus;
}

void BoundedBuffer::unlock(uint64 sstatus) {
    if (sstatus & Riscv::SSTATUS_SIE) {
        Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    }
}

void BoundedBuffer::append(char value) {
    spaceAvailable.kSem_wait();

    uint64 sstatus = lock();
    buffer[tail] = value;
    tail = (tail + 1) % capacity;
    count++;
    unlock(sstatus);

    itemAvailable.kSem_signal();
}

char BoundedBuffer::take() {
    itemAvailable.kSem_wait();

    uint64 sstatus = lock();
    char value = buffer[head];
    head = (head + 1) % capacity;
    count--;
    unlock(sstatus);

    spaceAvailable.kSem_signal();
    return value;
}

int BoundedBuffer::appendTry(char value) {
    uint64 sstatus = lock();
    if (count >= capacity) {
        unlock(sstatus);
        return -1;
    }

    buffer[tail] = value;
    tail = (tail + 1) % capacity;
    count++;
    unlock(sstatus);

    itemAvailable.kSem_signal();
    return 0;
}

int BoundedBuffer::takeTry(char& value) {
    uint64 sstatus = lock();
    if (count <= 0) {
        unlock(sstatus);
        return -1;
    }

    value = buffer[head];
    head = (head + 1) % capacity;
    count--;
    unlock(sstatus);

    spaceAvailable.kSem_signal();
    return 0;
}
