#include "if_thread.h"
using namespace std;
if_thread::if_thread()
{

}

void if_thread::run(void* data){
    final_bar->Wait();
    cout<<"Hola from if"<<endl;
}
