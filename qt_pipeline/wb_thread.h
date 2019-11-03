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
    int clock_ticks=0;
    int end_of_program=0; ///< Bandera que se usa para saber si se debe terminar el ciclo while de mem

    /**
     * @brief wb_thread Constructor de master thread
     */
    wb_thread();

    /**
     * @brief run: Primera rutina de ejecucion de un thread
     * @param data: Elemento para pasar datos al thread cuando inicia
     */
    void run (void* data);

    void execute_pahse();

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

#endif // WB_THREAD_H
