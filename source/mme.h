#ifndef MME_H
#define MME_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "mme_data.h"

struct MME {	
	UDPClient to_hss;
	UDPClient to_sgw;

	struct sockaddr_in client_sock_addr;
	Packet pkt;

	int type;
	int subtype;
	int ue_num;

	unsigned long long imsi;
	unsigned long long msisdn;
	unsigned long long autn_num;
	unsigned long long rand_num;
	unsigned long long autn_res;	

	int status;
	string reply;
	bool success;

	MME();

	uint16_t generate_cteid(int&);
	int generate_bearer_id(int&);

	void setup_tohss();
	void setup_tosgw();
			
	void read_data();
	void set_metadata();

	void fetch_ue_data();
	void store_ue_data();
	void send_autn_req();

	void process_autn();
	void set_sgw_details(int&);
	void set_pgw_details(int&);	
	void create_session_req_to_sgw();
	void create_session_res_from_sgw();

	void store_enodeb_data();
	void modify_session_req_to_sgw();
	void modify_session_res_from_sgw();
	void send_attach_res();

	void delete_session_req_to_sgw();
	void delete_session_res_from_sgw();
	void send_detach_res();
	void delete_session_data();

	~MME();
};

extern UDPServer g_mme_server;
extern vector<pthread_t> g_tid;
extern int g_tcount;

#endif //MME_H
