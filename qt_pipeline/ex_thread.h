#ifndef EX_THREAD_H
#define EX_THREAD_H
#include "thread"
#include "barrier.h"
#include <iostream>

class ex_thread
{
public:
    bool branch_result = false;
    int  overwrite_cycles=0;
    Barrier* master_bar;
    Barrier* final_bar;
    int thread_id=3;
    int* input_box;
    int* output_box;

    ex_thread();
    void run (void* data);

    void executePhase(); //Ejecuta la etapa evaluando el IR.

    void passInstrToMem(int instruction); //Pasa la instruccion del parametro al buzon IR de MEM.

    void addi();
    void add();
    void sub();
    void mul();
    void div();
    void lw();
    void lr();
    void sw();
    void sc();
    void beq();
    void bne();
    void jal();
    void jalr();

};

#endif // EX_THREAD_H
