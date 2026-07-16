
#include  "../lib/hw.h"
#include "../h/syscall_cpp.hpp"

void* operator new (size_t size) {
    return mem_alloc(size);
}

void* operator new [](size_t size) {
    return mem_alloc(size);
}

void operator delete(void* adr) noexcept{
    mem_free(adr);
}

void operator delete[](void* adr) noexcept{
    mem_free(adr);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
    myHandle = nullptr;
}

Thread::Thread() {
    myHandle = nullptr;
    this->body=Thread::threadWrapper;
    this->arg=this;
}

Thread::~Thread() noexcept {
    //MemAlloc::free(this->myHandle);
}

int Thread::start() {
    // int ret = -3;
    // if (body!=nullptr) {
    //     ret = thread_create(&myHandle,body,arg);
    // }else {
    //     ret = thread_create(&myHandle, threadWrapper,this);
    // }
    // return ret;
    return thread_create(&myHandle,body,arg);
}

// DODATO
/*void Thread::join() {
    if (myHandle) {
        thread_join(&myHandle);
    }
}*/

void Thread::threadWrapper(void *arg) {
    Thread* t = (Thread*)arg;
    if (t) t->run();
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}


//ova klasa predstavlja nit koja se budi na svakih n perioda tajmera
//i  izvrsava periodicActivation

PeriodicThread::PeriodicThread(time_t period) : Thread(){
    this->period = period;
}

void PeriodicThread::terminate() {
    thread_exit();
}

void PeriodicThread::run() {
    while (true) {
        periodicActivation();
        time_sleep(period);
    }
}


char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
