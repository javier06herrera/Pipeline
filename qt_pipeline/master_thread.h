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
#include <queue>
/**
 * @brief The PCB struct guarda el contexto de un hilillo
 */
struct PCB{
    int rgstrs[33]={0};
    int rgstrs_state[33]={0};
    int PC;
    int thread_id;
};

/**
 * @brief The master_thread class Se encarga de administrar las comunicacion entre etapas
 * cambios de contexto y resolucion de casos de conflicto
 */
class master_thread
{
public:
    int current_threadie = -1;
    int end_of_program=0;///<Bandera que controla si aun quedan hilos por correr
    int threadie_finished=0;///<Se usa para saber si el cambio de contexto fue por final de hilillo o final de quantum
    int swt_ctxt_flg=0;///<Bandera que indica si el quantum terminó
    int quantum_value=0;///<Valor de quantum que ingresa el usuario
    int itrn_che_fails=0;///<Contador de fallos de cache de instrucciones
    int dta_che_fails=0;///<Contador de fallos de cache de datos
    int overwrite_cycles=2; ///<Contador de cuantas instrucciones se van a sobreescribir en caso de branch tomado
    int thread_id=0;///<Identificador del thread
    queue<PCB> context_list;///<Cola donde se guardan los contextos

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
     * @brief update_op_cod Actualiza los cuatro digitos del codigo de operacion
     * @param source_mail_box Puntero a arreglo de donde se toma la instruccion
     * @param dest_mail_box Puntero a arreglo en donde se escribe la instruccion
     */
    void update_op_cod(int* source_mail_box, int *dest_mail_box);

    /**
     * @brief pass_NOP Pasa NOPs a una de las etapas del pipeline
     * @param accountableNOP Esta bandera indica si el NOP le resta o no al quantum (1-si/0-no)
     * @param dest_mail_box Puntero a arreglo en donde se escribe la instruccion
     */
    void pass_NOP(int accountableNOP,int *dest_mail_box);
    /**
     * @brief switch_context Metodo que se encarga de hacer los cambios de contexto agregando a la cola un nuevo contexto y sacando el siguiente
     * @param type
     * 0: El cambio de contexto es normal
     * 1: El cambio de contexto es por final de hilillo
     * @return Determina si aun quedan hilillos en la cola de contextos
     * 0: Aun quedan
     * 1: Esta vacia la cola
     */
    int switch_context(int type);
    /**
     * @brief reset_variables Se encarga de volver las variables de control pertinentes luego de un cambio de contexto
     */
    void reset_variables();



};

#endif // MASTER_THREAD_H
