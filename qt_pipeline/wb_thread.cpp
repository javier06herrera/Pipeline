#include "wb_thread.h"

using namespace std;
wb_thread::wb_thread()
{

}

void wb_thread::run(void* data){
    pthread_barrier_wait(final_bar);
    cout<<"Hola from wb"<<endl;
}
