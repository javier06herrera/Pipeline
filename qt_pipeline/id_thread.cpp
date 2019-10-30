#include "id_thread.h"


using namespace std;

id_thread::id_thread()
{

}

void id_thread::run(void *data){
    final_bar->Wait();
    cout<<"Hola from id"<<endl;
}
