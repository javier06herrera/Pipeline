#include <iostream>
#include "master_thread.h"
#include "if_thread.h"
#include "id_thread.h"
#include "ex_thread.h"
#include "mem_thread.h"
#include "wb_thread.h"
#include "barrier.h"

using namespace std;

void init_structures(if_thread* if_p, id_thread* id_p, ex_thread* ex_p, mem_thread* mem_p, wb_thread* wb_p, master_thread* master_p){
    //Datos y estructuras compartidas
    Barrier* master_bar=new Barrier(6);
    Barrier* final_bar = new Barrier(6);
    Barrier* wb_id_coord = new Barrier(2);
    int intr_mem[640]={0};
    int data_mem[96]={1};
    int rgstrs[33]={0};
    int rgstr_state[33]={0};

    //Datos y estructuras if
    if_p->master_bar=master_bar;
    if_p->final_bar=final_bar;
    int intr_che[64]={0};
    if_p->intr_che=intr_che;
    //Guarda los identificadores de bloque presentes en la cache de instrucciones
    int block_id_intr_che[4]={-1};
    if_p->block_id_intr_che=block_id_intr_che;
    if_p->intr_mem=intr_mem;
    int if_input_box[2]={0};
    int if_output_box[5]={0};
    if_p->input_box=if_input_box;
    if_p->output_box=if_output_box;

    //Datos y estructuras id
    id_p->rgstrs=rgstrs;
    id_p->rgstrs_state=rgstr_state;
    id_p->master_bar=master_bar;
    id_p->final_bar=final_bar;
    id_p->wb_id_coord=wb_id_coord;
    int id_input_box[6]={0};
    int id_output_box[9]={0};
    id_p->input_box=id_input_box;
    id_p->output_box=id_output_box;

    //Datos y estructuras ex
    ex_p->master_bar=master_bar;
    ex_p->final_bar=final_bar;
    int ex_input_box[10]={0};
    int ex_output_box[7]={0};
    ex_p->input_box=ex_input_box;
    ex_p->output_box=ex_output_box;

    //Datos y estructuras mem
    int data_cache[16]={0};
    mem_p->data_che=data_cache;
    mem_p->data_mem=data_mem;
    //Guarda los identificadores de bloque presentes en la cache de datos
    int block_id_dta_che[4]={-1};
    mem_p->block_id_dta_che=block_id_dta_che;
    int mem_input_box[6]={0};
    int mem_output_box[7]={0};
    mem_p->input_box=mem_input_box;
    mem_p->output_box=mem_output_box;
    mem_p->master_bar=master_bar;
    mem_p->final_bar=final_bar;

    //Datos y estructuras wb
    int wb_input_box[6]={0};
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

    thread master_t(&master_thread::run, master_p, nullptr);
    thread if_t(&if_thread::run, if_p, nullptr);
    thread id_t(&id_thread::run, id_p, nullptr);
    thread ex_t(&ex_thread::run, ex_p, nullptr);
    thread mem_t(&mem_thread::run, mem_p, nullptr);
    thread wb_t(&wb_thread::run, wb_p, nullptr);

    if_t.join();
    id_t.join();
    ex_t.join();
    mem_t.join();
    wb_t.join();
    master_t.join();

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

    return 0;
}


