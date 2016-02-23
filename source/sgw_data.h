#ifndef SGW_DATA_H
#define SGW_DATA_H

#include "utils.h"

struct SGWData{
	bool valid;

	int ue_num;
	int bearer_id;
	string ue_ip;

	uint16_t mme_cteid;

	int enodeb_port;
	string enodeb_addr;
	uint16_t enodeb_uteid;

	uint16_t sgw_cteid;
	uint16_t sgw_uteid;
	
	int pgw_port;
	string pgw_addr;
	uint16_t pgw_cteid;
	uint16_t pgw_uteid;

	SGWData();

	~SGWData();
};

extern struct SGWData *g_sgw_data;
extern pthread_mutex_t g_arr_lock; // To lock g_sgw_data

void setup_sgw_data();
void free_sgw_data();

#endif //SGW_DATA_H
