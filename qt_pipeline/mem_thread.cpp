#include "mem_thread.h"
using namespace std;
mem_thread::mem_thread()
{

}

void mem_thread::run(void *data){
    pthread_barrier_wait(final_bar);
    cout<<"Hola from mem"<<endl;
}
