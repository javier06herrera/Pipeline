#include "mem_thread.h"
using namespace std;
mem_thread::mem_thread()
{

}

void mem_thread::run(void *data){
    final_bar->Wait();
    cout<<"Hola from mem"<<endl;
}
