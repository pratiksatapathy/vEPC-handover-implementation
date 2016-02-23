#include "sgw_data.h"

struct SGWData *g_sgw_data;
pthread_mutex_t g_arr_lock = PTHREAD_MUTEX_INITIALIZER;

SGWData::SGWData(){

	valid = false;
}

SGWData::~SGWData(){


}

void setup_sgw_data(){
	int i;

	g_sgw_data = new struct SGWData[MAX_CONNS];
	for(i=0;i<MAX_CONNS;i++)
		g_sgw_data[i].ue_num = i;
}

void free_sgw_data(){

	delete[] g_sgw_data;
}