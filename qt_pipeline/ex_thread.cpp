#include "ex_thread.h"
using namespace std;
ex_thread::ex_thread()
{

}

void ex_thread::run(void *data){
    //Espera a que el master thread inicialice todo
    final_bar->Wait();
    executePhase();
    master_bar->Wait();
}

void ex_thread::executePhase(){
    if(input_box[8]){
        output_box[7] = 1;
        return ;
    }

    output_box[7] = 0;
    switch (input_box[0]) { //Codigo de operacion
        case 19: //Addi
            addi();
            break;
        case 71: //Add
            add();
            break;
        case 83: //Sub
            sub();
            break;
        case 72: //Mul
            mul();
            break;
        case 56: //Div
            div();
            break;
        case 5:  //Lw
            lw();
            break;
        case 37: //Sw
            sw();
            break;
        case 99: //Beq
            beq();
            break;
        case 100: //Bne
            bne();
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
        default: //FIN o NOP
            cout << "No operation executed in EX" << endl;
    }

    passInstrToMem();

}

void ex_thread::passInstrToMem(){
    for(int i = 0 ; i < 4 ; i++){
        output_box[i] = input_box[i];
    }
}

void ex_thread::addi(){   
    output_box[5] = input_box[4] + input_box[6];
}

void ex_thread::add(){
    output_box[5] = input_box[4] + input_box[5];
}

void ex_thread::sub(){
    output_box[5] = input_box[4] - input_box[5];
}

void ex_thread::mul(){
    output_box[5] = input_box[4] * input_box[5];
}

void ex_thread::div(){
    output_box[5] = input_box[4] / input_box[5];
}

void ex_thread::lw(){
    output_box[5] = input_box[4] + input_box[6];
}

void ex_thread::lr(){
    output_box[5] = input_box[4];
}

void ex_thread::sw(){
    output_box[5] = input_box[4] + input_box[6];
    output_box[6] = input_box[5];
}

void ex_thread::sc(){
    int address = input_box[4]+input_box[6];
    if(address==input_box[9]){
        output_box[5] = address;
    }else{
        output_box[5] = -1;
    }

    output_box[6] = input_box[5];
}

void ex_thread::beq(){
    if(input_box[4] == input_box[5]){
        branch_result = true;
    }else{
        branch_result = false;
    }

    output_box[4] = input_box[7];
}

void ex_thread::bne(){
    if(input_box[4] != input_box[5]){
        branch_result = true;
    }else{
        branch_result = false;
    }

    output_box[4] = input_box[7];
}

void ex_thread::jal(){
    output_box[4] = input_box[7];
    branch_result = true;
}

void ex_thread::jalr(){
    output_box[5] = input_box[4];
    output_box[4] = input_box[7];
    branch_result = true;
}
