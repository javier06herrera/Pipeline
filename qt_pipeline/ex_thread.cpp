#include "ex_thread.h"
using namespace std;
ex_thread::ex_thread()
{

}

void ex_thread::run(void *data){
    //Espera a que el master thread inicialice todo
    final_bar->Wait();
    cout<<"Hola from ex"<<endl;
}
