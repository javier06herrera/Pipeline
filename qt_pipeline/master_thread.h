#ifndef MASTER_THREAD_H
#define MASTER_THREAD_H
#include "thread"
#include "if_thread.h"
#include "id_thread.h"
#include "ex_thread.h"
#include "mem_thread.h"
#include "wb_thread.h"
#include "barrier.h"
#include <iostream>

/**
 * @brief The master_thread class Se encarga de administrar las comunicacion entre etapas
 * cambios de contexto y resolucion de casos de conflicto
 */
class master_thread
{
public:
    int quantum_finished=0;
    int quatum_counter=0;
    int itrn_che_fails=0;
    int dta_che_fails=0;
    int thread_id=0;

    if_thread* if_p; ///<Puntero a objeto en el que corre if
    id_thread* id_p;///<Puntero a objeto en el que corre id
    ex_thread* ex_p;///<Puntero a objeto en el que corre ex
    mem_thread* mem_p;///<Puntero a objeto en el que corre mem
    wb_thread* wb_p;///<Puntero a objeto en el que corre wb

    Barrier* master_bar;///<Barrera con la cual se le da tiempo a los threads de ejecutar etapa
    Barrier* final_bar;///<Barrera que permite al master realizar entrega de correos

    //Puntero a lista de hilillos
    /**
     * @brief master_thread Constructor de master thread
     */
    master_thread();

    /**
     * @brief run: Primera rutina de ejecucion de un thread
     * @param data: Elemento para pasar datos al thread cuando inicia
     */
    void run (void* data);

    /**
     * @brief readThreadies: Lee los threads y los guarda en la memoria de instrucciones
     * @param instructionVector: Puntero a vector de memoria
     */
    void read_threadies(int *intr_mem);

    /**
     * @brief init_mail_boxes Le asigna los valores por defecto a cada uno de los buzones
     */
    void init_mail_inboxes();

    /**
     * @brief execute_phase Rutina normal del master thread
     */
    void execute_phase();

    /**
     * @brief deliver_if: Actualiza inbox de if
     * Estructura del input box:
     * | Estado de ID | PC Branch |
     * Estructura del output box:
     * | Cod Op | PC |
     */
    void deliver_if();

    /**
     * @brief deliver_id: Actualiza inbox de id
     * Estructura del input box:
     * | Cod Op | Estado EX | PC Normal |
     * Estructura del output box:
     * | Cod Op | Estado ID | A | B | Imm | PC Normal |
     */
    void deliver_id();

    /**
     * @brief deliver_ex: Actualiza inbox de ex
     * Estructura del input box:
     * | Cod Op | A | B | Imm | PC Branch | Estado de Mem |
     * Estructura del output box:
     * | Cod Op | PC Branch | ALU OUT | B |
     */
    void deliver_ex();

    /**
     * @brief deliver_mem: Actualiza inbox de mem
     * Estructura del input box:
     * | Cod Op | ALU OUT | B |
     * Estructura del output box:
     * | Cod Op | ALU OUT | LMD |
     */
    void deliver_mem();

    /**
     * @brief deliver_wb: Actualiza inbox de wb
     * Estructura del input box:
     * | Cod Op | ALU OUT | LMD |
     */
    void deliver_wb();

    /**
     * @brief bring_op_cod Actualiza los cuatro digitos del codigo de operacion
     */
    void update_op_cod(int* source_mail_box, int *dest_mail_box);




};

#endif // MASTER_THREAD_H
