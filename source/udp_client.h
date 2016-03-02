#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"
#include "packet.h"
#include "network.h"

class UdpClient {
public:
	/* Address parameters */
	int conn_fd;
	int port;
	struct sockaddr_in sock_addr;

	/* Server parameters */
	int server_port;
	string server_ip_addr;
	struct sockaddr_in server_sock_addr;

	UdpClient();
	void conn(const char*, int);
	void init(const char*, int);
	void set_port();
	void snd(Packet);
	void rcv(Packet&);
	~UdpClient();		
};

#endif /* UDP_CLIENT_H */