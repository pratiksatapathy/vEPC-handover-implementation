#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "utils.h"
#include "packet.h"

struct UDPClient {
	int client_socket;
	int client_port;
	string client_addr;
	struct sockaddr_in client_sock_addr;
	
	int server_port;
	string server_addr;
	struct sockaddr_in server_sock_addr;

	int status;
	Packet pkt;
	
	UDPClient();
	int create_udp_socket();
	void bind_client();
	void set_server_details(int, const char*);
	void read_data(bool&);
	void write_data();	
	void close_client();
	~UDPClient();		
};

#endif //UDP_CLIENT_H