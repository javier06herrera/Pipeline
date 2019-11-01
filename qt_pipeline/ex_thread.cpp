#include "ex_thread.h"
using namespace std;
ex_thread::ex_thread()
{

}

void ex_thread::run(void *data){
    //Espera a que el master thread inicialice todo
    final_bar->Wait();
    //cout<<"Hola from ex"<<endl;
    executePhase();
}

void ex_thread::executePhase(){
    //Condicion con Estado MEM
    if (branch_result && overwrite_cycles < 2){
        overwrite_cycles++;
        passInstrToMem(0); //Pasar 2 NOPs a MEM
    }else{
        branch_result = false;
        overwrite_cycles = 0;
        switch (input_box[0]) {
            case 19:
                addi();
                break;
            case 71:
                add();
                break;
            case 83:
                sub();
                break;
            case 72:
                mul();
                break;
            case 56:
                div();
                break;
            case 5:
                lw();
                break;
            case 37:
                sw();
                break;
            case 99:
                beq();
                break;
            case 100:
                bne();
                break;
            case 51:
                lr();
                break;
            case 52:
                sc();
                break;
            case 111:
                jal();
                break;
            case 103:
                jalr();
                break;
            default:
                //FIN o NOP
                cout << "No operation executed in EX" << endl;
        }

        passInstrToMem(input_box[0]);
    }
}

void ex_thread::passInstrToMem(int instruction){
    output_box[1] = instruction;
}

void ex_thread::addi(){
    //Suma Imm y registro A y lo pasa a ALU Out
    output_box[2] = input_box[1] + input_box[3];
}

void ex_thread::add(){
    //Suma registro A y B y lo pasa a ALU Out
    output_box[2] = input_box[1] + input_box[2];
}

void ex_thread::sub(){
    //Resta registro A y B y lo pasa a ALU Out
    output_box[2] = input_box[1] - input_box[2];
}

void ex_thread::mul(){
    //Multiplica registro A y B y lo pasa a ALU Out
    output_box[2] = input_box[1] * input_box[2];
}

void ex_thread::div(){
    //Divide registro A y B y lo pasa a ALU Out
    output_box[2] = input_box[1] / input_box[2];
}

void ex_thread::lw(){
    //Calcula la direccion sumando el registro A e Imm
    output_box[2] = input_box[1] + input_box[3];
}

void ex_thread::lr(){
    //Calcula la direccion sumando el registro A e Imm
    output_box[2] = input_box[1];
}

void ex_thread::sw(){
    //Calcula la direccion sumando el registro A e Imm y pasa el registro B a MEM
    output_box[2] = input_box[1] + input_box[3];
    output_box[3] = input_box[2];
}

void ex_thread::sc(){
    //Calcula la direccion sumando el registro A e Imm y pasa el registro B a MEM
    output_box[2] = input_box[1] + input_box[3];
    output_box[3] = input_box[2];
}

void ex_thread::beq(){
    //Compara si los registro A y B son iguales, pasa el PC branch a IF
    if(input_box[1] == input_box[2]){
        branch_result = true;
    }else{
        branch_result = false;
    }

    output_box[4] = input_box[0];
}

void ex_thread::bne(){
    //Compara si los registro A y B son distintos, pasa el PC branch a IF
    if(input_box[1] != input_box[2]){
        branch_result = true;
    }else{
        branch_result = false;
    }

    output_box[0] = input_box[4];
}

void ex_thread::jal(){
    //Pasa el PC branch a IF
    output_box[0] = input_box[4];
    branch_result = true;
}

void ex_thread::jalr(){
    //Pasa el PC branch a IF
    output_box[2] = input_box[1];
    output_box[4] = input_box[0];
    branch_result = true;
}
