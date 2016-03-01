#include "network.h"

void bind_sock(int sock_fd, struct sockaddr_in sock_addr) {
	int status;
	
	status = bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
	handle_event_a(status, "Bind error");
}

void set_sock_addr(const char *ip_addr, int port, struct sockaddr_in &sock_addr) {

}

void get_sock_addr(int sock_fd, struct sockaddr_in &sock_addr) {

}

void set_timeout(int sock_fd) {

}

void set_sock_reuse(int sock_fd) {

}
