#ifndef MASTER_THREAD_H
#define MASTER_THREAD_H
#include "thread"
#include "if_thread.h"
#include "id_thread.h"
#include "ex_thread.h"
#include "mem_thread.h"
#include "wb_thread.h"
#include <iostream>

class master_thread
{
public:
    int quantum_finished;
    int quatum_counter;
    int itrn_che_fails;
    int dta_che_fails;

    if_thread* if_data;
    id_thread* id_data;
    ex_thread* ex_data;
    mem_thread* mem_data;
    wb_thread* wb_data;

    pthread_barrier_t* master_bar;
    pthread_barrier_t* final_bar;

    //Puntero a lista de hilillos

    master_thread();
    void readThreadies(int *instructionVector);
    void run (void* data);
};

#endif // MASTER_THREAD_H
