#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "utils.h"
#include "packet.h"

struct UDPServer {
	int server_socket;
	int server_port;
	string server_addr;
	struct sockaddr_in server_sock_addr;
	
	int status;

	UDPServer();
	void bind_server(int, const char*);
	void write_data(struct sockaddr_in&, Packet&);
	void print_status(const char*);
	~UDPServer();
};

#endif //UDP_SERVER_H