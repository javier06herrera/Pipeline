#include "if_thread.h"
using namespace std;
if_thread::if_thread(){

}

void if_thread::run(void* data){
    final_bar->Wait();
    instr_fetch();
    master_bar->Wait();
}

int if_thread::instr_fetch(){
    int branch = branch_cmp();

    if (fail_cycle && !branch){
        work_fail();
        return 0;
    }

    if (input_box[0]) {
        return 0;
    }

    fail_cycle = 0;

    if (quantum_finished) {
        send_NOP();
        return 0;
    }

    int num_blk = addr_to_block(pc);
    int num_word = addr_to_word(pc);
    if (exists(num_blk)) {
        extract_from_che(num_blk, num_word);
        output_box[4]=pc;
        pc = pc + 4;
        return 1;
    }
    activate_fail();
    resolve_fault(num_blk,num_word);
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
    send_NOP();
    che_fails++;
}

void if_thread::send_NOP(){
    output_box[0]=1;
    output_box[1]=0;
    output_box[2]=0;
    output_box[3]=0;
    return;
}

void if_thread::work_fail(){
    fail_cycle--;
    send_NOP();
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
    int num_blk_mem = num_blk-24;
    num_blk_mem = num_blk_mem*16;

    int num_blk_che = num_blk%4;

    block_id_intr_che[num_blk_che] = num_blk;

    num_blk_che = num_blk*16;

    for (int i = 0; i < 16; i++) {
        intr_che[num_blk_che] = intr_mem[num_blk_mem];
        num_blk_che ++;
        num_blk_mem ++;
    }
    mem_request++;
}

int if_thread::exists(int num_blk){
    for (int i = 0; i < 4; i++) {
        if (block_id_intr_che[i] == num_blk) {
            return 1;
        }
    }
    return 0;
}

int if_thread::addr_to_block(int p_c){
    return p_c/16;
}

int if_thread::addr_to_word(int p_c){
    return (p_c%16)/4;
}
