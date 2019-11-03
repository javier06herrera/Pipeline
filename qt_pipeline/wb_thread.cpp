#include "wb_thread.h"

using namespace std;
wb_thread::wb_thread()
{

}

void wb_thread::run(void* data){
    while (true) {
        final_bar->Wait();
        if(end_of_program)
            break;
        execute_pahse();
        master_bar->Wait();
    }

}

void wb_thread::execute_pahse()
{
    if(input_box[0]!=1)
        clock_ticks++;

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
        cout << "No operation executed in Wb" << endl;
    }
}

void wb_thread::addi(){
    rgstrs[input_box[1]]=input_box[4];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}

void wb_thread::add(){
    rgstrs[input_box[1]]=input_box[4];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}

void wb_thread::sub(){
    rgstrs[input_box[1]]=input_box[4];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}

void wb_thread::mul(){
    rgstrs[input_box[1]]=input_box[4];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}

void wb_thread::div(){
    rgstrs[input_box[1]]=input_box[4];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}

void wb_thread::lw(){
    rgstrs[input_box[1]]=input_box[5];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}

void wb_thread::lr(){
    //Actualiza a RL
    rgstrs[32]=input_box[4];
    //Guarda en el registro destino el LMD
    rgstrs[input_box[1]]=input_box[5];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}

void wb_thread::sw(){
    //No hace nada en wb
}

void wb_thread::sc(){
    //Guarda en el registro destino lo que leyo en mem
    rgstrs[input_box[2]]=input_box[4];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();

}

void wb_thread::beq(){
    wb_id_coord->Wait();
}

void wb_thread::bne(){
    wb_id_coord->Wait();
}

void wb_thread::jal(){
    wb_id_coord->Wait();
}

void wb_thread::jalr(){
    rgstrs[input_box[1]]=input_box[4];
    wb_id_coord->Wait();
}


