#ifndef UE_H
#define UE_H

#include "utils.h"
#include "packet.h"
#include "udp_client.h"
#include "ran_data.h"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::microseconds microseconds;

struct UE {
	int ue_num;
	int type;
	int subtype;
	
	unsigned long long key;
	unsigned long long imsi;
	unsigned long long msisdn;
	string ue_ip;

	UDPClient to_mme;

	unsigned long long autn_num;
	unsigned long long rand_num;
	unsigned long long autn_res;

	int sink_port;
	string sink_addr;
	
	string reply;
	int status;
	bool success;

	string interface_name;

	Clock::time_point	start_time;
	Clock::time_point stop_time;
	microseconds time_diff_ms;

	int iperf_count;

	UE(int&);

	unsigned long long generate_key(int&);
	
	void set_mme_details();
	void startup_mme_client();

	void send_attach_req();
	void recv_autn_req();
	unsigned long long set_autn_res();
	void send_autn_res();
	void recv_autn_check();

	void send_tun_data();
	void set_sgw_details();
	void recv_tun_data();
	void add_map_entry();

	void turn_up_interface();
	void setup_sink();
	void send_iperf3_traffic();
	void turn_down_interface();

	void send_detach_req();
	void recv_detach_res();
	void delete_session_data();

	~UE();
};

#endif //UE_H
