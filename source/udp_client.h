#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "utils.h"

class UdpClient {
private:
	void init(string, uint64_t);
	void set_port();
	
public:
	/* Address parameters */
	int conn_fd;
	uint64_t port;
	struct sockaddr_in sock_addr;

	/* Server parameters */
	uint64_t server_port;
	string server_ip_addr;
	struct sockaddr_in server_sock_addr;

	UdpClient();
	void conn(string, uint64_t);
	void snd(Packet);
	void rcv(Packet&);
	~UdpClient();		
};

#endif /* UDP_CLIENT_H */