#ifndef SCTP_CLIENT_H
#define SCTP_CLIENT_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "utils.h"

class SctpClient {
private:
	void init(string, uint64_t);
	void connect_with_server();
	
public:
	/* Address parameters */
	int conn_fd;
	
	/* Server address parameters */
	uint64_t server_port;
	string server_ip_addr;
	struct sockaddr_in server_sock_addr;

	SctpClient();
	void conn(string, uint64_t);
	void snd(Packet);
	void rcv(Packet&);
	~SctpClient();
};

#endif /* SCTP_CLIENT_H */