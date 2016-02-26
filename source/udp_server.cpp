#include "udp_server.h"

UDPServer::UDPServer() {
	int status;

	conn_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	handle_failure(conn_fd, "Socket error");	
	status = setsockopt(conn_fd, SOL_SOCKET, SO_REUSEADDR, &g_reuse, sizeof(int));
	handle_failure(status, "Setsockopt reuse error");	
}

void UDPServer::run(const char *arg_ip_addr, int arg_port) {
	init(arg_ip_addr, arg_port);
	bind_server();
}

void UDPServer::init(const char *arg_ip_addr, int arg_port) {
	int status;

	port = arg_port;
	ip_addr.assign(arg_ip_addr);
	bzero((char *) &sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;  	
	sock_addr.sin_port = htons(port);
	status = inet_aton(ip_addr.c_str(), &sock_addr.sin_addr);	
	if (status == 0) {
		cout << "inet_aton error" << endl;
		exit(EXIT_FAILURE);
	}	
}

void UDPServer::bind_server() {
	int status;

	status = bind(conn_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
	handle_failure(status, "Bind error");	
}

void UDPServer::snd(struct sockaddr_in &dst_sock_addr, Packet &pkt){
	int status;

	while (1) {
		status = sendto(conn_fd, pkt.data, pkt.data_len, 0, (sockaddr*)&dst_sock_addr, g_sock_addr_len);
		if (errno == EPERM) {
			errno = 0;
			usleep(1000);
			continue;
		}
		else {
			break;
		}
	}
	handle_error(status, "Sendto error");
}

void SCTPServer::handle_failure(int arg, const char *c_msg) {
	string msg(c_msg);

	msg = msg + ": UDP server";
	if (arg < 0) {
		perror(msg.c_str());
		exit(EXIT_FAILURE);
	}
}

void SCTPServer::handle_error(int arg, const char *c_msg) {
	string msg(c_msg);

	msg = msg + ": UDP server";
	if (arg < 0) {
		perror(msg.c_str());
	}
}

UDPServer::~UDPServer() {
	close(conn_fd);
}