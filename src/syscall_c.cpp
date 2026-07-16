//
// Created by os on 5/16/26.
//

#include "../h/syscall_c.hpp"
#include "../test/mojprint.hpp"


void* mem_alloc(size_t size) {
    if (!size) return 0;
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (blocks));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x01));
    __asm__ volatile("ecall");

    // void* result = nullptr;
    // __asm__ volatile("mv %[res], a0" : [res] "=r" (result));
    //return result;

    volatile uint64 result;
    __asm__ volatile("mv %0, a0": "=r"(result));
    return (void*) result;
}

int mem_free(void* adr) {
    __asm__ volatile("mv a1, %0" : : "r"(adr));
    __asm__ volatile("li a0, 0x02");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %0,a0" : "=r"(result));
    return (int)result;
}

int thread_create(thread_t* handle, void(*f)(void*), void* arg) {

    void* stack = mem_alloc(DEFAULT_STACK_SIZE);

    if (stack==nullptr) {
        return -1;
    }

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a2, %0" : : "r" (f));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a4, %0" : : "r" (stack));

    __asm__ volatile("li a0, 0x11");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %0,a0" : "=r"(result));
    return result;
}

int thread_exit() {
    __asm__ volatile("li a0, 0x12");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %0,a0" : "=r"(result));
    return result;
}

void thread_dispatch() {
    __asm__ volatile("li a0, 0x13");
    __asm__ volatile("ecall");
}

// DODATO
/*void thread_join(thread_t* handle) {
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("li a0, 0x14");
    __asm__ volatile("ecall");
}*/

int sem_open(sem_t* handle, unsigned init) {
    uint64 result;
    __asm__ volatile(
        "mv a1, %1\n"
        "mv a2, %2\n"
        "li a0, 0x21\n"
        "ecall\n"
        "mv %0, a0\n"
        : "=r"(result)
        : "r"(handle), "r"((uint64)init)
        : "a0", "a1", "a2"
    );
    return (int)result;
}

// int sem_open(sem_t* handle, unsigned init) {
//     __asm__  volatile("mv a1, %0" : : "r" (handle));
//     __asm__ volatile("mv a2, %0" : : "r" (init));
//
//     __asm__ volatile("li a0,0x21");
//     __asm__ volatile("ecall");
//
//     uint64 result;
//     __asm__ volatile ("mv %0,a0" : "=r"(result));
//     return result;
// }

int sem_close(sem_t handle) {
    __asm__ volatile("mv a1,%0" :: "r"(handle));

    __asm__ volatile ("li a0,0x22");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile ("mv %0,a0" : "=r"(result));
    return result;
}

// Ako ne valja oanaj wait i signal uradi ovaj u jednom bloku
// int sem_wait(sem_t id) {
//     uint64 result;
//     __asm__ volatile(
//         "mv a1, %1\n"
//         "li a0, 0x23\n"
//         "ecall\n"
//         "mv %0, a0\n"
//         : "=r"(result)
//         : "r"(id)
//         : "a0", "a1"
//     );
//     return (int)result;
// }
//
// int sem_signal(sem_t id) {
//     uint64 result;
//     __asm__ volatile(
//         "mv a1, %1\n"
//         "li a0, 0x24\n"
//         "ecall\n"
//         "mv %0, a0\n"
//         : "=r"(result)
//         : "r"(id)
//         : "a0", "a1"
//     );
//     return (int)result;
// }

int sem_wait(sem_t id) {
    __asm__ volatile("mv a1,%0"::"r"(id));

    __asm__  volatile("li a0,0x23");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %0,a0" : "=r"(result));
    return result;
}

int sem_signal(sem_t id) {
    __asm__ volatile("mv a1,%0"::"r"(id));

    __asm__  volatile("li a0,0x24");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %0,a0" : "=r"(result));
    return result;
}

int sem_wait_n(sem_t id, unsigned n) {
    uint64 result;
    __asm__ volatile(
        "mv a1, %1\n"
        "mv a2, %2\n"
        "li a0, 0x25\n"
        "ecall\n"
        "mv %0, a0\n"
        : "=r"(result)
        : "r"(id), "r"((uint64)n)
        : "a0", "a1", "a2"
    );
    return (int)result;
}

int sem_signal_n(sem_t id, unsigned n) {
    uint64 result;
    __asm__ volatile(
        "mv a1, %1\n"
        "mv a2, %2\n"
        "li a0, 0x26\n"
        "ecall\n"
        "mv %0, a0\n"
        : "=r"(result)
        : "r"(id), "r"((uint64)n)
        : "a0", "a1", "a2"
    );
    return (int)result;
}
// int sem_wait_n(sem_t id, unsigned n) {
//     __asm__ volatile("mv a1,%0"::"r"(id));
//     __asm__ volatile("mv a2, %0" :: "r"(n));
//
//     __asm__  volatile("li a0,0x25");
//     __asm__ volatile("ecall");
//
//     uint64 result;
//     __asm__ volatile("mv %0,a0" : "=r"(result));
//     return result;
// }
//
// int sem_signal_n(sem_t id, unsigned n) {
//     __asm__ volatile("mv a1,%0"::"r"(id));
//     __asm__ volatile("mv a2, %0" :: "r"(n));
//
//     __asm__  volatile("li a0,0x26");
//     __asm__ volatile("ecall");
//
//     uint64 result;
//     __asm__ volatile("mv %0,a0" : "=r"(result));
//     return result;
// }

int time_sleep(time_t t) {
    __asm__ volatile("mv a1,%0" :: "r"(t));
    __asm__ volatile("li a0,0x31");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %0,a0" : "=r"(result));
    return result;
}

char getc() {
    // Stavimo kod sistemskog poziva u a0
    __asm__ volatile("li a0, 0x41");
    __asm__ volatile("ecall");

    // Kada se vrati iz kernela, rezultat je u a0. Čitamo ga bezbedno:
    char returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

void putc(char c) {
    // Prvo stavimo karakter u a1, pa tek onda šifru poziva u a0
    // Time garantujemo da se registri ne gaze međusobno
    __asm__ volatile("mv a1, %0" : : "r"((uint64)c));
    __asm__ volatile("li a0, 0x42");
    __asm__ volatile("ecall");
}

// char getc() {
//     __asm__ volatile("li a0,0x41");
//     __asm__ volatile("ecall");
//
//     char result;
//     __asm__ volatile("mv %0, a0": "=r" (result));
//     return result;
// }
//
// void putc(char c) {
//     __asm__ volatile ("mv a1, %0" : : "r"(c));
//     __asm__ volatile("li a0,0x42");
//     __asm__ volatile("ecall");
// }
