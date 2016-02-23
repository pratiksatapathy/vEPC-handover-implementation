#ifndef RAN_H
#define RAN_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "ran_data.h"
#include "ue.h"
#include "enodeb.h"

extern EnodeB g_enodeb;

extern int g_total_connections;
extern double g_req_duration;
extern time_t g_start_time;
extern unsigned long long g_run_dur;

extern vector<int> g_ue_num;
extern pthread_t g_mon_tid;
extern vector<pthread_t> g_tid;

extern pthread_t g_ping_tid;

void setup_tun();

void* monitor_traffic(void*);

void uplink_data_transfer();
void downlink_data_transfer();

void* generate_traffic(void*);

void authenticate(UE&);
void setup_tunnel(UE&);
void set_tun_data(UE&);
void set_map_data(UE&);

void send_traffic(UE&);

void detach(UE&);

void startup_ran(char**);

#endif //RAN_H
