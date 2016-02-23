#ifndef UTILS_H
#define UTILS_H

//(C++) Operations: Input/Output
#include <iostream>

//(C++) STL Operations: String, Vector, String stream, Unordered map
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

//(C) Operations: Input/Output, String, Standard libraries(like atoi, malloc)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//(C) Operations: Socket programming
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>

//(C) Operations: Multithreading
#include <pthread.h>

//(C) Operations: Raw socket programming with headers(IP, TCP, UDP)
#include <netinet/ip.h>
#include <netinet/tcp.h>     
#include <netinet/udp.h>    
#include <sys/ioctl.h>      
#include <bits/ioctls.h>    
#include <net/if.h>         

//(C) Operations: TUN interface Programming
#include <linux/if_tun.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>

//(C) Operations: Date & Time
#include <ctime>

//(C++) To measure Time in precision of milliseconds
#include <chrono>

using namespace std;

#define MAX_CONNS 10000
#define BUF_SIZE 1024

extern int g_reuse;

extern int g_freeport;
extern int g_enodeb_port;
extern int g_mme_port;
extern int g_hss_port;
extern int g_sgw1_port;
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

extern socklen_t g_addr_len;

extern struct timeval g_timeout;

// extern int g_sgw3_port;
// extern int g_sgw2_port;
// extern int g_public_sink_port;
// extern int g_private_sink_port;
// extern int g_pgw_server_for_sink_port;
// extern string g_sgw2_addr;
// extern string g_sgw3_addr;
// extern string g_public_sink_addr;
// extern string g_private_sink_addr;
// extern string g_pgw_server_for_sink_addr;
// extern int g_fail_count;

void report_error(int);
void report_error(int, const char*);
void report_error(int, bool&);

void print(string);
void print(int);
void print(string, int);
void print_message(string);
void print_message(string, int);
void print_message(string, unsigned long long);

const char* to_char_array(unsigned long long);

char* allocate_str_mem(int);
uint8_t* allocate_uint8_mem(int);
struct ip* allocate_ip_mem(int);
struct tcphdr* allocate_tcp_mem(int);

void check_server_usage(int, char*[]);
void check_client_usage(int, char*[]);

void time_check(time_t&, double&, bool&);

void handle_exceptions();

int create_udp_socket();

#endif //UTILS_H
