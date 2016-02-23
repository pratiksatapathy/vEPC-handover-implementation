#ifndef PGW_DLINK_H
#define PGW_DLINK_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "pgw_data.h"

struct PGWDlink {
	UDPClient to_sgw;

	struct sockaddr_in client_sock_addr;
	Packet pkt;

	int type;
	int subtype;
	int ue_num;

	string ue_ip;

	int status;
	bool success;

	PGWDlink();

	void setup_tosgw();

	void read_data();
	void set_ue_num();
	void make_downlink_data();
	void send_sgw();	

	~PGWDlink();
};

extern UDPServer g_pgw_dlink_server;

#endif //PGW_DLINK_H
