#ifndef PGW_DATA_H
#define PGW_DATA_H

#include "utils.h"

struct PGWData{
	bool valid;

	int ue_num;
	int bearer_id;
	string ue_ip;

	int sgw_port;
	string sgw_addr;
	uint16_t sgw_cteid;
	uint16_t sgw_uteid;
	
	uint16_t pgw_cteid;
	uint16_t pgw_uteid;

	PGWData();

	~PGWData();
};

extern struct PGWData *g_pgw_data;
extern pthread_mutex_t g_arr_lock; // To lock g_sgw_data

extern unordered_map<string, int> g_ue_maptable; 
extern pthread_mutex_t g_map_lock; // To lock g_ue_maptable

void setup_pgw_data();
void free_pgw_data();

#endif //PGW_DATA_H
