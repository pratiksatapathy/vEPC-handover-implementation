#include "mme_server.h"

SctpServer g_mme_server;
int g_workers_count;

int handle_ue(int conn_fd) {
	Packet pkt;
	
	g_mme_server.rcv(conn_fd, pkt);
	pkt.extract_s1ap_hdr();
	switch (pkt.s1ap_hdr.msg_type) {
		case 1: /* Attach request - Initial UE message */
			g_mme.get_data(1);
			g_mme.init_authentication(pkt);
			g_mm_server.snd(conn_fd, pkt);
			break;
		case 2:
			break;
		default:
	}
	return 1;
}

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<mme_server_exec> WORKER_THREADS_COUNT" << endl;
		handle_type1_error(-1, "Invalid usage error");
	}
}

void init(char *argv[]) {
	g_workers_count = atoi(argv[1]);
}

void run() {
	g_mme_server.run(g_mme_ip_addr.c_str(), g_mme_port, g_workers_count, handle_ue);
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}


