#include "network.h"

socklen_t g_sock_addr_len = sizeof(sockaddr_in);
Network g_nw;

void Network::set_inet_sock_addr(const char *ip_addr, int port, struct sockaddr_in &sock_addr) {
	int status;
	
	bzero((void*)&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	status = inet_aton(ip_addr, &sock_addr.sin_addr);	
	if (status == 0) {
		g_utils.handle_type1_error(-1, "inet_aton error: network_setinetsockaddr");
	}
}

void Network::bind_sock(int sock_fd, struct sockaddr_in sock_addr) {
	int status;
	
	status = bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
	g_utils.handle_type1_error(status, "Bind error: network_bindsock");
}

void Network::get_sock_addr(int sock_fd, struct sockaddr_in &sock_addr) {
	int status;

	status = getsockname(sock_fd, (struct sockaddr*)&sock_addr, &g_sock_addr_len);
	g_utils.handle_type1_error(status, "Getsockname error: network_getsockaddr");
}

void Network::set_sock_reuse(int sock_fd) {
	int status;

	status = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &g_reuse, sizeof(int));
	g_utils.handle_type1_error(status, "Setsockopt reuse error: network_setsockreuse");
}

void Network::set_rcv_timeout(int sock_fd) {
	int status;

	status = setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&g_timeout, sizeof(struct timeval));
	g_utils.handle_type1_error(status, "Setsockopt rcv timeout error: network_setrcvtimeout");
}

string Network::get_dst_ip_addr(Packet pkt) {
	struct ip *ip_hdr;
	string str_ip_addr;
	char *cstr_ip_addr;

	ip_hdr = pkt.allocate_ip_hdr_mem(IP_HDR_LEN);
	cstr_ip_addr = g_utils.allocate_str_mem(INET_ADDRSTRLEN);
	memmove(ip_hdr, pkt.data, IP_HDR_LEN * sizeof(uint8_t));
	if (inet_ntop(AF_INET, &(ip_hdr->ip_dst), cstr_ip_addr, INET_ADDRSTRLEN) == NULL) {
		g_utils.handle_type1_error(-1, "inet_ntop error: network_getdstipaddr");
	}
	str_ip_addr.assign(cstr_ip_addr);
	free(ip_hdr);
	free(cstr_ip_addr);
	return str_ip_addr;
}