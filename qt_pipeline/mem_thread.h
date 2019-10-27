#ifndef MEM_THREAD_H
#define MEM_THREAD_H
#include "thread"
#include <iostream>

class mem_thread
{
public:
    int** block_id_dta_che;
    int* input_box;
    int* output_box;
    pthread_barrier_t* master_bar;
    pthread_barrier_t* final_bar;
    int read_che_fails;
    int write_che_fails;
    int read_mem_rqst;
    int write_mem_rqst;
    mem_thread();
    void run (void* data);
};

#endif // MEM_THREAD_H
