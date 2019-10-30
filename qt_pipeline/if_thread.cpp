#include "if_thread.h"
using namespace std;
if_thread::if_thread()
{

}

void if_thread::run(void* data){
    pthread_barrier_wait(final_bar);
    cout<<"Hola from if"<<endl;
}
