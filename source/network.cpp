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

void Network::set_snd_timeout(int sock_fd) {
	int status;

	status = setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval*)&g_timeout, sizeof(struct timeval));
	g_utils.handle_type1_error(status, "Setsockopt snd timeout error: network_setsndtimeout");
}

void Network::set_rcv_timeout(int sock_fd) {
	int status;

	status = setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&g_timeout, sizeof(struct timeval));
	g_utils.handle_type1_error(status, "Setsockopt rcv timeout error: network_setrcvtimeout");
}
