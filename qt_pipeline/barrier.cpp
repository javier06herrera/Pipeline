#include "barrier.h"

void Barrier::Wait() {

    unique_lock<mutex> lLock(mMutex);
    auto lGen = mGeneration;
    if (!--mCount) {
        mGeneration++;
        mCount = mThreshold;
        mCond.notify_all();
    } else {
        mCond.wait(lLock, [this, lGen] { return lGen != mGeneration; });
    }
}
