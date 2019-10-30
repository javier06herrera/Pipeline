#include "id_thread.h"


using namespace std;

id_thread::id_thread()
{

}

void id_thread::run(void *data){
    pthread_barrier_wait(final_bar);
    cout<<"Hola from id"<<endl;
}
