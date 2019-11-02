#ifndef MEM_THREAD_H
#define MEM_THREAD_H
#include "thread"
#include <iostream>
#include "barrier.h"

class mem_thread
{
public:
    bool in_cache_fail_store = false;
    bool in_cache_fail_load = false;
    int* data_che;
    int* data_mem;
    int* block_id_dta_che;
    int* input_box;
    int* output_box;
    Barrier* master_bar;
    Barrier* final_bar;
    int read_che_fails=0;
    int write_che_fails=0;
    int read_mem_rqst=0;
    int write_mem_rqst=0;
    int thread_id=4;
    mem_thread();
    void run (void* data);

    int memAddressToBlock(int address);
    int memAddressToWord(int address);
    int getIndexInDataMemory(int address);
    int getIndexInDataCache(int address);
    bool isBlockInDataCache(int address);
    void loadBlockToCache(int address);

    void executePhase(); //Ejecuta la etapa evaluando el IR.

    void passALUOutToWB();
    void passInstrToWB();

    void lw();
    void sw();
    void lr();
    void sc();
};

#endif // MEM_THREAD_H
