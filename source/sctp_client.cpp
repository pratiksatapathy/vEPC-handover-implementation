#include "sctp_client.h"

SctpClient::SctpClient() {
	conn_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	handle_failure(conn_fd, "Socket error");
}

void SctpClient::conn(const char *arg_server_ip_addr, int arg_server_port) {
	init(arg_server_ip_addr, arg_server_port);
	connect_with_server();
}

void SctpClient::init(const char *arg_server_ip_addr, int arg_server_port) {
	int status;

	server_port = arg_server_port;
	server_ip_addr.assign(arg_server_ip_addr);
	bzero((void*)&server_sock_addr, sizeof(server_sock_addr));
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(server_port);
	status = inet_aton(server_ip_addr.c_str(), &server_sock_addr.sin_addr);	
	if (status == 0) {
		cout << "inet_aton error: SCTP Client" << endl;
		exit(EXIT_FAILURE);
	}		
}

void SctpClient::connect_with_server() {
	int status;

	status = connect(conn_fd, (struct sockaddr *)&server_sock_addr, sizeof(server_sock_addr));
	handle_error(status, "Connect error");
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
	handle_error(status, "Write error");
}

void SctpClient::rcv(Packet &pkt) {
	int status;

	pkt.clear_pkt();
	status = read(conn_fd, pkt.data, BUF_SIZE);
	handle_error(status, "Read error");
	pkt.data_ptr = 0;
	pkt.len = status;
}

void SctpClient::handle_failure(int, const char*) {
	string msg(c_msg);
	msg = msg + ": SCTP client";
	if (arg < 0) {
		perror(msg.c_str());
		exit(-1);
	}
}

void SctpClient::handle_error(int, const char*); {
	string msg(c_msg);
	msg = msg + ": SCTP client";
	if (arg < 0) {
		perror(msg.c_str());
	}
}

SctpClient::~SctpClient() {
	close(conn_fd);
}
