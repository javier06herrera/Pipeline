#ifndef ID_THREAD_H
#define ID_THREAD_H
#include <iostream>
#include "thread"

class id_thread
{
public:
    int* rgstrs;
    int* rgstrs_state;
    int thread_id=2;
    pthread_barrier_t* master_bar;
    pthread_barrier_t* final_bar;
    pthread_barrier_t* wb_id_coord;
    int* input_box;
    int* output_box;

    id_thread();
    void run (void* data);
};

#endif // ID_THREAD_H
