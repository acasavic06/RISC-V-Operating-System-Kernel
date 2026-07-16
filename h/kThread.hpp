#ifndef PROJECT_BASE_V1_1_KTHREAD_HPP
#define PROJECT_BASE_V1_1_KTHREAD_HPP

#include "kMemAlloc.hpp"
#include "../lib/hw.h"
#include "../h/kScheduler.hpp"
#include "riscv.hpp"


class kScheduler;

class kThread {
public:
    ~kThread();

    using Body = void(*)(void*);

    static void threadWrapper();
    static void dispatch(); //sinhrona promena konteksta

    void setFinished(bool value) { isFinished = value; }
    void setBlocked(bool value) { isBlocked = value; }
    bool getFinished() const { return isFinished; }
    bool getBlocked() const { return isBlocked; }

    static kThread* create(Body body, void* arg, uint64* stack);
    int sleep(time_t time);

    static kThread* createMainThread();
    static kThread* createIdleThread();

    // Brojac
    static uint64 timeSliceCounter;

    //pokazivac na nit koja se trenutno izvrsava
    static kThread* running;
    // rezervna nit u slucaju da scheduler u redu za cekanje nema ni jednu spremnu nit, salje se ova i ceka se da scheduler dobije neku nit
    static kThread* idleThread; // nikada ne ide u red za cekanje kod schedulera

    //  DODATO
    /*static uint64 idGlobal;
    uint64 id;
    List<kThread> joinWaiters;
    static void join(kThread* thread);
    uint64 getId() const { return id; }*/

    void* operator new(size_t size) {
        return MemAlloc::malloc(size);
    }
    void* operator new[](size_t size) {
        return MemAlloc::malloc(size);
    }

    void operator delete(void *ptr) {
        MemAlloc::free(ptr);
    }
    void operator delete[](void *ptr) {
        MemAlloc::free(ptr);
    }

    void setWaitNeed(unsigned n) { waitNeed = n; }
    unsigned getWaitNeed() const { return waitNeed; }

private:
    explicit kThread(Body body, void* arg, uint64* stack);

    struct Context {
        uint64 ra;  // povratna adresa na koju procesor ide kada se nastavi sa izvrsvanjem niti (npr. ako je nit uspavana mora da se sascuva dokle je stigla)
        uint64 sp;
    };
    //na pocetku ra pokazuje na thread Wrapper

    //sve registre cemo prilikom promene konteksta cuvati na steku, tako da jedino
    // sto treba da znamo o niti je ra i sp, svaka nit ima neki svoj stek

    friend class Riscv;

    Body body;
    uint64* stack;
    Context context;
    bool isFinished;
    bool isBlocked;
    void* arg;

    unsigned waitNeed;

    static List<kThread> finishedThreadQueue; // ova lista nam pokazuje na listu niti koje trebaju da se obrisu
    static void reclaimFinishedThreads(); // funkcija koja prolazi kroz listu i brise te niti

    static void contextSwitch(Context* old, Context* next);  // definisana u asemblerskom fajlu
    static void idleBody(void*);
};

#endif
