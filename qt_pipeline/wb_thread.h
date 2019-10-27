#ifndef WB_THREAD_H
#define WB_THREAD_H
#include "thread"
#include <iostream>

class wb_thread
{
public:
    int* input_box;
    int** rgstrs;
    int** rgstrs_state;
    pthread_barrier_t* master_bar;
    pthread_barrier_t* final_bar;
    pthread_barrier_t* wb_id_coord;

    wb_thread();
    void run (void* data);
};

#endif // WB_THREAD_H
