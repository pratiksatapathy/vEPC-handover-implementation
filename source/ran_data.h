#ifndef RAN_DATA_H
#define RAN_DATA_H

#include "utils.h"

struct RANData{
	bool valid;

	int ue_num;

	uint16_t enodeb_uteid;

	int mme_port;
	string mme_addr;

	int sgw_port;
	string sgw_addr;
	uint16_t sgw_uteid;

	RANData();

	~RANData();
};

extern struct RANData *g_ran_data; 
extern pthread_mutex_t g_arr_lock; // To lock g_ran_data

extern unordered_map<string, int> g_ue_maptable; 
extern pthread_mutex_t g_map_lock; // To lock g_ue_maptable

extern int g_total_regs;
extern unsigned long long g_total_regstime;
extern pthread_mutex_t g_time_lock; // To lock variables holding registration time and count

void setup_ran_data();
void free_ran_data();

#endif //RAN_DATA_H
