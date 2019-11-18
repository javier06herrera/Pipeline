#include "master_thread.h"
#include <fstream>

using namespace std;
master_thread::master_thread()
{

}

void master_thread::run()
{
    read_threadies(if_p->intr_mem);
    init_mail_inboxes();
    upld_frst_ctxt();

    final_bar->Wait(); //Aqui estan esperando todos los threads a que se les inicialice sus valores
    while (!end_of_program)
    {
        master_bar->Wait();
        execute_phase();
        cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        cout << "Reloj actual del procesador: " <<wb_p->cpu_clock << endl;
        cout << "Hilillo actual en ejecucion: " << current_threadie_id << endl;
        cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        final_bar->Wait();
    }

    print_final_statistics();

}

void master_thread::read_threadies(int *instructionVector)
{
    int vecCounter = 384;
    for (int i = 0; i < 7; i++)
    {
        //Esta seccion se encarga de crear los contextos iniciales
        //*********************************************
        PCB* context=new PCB();
        for(int i=0;i<33;i++)
            context->rgstrs[i]=0;
        for(int i=0;i<32;i++)
            context->rgstrs_state[i]=0;
        context->rgstrs_state[32]=-1;
        context->PC=vecCounter;
        context->threadie_id=i;
        context->execution_cycles = 0;
        context->execution_switches = 0;
        context_list.push_back(*context);
        //*********************************************

        string filename = "HilillosPruebaFinal/" + to_string(i) + ".txt";
        ifstream infile(filename);
        int byte1, byte2, byte3, byte4;
        while(infile >> byte1 >> byte2 >> byte3 >> byte4)
        {
            instructionVector[vecCounter-384] = byte1;
            instructionVector[vecCounter+1-384] = byte2;
            instructionVector[vecCounter+2-384] = byte3;
            instructionVector[vecCounter+3-384] = byte4;
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
    current_threadie_execution_cycles++;
    //Pregunta si hay un cambio de contexto que aplicar
    if(wb_p->input_box[0]==3)
    {
        if(threadie_finished)
        {
            switch_context(1);
        }
        else
            switch_context(0);
    }

}

void master_thread::deliver_if()
{
    //Este primer if corrige desfaces de ciclo en el reconcimiento de fallos en mem
    if(mem_p->output_box[6])
        if_p->input_box[0]=2;
    else if(id_p->output_box[4]==1)
        if_p->input_box[0]=1;
    else
        if_p->input_box[0]= 0; //Se le pasa el estado de ID

    if(if_p->output_box[0]==999)//Pregunta si la ultima instruccion leida en if es la ultima del hilillo
    {
        if_p->swt_ctxt_flg=1;
        threadie_finished=1;
    }
    if(if_p->input_box[0]>0)
    {
        if_p->input_box[1]=-1;//Como ex esta detenido el master es el que le pasa el -1 en el branch
        return;
    }
    if_p->input_box[1]=ex_p->output_box[4];
}

void master_thread::deliver_id()
{
    id_p->input_box[4]=ex_p->input_box[8];//Siempre hay que pasarle el estado de ex para verificar si puede avanzar, pero la informacion esta en el input box de ex
    if(ex_p->input_box[8])// Se comprueba el estado de ex para saber si escribir buzones o no
        return;
    if(id_p->output_box[4]==1)
        return;

    update_op_cod(if_p->output_box,id_p->input_box);
    id_p->input_box[5]=if_p->output_box[4];//Trae el valor del pc para calcular el PC branch
}

void master_thread::deliver_ex()
{
    ex_p->input_box[8]=mem_p->output_box[6];//Siempre hay que pasarle el estado de mem para verificar si puede avanzar
    ex_p->output_box[7]=mem_p->output_box[6];
    //Verifica si mem esta detenido por un fallo de cache
    if(ex_p->input_box[8])
        return;
    /*Si el branch resulto verdadero se le debe pasar por dos ciclos NOPs para simular los retrasos de las
    dos instrucciones malas*/
    //El overwrite cycles va primero ya que solo se va cumplir la condicion si pasa por el elseif primero
    if(overwrite_cycles<2)
    {
        if(swt_ctxt_flg == 1){
            if(id_p->output_box[0]==1 && if_p->output_box[0]==1){
                id_p->output_box[0]=3;
            }
            if (id_p->output_box[0]==3) {
                update_op_cod(id_p->output_box,ex_p->input_box);//Se pasa la instruccion branch en el mismo ciclo que se detecta que fue tomado
            }
            else {
                pass_NOP(1,ex_p->input_box);//Estos NOP si cuentan, son los nop generados por un branch tomado
            }
        }else{
            pass_NOP(1,ex_p->input_box);//Estos NOP si cuentan, son los nop generados por un branch tomado
        }
        id_p->output_box[4] = 0;
        overwrite_cycles++;
    }
    else if(ex_p->branch_result)//Se pregunta si hubo branch tomado
    {
        pass_NOP(1,ex_p->input_box);
        //update_op_cod(id_p->output_box,ex_p->input_box);//Se pasa la instruccion branch en el mismo ciclo que se detecta que fue tomado
        overwrite_cycles=1;
        ex_p->branch_result=false;
        id_p->output_box[4] = 0;
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

        free_branch_rgstr();

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

    if(threadie_finished){ //Si el hilillo en ejecucion ya termino, guardar el PCB final para imprimirlo despues como estadistica
        PCB* final_context = new PCB();
        for(int i=0; i<33; i++){
            final_context->rgstrs[i] = id_p->rgstrs[i];
            final_context->rgstrs_state[i]=id_p->rgstrs_state[i];
        }
        final_context->PC = if_p->pc;
        final_context->threadie_id=current_threadie_id;
        final_context->execution_cycles = current_threadie_execution_cycles;
        final_context->execution_switches = current_threadie_execution_switches;
        final_context_list.push_front(*final_context);
    }

    reset_variables();

    if(type==0){
        PCB* old_context=new PCB();
        for(int i=0; i<33; i++){
            old_context->rgstrs[i] = id_p->rgstrs[i];
            old_context->rgstrs_state[i]=id_p->rgstrs_state[i];
        }
        old_context->PC=if_p->pc;
        old_context->threadie_id=current_threadie_id;
        old_context->execution_cycles = current_threadie_execution_cycles;
        old_context->execution_switches = current_threadie_execution_switches;

        context_list.push_back(*old_context);
    }
    if(!context_list.empty()){
        PCB new_context= context_list.front();
        for(int i=0; i<32; i++){
            id_p->rgstrs[i]=new_context.rgstrs[i];
            id_p->rgstrs_state[i]=new_context.rgstrs_state[i];
        }
        id_p->rgstrs[32]=-1;
        if_p->pc=new_context.PC;
        current_threadie_id=new_context.threadie_id;
        current_threadie_execution_cycles = new_context.execution_cycles;
        current_threadie_execution_switches = new_context.execution_switches + 1;

        context_list.pop_front();
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
    wb_p->clock_ticks = 0;
}

void master_thread::print_final_statistics(){
    printf("\nFinal State Shared Data Memory\n----------------------------------------------------\n");

    for (int i = 0 ; i < 96 ; i+=4){ //Estado final de la memoria compartida de datos
        int block = i/4;
        int pos = i*4;
        printf("Block %d|\t[%d]\t[%d]\t[%d]\t[%d]\n",block,pos,pos+4,pos+8,pos+12);
        int word0 = mem_p->data_mem[i];
        int word1 = mem_p->data_mem[i+1];
        int word2 = mem_p->data_mem[i+2];
        int word3 = mem_p->data_mem[i+3];
        printf("\t\t%d\t%d\t%d\t%d\n\n", word0, word1, word2, word3 );
    }
    printf("\n----------------------------------------------------\n");

    printf("\nFinal State Data Cache\n----------------------------------------------------\n");
    printf("Block|\tWord 0|\tWord 1|\tWord 2|\tWord 3\n");
    for (int i = 0 ; i < 16 ; i+=4){//Estado final de la cache de datos
        int block = mem_p->block_id_dta_che[i/4];
        int word0 = mem_p->data_che[i];
        int word1 = mem_p->data_che[i+1];
        int word2 = mem_p->data_che[i+2];
        int word3 = mem_p->data_che[i+3];
        printf("%d\t%d\t%d\t%d\t%d\n" , block, word0, word1, word2, word3 );
    }

    printf("\n----------------------------------------------------\n");

    printf("General Data Cache Failure Rate: %f", double(mem_p->read_che_fails+mem_p->write_che_fails)/double(mem_p->read_mem_rqst+mem_p->write_mem_rqst));

    printf("\n----------------------------------------------------\n");

    printf("Load Data Cache Failure Rate: %f", double(mem_p->read_che_fails)/double(mem_p->read_mem_rqst));

    printf("\n----------------------------------------------------\n");

    printf("Store Data Cache Failure Rate: %f", double(mem_p->write_che_fails)/double(mem_p->write_mem_rqst));

    printf("\n----------------------------------------------------\n");

    printf("\nFinal Threadie Contexts\n----------------------------------------------------\n");
    size_t final = final_context_list.size();
    for(size_t i = 0 ; i < final ; i++){
        PCB current = final_context_list.back();
        final_context_list.pop_back();
        printf("Threadie #%d\n", current.threadie_id);
        printf("Registers:\n");
        for (int j = 0 ; j < 33 ; j+=3){
            if(j+2==32){
                printf("X%d: %d | X%d: %d | RL: %d\n", j, current.rgstrs[j] , j+1 , current.rgstrs[j+1], current.rgstrs[j+2]);
            }else{
                printf("X%d: %d | X%d: %d | X%d: %d\n", j, current.rgstrs[j] , j+1 , current.rgstrs[j+1], j+2 , current.rgstrs[j+2]);
            }
            printf("-----------------------------\n");
        }
        printf("Execution Cycles: %d\n", current.execution_cycles);
        printf("Execution Switches: %d\n", current.execution_switches);
        printf("########################################\n");
    }
}

void master_thread::upld_frst_ctxt()
{
    PCB new_context = context_list.front();
    for(int i=0; i<32; i++)
    {
        id_p->rgstrs[i]=new_context.rgstrs[i];
        id_p->rgstrs_state[i]=new_context.rgstrs_state[i];
    }

    id_p->rgstrs[32]=-1;
    id_p->rgstrs_state[32]=0;
    if_p->pc=new_context.PC;
    current_threadie_id=new_context.threadie_id;
    current_threadie_execution_cycles = new_context.execution_cycles;
    current_threadie_execution_switches = new_context.execution_switches + 1;

    context_list.pop_front();
}


void master_thread::free_branch_rgstr()
{
    switch (id_p->output_box[0]) {
    case 19: //Addi
        id_p->rgstrs_state[id_p->output_box[1]]--;
        break;
    case 71: //Add
        id_p->rgstrs_state[id_p->output_box[1]]--;
        break;
    case 83: //Sub
        id_p->rgstrs_state[id_p->output_box[1]]--;
        break;
    case 72: //Mul
        id_p->rgstrs_state[id_p->output_box[1]]--;
        break;
    case 56: //Div
        id_p->rgstrs_state[id_p->output_box[1]]--;
        break;
    case 5:  //Lw
        id_p->rgstrs_state[id_p->output_box[1]]--;
        break;
    case 37: //Sw
        //No tiene que liberar nada
        break;
    case 99: //Beq
        //No tiene que liberar nada
        break;
    case 100: //Bne
        //No tiene que liberar nada
        break;
    case 51: //Lr
        id_p->rgstrs_state[id_p->output_box[1]]--;
        id_p->rgstrs_state[32]--;
        break;
    case 52: //Sc
        id_p->rgstrs_state[id_p->output_box[2]]--;
        break;
    case 111: //Jal
        id_p->rgstrs_state[id_p->output_box[1]]--;
        break;
    case 103: //Jalr
        id_p->rgstrs_state[id_p->output_box[1]]--;
        break;
    default: //FIN o NOP
        //No tiene que liberar nada
        break;

    }

}



