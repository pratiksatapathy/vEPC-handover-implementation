#include "sink.h"

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
	pgw_sgi_client.conn(g_pgw_sgi_ip_addr, g_pgw_sgi_port);
	pgw_sgi_client.snd(pkt);
}