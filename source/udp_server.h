#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "utils.h"
#include "packet.h"

struct UDPServer {
	/* Address parameters */
	int conn_fd;
	int port;
	string ip_addr;
	struct sockaddr_in sock_addr;
	
	UDPServer();
	void run(const char*, int);
	void init(const char*, int);
	void bind_server();
	void snd(struct sockaddr_in&, Packet&);
	~UDPServer();
};

#endif //UDP_SERVER_H