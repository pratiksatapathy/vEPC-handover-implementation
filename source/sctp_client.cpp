#include "sctp_client.h"

SctpClient::SctpClient() {
	conn_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	handle_type1_error(conn_fd, "Socket error: sctpclient_sctpclient");
}

void SctpClient::conn(const char *arg_server_ip_addr, int arg_server_port) {
	init(arg_server_ip_addr, arg_server_port);
	connect_with_server();
}

void SctpClient::init(const char *arg_server_ip_addr, int arg_server_port) {
	int status;

	server_port = arg_server_port;
	server_ip_addr.assign(arg_server_ip_addr);
	set_inet_sock_addr(server_ip_addr.c_str(), server_port, server_sock_addr);
}

void SctpClient::connect_with_server() {
	int status;

	status = connect(conn_fd, (struct sockaddr *)&server_sock_addr, sizeof(server_sock_addr));
	handle_type2_error(status, "Connect error: sctpclient_connectwithserver");
}

void SctpClient::snd(Packet pkt) {
	int status;

	while (1) {
		status = write(conn_fd, pkt.data, pkt.len);
		if (errno == EPERM) {
			errno = 0;
			usleep(1000);
			continue;
		}
		else {
			break;
		}		
	}
	handle_type2_error(status, "Write error: sctpclient_snd");
}

void SctpClient::rcv(Packet &pkt) {
	int status;

	pkt.clear_pkt();
	status = read(conn_fd, pkt.data, BUF_SIZE);
	handle_type2_error(status, "Read error: sctpclient_rcv");
	pkt.data_ptr = 0;
	pkt.len = status;
}

SctpClient::~SctpClient() {
	close(conn_fd);
}
