#ifndef RAN_SIMULATOR_H
#define RAN_SIMULATOR_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"
#include "packet.h"
#include "network.h"
#include "sctp_client.h"
#include "udp_client.h"
#include "telecom.h"
#include "ran.h"

extern int g_threads_count;
extern vector<thread> g_threads;
extern double g_req_duration;
extern time_t g_start_time;

void simulator(int);
void check_usage(int);
void init(char**);
void run();
void print_results();

#endif /* RAN_SIMULATOR_H */