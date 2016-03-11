#include "mme_server.h"

SctpServer g_mme_server;
Mme g_mme;
int g_workers_count;

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<mme_server_exec> WORKER_THREADS_COUNT" << endl;
		handle_type1_error(-1, "Invalid usage error: mmserver_mmeserver");
	}
}

void init(char *argv[]) {
	g_workers_count = atoi(argv[1]);
}

void run() {
	g_mme_server.run(g_mme_ip_addr.c_str(), g_mme_port, g_workers_count, handle_ue);
}

void handle_ue(int conn_fd) {
	Packet pkt;

	g_mme_server.rcv(conn_fd, pkt);
	pkt.extract_s1ap_hdr();
	if (pkt.s1ap_hdr.mme_ue_id == -1) {
		switch (pkt.s1ap_hdr.msg_type) {
			case 1: /* Attach request - Initial UE message */
				g_mme.handle_type1_attach(pkt);
				g_mme_server.snd(conn_fd, pkt);
				break;
			case 2:
				break;
			default:
				break;
		}		
	}
	else if (pkt.s1ap_hdr.mme_ue_id > 0) {
		switch (pkt.s1ap_hdr.msg_type) {
			case 2: /* Authentication response from UE */
				g_mme.handle_authentication(pkt);
				g_mme_server.snd(conn_fd, pkt);
				break;
			case 3:
				break;
			default:
				break;
		}				
	}
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}


