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

void check_usage(int);
void init(char**);
void run();

#endif /* RAN_SIMULATOR_H */