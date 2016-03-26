#include "mme_server.h"

Mme g_mme;
int g_workers_count;

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<mme_server_exec> THREADS_COUNT" << endl;
		handle_type1_error(-1, "Invalid usage error: mmserver_checkusage");
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
			case 1: /* Attach request - Initial UE message */
				cout << "mmeserver_handleue:" << " case 1:" << endl;
				g_mme.handle_type1_attach(conn_fd, pkt);
				break;
			case 2:
				break;
			default:
				cout << "mmeserver_handleue:" << " default case: new" << endl;
				break;
		}		
	}
	else if (pkt.s1ap_hdr.mme_s1ap_ue_id > 0) {
		switch (pkt.s1ap_hdr.msg_type) {
			case 2: /* Authentication response from UE */
				cout << "mmeserver_handleue:" << " case 2:" << endl;
				res = g_mme.handle_autn(conn_fd, pkt);
				if (res) {
					g_mme.handle_security_setup(conn_fd, pkt);
					g_mme.handle_ue_location_update();
				}
				break;
			case 3:
				break;
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