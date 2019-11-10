#ifndef EX_THREAD_H
#define EX_THREAD_H
#include "thread"
#include "barrier.h"
#include <iostream>

class ex_thread
{
public:
    int end_of_program=0;///<Bandera que controla si aun quedan hilos por correr
    bool branch_result = false; ///<Bandera que indica si un branch fue tomado o no
    Barrier* master_bar; ///<Barrera con la cual se le da tiempo a EX de ejecutarse
    Barrier* final_bar; ///<Barrera que permite al master realizar entrega de correos
    int thread_id=3; ///<Identificador del thread.
    int* input_box; ///<Buzon de mensajes de entrada (Registros ID/EX)
    int* output_box; ///<Buzon de mensajes de salida (Registros EX/MEM)

    /**
     * @brief ex_thread: Constructor de EX thread
     */
    ex_thread();

    /**
     * @brief run: Primera rutina de ejecución de un thread
     * @param data: Elemento para pasar datos al thread cuando inicia
     */
    void run ();

    /**
     * @brief executePhase: Rutina que lee el código de operación del IR y ejecuta la instrucción respectiva, luego pasa el IR a MEM
     */
    void executePhase();

    /**
     * @brief passInstrToMem: Rutina que pasa el IR a MEM
     */
    void passInstrToMem();

    /**
     * @brief addi: Rutina que ejecuta la instrucción addi. Suma Imm y registro A y lo pasa a ALU Out
     */
    void addi();

    /**
     * @brief add: Rutina que ejecuta la instrucción add. Suma registro A y B y lo pasa a ALU Out
     */
    void add();

    /**
     * @brief sub: Rutina que ejecuta la instrucción sub. Resta registro A y B y lo pasa a ALU Out
     */
    void sub();

    /**
     * @brief mul: Rutina que ejecuta la instrucción mul. Multiplica registro A y B y lo pasa a ALU Out
     */
    void mul();

    /**
     * @brief div: Rutina que ejecuta la instrucción div. Divide registro A y B y lo pasa a ALU Out
     */
    void div();

    /**
     * @brief lw: Rutina que ejecuta la instrucción lw. Calcula la direccion a consultar sumando el registro A e Imm
     */
    void lw();

    /**
     * @brief lr: Rutina que ejecuta la instrucción lr. Pasa la direccion en A a ALU Out
     */
    void lr();

    /**
     * @brief sw: Rutina que ejecuta la instrucción sw. Calcula la direccion a escribir sumando el registro A e Imm y pasa el registro B a MEM
     */
    void sw();

    /**
     * @brief sc: Rutina que ejecuta la instrucción sc. Calcula la direccion a escribir sumando el registro A e Imm y pasa el registro B a MEM
     */
    void sc();

    /**
     * @brief beq: Rutina que ejecuta la instrucción beq. Compara si los registro A y B son iguales, activa branch_result en caso de serlo y pasa el PC branch a IF
     */
    void beq();

    /**
     * @brief bne: Rutina que ejecuta la instrucción bne. Compara si los registro A y B son distintos, activa branch_result en caso de serlo y pasa el PC branch a IF
     */
    void bne();

    /**
     * @brief jal: Rutina que ejecuta la instrucción jal. Activa branch_result como si fuera un branc_tomado y pasa el PC branch a IF
     */
    void jal();

    /**
     * @brief jalr: Rutina que ejecuta la instrucción jalr. Activa branch_result como si fuera un branc_tomado, pasa el valor del registro A a ALU Out y pasa el PC branch a IF
     */
    void jalr();

};

#endif // EX_THREAD_H
