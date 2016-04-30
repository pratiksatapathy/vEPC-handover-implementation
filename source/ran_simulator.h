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
#include "sctp_server.h"

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

//handover changes
SctpServer server;

//Ran ranS;
//Ran ranT;
string g_ran_sctp_ip_addr = "127.0.0.1";
int g_ran_port = 4901;


//


void simulate(int);
void check_usage(int);
void init(char**);
void run();
void print_results();

//hochanges
int handle_mme_conn(int conn_fd);
void simulateHandover(int arg);


#endif /* RAN_SIMULATOR_H */
