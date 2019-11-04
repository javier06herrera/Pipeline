#include "mem_thread.h"
using namespace std;
mem_thread::mem_thread()
{

}

void mem_thread::run(void *data){
    final_bar->Wait();
    executePhase();
    master_bar->Wait();
}


int mem_thread::memAddressToBlock(int address){
    return address/16;
}

int mem_thread::memAddressToWord(int address){
    return (address%16)/4;
}

int mem_thread::getIndexInDataMemory(int address){
   return (memAddressToBlock(address)*4)+memAddressToWord(address);
}

int mem_thread::getIndexInDataCache(int address){
    return ((memAddressToBlock(address)%4)*4)+memAddressToWord(address);
}

bool mem_thread::isBlockInDataCache(int address){
    int block = memAddressToBlock(address);
    bool isInCache = false;
    if(block_id_dta_che[block%4] == block){
        isInCache = true;
    }

    return isInCache;
}

void mem_thread::loadBlockToCache(int address){
    int block = memAddressToBlock(address);
    int indexInMem = block*4;
    int indexInCache = (block%4)*4;

    for(int i=0;i<4;i++){
        data_che[indexInCache+i] = data_mem[indexInMem+i];
    }

    block_id_dta_che[block%4] = block;
}

void mem_thread::executePhase(){
    if(in_cache_fail_load || in_cache_fail_store){
        if(in_cache_fail_load){
            if(cache_fail_cycles < 48){
                cache_fail_cycles++;
            }else{
                in_cache_fail_load = false;
                cache_fail_cycles = 0;
                output_box[6] = 0;
            }
        }else{
            if(cache_fail_cycles < 12){
                cache_fail_cycles++;
            }else{
                in_cache_fail_store = false;
                cache_fail_cycles = 0;
                output_box[6] = 0;
            }
        }
        passNOPsToWB();
    }else{
        switch (input_box[0]) { //Codigo de operacion
            case 19: //Addi
                passALUOutToWB();
                break;
            case 71: //Add
                passALUOutToWB();
                break;
            case 83: //Sub
                passALUOutToWB();
                break;
            case 72: //Mul
                passALUOutToWB();
                break;
            case 56: //Div
                passALUOutToWB();
                break;
            case 5:  //Lw
                lw();
                break;
            case 37: //Sw
                sw();
                break;
            case 99: //Beq
                //Beq no hace nada en MEM
                break;
            case 100: //Bne
                //Bne no hace nada en MEM
                break;
            case 51: //Lr
                lr();
                break;
            case 52: //Sc
                sc();
                break;
            case 111: //Jal
                //Jal no hace nada en MEM
                break;
            case 103: //Jalr
                passALUOutToWB();
                break;
            default: //FIN o NOP
                cout << "No operation executed in MEM" << endl;
        }

        passInstrToWB();
    }


}

void mem_thread::passALUOutToWB(){
    output_box[4] = input_box[4];
}

void mem_thread::passInstrToWB(){
    for(int i = 0 ; i < 4 ; i++){
        output_box[i] = input_box[i];
    }
}

void mem_thread::passNOPsToWB(){
    output_box[0] = 1;
    for(int i = 1 ; i < 4 ; i++){
        output_box[i] = 0;
    }
}

void mem_thread::lw(){
    int address = input_box[4];
    if(!isBlockInDataCache(address)){
        in_cache_fail_load = true;
        loadBlockToCache(address);
        read_che_fails++;
        output_box[6] = 1;
    }

    int word = data_che[getIndexInDataCache(address)];
    output_box[5] = word;

    read_mem_rqst++;
}

void mem_thread::sw(){
    int address = input_box[4];
    if(!isBlockInDataCache(address)){ //Cache miss
        in_cache_fail_store = true;
        write_che_fails++;
        data_mem[getIndexInDataMemory(address)] = input_box[5]; //Write No-Allocate
        output_box[6] = 1;
    }else{ //Cache hit
        in_cache_fail_store = true; //Como es Write-Through se utiliza la misma bandera de fallo
                              //para avisar el retraso por el acceso a memoria, pero sin
                              //aumentar su respectivo contador.

        data_che[getIndexInDataCache(address)] = input_box[5];
        data_mem[getIndexInDataMemory(address)] = input_box[5];//Write-Through
    }

    write_mem_rqst++;
}



void mem_thread::lr(){
    lw();
    output_box[4] = input_box[4];
}

void mem_thread::sc(){
    if(input_box[4] != -1){ //Tomo el candado
        sw();
    }else{ //RL == -1 (No logro tomar el candado)
        output_box[4] = 0;
    }
}

