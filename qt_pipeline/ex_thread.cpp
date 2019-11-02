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
    //Condicion con Estado MEM
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
    //Suma Imm y registro A y lo pasa a ALU Out
    output_box[5] = input_box[4] + input_box[6];
}

void ex_thread::add(){
    //Suma registro A y B y lo pasa a ALU Out
    output_box[5] = input_box[4] + input_box[5];
}

void ex_thread::sub(){
    //Resta registro A y B y lo pasa a ALU Out
    output_box[5] = input_box[4] - input_box[5];
}

void ex_thread::mul(){
    //Multiplica registro A y B y lo pasa a ALU Out
    output_box[5] = input_box[4] * input_box[5];
}

void ex_thread::div(){
    //Divide registro A y B y lo pasa a ALU Out
    output_box[5] = input_box[4] / input_box[5];
}

void ex_thread::lw(){
    //Calcula la direccion sumando el registro A e Imm
    output_box[5] = input_box[4] + input_box[6];
}

void ex_thread::lr(){
    //Pasa la direccion en A a ALU Out
    output_box[5] = input_box[4];
}

void ex_thread::sw(){
    //Calcula la direccion sumando el registro A e Imm y pasa el registro B a MEM
    output_box[5] = input_box[4] + input_box[6];
    output_box[6] = input_box[5];
}

void ex_thread::sc(){
    //Calcula la direccion sumando el registro A e Imm y pasa el registro B a MEM
    output_box[5] = input_box[4] + input_box[6];
    output_box[6] = input_box[5];
}

void ex_thread::beq(){
    //Compara si los registro A y B son iguales, pasa el PC branch a IF
    if(input_box[4] == input_box[5]){
        branch_result = true;
    }else{
        branch_result = false;
    }

    output_box[4] = input_box[7];
}

void ex_thread::bne(){
    //Compara si los registro A y B son distintos, pasa el PC branch a IF
    if(input_box[4] != input_box[5]){
        branch_result = true;
    }else{
        branch_result = false;
    }

    output_box[4] = input_box[7];
}

void ex_thread::jal(){
    //Pasa el PC branch a IF
    output_box[4] = input_box[7];
    branch_result = true;
}

void ex_thread::jalr(){
    //Pasa el PC branch a IF y el valor del registro A a ALU Out
    output_box[5] = input_box[4];
    output_box[4] = input_box[7];
    branch_result = true;
}
