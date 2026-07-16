#include "../h/kMemAlloc.hpp"
#include "../test/mojprint.hpp"
#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/kThread.hpp"
#include "../h/sleepThreadList.hpp"
#include "../h/syscall_c.hpp"
#include "../h/kConsole.hpp"

void Riscv::popSppSpie() {
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 scause=r_scause();
    uint64 volatile sepc = r_sepc();
    //uint64 volatile sstatus=r_sstatus();

    if (scause == 0x08 || scause == 0x09) {
        sepc += 4; // trenutno sepc pokazuje na eccal, zelimo to da preskoicmo posto ga ovde obradjujemo, da se ne bi iznova i iznova zvalo

        uint64 opCode;
        uint64 param1;

        __asm__ volatile("ld %0, 80(fp)" : "=r"(opCode));
        __asm__ volatile("ld %0, 88(fp)" : "=r"(param1));

        switch (opCode) {
            case 0x01: {
                // malloc sistemski poziv
                void *ret = MemAlloc::malloc(param1);
                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile("sd t0, 80(x8)"); //x8 = frame pointer
                //__asm__ volatile("mv a0, %0" :: "r"(ret));
                break;
            }

            case 0x02: {
                // mem_free sistemski poziv
                int ret = MemAlloc::free((void*)param1);
                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile ("sw t0, 80(x8)");
                //__asm__ volatile("mv a0, %0" :: "r"(ret));
                break;
            }
            case 0x11: {
                kThread** handle = (kThread**)param1;
                //thread_t* handle = (thread_t*)param1;
                void(*f)(void*);
                void* arg;
                uint64* stack;

                __asm__ volatile("ld %0, 96(fp)" : "=r"(f));
                __asm__ volatile("ld %0, 104(fp)" : "=r"(arg));
                __asm__ volatile("ld %0, 112(fp)" : "=r"(stack));

                kThread* newThread = kThread::create(f,arg,stack);

                int ret=0;

                if (!newThread)ret=-1;
                else *handle=newThread;

                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile ("sw t0, 80(x8)");
                break;
            }
            case 0x12: {//exit
                kThread::running->setFinished(true);

                // DODATO
                /*
                while (kThread::running->joinWaiters.peekFirst()) {
                    kThread* t = kThread::running->joinWaiters.removeFirst();
                    t->setBlocked(false);
                    kScheduler::put(t);
                }
                */

                kThread::dispatch();

                // int ret = 0;
                // __asm__ volatile("mv t0, %0" :: "r"(ret));
                // __asm__ volatile ("sw t0, 80(x8)");

                break;
            }
            case 0x13: {
                //printStr("DISPATCH\n");
                kThread::dispatch();
                //thread dispatch
                break;
            }

            // DODATO
            /*case 0x14: {
                kThread** handle = (kThread**)param1;
                kThread::join(handle ? *handle : nullptr);
                break;
            }*/

            case 0x21: { //sem open
                sem_t* handle=(sem_t*)param1;
                uint64 tmp;
                __asm__ volatile("ld %0, 96(fp)" : "=r"(tmp));
                unsigned init = (unsigned)tmp;

                kSemaphore* sem = kSemaphore::create(init);

                int ret=0;
                if (!sem)ret=-1;
                else *handle=sem;

                __asm__ volatile("mv t0,%0" :: "r"(ret));
                __asm__ volatile("sw t0, 80(x8)");
                break;
            }
            case 0x22: { //sem cloce
                kSemaphore* sem = (kSemaphore*)param1;
                int ret=-1;
                if (sem) ret = sem->kSem_close();
                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile ("sw t0, 80(x8)");
                break;
            }
            case 0x23: { //sem wait
                kSemaphore* sem = (kSemaphore*)param1;
                int ret=-1;
                if (sem) ret = sem->kSem_wait();
                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile ("sw t0, 80(x8)");
                break;
            }
            case 0x24: { //sem signal
                kSemaphore* sem = (kSemaphore*)param1;
                int ret=-1;
                if (sem) ret = sem->kSem_signal();
                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            }
            case 0x25: { //sem wait with parameter n
                uint64 tmp;
                __asm__ volatile("ld %0, 96(fp)" : "=r"(tmp));
                unsigned n = (unsigned)tmp;
                kSemaphore* sem = (kSemaphore*)param1;
                int ret=-1;
                if (sem) ret = sem->kSem_wait_N_time(n);
                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile ("sd t0, 80(x8)");
                break;
            }
            case 0x26: { //sem signal with parameter n
                uint64 tmp;
                __asm__ volatile("ld %0, 96(fp)" : "=r"(tmp));
                unsigned n = (unsigned)tmp;
                kSemaphore* sem = (kSemaphore*)param1;
                int ret=-1;
                if (sem) ret = sem->kSem_signal_N_time(n);
                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile ("sd t0, 80(x8)");
                break;
            }
            case 0x31: { //time sleep
                time_t time=param1;
                int ret= kThread::running->sleep(time);

                __asm__ volatile("mv t0, %0" :: "r"(ret));
                __asm__ volatile ("sw t0, 80(x8)");
                break;
            }

            case 0x41: {
                char c = kConsole::getc();
                __asm__ volatile("mv t0, %0" :: "r"((uint64)c));
                __asm__ volatile("sd t0, 80(x8)");
                break;
            }

            case 0x42: {
                uint64 c2_reg;
                __asm__ volatile("ld %0, 88(fp)" : "=r"(c2_reg));
                kConsole::putc((char)c2_reg);
                break;
            }

            default:{
                printStr("Nepoznat sistemski poziv!\n");
                opCode = -1;
                break;
            }
        }
        w_sepc(sepc); // posto smo povecali sepc za 4 sada ce da pokazuje na instrukciju posle ecall, sto nam i treba
    }else if (scause == 0x8000000000000001UL) { // TAJMER
        sleepThreadList::tick(); // odbrojava nitima koje cekaju

        kThread::timeSliceCounter++;

        if (kThread::timeSliceCounter >= DEFAULT_TIME_SLICE) {
            kThread::timeSliceCounter = 0;
            kThread::dispatch();
        }

        //sleepThreadList::tick();
        __asm__ volatile("csrc sip, %0" : : "r"(2)); // brisemo bit 1 u sip, ovo govori procesoru da je ovaj prekid gotov i da ne mora ponovo da poziva prekid

        w_sepc(sepc); // obezbedjuje da se nastavi izvrsavnanje tacno od prekinute instrukcije posle sret, kod tajmera zelimo da nastavimo tacno tamo gde smo stali (za razliku od sistemskog prekida "ecall")
        //printStr("IZVRSAVNJE TAJMER PREKIDA\n");
        return;
    }else if (scause == 0x8000000000000009UL) {
        kConsole::handleInterrupt();

        //printStr("IZVRSAVNJE CONSOLE PREKIDA\n");
        return;
    }else {
        // Ako dodje do bilo kod drugog prekida
        printStr("Neocekivan prekid! SCAUSE: ");
        printHexa(scause);
        printStr(" SEPC: ");
        printHexa(r_sepc());
        printStr(" STVAL: ");
        printHexa(r_stval());
        printStr("\n");
        while (true) {}
    }
}


// case 0x41: {
//     // char c = __getc();
//     // __asm__ volatile("mv a0, %0" :: "r"((uint64)c));
//     // //__asm__ volatile("sd %0, 80(fp)" : : "r"((uint64)c));
//     // break;
//
//     volatile char* uart0 = (volatile char*)0x10000000;
//
//     while ((*(uart0 + 5) & 1) == 0) {
//     }
//
//     char c = *uart0;
//
//     __asm__ volatile("sd %0, 80(fp)" : : "r"((uint64)c));
//     break;
// }
//
// case 0x42: {
//     // uint64 inputChar;
//     // __asm__ volatile("ld %0, 88(fp)" : "=r"(inputChar));
//     // __putc((char)inputChar);
//     __putc((char)param1);
//     break;
//
// }
// case 0x41: {
//     char c1 = __getc();
//
//     // Upisujemo rezultat u sačuvani a0 na steku
//     __asm__ volatile("sd %0, 80(fp)" : : "r"((uint64)c1));
//     break;
// }
// case 0x41: {
//     char c = kConsole::getc();
//     __asm__ volatile("mv a0, %0" :: "r"((uint64)c));
//     break;
// }
//
// case 0x42: {
//     kConsole::putc((char)param1);
//     break;
// }
