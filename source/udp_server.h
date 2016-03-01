#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"
#include "packet.h"

class UdpServer {
	/* Address parameters */
	int conn_fd;
	int port;
	string ip_addr;
	struct sockaddr_in sock_addr;
	
	UdpServer();
	void run(const char*, int);
	void init(const char*, int);
	void bind_server();
	void snd(struct sockaddr_in, Packet);
	void rcv(struct sockaddr_in, Packet&);
	void handle_failure(int, const char*);
	void handle_error(int, const char*);	
	~UdpServer();
};

#endif /* UDP_SERVER_H */