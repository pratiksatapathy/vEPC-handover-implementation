#include "pgw_data.h"

struct PGWData *g_pgw_data;
pthread_mutex_t g_arr_lock = PTHREAD_MUTEX_INITIALIZER;

unordered_map<string, int> g_ue_maptable;
pthread_mutex_t g_map_lock = PTHREAD_MUTEX_INITIALIZER;

PGWData::PGWData(){

	valid = false;
}

PGWData::~PGWData(){


}

void setup_pgw_data(){
	int i;

	g_pgw_data = new struct PGWData[MAX_CONNS];
	for(i=0;i<MAX_CONNS;i++)
		g_pgw_data[i].ue_num = i;
}

void free_pgw_data(){

	delete[] g_pgw_data;
}