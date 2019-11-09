#ifndef IF_THREAD_H
#define IF_THREAD_H
#include "thread"
#include <unistd.h>
#include <iostream>
#include "barrier.h"

/**
 * @brief The if_thread class se encarga de cargar la nueva instrucción, resolver fallos
 * de caché y enviar NOP's de fallo de caché, asi como cargar bloques desde la
 * memoria de instrucciones hasta la caché de instrucciones.
 */
class if_thread
{
public:
    int sent=0;///<Controla que solo pase una instruccion de cambio de contexto y no varias
    int fail_cycle; ///< Indica los ciclos de fallos restantes a resolver
    int swt_ctxt_flg=0;///<Bandera que identifica si e quantum se acabo
    Barrier* master_bar; ///< Barrera para que empiece a trabajar el hilo master.
    Barrier* final_bar; ///< Barrera final de ejecución.
    int* intr_che; ///< Caché de instrucciones.
    int* block_id_intr_che; ///< Identifica los bloques cargados en la caché.
    int* intr_mem; ///< Memoria de instrucciones.
    int thread_id=1; ///< Número identificador de hilo.
    int* input_box; ///< Buzon utilizado para recibir pc_branch en el [0] y el nuevo pc en el [1].
    int* output_box; ///< Buzón usado para enviar datos a ID.
    int pc=0; ///< Guarda el contador de la instrucción a extraer.
    int che_fails=0; ///< Contabiliza la cantidad de fallos de caché que se han generado.
    int mem_request = 0; ///< Contabiliza las solicitudes de acceso a memoria.

    if_thread();
    /**
     * @brief run: Primera rutina de ejecución de un thread.
     * @param data: Elemento para pasar datos al thread cuando inicia
     */
    void run(void* data);
    /**
    \brief instr_fetch: Se encarga de hacer todos los llamados para completar la
    ejecución de la etapa IF.
    \returns Retorna 1 si no hubo fallo, de lo contrario 0.
    */
    int instr_fetch();
    /**
    \brief send_NOP: Se envía al output_box un nop de fallo de caché.
    */
    void send_NOP(int type);
    /**
    \brief activate_fail: Activa el fallo de caché por la cantidad de ciclos en los
    cuales va a durar. También envía un NOP de fallo de caché a la
    siguiente instrucción.
    */
    void activate_fail();
    /**
    \brief branch_cmp: comprueba si se debe cambiar el valor del pc por el calculado
    y comprobado en ID y EX respectivamente.
    \returns Retorna 0 en caso de que no se tenga que saltar y 1 en caso de que sí.
    */
    int branch_cmp();
    /**
    \brief work_fail: Concluye un ciclo en la resolución del fallo de caché y
    envía un NOP de fallo de caché a la siguiente instrucción.
    */
    void work_fail();
    /**
    \brief extract_from_che: Extrae de la caché la instrucción señalada y la envía al
    buzón. Para esto calcula el número de bloque en la caché, y se
    desplaza la cantidad de palabras que se señalan.
    \param num_blk: Numero de bloque de la instrucción.
    \param num_word: Número de palabra de la instrucción.
    \returns Retorna la instrucción presente en la caché.
    */
    void extract_from_che(int num_blk, int num_word);
    /**
    \brief resolve_fault: Carga en caché el bloque de memoria ausente. Primero obtiene
    el número de bloque en la memoria de instrucciones y luego el de la
    caché de instrucciones. Después remplaza el bloque en la caché por
    el señalado en la memoria de instrucciones.
    \param num_blk: Numero de bloque de la instrucción.
    \param num_word: Número de palabra de la instrucción.
    */
    void resolve_fault(int num_blk, int num_word);
    /**
    \brief exists: Verifica si el bloque está cargado en la caché de instrucciones.
    \param num_blk: Numero de bloque a comprobar.
    \returns Retorna 1 si estaba, o 0 de lo contrario.
    */
    int exists(int num_blk);
    /**
    \brief addr_to_block: Toma la dirección dada en el pc y la convierte al número de
    bloque en memoria de instrucciones.
    \returns Retorna el número de bloque de memoria de instrucciones.
    */
    int addr_to_block(int p_c);
    /**
    \brief addr_to_word: Toma la dirección dada en el pc y la convierte al número de
    palabra en el bloque de instrucciones
    \returns Retorna el número de palabra en el bloque de instrucciones.
    */
    int addr_to_word(int p_c);

};

#endif // IF_THREAD_H
