//
// Created by os on 6/18/26.
//


#include "../h/sleepThreadList.hpp"

sleepThreadList::sleepElem* sleepThreadList::head = nullptr;

void sleepThreadList::put(kThread *thread, time_t time) {
    sleepElem* newElem = new sleepElem(thread,time,nullptr);

    if (!head) {
        head=newElem;
        return;
    }

    sleepElem* prev=nullptr;
    sleepElem* cur = head;

    while (cur && cur->time <= time) {
        time -= cur->time;
        prev=cur;
        cur=cur->next;
    }

    newElem->time = time;
    newElem->next = cur;

    if (cur) cur->time-=time;

    if (prev) {
        prev->next = newElem;
    }else {
        head=newElem;
    }

}

void sleepThreadList::tick() {
    if  (!head) return;

    if (head->time > 0) head->time--;

    while (head && head->time == 0) {
        sleepElem* cur=head;
        head = head->next;

        cur->thread->setBlocked(false);
        kScheduler::put(cur->thread);

        delete cur;
    }
}
