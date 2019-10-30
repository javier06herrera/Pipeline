#ifndef ID_THREAD_H
#define ID_THREAD_H
#include <iostream>
#include "barrier.h"
#include "thread"

class id_thread
{
public:
    int* rgstrs;
    int* rgstrs_state;
    int thread_id=2;
    Barrier* master_bar;
    Barrier* final_bar;
    Barrier* wb_id_coord;
    int* input_box;
    int* output_box;

    id_thread();
    void run (void* data);
};

#endif // ID_THREAD_H
