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
    int quantum_finished=0;
    int quatum_counter=0;
    int itrn_che_fails=0;
    int dta_che_fails=0;
    int thread_id=0;

    if_thread* if_p;
    id_thread* id_p;
    ex_thread* ex_p;
    mem_thread* mem_p;
    wb_thread* wb_p;

    pthread_barrier_t* master_bar;
    pthread_barrier_t* final_bar;

    //Puntero a lista de hilillos

    master_thread();
    void readThreadies(int *instructionVector);
    void run (void* data);
};

#endif // MASTER_THREAD_H
