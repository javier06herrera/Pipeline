#include "wb_thread.h"

using namespace std;
wb_thread::wb_thread()
{

}

void wb_thread::run(void* data){

    cout<<"Hola from wb"<<endl;
    final_bar->Wait();
}
