#ifndef EX_THREAD_H
#define EX_THREAD_H
#include "thread"
#include <iostream>

class ex_thread
{
public:
    bool branch_result=false;
    int  overwrite_cycles=0;
    pthread_barrier_t* master_bar;
    pthread_barrier_t* final_bar;
    int thread_id=3;
    int* input_box;
    int* output_box;

    ex_thread();
    void run (void* data);
};

#endif // EX_THREAD_H
