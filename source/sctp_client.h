#ifndef SCTP_CLIENT_H
#define SCTP_CLIENT_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"
#include "packet.h"

class SctpClient {
public:
	/* Address parameters */
	int conn_fd;
	
	/* Server address parameters */
	int server_port;
	string server_ip_addr;
	struct sockaddr_in server_sock_addr;

	SctpClient();
	void conn(const char*, int);
	void init(const char*, int);
	void connect_with_server();
	void snd(Packet);
	void rcv(Packet&);
	void handle_failure(int, const char*);
	void handle_error(int, const char*);	
	~SctpClient();
};

#endif /* SCTP_CLIENT_H */