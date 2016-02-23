#ifndef SINK_MONITOR_H
#define SINK_MONITOR_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"

struct SinkMonitor {
	UDPClient to_pgw_dlink;

	struct sockaddr_in client_sock_addr;
	Packet pkt;

	int status;
	bool success;

	int tun_fd;
	string tun_name;

	SinkMonitor();
	
	void attach_to_tun();
	
	void setup_topgwdlink();

	void read_pgw();
	void make_uplink_data();
	void write_tun();

	void read_tun();
	void make_downlink_data();
	void send_pgw_dlink();

	~SinkMonitor();
};

extern UDPServer g_public_sink_server;

#endif //SINK_MONITOR_H
