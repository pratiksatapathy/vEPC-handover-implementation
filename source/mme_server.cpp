#include "mme_server.h"

Mme g_mme;
int g_workers_count;

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<mme_server_exec> THREADS_COUNT" << endl;
		g_utils.handle_type1_error(-1, "Invalid usage error: mmserver_checkusage");
	}
}

void init(char *argv[]) {
	g_workers_count = atoi(argv[1]);
}

void run() {
	cout << "MME server started" << endl;
	g_mme.server.run(g_mme_ip_addr.c_str(), g_mme_port, g_workers_count, handle_ue);
}

int handle_ue(int conn_fd) {
	bool res;
	Packet pkt;

	g_mme.server.rcv(conn_fd, pkt);
	if (pkt.len <= 0) {
		cout << "mmeserver_handleue:" << " Connection closed" << endl;
		return 0;
	}
	pkt.extract_s1ap_hdr();
	if (pkt.s1ap_hdr.mme_s1ap_ue_id == 0) {
		switch (pkt.s1ap_hdr.msg_type) {
			/* Initial Attach request */
			case 1: 
				cout << "mmeserver_handleue:" << " case 1:" << endl;
				g_mme.handle_type1_attach(conn_fd, pkt);
				break;

			/* For error handling */
			default:
				cout << "mmeserver_handleue:" << " default case: new" << endl;
				break;
		}		
	}
	else if (pkt.s1ap_hdr.mme_s1ap_ue_id > 0) {
		switch (pkt.s1ap_hdr.msg_type) {
			/* Authentication response */
			case 2: 
				cout << "mmeserver_handleue:" << " case 2:" << endl;
				res = g_mme.handle_autn(conn_fd, pkt);
				if (res) {
					g_mme.handle_security_mode_cmd(conn_fd, pkt);
				}
				break;

			/* Security Mode Complete */
			case 4: 
				cout << "mmeserver_handleue:" << " case 4:" << endl;
				res = g_mme.handle_security_mode_complete(conn_fd, pkt);
				if (res) {
					g_mme.handle_location_update(pkt);
					g_mme.handle_create_session(conn_fd, pkt);
				}
				break;

			/* Attach Complete */
			case 5: 
				cout << "mmeserver_handleue:" << " case 5:" << endl;
				g_mme.handle_modify_bearer(pkt);
				break;

			/* For error handling */	
			default:
				cout << "mmeserver_handleue:" << " default case: attached" << endl;
				break;
		}				
	}		
	return 1;
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}