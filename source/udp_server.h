#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"
#include "packet.h"
#include "network.h"

class UdpServer {
public:
	/* Address parameters */
	int conn_fd;
	int port;
	string ip_addr;
	struct sockaddr_in sock_addr;
	
	UdpServer();
	void run(const char*, int);
	void init(const char*, int);
	void snd(struct sockaddr_in, Packet);
	void rcv(struct sockaddr_in, Packet&);
	~UdpServer();
};

#endif /* UDP_SERVER_H */