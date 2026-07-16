#ifndef PROJECT_BASE_V1_1_KSCHEDULER_HPP
#define PROJECT_BASE_V1_1_KSCHEDULER_HPP

#include "list.hpp"

class kThread;

// u scheduleru ce biti odradjen najednostavniji algoritam FIFO
// ko prvi dodje na red taj ce prvi biti i odradjen

class kScheduler
{
public:
    static kThread *get();
    static void put(kThread *ccb);

    static bool isEmpty(){return readyCoroutineQueue.empty();}

private:
    static List<kThread> readyCoroutineQueue;
};
#endif