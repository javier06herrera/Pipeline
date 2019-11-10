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
    while (!end_of_program)
    {
        master_bar->Wait();
        execute_phase();
        final_bar->Wait();
    }

}

void master_thread::read_threadies(int *instructionVector)
{
    int vecCounter = 0;
    for (int i = 0; i < 7; i++) {
        //No tocar
        //*********************************************
        PCB* context=new PCB();
        context->PC=vecCounter;
        context->thread_id=i;
        context_list.push(*context);
        //*********************************************


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
    deliver_wb();
    deliver_mem();
    deliver_ex();
    deliver_id();
    deliver_if();
    //Pregunta si hay un cambio de contexto que aplicar
    if(wb_p->input_box[0]==3){
        //Decide que escenario de cambio de contexto es
        if(threadie_finished)
            switch_context(1);
        else
            switch_context(0);
    }

}

void master_thread::deliver_if()
{
    if_p->input_box[0]= id_p->output_box[4]; //Se le pasa el estado de ID
    if(if_p->output_box[0]==999)//Pregunta si la ultima instruccion leida en if es la ultima del hilillo
    {
        if_p->swt_ctxt_flg=1;
        threadie_finished=1;
    }
    if(if_p->input_box[0])
    {
        if_p->input_box[1]=-1;//Como ex esta detenido el master es el que le pasa el -1
        return;
    }
    if_p->input_box[1]=ex_p->output_box[4];
}

void master_thread::deliver_id()
{
    id_p->input_box[4]=ex_p->output_box[7];//Siempre hay que pasarle el estado de ex para verificar si puede avanzar
    if(ex_p->output_box[7])// Se comprueba el estado de ex para saber si escribir buzones o no
        return;
    update_op_cod(if_p->output_box,id_p->input_box);
    id_p->input_box[5]=if_p->output_box[4];//Trae el valor del pc para calcular el PC branch
}

void master_thread::deliver_ex()
{
    ex_p->input_box[8]=mem_p->output_box[6];//Siempre hay que pasarle el estado de mem para verificar si puede avanzar

    //Verifica si mem esta detenido por un fallo de cache
    if(ex_p->input_box[8])
        return;
    /*Si el branch resulto verdadero se le debe pasar por dos ciclos NOPs para simular los retrasos de las
    dos instrucciones malas*/
    if(ex_p->branch_result){
        update_op_cod(id_p->output_box,ex_p->input_box);//Se pasa la instruccion branch en el mismo ciclo que se detecta que fue tomado
        overwrite_cycles=0;
        ex_p->branch_result=false;
    }
    else if(overwrite_cycles<2)
    {
        pass_NOP(1,ex_p->input_box);//Estos NOP si cuentan
        overwrite_cycles++;
    }
    else
    {
        update_op_cod(id_p->output_box,ex_p->input_box);
        ex_p->input_box[4]=id_p->output_box[5];//Trae el valor de A
        ex_p->input_box[5]=id_p->output_box[6];//Trae el valor de B
        ex_p->input_box[6]=id_p->output_box[7];//Trae el valor del inmediato
        ex_p->input_box[7]=id_p->output_box[8];//Trae el valor del PC branch
        ex_p->input_box[9]=id_p->output_box[9];//Trae el valor del RL
    }

}

void master_thread::deliver_mem()
{
    //Si mem esta en fallo de cache no se le pasan nuevas instrucciones
    if(mem_p->in_cache_fail_load || mem_p->in_cache_fail_store)
        return;
    update_op_cod(ex_p->output_box,mem_p->input_box);//Se pasa la nueva instruccion
    mem_p->input_box[4]=ex_p->output_box[5];//Trae el ALU OUT
    mem_p->input_box[5]=ex_p->output_box[6];//Trae al operador B

}

void master_thread::deliver_wb(){
    update_op_cod(mem_p->output_box,wb_p->input_box); //Se pasa la nueva instruccion
    wb_p->input_box[4]=mem_p->output_box[4];//Trae el ALU OUT
    wb_p->input_box[5]=mem_p->output_box[5];//Trae a LMD
    //Verificamos si llegamos a final de quantum
    if(wb_p->clock_ticks == quantum_value && swt_ctxt_flg==0){
        swt_ctxt_flg=1;
        if_p->swt_ctxt_flg=1;
    }


}

void master_thread::update_op_cod(int *source_mail_box, int *dest_mail_box)
{
    for(int i=0;i<4;i++){
        dest_mail_box[i]=source_mail_box[i];
    }

}

void master_thread::pass_NOP(int accountableNOP, int *dest_mail_box)
{
    if(accountableNOP)
    {
        dest_mail_box[0]=2;
        for(int i=1; i<4;i++)
            dest_mail_box[i]=0;
    }
    else
    {
        dest_mail_box[0]=1;
        for(int i=1; i<4;i++)
            dest_mail_box[i]=0;
    }
}

int master_thread::switch_context(int type)
{
    reset_variables();
    if(type==0){
        PCB* old_context=new PCB();
        for(int i=0; i<33; i++){
            old_context->rgstrs[i] = id_p->rgstrs[i];
            old_context->rgstrs_state[i]=id_p->rgstrs_state[i];
        }
        old_context->PC=if_p->pc;
        old_context->thread_id=current_threadie;
        context_list.push(*old_context);
    }
    if(!context_list.empty()){
        PCB new_context= context_list.front();
        for(int i=0; i<32; i++){
            id_p->rgstrs[i]=new_context.rgstrs[i];
            id_p->rgstrs_state[i]=new_context.rgstrs_state[i];
        }
        id_p->rgstrs[32]=-1;
        if_p->pc=new_context.PC;
        current_threadie=new_context.thread_id;
        context_list.pop();
        return 0;
    }else{
        end_of_program=1;
        if_p->end_of_program=1;
        id_p->end_of_program=1;
        ex_p->end_of_program=1;
        mem_p->end_of_program=1;
        wb_p->end_of_program=1;

        return 1;
    }


}

void master_thread::reset_variables()
{
    swt_ctxt_flg=0;
    if_p->swt_ctxt_flg=0;
    threadie_finished=0;
    if_p->sent=0;
}



