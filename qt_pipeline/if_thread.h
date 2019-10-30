#ifndef IF_THREAD_H
#define IF_THREAD_H
#include "thread"
#include <iostream>
#include "barrier.h"

class if_thread
{
public:
    int quantum_finished=0;
    Barrier* master_bar;
    Barrier* final_bar;
    int* intr_che;
    int* block_id_intr_che;
    int* intr_mem;
    int thread_id=1;
    int* input_box;
    int* output_box;
    int pc=0;
    int che_fails=0;

    if_thread();
    void run (void* data);
};

#endif // IF_THREAD_H
