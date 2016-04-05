#ifndef UTILS_H
#define UTILS_H

/* (C++) chrono: high_resolution_clock, microseconds */
#include <chrono>

/* (C++) cout, endl */
#include <iostream> 

/* (C) INT_MAX */
#include <limits.h>

/* (C) pthread_create, pthread_kill */
#include <pthread.h>

/* (C++) STL: queue */
#include <queue>

/* (C++) default_random_engine, exponential_distribution<T> */
#include <random>

/* (C) signal */
#include <signal.h>

/* (C) memset, memmove */
#include <stdio.h>

/* (C) strlen */
#include <string.h>

/* (C++) STL: string */
#include <string>

/* (C++) stringstream */
#include <sstream>

/* (C++) STL: thread */
#include <thread>

/* (C++) STL: unordered map */
#include <unordered_map>

/* (C++) STL: vector */
#include <vector>

using namespace std;

const uint64_t MAX_UE_COUNT = 10000;
extern int g_reuse;
extern uint64_t g_freeport;
extern uint64_t g_enodeb_port;
extern uint64_t g_mme_port;
extern uint64_t g_hss_port;
extern uint64_t g_sgw_s11_port;
extern uint64_t g_sgw_s1_port;
extern uint64_t g_sgw_s5_port;
extern uint64_t g_pgw_s5_port;
extern uint64_t g_pgw_sgi_port;
extern uint64_t g_sink_port;
extern string g_enodeb_ip_addr;
extern string g_mme_ip_addr;
extern string g_hss_ip_addr;
extern string g_sgw_s11_ip_addr;
extern string g_sgw_s1_ip_addr;
extern string g_sgw_s5_ip_addr;
extern string g_pgw_s5_ip_addr;
extern string g_pgw_sgi_ip_addr;
extern string g_sink_ip_addr;
extern struct timeval g_timeout;

class Utils {
public:
	void handle_type1_error(int, string);
	void handle_type2_error(int, string);
	char* allocate_str_mem(uint64_t);
	uint8_t* allocate_uint8_mem(uint64_t);
	void time_check(time_t, double, bool&);
};

extern Utils g_utils;

#endif /* UTILS_H */
