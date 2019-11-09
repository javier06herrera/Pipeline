#include "id_thread.h"


using namespace std;

id_thread::id_thread(){

}

void id_thread::run(void *data){
    while (true)
    {
        final_bar->Wait();
        instr_decode();
        master_bar->Wait();
    }

}

int id_thread::instr_decode(){
    if (input_box[4]) {
        output_box[4]=1;
        return 0;
    }
    switch (input_box[0]) { //Codigo de operacion
        case 3:
            send_NOP(3); //Nop de cambio de contexto
            break;
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
    wb_id_coord->Wait();
    if (check_status(input_box[2])) {
        send_NOP(1);
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[1]);
    //A <- PC normal
    output_box[5] = input_box[5];
    //Pc_branch <- Xn+Pc Normal
    output_box[8] = rgstrs[input_box[2]] + input_box[3];
    //Estado de ID bien
    output_box[4]=0;
    load_instr();
}

void id_thread::jal(){
    wb_id_coord->Wait();
    if (check_status(input_box[2])) {
        send_NOP(1);
        //Estado de ID bien
        output_box[4]=1;
        return;
    }
    //Reserva operando 1
    use_rgstr(input_box[1]);
    //A <- PC normal
    output_box[5] = input_box[5];
    //Pc_branch <- Xn+Pc Normal
    output_box[8] = input_box[5] + input_box[3];
    //Estado de ID bien
    output_box[4]=0;
    load_instr();
}

void id_thread::lr(){
    wb_id_coord->Wait();
    if (check_status(input_box[2])) {
        send_NOP(1);
        output_box[4]=1;
        return;
    }
    //Reserva operando 1
    use_rgstr(input_box[1]);
    //Reserva RL
    use_rgstr(32);
    //A <- Xn
    output_box[5] = rgstrs[input_box[2]];
    //Estado de ID bien
    output_box[4]=0;
    load_instr();
}

void id_thread::sc(){
    wb_id_coord->Wait();
    if (check_status(input_box[1]) || check_status(input_box[2]) || check_status(32)) {
        send_NOP(1);
        output_box[4]=1;
        return;
    }
    //Reserva registro destino de la operacion
    use_rgstr(input_box[2]);
    //A <- Xn : Guarda en A la direccion de memoria a escribir
    output_box[5] = rgstrs[input_box[1]];
    //B <- Xn : Guarda en B el dato a guardar en direccion de memoria
    output_box[6] = rgstrs[input_box[2]];
    //Imm<-Imm : Se le pasa un inmediato a sumar a la direccion
    output_box[7] = input_box[3];
    //Se pasa el valor del RL
    output_box[9] = rgstrs[32];
    //Estado de ID bien
    output_box[4] = 0;

    load_instr();
}

void id_thread::addi(){
    wb_id_coord->Wait();
    if (check_status(input_box[2])) {
        send_NOP(1);
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[1]);
    //A <- Xn
    output_box[5] = rgstrs[input_box[2]];
    //Imm <- Imm
    output_box[7] = input_box[3];
    //Estado de ID bien
    output_box[4]=0;
    load_instr();
}

void id_thread::branch(){
    wb_id_coord->Wait();
    if (check_status(input_box[1]) || check_status(input_box[2])) {
        send_NOP(1);
        output_box[4]=1;
        return;
    }
    //A <- Xn
    output_box[5] = rgstrs[input_box[1]];
    //B <- Xn
    output_box[6] = rgstrs[input_box[2]];
    //Pc Branch <- Pc Normal + Imm*4
    output_box[8] = input_box[5] + input_box[3]*4;
    //Estado de ID bien
    output_box[4]=0;
    load_instr();
}

void id_thread::op_arithm(){
    wb_id_coord->Wait();
    if (check_status(input_box[2])||check_status(input_box[3])) {
        send_NOP(1);
        output_box[4]=1;
        return;
    }
    //Reserva el registro destino
    use_rgstr(input_box[1]);
    //A <- Xn
    output_box[5] = rgstrs[input_box[2]];
    //B <- Xn
    output_box[6] = rgstrs[input_box[3]];
    //Estado de ID bien
    output_box[4]=0;
    load_instr();
}

void id_thread::sw(){
    wb_id_coord->Wait();
    if (check_status(input_box[1]) || check_status(input_box[2])) {
        send_NOP(1);
        output_box[4]=1;
        return;
    }
    //A <- Xn
    output_box[5] = rgstrs[input_box[1]];
    //B <- Xn
    output_box[6] = rgstrs[input_box[2]];
    //Imm <- Imm
    output_box[7] = input_box[3];
    //Estado de ID bien
    output_box[4] = 0;
    load_instr();
}

void id_thread::lw(){
    wb_id_coord->Wait();
    if (check_status(input_box[2])) {
        send_NOP(1);
        output_box[4]=1;
        return;
    }
    use_rgstr(input_box[1]);
    //A <- Xn
    output_box[5] = rgstrs[input_box[2]];
    //Imm <- Imm
    output_box[7] = input_box[3];
    //Estado de ID bien
    output_box[4]=0;
    load_instr();
}

void id_thread::use_rgstr(int rgstr){
    rgstrs_state[rgstr]++;
}

void id_thread::send_NOP(int type){
    if(type==1){
        output_box[0]=1;
        output_box[1]=0;
        output_box[2]=0;
        output_box[3]=0;
    }
    else if(type==2)
    {
        output_box[0]=2;
        output_box[1]=0;
        output_box[2]=0;
        output_box[3]=0;
    }
    else
    {
        output_box[0]=3;
        output_box[1]=0;
        output_box[2]=0;
        output_box[3]=0;
    }
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
