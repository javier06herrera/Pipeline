#ifndef BARRIER_H
#define BARRIER_H
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;
class Barrier
{
public:
    explicit Barrier(size_t iCount) :
      mThreshold(iCount),
      mCount(iCount),
      mGeneration(0) {
    }

    void Wait();
private:
    mutex mMutex;
    condition_variable mCond;
    size_t mThreshold;
    size_t mCount;
    size_t mGeneration;
};

#endif // BARRIER_H
