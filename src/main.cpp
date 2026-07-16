#include "../h/kMemAlloc.hpp"
#include "../h/riscv.hpp"
#include "../h/kThread.hpp"
#include "../h/kConsole.hpp"
#include "../h/syscall_c.hpp"
#include "../test/mojprint.hpp"


extern "C" void supervisorTrap();
void userMain();
static sem_t userDone = nullptr;

static void userWrapper(void*) {
    userMain();
    sem_signal(userDone);
}

int main() {
    MemAlloc::init();

    kThread::running = kThread::createMainThread();
    kThread::createIdleThread();
    kConsole::init();

    Riscv::w_stvec((uint64)&supervisorTrap);
    Riscv::ms_sie(Riscv::SIE_SSIE | Riscv::SIE_SEIE);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    sem_open(&userDone, 0);

    thread_t userThread = nullptr;
    thread_create(&userThread, userWrapper, nullptr);

    sem_wait(userDone);
    sem_close(userDone);

    delete userDone;
    thread_dispatch();
    *(volatile unsigned int*)0x100000 = 0x5555;
    return 0;
}

// Za zaustavljanje emulatora
//*(volatile unsigned int*)0x100000 = 0x5555;
// __asm__ volatile (
//     "li t0,0x100000\n"
//     "li t1,0x5555\n"
//     "sw t1, (t0)"
// );

// size_t size = 100;
//
// MemAlloc::printFreeMem();
//
// void *adr = MemAlloc::malloc(size);
//
// MemAlloc::printFreeMem();
// printStr("A\n");
// printInteger((uint64)adr);
// printStr("\nB\n");
// int ret=MemAlloc::free(adr);
//
// printInteger(ret);
// printStr("\nB\n");
//
// MemAlloc::printFreeMem();


// userPrint("Console test\n");
// userPrint("Type characters:\n");
//
// while (true) {
//     char c = getc();
//     putc(c);
//
//     if (c == '\n') {
//         userPrint("line end\n");
//     }
// }

//TEST ZA ASINHRONU PROVERU
// #include "../h/kMemAlloc.hpp"
// #include "../h/riscv.hpp"
// #include "../h/kThread.hpp"
// #include "../h/syscall_cpp.hpp"
// #include "../test/mojprint.hpp"
//
// extern "C" void supervisorTrap();
//
// extern void t1Run(void*);
// extern void t2Run(void*);
// extern void t3Run(void*);
// extern void t4Run(void*);
// extern void t5Run(void*);
//
// uint64 x1;
// uint64 x2;
// uint64 t3Cnt;
// uint64 t4Cnt;
//
// int main() {
//     MemAlloc::init();
//
//     kThread::running = kThread::createMainThread();
//     kThread::createIdleThread();
//     kConsole::init();
//
//     Riscv::w_stvec((uint64)&supervisorTrap);
//     Riscv::ms_sie(Riscv::SIE_SSIE | Riscv::SIE_SEIE);
//     Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
//
//     x1 = 39;
//     x2 = 30015;
//     t3Cnt = 0;
//     t4Cnt = 0;
//
//     Thread* t1 = new Thread(&t1Run, &x1);
//     Thread* t2 = new Thread(&t2Run, &x2);
//     Thread* t3 = new Thread(&t3Run, &x1);
//     Thread* t4 = new Thread(&t4Run, &x1);
//     Thread* t5 = new Thread(&t5Run, &x1);
//
//     t1->start();
//     t2->start();
//     t3->start();
//     t4->start();
//     t5->start();
//
//     while (true) {
//         Thread::dispatch();
//     }
//
//     return 0;
// }



// extern "C" void supervisorTrap();
//
// void a(void*) {
//     while (true) {
//         printStr("A\n");
//     }
// }
//
// void b(void*) {
//     while (true) {
//         printStr("B\n");
//     }
// }
//
// int main() {
//     MemAlloc::init();
//     Riscv::w_stvec((uint64)&supervisorTrap);
//     Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
//     Riscv::ms_sip(Riscv::SIP_SSIE);
//
//     kThread::running = kThread::createMainThread();
//
//     thread_t t1, t2;
//     thread_create(&t1, a, nullptr);
//     thread_create(&t2, b, nullptr);
//
//     while (true) {
//         thread_dispatch();
//     }
//
//     return 0;
// }

