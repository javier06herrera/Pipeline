#include <iostream>
#include "master_thread.h"
#include "if_thread.h"
#include "id_thread.h"
#include "ex_thread.h"
#include "mem_thread.h"
#include "wb_thread.h"

using namespace std;
int main()
{
    master_thread* master_p=new master_thread();
    if_thread* if_p=new if_thread;
    id_thread* id_p=new id_thread();
    ex_thread* ex_p=new ex_thread;
    mem_thread* mem_p= new mem_thread;
    wb_thread* wb_p= new wb_thread;
    thread master_t(&master_thread::run, master_p, nullptr);
    thread if_t(&if_thread::run, if_p, nullptr);
    thread id_t(&id_thread::run, id_p, nullptr);
    thread ex_t(&ex_thread::run, ex_p, nullptr);
    thread mem_t(&mem_thread::run, mem_p, nullptr);
    thread wb_t(&wb_thread::run, wb_p, nullptr);
    master_t.join();
    if_t.join();
    id_t.join();
    ex_t.join();
    mem_t.join();
    wb_t.join();
    delete master_p;
    delete if_p;
    delete id_p;
    delete ex_p;
    delete mem_p;
    delete wb_p;
    return 0;
}
