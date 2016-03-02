#ifndef UTILS_H
#define UTILS_H

/* (C++) cout, endl */
#include <iostream> 

/* (C++) STL: string, vector, queue, thread, unordered map */
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <unordered_map>

/* (C++) stringstream */
#include <sstream>

/* (C++) chrono: high_resolution_clock, microseconds */
#include <chrono>

/* (C++) default_random_engine, exponential_distribution<T> */
#include <random>

/* (C) memset, memmove */
#include <stdio.h>

/* (C) strlen */
#include <string.h>

/* (C) signal */
#include <signal.h>

/* (C) pthread_create, pthread_kill */
#include <pthread.h>

/* (C) INT_MAX */
#include <limits.h>

using namespace std;

extern int g_reuse;
extern int g_freeport;
extern int g_enodeb_port;
extern int g_mme_port;
extern int g_hss_port;
extern int g_sgw_port;
extern int g_pgw_port;
extern int g_pgw_dlink_port;
extern int g_public_sink_port;
extern string g_enodeb_ip_addr;
extern string g_mme_ip_addr;
extern string g_hss_ip_addr;
extern string g_sgw1_ip_addr;
extern string g_pgw_ip_addr;
extern string g_pgw_dlink_ip_addr;
extern string g_public_sink_ip_addr;
extern string g_private_sink_ip_addr;
extern struct timeval g_timeout;

void handle_type1_error(int, const char*);
void handle_type2_error(int, const char*);
string ulltos(unsigned long long);
char* allocate_str_mem(int);
uint8_t* allocate_uint8_mem(int);
void time_check(time_t, double, bool);

#endif /* UTILS_H */