// #include "../h/kMemAlloc.hpp"
// #include "../h/riscv.hpp"
// #include "../h/kThread.hpp"
// #include "../h/syscall_c.hpp"
// #include "../test/print.hpp"
//
// #include  "../h/syscall_cpp.hpp"
//
// extern "C" void supervisorTrap();
//
//
// sem_t sem;
//
// void sleepy(void*) {
//     printStr("wait 3 pre\n");
//     sem_wait_n(sem, 3);
//     printStr("wait 3 posle\n");
// }
//
// void worker(void*) {
//     printStr("signal 1\n");
//     sem_signal(sem);
//     thread_dispatch();
//
//     printStr("signal 2\n");
//     sem_signal(sem);
//     thread_dispatch();
//
//     printStr("signal 3\n");
//     sem_signal(sem);
//     thread_dispatch();
// }
// void a(void*) {
//     while (true) {
//         printStr("A\n");
//     }
// }
//
// void b(void*) {
//     while (true) {
//         printStr("B\n");
//     }
// }
//
//
// void main(){
//     MemAlloc::init();
//     Riscv::w_stvec((uint64)&supervisorTrap);
//     Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
//     Riscv::ms_sip(Riscv::SIP_SSIE);
//     kThread::running = kThread::createMainThread();
//
//     sem_open(&sem,0);
//
//
//     thread_t t1, t2;
//     thread_create(&t1, sleepy, nullptr);
//     thread_create(&t2, worker, nullptr);
//
//     // while (!t1->getFinished() || !t2->getFinished()) {
//     //     thread_dispatch();
//     // }
//
//     // sem_close(sem);
//     // printStr("Main end\n");
//
//     // while (true) {
//     //     thread_dispatch();
//     // }
// }
//
//
// // int main() {
// //     MemAlloc::init();
// //     Riscv::w_stvec((uint64)&supervisorTrap);
// //     Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
// //     Riscv::ms_sip(Riscv::SIP_SSIE);
// //     kThread::running = kThread::createMainThread();
// //
// //     thread_t t1, t2;
// //     thread_create(&t1, sleepy, nullptr);
// //     thread_create(&t2, worker, nullptr);
// //
// //     while (!t1->getFinished() || !t2->getFinished()) {
// //         thread_dispatch();
// //     }
// //
// //     printStr("Main end\n");
// // }void a(void*) {


/*
#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"

class JoinWorkerB : public Thread {
public:
    JoinWorkerB() : Thread() {}

protected:
    void run() override {
        printString("B started\n");
        for (uint64 i = 0; i < 5; i++) {
            printString("B working\n");
            for (uint64 j = 0; j < 10000; j++) {
                for (uint64 k = 0; k < 30000; k++) { }
            }
            thread_dispatch();
        }
        printString("B finished\n");
    }
};

class JoinWorkerC : public Thread {
public:
    JoinWorkerC() : Thread() {}

protected:
    void run() override {
        printString("C started\n");
        for (uint64 i = 0; i < 5; i++) {
            printString("C working\n");
            for (uint64 j = 0; j < 10000; j++) {
                for (uint64 k = 0; k < 30000; k++) { }
            }
            thread_dispatch();
        }
        printString("C finished\n");
    }
};

class JoinWorkerA : public Thread {
public:
    JoinWorkerA(Thread* b, Thread* c) : Thread(), b(b), c(c) {}

protected:
    void run() override {
        printString("A started\n");
        printString("A waiting for B\n");
        b->join();
        printString("A detected B finished\n");
        printString("A waiting for C\n");
        c->join();
        printString("A detected C finished\n");
        printString("A finished\n");
    }

private:
    Thread* b;
    Thread* c;
};

void userMain() {
    printString("=== JOIN TEST START ===\n");
    JoinWorkerB* b = new JoinWorkerB();
    JoinWorkerC* c = new JoinWorkerC();
    JoinWorkerA* a = new JoinWorkerA(b, c);

    b->start();
    c->start();
    a->start();

    a->join();

    delete a;
    delete b;
    delete c;

    printString("joinTest finished\n");

}
*/
