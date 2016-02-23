#ifndef SGW_H
#define SGW_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "sgw_data.h"

struct SGW {
	UDPClient to_pgw;
	UDPClient to_enodeb;

	struct sockaddr_in client_sock_addr;
	Packet pkt;

	int type;
	int subtype;
	int ue_num;

	int status;
	string reply;
	bool success;

	SGW();

	uint16_t generate_cteid(int);
	uint16_t generate_uteid(int);

	void setup_topgw();
	void setup_toenodeb();

	void read_data();
	void set_metadata();

	void store_create_session_data();
	void set_pgw_details(int);
	void create_session_req_to_pgw();
	void create_session_res_from_pgw();
	void create_session_res_to_mme();

	void set_enodeb_details(int);
	void store_modify_session_data();
	void modify_session_res_to_mme();

	void make_uplink_data();
	void send_pgw();

	void make_downlink_data();
	void send_enodeb();

	void delete_session_req_to_pgw();
	void delete_session_res_from_pgw();
	void delete_session_res_to_mme();
	void delete_session_data();

	~SGW();
};

extern UDPServer g_sgw_server;
extern vector<pthread_t> g_tid;
extern int g_tcount;

#endif //SGW_H