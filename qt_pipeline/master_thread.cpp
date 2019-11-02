#include "master_thread.h"
#include <fstream>

using namespace std;
master_thread::master_thread()
{

}

void master_thread::run(void *data){
    read_threadies(if_p->intr_mem);
    init_mail_inboxes();
    final_bar->Wait(); //Aqui estan esperando todos los threads a que se les inicialice sus valores
    execute_phase();
}

void master_thread::read_threadies(int *instructionVector)
{
    int vecCounter = 0;
    for (int i = 0; i < 7; i++) {
        string filename = "HilillosPruebaFinal/" + to_string(i) + ".txt";
        ifstream infile(filename);
        int byte1, byte2, byte3, byte4;
        while(infile >> byte1 >> byte2 >> byte3 >> byte4)
        {
            instructionVector[vecCounter] = byte1;
            instructionVector[vecCounter+1] = byte2;
            instructionVector[vecCounter+2] = byte3;
            instructionVector[vecCounter+3] = byte4;
            vecCounter+=4;
        }
    }
}


void master_thread::init_mail_inboxes()
{
    //If inbox
    if_p->input_box[0]=0;//Estado de ID
    if_p->input_box[1]=-1;//El pc branch debe ser -1 lo que indica que no debe ser tomado
    //Falta el contador de resolucion de fallos

    //Id inbox
    id_p->input_box[0]=1;//Nop que no resta
    id_p->input_box[4]=0;//Estado de ex

    //Ex inbox
    ex_p->input_box[0]=1;//Nop que no resta
    ex_p->input_box[8]=0;//Estado de mem

    //Mem inbox
    mem_p->input_box[0]=1;//Nop que no resta

    //Wb inbox
    wb_p->input_box[0]=1;//Nop que no resta


}

void master_thread::execute_phase()
{
    master_bar->Wait();
    //Leer de atras hacia adelante

}

void master_thread::deliver_if()
{
    //Se debe preguntar primero si esta en fallo de cache
    if_p->input_box[0]=id_p->output_box[4];
    if_p->input_box[1]=ex_p->output_box[4];
}

void master_thread::deliver_id()
{

}

void master_thread::deliver_ex()
{

}

void master_thread::deliver_mem()
{

}

void master_thread::deliver_wb(){
    update_op_cod(mem_p->output_box,wb_p->input_box);
    wb_p->input_box[4]=mem_p->output_box[4];
    wb_p->input_box[5]=mem_p->output_box[5];
}

void master_thread::update_op_cod(int *source_mail_box, int *dest_mail_box)
{
    for(int i=0;i<4;i++){
        dest_mail_box[i]=source_mail_box[i];
    }

}



