#include "master_thread.h"
#include <fstream>

using namespace std;
void readThreadies(int *instructionVector)
{
    int vecCounter = 0;
    for (int i = 0; i < 7; i++) {
        string filename = "HilillosPruebaFinal/" + to_string(i) + ".txt";
        ifstream infile(filename);
        int byte1, byte2, byte3, byte4;
        while(infile >> byte1 >> byte2 >> byte3 >> byte4)
        {
            instructionVector[vecCounter] = byte1;
            instructionVector[vecCounter+1] = byte2;
            instructionVector[vecCounter+2] = byte3;
            instructionVector[vecCounter+3] = byte4;
            vecCounter+=4;
        }
    }
}

master_thread::master_thread()
{

}

void master_thread::run(void *data){
    cout<<"Hola from master"<<endl;
}

