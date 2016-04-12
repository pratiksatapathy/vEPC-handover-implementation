#include "sink.h"

string g_pgw_sgi_ip_addr = "10.14.13.29";
string g_sink_ip_addr = "10.129.5.193";
int g_pgw_sgi_port = 8100;
int g_sink_port = 8500;

// string g_pgw_sgi_ip_addr = "10.129.5.193";
// string g_sink_ip_addr = "10.129.5.193";
// int g_pgw_sgi_port = 8100;
// int g_sink_port = 8500;

void TrafficMonitor::handle_uplink_udata() {
	struct sockaddr_in src_sock_addr;
	Packet pkt;

	server.rcv(src_sock_addr, pkt);
	tun.snd(pkt);
}

void TrafficMonitor::handle_downlink_udata() {
	UdpClient pgw_sgi_client;
	Packet pkt;

	tun.rcv(pkt);
	pgw_sgi_client.conn(g_sink_ip_addr, g_pgw_sgi_ip_addr, g_pgw_sgi_port);
	pgw_sgi_client.snd(pkt);
}