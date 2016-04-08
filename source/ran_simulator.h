#ifndef RAN_SIMULATOR_H
#define RAN_SIMULATOR_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "ran.h"
#include "s1ap.h"
#include "security.h"
#include "sctp_client.h"
#include "sync.h"
#include "telecom.h"
#include "tun.h"
#include "udp_client.h"
#include "udp_server.h"
#include "utils.h"

extern time_t g_start_time;
extern int g_threads_count;
extern double g_req_duration;
extern uint64_t g_run_duration;
extern int g_tot_regs;
extern uint64_t g_tot_regstime;
extern pthread_mutex_t g_mux;
extern thread g_mon_thread;
extern vector<thread> g_threads;
extern TrafficMonitor g_traf_mon;

void simulate(int);
void check_usage(int);
void init(char**);
void run();
void print_results();

#endif /* RAN_SIMULATOR_H */