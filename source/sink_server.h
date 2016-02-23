#ifndef SINK_SERVER_H
#define SINK_SERVER_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "sink_monitor.h"

extern SinkMonitor g_sink_monitor;

extern int g_total_connections;
extern vector<int> g_conn_num;
extern pthread_t g_mon_tid;
extern vector<pthread_t> g_tid;

void setup_interface();
void setup_tun();

void* monitor_traffic(void*);
void uplink_data_transfer();
void downlink_data_transfer();

void* server_iperf3(void*);

void startup_sink(char**, vector<int>&, vector<pthread_t>&);

#endif //SINK_SERVER_H
