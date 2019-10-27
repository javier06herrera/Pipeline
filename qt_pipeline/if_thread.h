#ifndef IF_THREAD_H
#define IF_THREAD_H
#include "thread"
#include <iostream>

class if_thread
{
public:
    int quantum_finished;
    pthread_barrier_t* master_bar;
    pthread_barrier_t* final_bar;
    int* intr_che;
    int** block_id_intr_che;
    int* intr_mem;
    int thread_id;
    int* input_box;
    int* output_box;
    int pc;
    int che_fails;

    if_thread();
    void run (void* data);
};

#endif // IF_THREAD_H
