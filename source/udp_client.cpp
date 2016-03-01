#include "udp_client.h"

UdpClient::UdpClient() {
	conn_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	handle_failure(conn_fd, "Socket error");
	port = g_freeport;
	bzero((char*)&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;  
	sock_addr.sin_addr.s_addr = INADDR_ANY;	
	sock_addr.sin_port = port;	
}

void UdpClient::conn(const char *arg_server_ip, int arg_server_port) {
	init(arg_server_ip, arg_server_port);
	set_timeout();
	bind_client();
	set_port();
}

void UdpClient::init(const char *arg_server_ip, int arg_server_port) {
	int status;

	server_port = arg_server_port;
	server_ip_addr.assign(arg_server_ip_addr);
	bzero((void*)&server_sock_addr, sizeof(server_sock_addr));
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(server_port);
	status = inet_aton(server_ip_addr.c_str(), &server_sock_addr.sin_addr);	
	if (status == 0) {
		cout << "inet_aton error: UDP Client" << endl;
		exit(EXIT_FAILURE);
	}
}

void UdpClient::set_timeout() {
	int status;

	status = setsockopt(conn_fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&g_timeout, sizeof(struct timeval));
	handle_failure(status, "Setsockopt rcv timeout error");	
}

void UdpClient::bind_client() {
	int status;
	
	status = bind(conn_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
	handle_failure(status, "Bind error");
}

void UdpClient::set_port() {
	int status;

	status = getsockname(conn_fd, (struct sockaddr*)&sock_addr, &g_addr_len);
	handle_failure(status, "Getsockname error");
	port = ntohs(sock_addr.sin_port);	
}

void UdpClient::snd(Packet pkt) {
	int status;

	while (1) {
		status = sendto(conn_fd, pkt.data, pkt.data_len, 0, (sockaddr*)&server_sock_addr, g_sock_addr_len);
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

void UdpClient::rcv(Packet &pkt) {
	int status;

	pkt.clear_pkt();
	status = recvfrom(conn_fd, pkt.data, BUF_SIZE, 0, (sockaddr*)&server_sock_addr, &g_sock_addr_len);
	handle_error(status, "Recvfrom error");
	pkt.data_ptr = 0;
	pkt.len = status;
}

void UdpClient::handle_failure(int arg, const char *c_msg) {
	string msg(c_msg);

	msg = msg + ": UDP client";
	if (arg < 0) {
		perror(msg.c_str());
		exit(EXIT_FAILURE);
	}
}

void UdpClient::handle_error(int arg, const char *c_msg) {
	string msg(c_msg);

	msg = msg + ": UDP client";
	if (arg < 0) {
		perror(msg.c_str());
	}
}

UdpClient::~UdpClient() {
	close(conn_fd);
}