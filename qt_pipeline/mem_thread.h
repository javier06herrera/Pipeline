#ifndef MEM_THREAD_H
#define MEM_THREAD_H
#include "thread"
#include <iostream>

class mem_thread
{
public:
    int* data_che;
    int* data_mem;
    int* block_id_dta_che;
    int* input_box;
    int* output_box;
    pthread_barrier_t* master_bar;
    pthread_barrier_t* final_bar;
    int read_che_fails=0;
    int write_che_fails=0;
    int read_mem_rqst=0;
    int write_mem_rqst=0;
    int thread_id=4;
    mem_thread();
    void run (void* data);
};

#endif // MEM_THREAD_H
