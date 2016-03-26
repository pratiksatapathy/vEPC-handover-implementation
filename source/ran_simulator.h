#ifndef RAN_SIMULATOR_H
#define RAN_SIMULATOR_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "ran.h"
#include "s1ap.h"
#include "sctp_client.h"
#include "telecom.h"
#include "udp_client.h"
#include "utils.h"

extern time_t g_start_time;
extern int g_threads_count;
extern double g_req_duration;
extern vector<thread> g_threads;

void simulate(int);
void check_usage(int);
void init(char**);
void run();
void print_results();

#endif /* RAN_SIMULATOR_H */