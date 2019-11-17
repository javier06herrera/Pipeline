#include "wb_thread.h"

using namespace std;
wb_thread::wb_thread()
{

}

void wb_thread::run(){
    final_bar->Wait();
    while (!end_of_program) {
        execute_pahse();
        master_bar->Wait();
        final_bar->Wait();
    }

}

void wb_thread::execute_pahse()
{
    cpu_clock++;
    if(input_box[0]!=1 && input_box[0]!=3 && input_box[0]!=2)
        clock_ticks++;

        cout << "QUANTUM ACTUAL: " << clock_ticks << endl;
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
        //cout<<"Aqui"<<endl;
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
        wb_id_coord->Wait();
        //cout << "No operation executed in Wb" << endl;
        break;
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
    //Libera el registro destino
    rgstrs_state[input_box[1]]--;
    //Libera RL ya que fue reservado en ID
    rgstrs_state[32]--;
    wb_id_coord->Wait();
}

void wb_thread::sw(){
    //No hace nada en wb
    wb_id_coord->Wait();
}

void wb_thread::sc(){
    //Guarda en el registro destino lo que leyo en mem solo si es 0 ya que sei no
    //significa que se debe dejar igual
    if(input_box[4]==0)
        rgstrs[input_box[2]]=input_box[4];
    rgstrs_state[input_box[2]]--;
    wb_id_coord->Wait();

}

void wb_thread::beq(){
    wb_id_coord->Wait();
}

void wb_thread::bne(){
    wb_id_coord->Wait();
}

void wb_thread::jal(){
    //Se actualizan y liberan los registros
    rgstrs[input_box[1]]=input_box[4];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}

void wb_thread::jalr(){
    //Se actualizan y liberan los registros
    rgstrs[input_box[1]]=input_box[4];
    rgstrs_state[input_box[1]]--;
    wb_id_coord->Wait();
}


