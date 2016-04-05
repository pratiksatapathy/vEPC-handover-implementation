#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "utils.h"

class UdpServer {
private:
	void init(string, uint64_t);
	
public:
	/* Address parameters */
	int conn_fd;
	uint64_t port;
	string ip_addr;
	struct sockaddr_in sock_addr;
	
	UdpServer();
	void run(string, uint64_t);
	void snd(struct sockaddr_in, Packet);
	void rcv(struct sockaddr_in&, Packet&);
	~UdpServer();
};

#endif /* UDP_SERVER_H */