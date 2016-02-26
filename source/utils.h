#ifndef UTILS_H
#define UTILS_H

/* (C++) cout, endl */
#include <iostream> 

/* (C++) STL: string, vector, unordered map */
#include <string>
#include <vector>
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

/* (C) read, write, close */
#include <unistd.h>

/* (C) socket */
#include <sys/socket.h>

/* (C) open */
#include <fcntl.h>

/* (C) select */
#include <sys/select.h>

/* (C) inet_aton, inet_ntop */
#include <arpa/inet.h>

/* (C) signal */
#include <signal.h>

/* (C) pthread_create, pthread_kill */
#include <pthread.h>

/* (C) ip_hdr */
#include <netinet/ip.h>

/* (C) ioctl */
#include <sys/ioctl.h>

/* (C) IFF_TUN, IFF_NO_PI, TUNSETIFF */
#include <linux/if_tun.h>

/* (C) ifreq, IFNAMSIZ */
#include <net/if.h>         

using namespace std;

#define MAX_CONNS 10000
#define BUF_SIZE 1024

extern int g_reuse;
extern int g_freeport;
extern int g_enodeb_port;
extern int g_mme_port;
extern int g_hss_port;
extern int g_sgw_port;
extern int g_pgw_port;
extern int g_pgw_dlink_port;
extern int g_public_sink_port;
extern string g_enodeb_addr;
extern string g_mme_addr;
extern string g_hss_addr;
extern string g_sgw1_addr;
extern string g_pgw_addr;
extern string g_pgw_dlink_addr;
extern string g_public_sink_addr;
extern string g_private_sink_addr;
extern socklen_t g_sock_addr_len;
extern struct timeval g_timeout;

string ulltos(unsigned long long);
char* allocate_str_mem(int);
uint8_t* allocate_uint8_mem(int);
struct ip* allocate_ip_mem(int);
void time_check(time_t&, double&, bool&);

#endif //UTILS_H
