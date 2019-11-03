#ifndef MEM_THREAD_H
#define MEM_THREAD_H
#include "thread"
#include <iostream>
#include "barrier.h"

/**
 * @brief The mem_thread class se encarga principalmente de manejar los accesos a la
 * memoria compartida de datos. También resuelve fallos de caché con la estrategia de
 * Write-Through para aciertos de escritura y Write No-Allocate para fallos de escritura.
 */
class mem_thread
{
public:
    bool in_cache_fail_store = false; ///<Bandera de aviso que ocurrió un fallo de caché por escritura(12 ciclos)
    bool in_cache_fail_load = false; ///<Bandera de aviso que ocurrió un fallo de caché por lectura(48 ciclos)
    int cache_fail_cycles = 0;///< Contador de los ciclos de solucion de fallo de cache que se llevan hasta el momento
    int* data_che; ///<Puntero al vector de la memoria caché
    int* data_mem; ///<Puntero al vector de memoria compartida de datos
    int* block_id_dta_che; ///<Puntero al vector de identificadores de los bloques actualmente cargados en caché
    int* input_box; ///<Buzon de mensajes de entrada (Registros EX/MEM)
    int* output_box; ///<Buzon de mensajes de salida (Registros MEM/WB)
    Barrier* master_bar; ///<Barrera con la cual se le da tiempo a MEM de ejecutarse
    Barrier* final_bar; ///<Barrera que permite al master realizar entrega de correos
    int read_che_fails=0; ///<Contador de la cantidad de fallos de caché registrados por lecturas(loads)
    int write_che_fails=0; ///<Contador de la cantidad de fallos de caché registrados por escrituras(stores)
    int read_mem_rqst=0; ///<Contador de la cantidad de solicitudes de lectura en memoria
    int write_mem_rqst=0; ///<Contador de la cantidad de solicitudes de escritura en memoria
    int thread_id=4; ///<Identificador del thread.


    /**
     * @brief mem_thread: Constructor del MEM thread.
     */
    mem_thread();

    /**
     * @brief run: Primera rutina de ejecución de un thread.
     * @param data: Elemento para pasar datos al thread cuando inicia
     */
    void run (void* data);

    /**
     * @brief memAddressToBlock: Función que transforma la dirección dada en la instrucción a su respectivo número de bloque
     * @param address: Dirección proveniente de la instrucción
     * @return Número de bloque
     */
    int memAddressToBlock(int address);

    /**
     * @brief memAddressToWord: Función que transforma la dirección dada en la instrucción a su respectivo número de palabra
     * @param address: Dirección proveniente de la instrucción
     * @return Número de palabra
     */
    int memAddressToWord(int address);

    /**
     * @brief getIndexInDataMemory: Función que transforma la dirección dada en la instrucción a su respectivo índice en el vector de memoria compartida de datos
     * @param address: Dirección proveniente de la instrucción
     * @return Índice en el vector de memoria compartida de datos.
     */
    int getIndexInDataMemory(int address);

    /**
     * @brief getIndexInDataCache: Función que transforma la dirección dada en la instrucción a su respectivo índice en el vector de memoria caché
     * @param address: Dirección proveniente de la instrucción
     * @return Índice en el vector de memoria caché
     */
    int getIndexInDataCache(int address);

    /**
     * @brief isBlockInDataCache: Función que retorna si un bloque está actualmente cargado en caché consultando el vector de identificadores de bloques
     * @param address: Dirección proveniente de la instrucción
     * @return True si el bloque está en caché y False si no
     */
    bool isBlockInDataCache(int address);

    /**
     * @brief loadBlockToCache: Rutina que carga el bloque calculado con la dirección a la memoria caché en su respectivo lugar.
     * @param address: Dirección proveniente de la instrucción
     */
    void loadBlockToCache(int address);

    /**
     * @brief executePhase: Rutina que lee el código de operación del IR y ejecuta la instrucción respectiva, luego pasa el IR a WB
     */
    void executePhase(); //Ejecuta la etapa evaluando el IR.

    /**
     * @brief passALUOutToWB: Rutina que copia el ALU Out de los registros intermedios EX/MEM a los registros intermedios MEM/WB
     */
    void passALUOutToWB();

    /**
     * @brief passInstrToWB: Rutina que pasa el IR a WB
     */
    void passInstrToWB();

    /**
     * @brief passNOPsToWB: Rutina que pasa instrucciones NOPs que no cuentan para el quantum a WB cuando MEM está en fallo de caché
     */
    void passNOPsToWB();

    /**
     * @brief lw: Rutina que ejecuta la instrucción lw. Revisa si el bloque está cargado en la caché,
     * si lo está copia el valor de la palabra seleccionada al registro LMD. Si el bloque no está, activa
     * la bandera de fallo de cache por lectura, sube el contador de fallos y carga el bloque respectivo, luego carga el valor al LMD
     */
    void lw();

    /**
     * @brief sw: Rutina que ejecuta la instrucción sw. Revisa si el bloque está cargado en la cache,
     * si lo esta aplica la estrategia de Write-Through escribiendo el valor en cache y tambien directamente
     * en memoria, activando la bandera de fallo para avisar que hay retraso. Si el bloque no está, activa la
     * la bandera de fallo de cache, sube el contador de fallos y escribe directamente en memoria el valor.
     */
    void sw();

    /**
     * @brief lr: Rutina que ejecuta la instrucción lr.
     */
    void lr();

    /**
     * @brief sc: Rutina que ejecuta la instrucción sc.
     */
    void sc();
};

#endif // MEM_THREAD_H
