#ifndef MME_DATA_H
#define MME_DATA_H

#include "utils.h"

struct MMEData{
	bool valid;

	int ue_num;
	unsigned long long autn_xres;
	int bearer_id;
	string ue_ip;

	uint16_t mme_cteid;

	uint16_t enodeb_uteid;

	int sgw_port;
	string sgw_addr;
	uint16_t sgw_cteid;
	uint16_t sgw_uteid;
	
	int pgw_port;
	string pgw_addr;

	MMEData();

	~MMEData();
};

extern struct MMEData *g_mme_data; 
extern pthread_mutex_t g_arr_lock; // To lock g_ran_data

void setup_mme_data();
void free_mme_data();

#endif //MME_DATA_H
