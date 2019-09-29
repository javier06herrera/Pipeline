#include <pthread.h>
#include <iostream>

using namespace std;

struct if_thread{
	int quantum_finished;
	pthread_barrier_t* master_bar;
	pthread_barrier_t* final_bar;
	int* intr_che;
	int** block_id_intr_che;
	int* intr_mem;
	int thread_id;
	int* input_box;
	int* output_box;
	int pc;
	int che_fails;
	
} ;


struct id_thread{
	int** rgstrs;
	int** rgstrs_state;
	int thread_id;
	pthread_barrier_t* master_bar;
	pthread_barrier_t* final_bar;
	pthread_barrier_t* wb_id_coord;
	int* input_box;
	int* output_box;
} ;

struct ex_thread{
	bool branch_result;
	int  overwrite_cycles;
	pthread_barrier_t* master_bar;
	pthread_barrier_t* final_bar;
	int* input_box;
	int* output_box;
};

struct mem_thread{
	int** block_id_dta_che;
	int* input_box;
	int* output_box;
	pthread_barrier_t* master_bar;
	pthread_barrier_t* final_bar;
	int read_che_fails;
	int write_che_fails;
	int read_mem_rqst;
	int write_mem_rqst;
};

struct wb_thread{
	int* input_box;
	int** rgstrs;
	int** rgstrs_state;
	pthread_barrier_t* master_bar;
	pthread_barrier_t* final_bar;
	pthread_barrier_t* wb_id_coord;
	
};

struct master_thread{
	int quantum_finished;
	int quatum_counter;
	int itrn_che_fails;
	int dta_che_fails;

	if_thread* if_data;
	id_thread* id_data;
	ex_thread* ex_data;
	mem_thread* mem_data;
	wb_thread* wb_data;

	pthread_barrier_t* master_bar;
	pthread_barrier_t* final_bar;

	//Puntero a lista de hilillos

} ;

int main(){
	
}
