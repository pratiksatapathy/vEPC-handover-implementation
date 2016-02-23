#include "mme_data.h"

struct MMEData *g_mme_data;
pthread_mutex_t g_arr_lock = PTHREAD_MUTEX_INITIALIZER;

MMEData::MMEData(){

	valid = false;
}

MMEData::~MMEData(){


}

void setup_mme_data(){
	int i;

	g_mme_data = new struct MMEData[MAX_CONNS];
	for(i=0;i<MAX_CONNS;i++)
		g_mme_data[i].ue_num = i;
}

void free_mme_data(){

	delete[] g_mme_data;
}