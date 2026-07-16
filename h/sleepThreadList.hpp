//
// Created by os on 6/18/26.
//

#ifndef PROJECT_BASE_V1_1_SLEEPTHREADLIST_HPP
#define PROJECT_BASE_V1_1_SLEEPTHREADLIST_HPP
#include "kThread.hpp"
#include "list.hpp"

class sleepThreadList {
public:
    static void put(kThread* thread, time_t time);
    static void tick();
private:
    struct sleepElem {
        kThread *thread;
        time_t time;
        sleepElem *next;

        sleepElem (kThread* t, time_t time, sleepElem* n) :
            thread(t), time(time), next(n) {}
    };

    static sleepElem* head;
};

#endif //PROJECT_BASE_V1_1_SLEEPTHREADLIST_HPP
