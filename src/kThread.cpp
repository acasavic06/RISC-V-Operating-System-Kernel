#include "../h/kThread.hpp"

#include "../h/sleepThreadList.hpp"
#include "../h/syscall_c.hpp"


kThread* kThread::running=nullptr;
kThread* kThread::idleThread=nullptr;
uint64 kThread::timeSliceCounter = 0;
List<kThread> kThread::finishedThreadQueue;

// DODATO
/*
uint64 kThread::idGlobal = 0;
void kThread::join(kThread*thread) {
    if (!thread || thread == running || thread->getFinished()) return;

    running->setBlocked(true);
    thread->joinWaiters.addLast(running);
    dispatch();
}
*/


kThread* kThread::createMainThread() {
    return new kThread(nullptr, nullptr, nullptr);
}

kThread* kThread::createIdleThread() {
    if (idleThread) return idleThread;

    size_t stackBlocks = (DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    uint64* stack = (uint64*)MemAlloc::malloc(stackBlocks);
    if (!stack) return nullptr;

    idleThread = new kThread(idleBody, nullptr, stack);
    return idleThread;
}

kThread::kThread(Body body, void* arg, uint64* stack) :  body(body),
    stack(body != nullptr ? stack : nullptr),
    context({(uint64) &threadWrapper,
                stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE >> 3] : 0 // stavljamo stack pointer na vrh steka (stack_size/8= last index)
    }), isFinished(false), isBlocked(false),arg (body!=nullptr ? arg : nullptr),  waitNeed(0)
    {
    /*id=++idGlobal; // DODATO*/
    if (body) {kScheduler::put(this);}
}

kThread::~kThread() {
    operator delete [](stack);
    //MemAlloc::free(stack);
}

kThread* kThread::create(Body body, void *arg, uint64 *stack) {
    kThread* thread = new kThread(body, arg, stack);
    return thread;
}

void kThread::reclaimFinishedThreads() {
    while (!finishedThreadQueue.empty()) {
        delete finishedThreadQueue.removeFirst();
    }
}

void kThread::dispatch() {
    kThread::timeSliceCounter = 0;
    reclaimFinishedThreads();

    kThread* oldThread = running;

    if (oldThread && oldThread != idleThread &&
        !oldThread->isFinished && !oldThread->isBlocked) {
        kScheduler::put(oldThread);
    } else if (oldThread && oldThread != idleThread && oldThread->isFinished) {
        finishedThreadQueue.addLast(oldThread);
    }

    kThread* nextThread = kScheduler::get();
    if (!nextThread) {
        if (oldThread && !oldThread->isFinished && !oldThread->isBlocked) return;
        nextThread = idleThread;
        if (!nextThread || nextThread == oldThread) return;
    }

    running = nextThread;

    if (oldThread) {
        contextSwitch(&oldThread->context, &running->context);
    } else {
        contextSwitch(nullptr, &running->context);
    }

    /*kThread* oldThread = running;
    if (oldThread && !oldThread->isFinished && !oldThread->isBlocked) kScheduler::put(oldThread);
    kThread* nextThread = kScheduler::get();
    if (!nextThread)return;

    running = nextThread;
    kThread::contextSwitch(&oldThread->context, &running->context);*/
}

void kThread::idleBody(void*) {
    while (true) {
        thread_dispatch();
    }
}

//
void kThread::threadWrapper() {
    Riscv::popSppSpie(); //
    running->body(running->arg); // poziva se boy funkcija sa argumentima
    //running->setFinished(true);
    thread_exit();
}

int kThread::sleep(time_t time) {
    if (time==0)return 0;
    running->setBlocked(true);
    sleepThreadList::put(running,time);
    dispatch(); // druga nit mora da se pozove, zamena

    return 0;
}
