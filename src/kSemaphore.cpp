#include "../h/kSemaphore.hpp"
#include "../h/kScheduler.hpp"
#include "../test/mojprint.hpp"

kSemaphore::kSemaphore(const int value) {
    //head = tail = nullptr;
    this->value = value;
    this->isClosed=false;
}

kSemaphore* kSemaphore::create(unsigned value) {
    return new kSemaphore(value);
}

int kSemaphore::kSem_close() {
    if (isClosed) return -1;

    isClosed=true;

    while (readyThreads.peekFirst()) {
        kThread* t = readyThreads.removeFirst();
        t->setWaitNeed(0);
        t->setBlocked(false);
        kScheduler::put(t);
        // unblock();
    }

    return 0;
}

int kSemaphore::kSem_signal() {
    return kSem_signal_N_time(1);
    // if (isClosed) return -1;
    //
    // if (++value <= 0) {
    //     unblock();
    // }
    //
    // return 0;
}

int kSemaphore::kSem_wait() {
    // if (isClosed) return -1;
    //
    // if (--value<0) {
    //     block();
    // }
    //
    // return 0;
    return kSem_wait_N_time(1);
}

int kSemaphore::kSem_signal_N_time(unsigned n) {
    // printStr("SIGNAL N = ");
    // printInteger(n);
    // printStr("\n");

    if (isClosed) return -1;
    if (n == 0) return 0;

    value += n;

    while (readyThreads.peekFirst() &&
           (unsigned)value >= readyThreads.peekFirst()->getWaitNeed()) {
        kThread* t = readyThreads.removeFirst();
        value -= t->getWaitNeed();
        t->setWaitNeed(0);
        t->setBlocked(false);
        kScheduler::put(t);
           }

    // printStr("signal_n value=");
    // printInteger(value);
    // printStr(" need=");
    // printInteger(readyThreads.peekFirst() ? readyThreads.peekFirst()->getWaitNeed() : 0);
    // printStr("\n");

    return 0;
}

int kSemaphore::kSem_wait_N_time(unsigned n) {
    // printStr("WAIT N = ");
    // printInteger(n);
    // printStr("\n");

    if (isClosed) return -1;
    if (n == 0) return 0;

    if ((unsigned)value >= n) {
        value -= n;
        return 0;
    }

    kThread::running->setWaitNeed(n-value);
    value = 0;
    block();

    //     if ((unsigned)value >= n) {
    //         value -= n;
    //         return 0;
    //     }
    //
    //     kThread::running->setWaitNeed(n);
    //     block();

    if (isClosed) return -1;
    return 0;
}

void kSemaphore::block() { // proveri zasto ovde ne mora da se proverava setjmp
    kThread::running->setBlocked(true);
    readyThreads.addLast(kThread::running);
    kThread::dispatch();
}


void kSemaphore::unblock() {
    kThread* t = readyThreads.removeFirst();
    if (t) {
        t->setWaitNeed(0);
        t->setBlocked(false);
        kScheduler::put(t);
    }
}



// int kSemaphore::kSem_wait() {
//     return kSem_wait_N_time(1);
// }
//
// int kSemaphore::kSem_signal() {
//     return kSem_signal_N_time(1);
// }
//
// int kSemaphore::kSem_wait_N_time(unsigned n) {
//     if (isClosed) return -1;
//     if (n == 0) return 0;
//
//     if ((unsigned)value >= n) {
//         value -= n;
//         return 0;
//     }
//
//     block(n);
//     if (isClosed) return -1;
//     return 0;
// }
//
// int kSemaphore::kSem_signal_N_time(unsigned n) {
//     if (isClosed) return -1;
//     if (n == 0) return 0;
//
//     value += n;
//     unblockReady();
//     return 0;
// }
//
// void kSemaphore::block(unsigned n) {
//     kThread::running->setBlocked(true);
//
//     Blocked* elem = new Blocked(kThread::running, n);
//
//     if (tail) tail->next = elem;
//     else head = elem;
//     tail = elem;
//
//     kThread::dispatch();
// }
//
// void kSemaphore::unblockReady() {
//     while (head && (unsigned)value >= head->need) {
//         Blocked* elem = head;
//         head = head->next;
//         if (!head) tail = nullptr;
//
//         value -= elem->need;
//
//         elem->thread->setBlocked(false);
//         kScheduler::put(elem->thread);
//
//         //delete elem;
//     }
// }
//
// int kSemaphore::kSem_close() {
//     if (isClosed) return -1;
//
//     isClosed = true;
//
//     while (head) {
//         Blocked* elem = head;
//         head = head->next;
//
//         elem->thread->setBlocked(false);
//         kScheduler::put(elem->thread);
//
//         //delete elem;
//     }
//
//     tail = nullptr;
//     return 0;
// }
