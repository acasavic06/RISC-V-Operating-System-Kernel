#ifndef _SYSCALL_C_HPP_
#define _SYSCALL_C_HPP_

#include "kThread.hpp"
#include "../lib/hw.h"
#include "kSemaphore.hpp"

// #ifdef __cplusplus
// extern "C" {
// #endif

/* Memory management */

void* mem_alloc(size_t size);

int mem_free(void* adr);


/* Threads */

typedef kThread* thread_t;

int thread_create(
        thread_t* handle,
        void (*start_routine)(void*),
        void* arg
);

int thread_exit();

void thread_dispatch();


/*
void thread_join(thread_t* handle); // DODATO
*/


/* Semaphores */

typedef kSemaphore* sem_t;

int sem_open(sem_t* handle,unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

int sem_wait_n(sem_t id, unsigned n);

int sem_signal_n(sem_t id, unsigned n);


/* Time */

typedef unsigned long time_t;

int time_sleep(time_t);


/* Console */

char getc();

void putc(char c);


// #ifdef __cplusplus
// }
// #endif

#endif
