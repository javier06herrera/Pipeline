#include "id_thread.h"


using namespace std;

id_thread::id_thread(){

}

void id_thread::run(void *data){
    final_bar->Wait();
    wb_id_coord->Wait();
    instr_decode();
    master_bar->Wait();
}

int id_thread::instr_decode(){
    if (input_box[4]) {
        output_box[4]=1;
        return 0;
    }
    switch (input_box[0]) { //Codigo de operacion
        case 19: //Addi
            addi();
            break;
        case 71: //Add
            op_arithm();
            break;
        case 83: //Sub
            op_arithm();
            break;
        case 72: //Mul
            op_arithm();
            break;
        case 56: //Div
            op_arithm();
            break;
        case 5:  //Lw
            lw();
            break;
        case 37: //Sw
            sw();
            break;
        case 99: //Beq
            branch();
            break;
        case 100: //Bne
            branch();
            break;
        case 51: //Lr
            lr();
            break;
        case 52: //Sc
            sc();
            break;
        case 111: //Jal
            jal();
            break;
        case 103: //Jalr
            jalr();
            break;
        case 999: //FIN o NOP
            load_instr();
            break;
    }
    return 1;
}

void id_thread::jalr(){
    if (check_status(input_box[2])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[1]);
    output_box[5] = rgstrs[input_box[2]];
    output_box[8] = rgstrs[input_box[2]] + input_box[3];
    output_box[4]=0;
    load_instr();
}

void id_thread::jal(){
    if (check_status(input_box[1])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    rgstrs[input_box[1]] = rgstrs[input_box[5]];
    output_box[8] = input_box[5] + input_box[3];
    output_box[4]=0;
    load_instr();
}

void id_thread::lr(){
    if (check_status(input_box[2])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[1]);
    output_box[5] = rgstrs[input_box[2]];
    output_box[4]=0;
    load_instr();
}

void id_thread::sc(){
    if (check_status(input_box[1])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[2]);
    output_box[5] = rgstrs[input_box[1]];
    output_box[6] = rgstrs[input_box[2]];
    output_box[7] = input_box[3];
    output_box[4] = 0;
    output_box[9] = rgstrs[32];
    load_instr();
}

void id_thread::addi(){
    if (check_status(input_box[2])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[1]);
    output_box[5] = rgstrs[input_box[2]];
    output_box[7] = input_box[3];
    output_box[4]=0;
    load_instr();
}

void id_thread::branch(){
    if (check_status(input_box[1])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    if (check_status(input_box[2])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    output_box[5] = rgstrs[input_box[1]];
    output_box[6] = rgstrs[input_box[2]];
    output_box[8] = input_box[5] + input_box[3]*4;
    output_box[4]=0;
    load_instr();
}

void id_thread::op_arithm(){
    if (check_status(input_box[2])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    if (check_status(input_box[3])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[1]);
    output_box[5] = rgstrs[input_box[2]];
    output_box[6] = rgstrs[input_box[3]];
    output_box[4]=0;
    load_instr();
}

void id_thread::sw(){
    if (check_status(input_box[1])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    if (check_status(input_box[2])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    output_box[5] = rgstrs[input_box[1]];
    output_box[6] = rgstrs[input_box[2]];
    output_box[7] = input_box[3];
    output_box[4] = 0;
    load_instr();
}

void id_thread::lw(){
    if (check_status(input_box[2])) {
        send_NOP();
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[1]);
    output_box[5] = rgstrs[input_box[2]];
    output_box[7] = input_box[3];
    output_box[4]=0;
    load_instr();
}

void id_thread::use_rgstr(int rgstr){
    rgstrs_state[rgstr]++;
}

void id_thread::send_NOP(){
    output_box[0]=2;
    output_box[1]=0;
    output_box[2]=0;
    output_box[3]=0;
}

int id_thread::load_instr(){
    for (int i = 0; i < 4; i++) {
        output_box[i] = input_box[i];
    }
    return 1;
}

int id_thread::check_status(int regist){
    if (rgstrs_state[regist]) {
        return 1;
    }
    return 0;
}
