#ifndef PGW_H
#define PGW_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "pgw_data.h"

struct PGW {
	UDPClient to_public_sink;

	struct sockaddr_in client_sock_addr;
	Packet pkt;

	int type;
	int subtype;
	int ue_num;

	int status;
	string reply;
	bool success;

	PGW();

	uint16_t generate_cteid(int);
	uint16_t generate_uteid(int);

	void setup_topublicsink();

	void read_data();
	void set_metadata();

	void set_sgw_details(int);
	void store_create_session_data();
	void create_session_res_to_sgw();
	void add_map_entry();

	void make_uplink_data();
	void send_public_sink();

	void delete_session_res_to_sgw();
	void delete_session_data();

	~PGW();
};

extern UDPServer g_pgw_server;
extern vector<pthread_t> g_tid;
extern pthread_t g_downlink_tid;
extern int g_tcount;

extern vector<string> g_ip_table; // No lock needed since only READ accesses are made by concurrent threads

void generate_ip_table();

#endif //PGW_H
