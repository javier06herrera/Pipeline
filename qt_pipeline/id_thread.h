#ifndef ID_THREAD_H
#define ID_THREAD_H
#include <iostream>
#include "barrier.h"
#include "thread"

/**
\brief La clase id_thread se encarga de decodificar la nueva instrucción,
y interpretarla según corresponda. También debe preveer conflictos de datos.
 */
class id_thread
{
public:
    int end_of_program=0;///<Bandera que controla si aun quedan hilos por correr
    int* rgstrs; ///< Son los registros principales del procesador
    int* rgstrs_state; ///< Señala los registros que se están usando como destino.
    int thread_id=2; ///< Identificador de hilo.
    Barrier* master_bar; ///< Barrera con el hilo master
    Barrier* final_bar; ///< Barrera de final de ciclo
    Barrier* wb_id_coord; ///< barrera de medio ciclo para funcionamiento de wb
    int* input_box; ///< buzón de entrada
    int* output_box; ///< buzón de salida

    id_thread();
    /**
     * @brief run: Primera rutina de ejecución de un thread.
     * @param data: Elemento para pasar datos al thread cuando inicia
     */
    void run ();
    /**
    \brief instr_decode: Se encarga en que formato de decodificarse la instrucción.
    \returns Retorna 1 si se realizó normalmente, de lo contrario 0.
    */
    int instr_decode();
    /**
    \brief load_instr: Se encarga de cargar la instrucción desde input_box hacia
    output_box.
    \returns Retorna 1 si se realizó normalmente, de lo contrario 0.
    */
    int load_instr();
    /**
    \brief addi: comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), luego reserva
    el registro destino, y coloca el operando A y el inmediato en su lugar respectivo:

    input_box[1] = registro destino.
    output_box[5]<--------rgstrs[ input_box[2] ] = registro A.
    output_box[8]<--------input_box[3] = inmediato.
    */
    void addi();
    /**
    \brief branch: comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), luego coloca A,
    B y calcula el PC branch y lo envía.

    output_box[5]<--------rgstrs[ input_box[1] ] = registro A.
    output_box[6]<--------rgstrs[ input_box[2] ] = registro B.
    output_box[8]<--------PC branch
    */
    void branch();
    /**
    \brief op_with_inm: comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), reserva el registro
    destino y coloca el contenido de los registros en los operandos A y B:

    input_box[1] = registro destino.
    output_box[5]<--------rgstrs[ input_box[2] ] = registro A.
    output_box[6]<--------rgstrs[ input_box[3] ] = registro B.
    */
    void op_arithm();
    /**
    \brief lw: comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), luego
    reserva el registro destino y coloca el contenido del registro señalado en
    el operando A y el inmediato respectivamente:

    input_box[1] = registro destino.
    output_box[5]<--------rgstrs[ input_box[2] ] = registro A.
    output_box[7]<--------input_box[3] = inmediato.
    */
    void lw();
    /**
    \brief sw: comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), coloca el
    contenido de los registros en los operandos A, B y el inmediato respectivamente:

    output_box[5]<--------rgstrs[ input_box[1] ] = registro A.
    output_box[6]<--------rgstrs[ input_box[2] ] = registro B.
    output_box[7]<--------input_box[3] = inmediato.
    */
    void sw();
    /**
    \brief lr: comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), luego reserva
    el registro destino, y coloca el contenido del registro de input_box en el operando
    A:

    input_box[1] = registro destino.
    output_box[5]<--------rgstrs[ input_box[2] ] = registro A.
    */
    void lr();
    /**
    \brief sc: comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), coloca desde
    el contenido de los registros los operandos A, B y el RL:

    input_box[2] = posible registro destino.
    output_box[5]<--------rgstrs[ input_box[1] ] = registro A.
    output_box[6]<--------rgstrs[ input_box[2] ] = registro B.
    output_box[9]<--------rgstrs[ 32 ] = registro RL.
    output_box[7]<--------input_box[3] = inmediato.
    */
    void sc();
    /**
    \brief jal: comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), calcula el
    PC branch y lo coloca junto con el operando A:

    output_box[8]<--------input_box[3] + input_box[5] = PC branch.
    */
    void jal();
    /**
    \brief jalr:  comprueba que no haya un conflicto de datos (si lo hay envía NOP's
    de conflicto de datos y enciende la bandera de conflicto de datos), reserva el
    registro destino y callcula el PC branch:

    input_box[1] : registro destino.
    output_box[5]<--------inputbox[5] : registro A/PC normal.
    output_box[8]<--------rgstrs[input_box[2]] + input_box[3] : PC branch.
    */
    void jalr();
    /**
    \brief send_NOP: carga al output_box, una instrucción de NOP de conflicto de
    datos.
    */
    void send_NOP(int type);
    /**
    \brief check_status: Verifica que el registro ingresado no esté siendo utilizado
    por una instrucción posterior.
    \param regist: Registro a comprobar estado.
    \returns Retorna 1 si está siendo utilizado, de lo contrario 0.
    */
    int check_status(int regist);
    /**
    \brief use_rgstr: marca el registro como destino de la instrucción.
    \param regist: Registro a marcar.
    */
    void use_rgstr(int regist);
};

#endif // ID_THREAD_H
