#include "../h/kScheduler.hpp"

List<kThread> kScheduler::readyCoroutineQueue;

kThread* kScheduler::get()
{
    return readyCoroutineQueue.removeFirst();
}

void kScheduler::put(kThread *ccb)
{
    readyCoroutineQueue.addLast(ccb);
}
