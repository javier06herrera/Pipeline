#include "if_thread.h"
using namespace std;
if_thread::if_thread(){

}

void if_thread::run(){
    final_bar->Wait();
    while (!end_of_program)
    {
        instr_fetch();
        master_bar->Wait();
        final_bar->Wait();
    }
}

int if_thread::instr_fetch(){
    int branch = branch_cmp();

    if (fail_cycle && !branch){
        if(fail_cycle==48){
            int num_blk = addr_to_block(pc);
            int num_word = addr_to_word(pc);
            resolve_fault(num_blk,num_word);
        }
        work_fail();
        //printf("\n fail cycle %d %d %d\n",fail_cycle, input_box[0], swt_ctxt_flg );
        return 0;
    }

    if (input_box[0]) {
        return 0;
    }

    fail_cycle = 0;

    //Si se llega a final de quantum o de hilillo se entra en cambio de contexto
    if (swt_ctxt_flg)
    {
        //Controla que solo se mande un cambio de contexto
        if(!sent)
        {
            send_NOP(3);
            sent++;
            return 0;
        }
        else
        {
            send_NOP(1);
            return 0;
        }
    }


    int num_blk = addr_to_block(pc);

    int num_word = addr_to_word(pc);
    printf("\nBlock: %d,  Word: %d\n", num_blk,num_word);
    if (exists(num_blk)) {
        extract_from_che(num_blk, num_word);
        output_box[4]=pc;
        pc = pc + 4;
        return 1;
    }
    printf("\nOops, estoy en fallo\n");
    activate_fail();
    return 0;

}

int if_thread::branch_cmp(){
    if (input_box[1]!=-1) {
        pc = input_box[1];
        return 1;
    }
    return 0;
}

void if_thread::activate_fail(){
    fail_cycle = 48;
    send_NOP(1);
    che_fails++;
}

void if_thread::send_NOP(int type){
    output_box[0]=type;
    output_box[1]=0;
    output_box[2]=0;
    output_box[3]=0;
    return;
}

void if_thread::work_fail(){
    fail_cycle--;
    send_NOP(1);
}

void if_thread::extract_from_che(int num_blk, int num_word){
    int num_blk_che = num_blk%4;
    num_blk_che = num_blk*16;
    num_blk_che =num_blk_che + num_word*4;
    for (int i = num_blk_che; i < num_blk_che+4; i++) {
        output_box[i-num_blk_che] = intr_che[i];
    }
}

void if_thread::resolve_fault(int num_blk, int num_word){
    int num_blk_mem = num_blk*16;

    int num_blk_che = num_blk%4;

    block_id_intr_che[num_blk_che] = num_blk;

    num_blk_che = num_blk_che*16;

    for (int i = 0; i < 16; i++) {
        intr_che[num_blk_che] = intr_mem[num_blk_mem];
        //printf(" %d ", intr_mem[num_blk_mem]);
        num_blk_che ++;
        num_blk_mem ++;
    }
   // printf("\n");
    mem_request++;
}

int if_thread::exists(int num_blk){
    if (block_id_intr_che[num_blk%4] == num_blk) {
            return 1;
    }
    //printf("Bloque no esta aqui");
    return 0;
}

int if_thread::addr_to_block(int p_c){
    return p_c/16;
}

int if_thread::addr_to_word(int p_c){
    return (p_c%16)/4;
}
