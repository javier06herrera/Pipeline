#include <iostream>
#include "master_thread.h"
#include "if_thread.h"
#include "id_thread.h"
#include "ex_thread.h"
#include "mem_thread.h"
#include "wb_thread.h"
#include "barrier.h"

using namespace std;
void init_vector(int* vector, int size, int value){
    for(int i=0; i<size;i++ )
        vector[i]=value;
}

void init_structures(if_thread* if_p, id_thread* id_p, ex_thread* ex_p, mem_thread* mem_p, wb_thread* wb_p, master_thread* master_p){
    //Datos y estructuras compartidas
    Barrier* master_bar=new Barrier(6);
    Barrier* final_bar = new Barrier(6);
    Barrier* wb_id_coord = new Barrier(2);
    int* intr_mem=new int[640];
    init_vector(intr_mem,640,0);
    int* data_mem = new int[96];
    init_vector(data_mem,96,1);
    int* rgstrs=new int[33];
    init_vector(rgstrs,32,0);
    rgstrs[32]=-1;
    int* rgstr_state = new int[33];
    init_vector(rgstr_state,33,0);

    //Datos y estructuras if
    if_p->master_bar=master_bar;
    if_p->final_bar=final_bar;
    int* intr_che=new int[64];
    init_vector(intr_che,64,0);
    if_p->intr_che=intr_che;
    //Guarda los identificadores de bloque presentes en la cache de instrucciones
    int* block_id_intr_che=new int[4];
    init_vector(block_id_intr_che,4,-1);
    if_p->block_id_intr_che=block_id_intr_che;
    if_p->intr_mem=intr_mem;
    int* if_input_box=new int[2];
    init_vector(if_input_box,2,0);
    int* if_output_box=new int[5];
    init_vector(if_output_box,5,0);
    if_p->input_box=if_input_box;
    if_p->output_box=if_output_box;

    //Datos y estructuras id
    id_p->rgstrs=rgstrs;
    id_p->rgstrs_state=rgstr_state;
    id_p->master_bar=master_bar;
    id_p->final_bar=final_bar;
    id_p->wb_id_coord=wb_id_coord;
    int* id_input_box=new int[6];
    init_vector(id_input_box,6,0);
    int* id_output_box=new int[9];
    init_vector(id_output_box,9,0);
    id_p->input_box=id_input_box;
    id_p->output_box=id_output_box;

    //Datos y estructuras ex
    ex_p->master_bar=master_bar;
    ex_p->final_bar=final_bar;
    int* ex_input_box=new int[10];
    init_vector(ex_input_box,10,0);
    int* ex_output_box=new int[7];
    init_vector(ex_output_box,7,0);
    ex_p->input_box=ex_input_box;
    ex_p->output_box=ex_output_box;

    //Datos y estructuras mem
    int* data_cache=new int[16];
    init_vector(data_cache,16,0);
    mem_p->data_che=data_cache;
    mem_p->data_mem=data_mem;
    //Guarda los identificadores de bloque presentes en la cache de datos
    int* block_id_dta_che=new int[4];
    init_vector(block_id_dta_che,4,-1);
    mem_p->block_id_dta_che=block_id_dta_che;
    int* mem_input_box=new int[6];
    init_vector(mem_input_box,6,0);
    int* mem_output_box=new int[7];
    init_vector(mem_output_box,7,0);
    mem_p->input_box=mem_input_box;
    mem_p->output_box=mem_output_box;
    mem_p->master_bar=master_bar;
    mem_p->final_bar=final_bar;

    //Datos y estructuras wb
    int* wb_input_box=new int[6];
    init_vector(wb_input_box,6,0);
    wb_p->input_box=wb_input_box;
    wb_p->rgstrs=rgstrs;
    wb_p->rgstrs_state=rgstr_state;
    wb_p->master_bar=master_bar;
    wb_p->final_bar=final_bar;
    wb_p->wb_id_coord=wb_id_coord;

    //Datos y estructuras master
    master_p->if_p=if_p;
    master_p->id_p=id_p;
    master_p->ex_p=ex_p;
    master_p->mem_p=mem_p;
    master_p->wb_p=wb_p;
    master_p->master_bar=master_bar;
    master_p->final_bar=final_bar;
    //string user_quantum;
    //cin>>master_p->quantum_value;
    master_p->quantum_value=1000;

}


int main()
{


    if_thread* if_p=new if_thread();
    id_thread* id_p=new id_thread();
    ex_thread* ex_p=new ex_thread();
    mem_thread* mem_p= new mem_thread();
    wb_thread* wb_p= new wb_thread();
    master_thread* master_p=new master_thread();

    init_structures(if_p,id_p,ex_p,mem_p,wb_p,master_p);

    thread master_t(&master_thread::run, master_p);
    thread if_t(&if_thread::run, if_p);
    thread id_t(&id_thread::run, id_p);
    thread ex_t(&ex_thread::run, ex_p);
    thread mem_t(&mem_thread::run, mem_p);
    thread wb_t(&wb_thread::run, wb_p);

    if_t.join();
    id_t.join();
    ex_t.join();
    mem_t.join();
    wb_t.join();
    master_t.join();
    //Array erasing
    delete [] if_p->intr_mem;
    delete [] if_p->block_id_intr_che;
    delete [] if_p->input_box;
    delete [] if_p->output_box;
    delete [] if_p->intr_che;
    delete [] id_p->rgstrs;
    delete [] id_p->rgstrs_state;
    delete [] id_p->input_box;
    delete [] id_p->output_box;
    delete [] ex_p->input_box;
    delete [] ex_p->output_box;
    delete [] mem_p->data_che;
    delete [] mem_p->block_id_dta_che;
    delete [] mem_p->data_mem;
    delete [] mem_p->input_box;
    delete [] mem_p->output_box;
    delete [] wb_p->input_box;

    //Barrier erasing
    delete master_p->final_bar;
    delete master_p->master_bar;
    delete id_p->wb_id_coord;

    //Stages erasing
    delete master_p;
    delete if_p;
    delete id_p;
    delete ex_p;
    delete mem_p;
    delete wb_p;

    //Tentativamente hcer delete de la estructura de contextos

    int a;
    cin >> a;
    return 0;
}


