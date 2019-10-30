#ifndef WB_THREAD_H
#define WB_THREAD_H
#include "thread"
#include <iostream>
#include "barrier.h"

class wb_thread
{
public:
    int* input_box;
    int* rgstrs;
    int* rgstrs_state;
    Barrier* master_bar;
    Barrier* final_bar;
    Barrier* wb_id_coord;
    int thread_id=5;

    wb_thread();
    void run (void* data);
};

#endif // WB_THREAD_H
